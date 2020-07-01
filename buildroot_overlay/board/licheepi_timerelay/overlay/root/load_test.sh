#!/bin/sh

x=0

while [ $x -le $1 ]
do	
	echo $x
	x=$(( $x + 1 ))
done
