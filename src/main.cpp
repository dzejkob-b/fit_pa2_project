#include <iostream>
#ifndef LINUX_ENV
#include <windows.h>
#endif // LINUX_ENV
#include "Program.h"
#include "tests.h"

int globMain(int argc, char * argv[]) {
    int result = 0;
    int argPos = 1;
    int autoSaveFormat = 0;

    Program p;

    if (argPos < argc && strcmp(argv[argPos], "test") == 0) {
        // tests only
        testsRun();
        return 0;

    } else if (argPos >= argc) {
        result = -5;

    } else if ((result = p.initScene(argv[argPos])) != 0) {
        // scene init error
        p.logPush("Error during scene init / loading.");
        result = -1;
    }

    argPos++;

    if (argPos < argc && result == 0 && strcmp(argv[argPos], "stdin") == 0) {
        // load from stdin

        std::string input;
        std::string content;

        do {
            getline(std::cin, input);
            content.append(input);

        } while (!std::cin.eof());

        if ((result = p.getArea()->loadTextString(content.c_str())) != 0) {
            // scene parse error
            p.logPush("Error during scene parsing from stdin.");
            result = -2;
        }

        argPos++;
    }

    if (result != 0) {
        // skip ...

    } else if (argPos >= argc || result == -5) {
        // not enough arguments
        result = -5;
        p.logPush("Not enough arguments. Usage: program scene_path edit|[save save_path]");

    } else if (strcmp(argv[argPos], "nothing") == 0) {
        // nothing ...

    } else if (strcmp(argv[argPos], "edit") == 0) {
        // graphical editing

        if ((result = p.setupSDL()) == 0) {

            result = p.runSDL();

        }

        p.quitSDL();

    } else if (strcmp(argv[argPos], "save") == 0 || strcmp(argv[argPos], "save_bmp") == 0) {
        // resave logic

        autoSaveFormat = 0;

        if (strcmp(argv[argPos], "save_bmp") == 0) {
            autoSaveFormat = 1;
        }

        argPos++;

        if (argPos >= argc) {
            // no filename specified - save as text

            if (autoSaveFormat == 0) {
                if ((result = p.getArea()->saveText(NULL)) != 0) {
                    result = -4;
                    p.logPush("Scene save error (default text).");
                }

            } else if (autoSaveFormat == 1) {
                if ((result = p.getArea()->saveBitmap(NULL)) != 0) {
                    result = -4;
                    p.logPush("Scene save error (default bitmap).");
                }
            }

        } else if ((result = p.getArea()->saveAuto(argv[argPos])) != 0) {

            if (result == -8) {
                result = -4;
                p.logPush("Invalid save extension.");

            } else {
                result = -4;
                p.logPush("Scene save error.");
            }

        } else {
            // save done ...
        }

    } else {
        // invalid

        result = -2;

        std::string logMsg("Invalid flag: '");
        logMsg.append(argv[2]);
        logMsg.append("'");

        p.logPush(logMsg.c_str());
    }

    return result;
}

#ifdef LINUX_ENV

int main(int argc, char* argv[]) {
    return globMain(argc, argv);
}

#else

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    int argc = 3;
    char * argv[10];

    argv[0] = "program";
    argv[1] = "examples/figures";
    argv[2] = "edit";

    return globMain(argc, argv);
}

#endif // LINUX_ENV
