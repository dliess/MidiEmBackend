#!/bin/bash 

TOP_DIR=$(git rev-parse --show-toplevel)
BUILD_DIR_STUB=$1
BUILD_TYPE=$2

BUILD_DIR=${BUILD_DIR_STUB}_${BUILD_TYPE}
mkdir -p $TOP_DIR/$BUILD_DIR
INSTALL_DIR="$BUILD_DIR/_INSTALL"

SDK_DIR="/opt/sdk/nomidi/0.0.1"

source $SDK_DIR/environment-setup-cortexa72-poky-linux
cmake -S $TOP_DIR \
      -B $TOP_DIR/$BUILD_DIR \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_TOOLCHAIN_FILE=$SDK_DIR/sysroots/x86_64-pokysdk-linux/usr/share/cmake/OEToolchainConfig.cmake \
      -DCPM_LOCAL_PACKAGES_ONLY=ON \
      -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR

cmake --build $TOP_DIR/$BUILD_DIR --target install -j8