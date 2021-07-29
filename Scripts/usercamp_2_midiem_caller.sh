#!/bin/bash

#Start from repo_dir/..

IFS=$'\n'
for file in $(find . -name "*.csv")
do
   echo $file
   DIRNAME=$(dirname $file)
   BASENAME=$(basename $file)
   DEVICE_DIR_NAME="${BASENAME%.*}"
   DEST_DIR=MidiEmBackend/MidiConfigs/ExternalStaging/$DIRNAME/$DEVICE_DIR_NAME
   mkdir -p $DEST_DIR
   CONV_SCRIPT=MidiEmBackend/Scripts/usercamp_2_midiem.py
   $CONV_SCRIPT $file > $DEST_DIR/Config.json

done
