#!/bin/sh

str="::respawn:/root/qt_app -c /root/qt_app.ini"
file="$TARGET_DIR/etc/inittab"

if ! grep -q $str $file; then
    echo $str >> $file
fi
