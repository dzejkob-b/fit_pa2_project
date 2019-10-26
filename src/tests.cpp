#include "tests.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include "SceneParser.h"
#include "GraphicPrinterBMP.h"

void testCollision1() {

    DrawEntity ent(true);

    ent.addPoint(Point(10, 10));
    ent.addPoint(Point(30, 10));
    ent.addPoint(Point(30, 30));
    ent.addPoint(Point(10, 30));
    ent.rebuild();

    Point ptFrom = Point(10, 15);
    Point ptDir = Point(20, 15);

    std::vector<Point> lst = ent.collidePoints(ptFrom, ptDir);
    assert(lst.size() == 2);
}

void testCollision2() {

    DrawEntity ent(true);

    ent.addPoint({50, 40});
    ent.addPoint({60, 50});
    ent.addPoint({50, 60});
    ent.addPoint({40, 50});
    ent.rebuild();

    Point ptFrom = Point(50, 50);
    Point ptDir = Point(55, 50);

    std::vector<Point> lst = ent.collidePoints(ptFrom, ptDir);
    assert(lst.size() == 2);

    ptFrom = Point(50, 50);
    assert(ent.isInside(ptFrom) == true);
}

void testScene1() {

    DrawArea area;
    SceneParser parse(&area);

    parse.loadFile("examples/test_scene.txt");

    assert(area.getNodesCount(DrawNode::TYPE_GROUP) == 7);
    assert(area.getNodesCount(DrawNode::TYPE_SHORTCUT) == 2);
    assert(area.getNodesCount(DrawNode::TYPE_ENTITY) == 1);
    assert(area.getNodesCount(DrawNode::TYPE_ELLIPSE) == 1);
    assert(area.getNodesCount(DrawNode::TYPE_CIRCLE) == 3);
    assert(area.getNodesCount(DrawNode::TYPE_RECTANGLE) == 5);
    assert(area.getNodesCount(DrawNode::TYPE_SQUARE) == 9);

    // print to BMP and forget

    GraphicPrinterBMP printer;

    assert(printer.initialize(&area) == 0);
    area.draw(printer);
}

void testSceneString1() {

    DrawArea area;
    SceneParser parse(&area);

    parse.loadString("group(main_group) { rectangle(10, 10, 50, 50, red) square(50, 50, 100, green) polyline(2, 10, 20, 40, 50, closed, red) ellipse(150, 150, 100, 50, 20, white) circle(110, 80, 30, 20, gray) }");
    parse.loadString("shortcut(main_group, sh1) shortcut(main_group, sh2) shortcut(main_group, sh3) shortcut(main_group, sh4) shortcut(main_group, sh5) ");

    assert(area.getNodesCount(DrawNode::TYPE_GROUP) == 2);
    assert(area.getNodesCount(DrawNode::TYPE_SHORTCUT) == 5);
    assert(area.getNodesCount(DrawNode::TYPE_ENTITY) == 1);
    assert(area.getNodesCount(DrawNode::TYPE_ELLIPSE) == 1);
    assert(area.getNodesCount(DrawNode::TYPE_CIRCLE) == 1);
    assert(area.getNodesCount(DrawNode::TYPE_RECTANGLE) == 1);
    assert(area.getNodesCount(DrawNode::TYPE_SQUARE) == 1);

    // print to BMP and forget

    GraphicPrinterBMP printer;

    assert(printer.initialize(&area) == 0);
    area.draw(printer);
}

void testSceneLinks() {

    DrawArea area;
    SceneParser parse(&area);

    parse.loadString("group(main_group) { rectangle(10, 10, 50, 50, red) square(50, 50, 100, green) polyline(2, 10, 20, 40, 50, closed, red) ellipse(150, 150, 100, 50, 20, white) circle(110, 80, 30, 20, gray) }");
    parse.loadString("shortcut(main_group, sh1) shortcut(main_group, sh2) shortcut(main_group, sh3) shortcut(main_group, sh4) shortcut(main_group, sh5) ");

    for (int sf = 0; sf < 10; sf++) {
        parse.loadString("group(main_group) { rectangle(10, 10, 50, 50, red) square(50, 50, 100, green) polyline(2, 10, 20, 40, 50, closed, red) ellipse(150, 150, 100, 50, 20, white) circle(110, 80, 30, 20, gray) }");
    }

    DrawNode * grpNd = area.getGroup("main_group");

    assert(grpNd != NULL);

    grpNd->getParentNode()->delNode(grpNd);

    assert(area.getNodesCount(DrawNode::TYPE_GROUP) == 1); // root "scene" group still contained
    assert(area.getNodesCount(DrawNode::TYPE_SHORTCUT) == 0);
    assert(area.getNodesCount(DrawNode::TYPE_ENTITY) == 0);
    assert(area.getNodesCount(DrawNode::TYPE_ELLIPSE) == 0);
    assert(area.getNodesCount(DrawNode::TYPE_CIRCLE) == 0);
    assert(area.getNodesCount(DrawNode::TYPE_RECTANGLE) == 0);
    assert(area.getNodesCount(DrawNode::TYPE_SQUARE) == 0);
}

void testSceneGarbage() {

    DrawArea area;
    SceneParser parse(&area);

    parse.loadString("asdjf ioasfd jioafgi apfdhg ioh2p ioefa[ [][14 l=2t3kwk=D}");
    parse.loadString("()2190F{WAF[pp[21[(AW)(2109[0fAW[920[9");

    area.reset();

    parse.loadString("rectangle() square() circle(1,1,1,1,1,1,1,1,1,1,1) ellipse(bla, ble)");
}

void testSceneRandom() {

    srand(time(NULL));

    std::string cnt;
    std::stringstream str;

    int mWidth = 1000;
    int mHeight = 1000;

    for (int sf = 0; sf < 500; sf++) {
        int rnd = rand() % 5, pts;

        str.str("");

        switch (rnd) {
            case 0 :
                pts = 2 + (rand() % 20);

                str << "polyline(" << pts << ", ";

                for (int qf = 0; qf < pts; qf++) {
                    str << (rand() % mWidth) << ", ";
                    str << (rand() % mHeight) << ", ";
                }

                if (rand() % 2 == 0) {
                    str << "closed, ";
                }

                str << "red)";
                break;

            case 1 :
                str << "ellipse(";
                str << (rand() % mWidth) << ", ";
                str << (rand() % mHeight) << ", ";
                str << (10 + rand() % 100) << ", ";
                str << (10 + rand() % 100) << ", ";
                str << (2 + rand() % 50) << ", ";
                str << "red)";
                break;

            case 2 :
                str << "circle(";
                str << (rand() % mWidth) << ", ";
                str << (rand() % mHeight) << ", ";
                str << (10 + rand() % 100) << ", ";
                str << (2 + rand() % 50) << ", ";
                str << "red)";
                break;

            case 3 :
                str << "rectangle(";
                str << (rand() % mWidth) << ", ";
                str << (rand() % mHeight) << ", ";
                str << (10 + rand() % 200) << ", ";
                str << (10 + rand() % 200) << ", ";
                str << "red)";
                break;

            case 4 :
                str << "square(";
                str << (rand() % mWidth) << ", ";
                str << (rand() % mHeight) << ", ";
                str << (10 + rand() % 200) << ", ";
                str << "red)";
                break;
        }

        cnt.append(str.str());
        cnt.append("\n");
    }

    DrawArea area;
    SceneParser parse(&area);

    // print to BMP and forget

    GraphicPrinterBMP printer;

    assert(printer.initialize(&area) == 0);
    area.draw(printer);
}

void testCall(void (*_pt)(), const char * caption) {
    std::cout << "Testing `" << caption << "` ... ";
    _pt();
    std::cout << "OK\n";
}

void testsRun() {
    std::cout << "===========\n";
    std::cout << "== TESTS ==\n";
    std::cout << "===========\n";

    testCall(testCollision1, "testCollision1");
    testCall(testCollision2, "testCollision2");
    testCall(testScene1, "testScene1");
    testCall(testSceneString1, "testSceneString1");
    testCall(testSceneString1, "testSceneLinks");
    testCall(testSceneGarbage, "testSceneGarbage");
    testCall(testSceneRandom, "testSceneRandom");

    std::cout << "... tests OK\n";
}
