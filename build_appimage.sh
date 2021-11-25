#!/bin/bash

export `cat VERSION`
make clean
rm .qmake.stash
qmake
make -j
rm -Rf AppImage
mkdir AppImage
cp LithoMaker AppImage/
cp -a examples AppImage/
cp AUTHORS AppImage/
cp LICENSE AppImage/
cp README.md AppImage/
cp default.png AppImage/
cp default.desktop AppImage/
../linuxdeployqt-continuous-x86_64.AppImage AppImage/LithoMaker -appimage
