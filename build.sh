#!/bin/bash
git submodule init
git submodule update
cp -rp buildroot_overlay/* buildroot/
cd buildroot
make emb_timerelay_defconfig
make -j 4
mkdir ../qt_app/build
cd ../qt_app/build
../../buildroot/output/build/qt5base*/bin/qmake ../ CONFIG+=RELEASE
make -j 4
