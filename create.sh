#!/bin/bash
rm -rf createOutput
mkdir createOutput
mkdir createOutput/hrabajak
mkdir createOutput/hrabajak/src
mkdir createOutput/hrabajak/doc
cp Makefile Doxyfile prohlaseni.txt zadani.txt createOutput/hrabajak
cp -rf examples createOutput/hrabajak
cp -rf doc createOutput/hrabajak
cp README.md createOutput/hrabajak/doc
cp src/*.cpp createOutput/hrabajak/src
cp src/*.h createOutput/hrabajak/src
