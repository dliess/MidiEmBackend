#!/bin/bash

TOPDIR=$(git rev-parse --show-toplevel)

sudo cp "$TOPDIR/build/MidiEmBackendBackend" /usr/bin/
sudo cp -r "$TOPDIR/MidiConfigs" /etc/MidiConfigs
sudo cp "$TOPDIR/systemd/midiem-backend.service" /usr/lib/systemd/user