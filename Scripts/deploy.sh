#!/bin/bash

TOP_DIR=$(git rev-parse --show-toplevel)

BUILD_DIR_STUB=$1
BUILD_TYPE=$2
TARGET_IP=$3

BUILD_DIR=${BUILD_DIR_STUB}_${BUILD_TYPE}

USER="root"
SERVICE="nomidi-backend"

echo " >>> stop service"
ssh ${USER}@${TARGET_IP} "systemctl stop ${SERVICE}"
echo " >>> copy files from ${TOP_DIR}/${BUILD_DIR}/_INSTALL"
scp -r ${TOP_DIR}/${BUILD_DIR}/_INSTALL/bin/NoMidiBackend ${USER}@${TARGET_IP}:/usr/bin/
echo " >>> daemon-reload"
ssh ${USER}@${TARGET_IP} "systemctl daemon-reload"
echo " >>> start service"
ssh ${USER}@${TARGET_IP} "systemctl start ${SERVICE}"