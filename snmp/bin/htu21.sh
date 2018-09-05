#!/bin/bash
#echo  'Temp 23.64 C;Humidity 40.26%'
"/opt/orangepizero-temperature/htu21-sensoror/build/htu21-orangepi-test.elf"

#Find the script base path (used to find mapping file)
#SCRIPT_PATH="`dirname \"$0\"`"              # relative
#SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"  # absolutized and normalized
#if [ -z "$SCRIPT_PATH" ] ; then
  # error; for some reason, the path is not accessible
  # to the script (e.g. permissions re-evaled after suid)
#  exit 1  # fail
#fi

#if [ "$1" = "-g" ]; then
	#deviceid=$(<$SCRIPT_PATH/../mapping/$2)
