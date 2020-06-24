#!/bin/sh
if [ "$2" != "" ]; then
	echo $1 > "/sys/class/gpio/gpio$2/value"
	exit 1
fi

for i in "36" "37" "38" "39" "64" "65" "66" "67" ; do
	echo $i > /sys/class/gpio/export
	echo "out" > "/sys/class/gpio/gpio${i}/direction"
	echo $1 > "/sys/class/gpio/gpio${i}/value"
done
