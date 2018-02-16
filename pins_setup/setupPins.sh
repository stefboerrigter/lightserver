#!/bin/sh
echo 60 > /sys/class/gpio/export
echo 48 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio60/direction
echo "out" > /sys/class/gpio/gpio48/direction
