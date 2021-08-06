#!/bin/bash

TOPDIR=$(git rev-parse --show-toplevel)

INSTALL_ROOT_DIR=""
if [[ -d $1 ]]; then
    INSTALL_ROOT_DIR=$1
fi

sudo cp -v "$TOPDIR/build/MidiEmBackend" $INSTALL_ROOT_DIR/usr/bin/
sudo cp -v -r "$TOPDIR/MidiConfigs" $INSTALL_ROOT_DIR/etc
sudo cp -v "$TOPDIR/systemd/midiem-backend.service" $INSTALL_ROOT_DIR/usr/lib/systemd/user