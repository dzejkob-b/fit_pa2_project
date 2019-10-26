# hrabajak draw Documentation

This is a semestral project for subject BIK-PA2 - Faculty of Information Technology of Czech Technical University in Prague (https://fit.cvut.cz).

Author: Jakub Hrabálek <br/>
Faculty email: hrabajak@fit.cvut.cz

<pre>
****************************************************************************************************************
* Prohlasuji, ze jsem semestralni praci vytvoril sam, vlastnimi silami a ze jsem program odnikud nezkopiroval. *
*                                                                                                              *
* Jakub Hrabalek                                                                                               *
* hrabajak@fit.cvut.cz                                                                                         *
****************************************************************************************************************
</pre>

## The main topic scope (czech)

Naprogramujte jednoduchý nástroj pro zobrazení obrázků definovaných v textovém souboru.

Program musí splňovat následující funkcionality:

* Načíst ze souboru a z konzole definici jednotlivých objektů v prostoru (velikost obrázku, pozice a parametry objektů, ...).
* Program podporuje minimálně čáru, polyline, kruh, elipsu, obdélník a skupinu objektů.
* Objektům se dají nastavovat různé barvy (v případě ASCII styl vykreslování) a výplně.
* Obrázek je nutné reprezentovat jako graf scény - stromovou strukturu (případně DAG)
* Skupina objektů funguje jako vzor, tudíž po vytvoření skupiny objektů můžu tuto skupinu vložit několikrát do obrázku na různá místa.
* Implementujte alespoň 2 různé výstupní formáty (ASCII, BMP, SVG, ..., SDL, OpenGL, ...)

Kde lze využít polymorfismus?

* objekty k vykreslení: kruh, trojúhelník, obdélník,...
* dekodér vstupních dat: příkazová řádka, (vlastní) textový formát, XML, ...
* enkodér výstupních dat: ASCII (různé druhy), BMP, SVG, ..., SDL, OpenGL, (výstup můžete zobrazit přímo na obrazovku) ...
* nástroje k vykreslení: stínování, vyplnění objektu barvou, ...

Další informace:

* https://www.root.cz/clanky/graficky-format-bmp-pouzivany-a-pritom-neoblibeny/
* http://www.vzhurudolu.cz/prirucka/svg
* http://www.onicos.com/staff/iz/formats/gif.html
* Možno implementovat i sekvenci obrázků (video)

## Project goals

The project goal is to implement following features:

* all mandatory points in the topic
* real-time user interface for drawing in SDL

Checklist:

_Načíst ze souboru a z konzole definici jednotlivých objektů v prostoru (velikost obrázku, pozice a parametry objektů, ...)._

* **supported** - possible loading from text file or stdin

_Program podporuje minimálně čáru, polyline, kruh, elipsu, obdélník a skupinu objektů._

* **supported** - polyline, ellipse, circle, rectangle, square

_Objektům se dají nastavovat různé barvy (v případě ASCII styl vykreslování) a výplně._

* **supported** - various colors and various fill styles

_Obrázek je nutné reprezentovat jako graf scény - stromovou strukturu (případně DAG)_

* **supported** - objects are stored in tree structure

_Skupina objektů funguje jako vzor, tudíž po vytvoření skupiny objektů můžu tuto skupinu vložit několikrát do obrázku na různá místa._

* **supported** - possible of creating groups, subgroups, group copying (in interactive user interface) and creating group links

_Implementujte alespoň 2 různé výstupní formáty (ASCII, BMP, SVG, ..., SDL, OpenGL, ...)_

* **supported** - output to screen (SDL), output to bitmap (BMP), output/input object tree structure to text file


## Used polymorphism

Used in the following cases:

* graphical object hierarchy and representation: DrawNode, DrawEntity, DrawEllipse, DrawRectangle, DrawCircle, DrawSquare
* draw abstraction: GraphicPrinter, GraphicPrinterSDL
* various menu item style and function: MenuItem, MenuItemTitle
* interfaces: IBorder, IMouse, IRenderer

## Implemented features

* graphics objects DrawEntity, DrawEllipse, DrawRectangle, DrawCircle, DrawSquare
* possible object manipulation like position, color and fill styles
* object hierarchy and organization into groups in DrawNode
* simple ray collision detection for object selection and filling in Point
* fill algorithm of non-convex polygon in DrawFiller
* Menu component for displaying the user interface
* user interface logic for drawing and editing in UserControl, UserDrawHelp
* simple draw abstraction for extending to various output sources in GraphicPrinter
* scene text format parser and loader in SceneParser
* save logic to text format + some text format features
* graphic output to a bitmap in the file (BMP)
* make copies and shortcuts of existing groups of objects
* support of console input stream of text format (let the program work as converter)

## Usage

Run program on scene in interactive edit mode:

~~~~~~~~~~~~~
./hrabajak scene_name edit
./hrabajak examples/locomotive.txt edit
./hrabajak examples/figures.txt edit
~~~~~~~~~~~~~

Save scene to another file:

~~~~~~~~~~~~~
./hrabajak scene_name save output.txt
~~~~~~~~~~~~~

Save scene to bitmap (bmp):

~~~~~~~~~~~~~
./hrabajak scene_name save output.bmp
~~~~~~~~~~~~~

Resave opened scene as text or bmp:

~~~~~~~~~~~~~
./hrabajak scene_name save
./hrabajak scene_name save_bmp
~~~~~~~~~~~~~

Load scene from stdin (must be specified scene name) and save:

~~~~~~~~~~~~~
echo "rectangle(10, 10, 100, 60, green)" | ./hrabajak scene_name stdin save scene_name.bmp
~~~~~~~~~~~~~

Load scene from stdin (must be specified scene name) and run interactive edit mode:

~~~~~~~~~~~~~
echo "ellipse(10, 10, 50, 120, red)" | ./hrabajak scene_name stdin edit
~~~~~~~~~~~~~

Run some tests:

~~~~~~~~~~~~~
./hrabajak test
~~~~~~~~~~~~~

Perform tests with valgrind:

~~~~~~~~~~~~~
valgrind -v --leak-check=full --show-reachable=yes --track-origins=yes ./hrabajak test
~~~~~~~~~~~~~

## Make

Supported make commands:

* **make all** / **make compile** - build the program
* **make run** - run the program (requires graphical environment)
* **make clean** - clean build cache (objects)
* **make doc** - create documentation

## Dependencies

* lSDL_main - core SDL library
* lSDL - core SDL library
* lSDL_ttf - font manipulation for SDL
* lSDL_image - texture / image library (dependency for SDL_ttf)

As standard packages: libsdl2-dev, libsdl2-image-dev, libsdl2-ttf-dev

## Project size

Last size output from cloc tool:

~~~~~~~~~~~~~
Language                     files          blank        comment           code
C++                             31           1402            243           3275
C/C++ Header                    33            287            101            830
make                             1              5              0             15
SUM:                            65           1694            344           4120
~~~~~~~~~~~~~
