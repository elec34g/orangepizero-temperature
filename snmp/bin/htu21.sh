#!/bin/bash

#Find the script base path (used to find mapping file)
SCRIPT_PATH="`dirname \"$0\"`"              # relative
SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"  # absolutized and normalized
if [ -z "$SCRIPT_PATH" ] ; then
  # error; for some reason, the path is not accessible
  # to the script (e.g. permissions re-evaled after suid)
  exit 1  # fail
fi

if [ "$1" = "-t" ]; then
	#deviceid=$(<$SCRIPT_PATH/../mapping/$2
	/opt/orangepizero-temperature/htu21-sensor/build/htu21-orangepi-test.elf "t"
else
	/opt/orangepizero-temperature/htu21-sensor/build/htu21-orangepi-test.elf "h"
fi
