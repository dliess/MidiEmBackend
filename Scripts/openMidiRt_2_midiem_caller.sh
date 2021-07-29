#!/bin/bash

#Start from repo_dir/..

JSON_SOURCE_DIR="open-midi-rtc-specs/specs/0.0.1/json/"

IFS=$'\n'
for file in $(find $JSON_SOURCE_DIR -name "*.json")
do
   echo $file
   DIRNAME=$(dirname $file)
   BASENAME=$(basename $file)
   DEVICE_DIR_NAME="${BASENAME%.*}"
   DEST_DIR=MidiEmBackend/MidiConfigs/ExternalStaging/OpenMidiRt/$DEVICE_DIR_NAME
   mkdir -p $DEST_DIR
   CONV_SCRIPT=MidiEmBackend/Scripts/openMidiRt_2_midiem.py
   $CONV_SCRIPT $file > $DEST_DIR/Config.json

done
