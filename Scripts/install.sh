#!/bin/bash

TOPDIR=$(git rev-parse --show-toplevel)

sudo cp -v "$TOPDIR/build/MidiEmBackend" /usr/bin/
sudo cp -v -r "$TOPDIR/MidiConfigs" /etc
sudo cp -v "$TOPDIR/systemd/midiem-backend.service" /usr/lib/systemd/user