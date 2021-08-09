#!/bin/bash

TOPDIR=$(git rev-parse --show-toplevel)

INSTALL_ROOT_DIR=""
SUDO="sudo"
if [[ -d $1 ]]; then
    INSTALL_ROOT_DIR=$1
    SUDO=""
fi

$SUDO cp -v "$TOPDIR/build/MidiEmBackend" $INSTALL_ROOT_DIR/usr/bin/
$SUDO mkdir -p $INSTALL_ROOT_DIR/etc/midiem
$SUDO cp -v -r "$TOPDIR/MidiConfigs" $INSTALL_ROOT_DIR/etc/midiem
$SUDO cp -v "$TOPDIR/systemd/midiem-backend.service" $INSTALL_ROOT_DIR/usr/lib/systemd/user