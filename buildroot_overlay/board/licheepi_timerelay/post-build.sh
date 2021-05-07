#!/bin/sh

str='::respawn:/bin/sh -c "cd /root; exec /root/qt_app -c /root/qt_app.ini"'
file="$TARGET_DIR/etc/inittab"

if ! grep -q $str $file; then
    echo $str >> $file
fi
