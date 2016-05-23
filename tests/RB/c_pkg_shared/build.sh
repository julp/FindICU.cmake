#!/usr/bin/env bash

declare -r __DIR__=$(dirname $(readlink -f "${BASH_SOURCE}"))

declare -r INSTALL_DIR=/tmp/
declare -r BUILD_DIR=/tmp/build/

mkdir -p $BUILD_DIR

rm -fr $BUILD_DIR/* $INSTALL_DIR/bin/*
cmake -H${__DIR__} -B$BUILD_DIR -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make -C $BUILD_DIR
make -C $BUILD_DIR install
