#!/bin/bash

TOP_DIR=$(git rev-parse --show-toplevel)

TARGET_IP=$1

USER="root"
SERVICE="nomidi-backend"

ssh ${USER}@${TARGET_IP} "journalctl -b -f -u ${SERVICE}"