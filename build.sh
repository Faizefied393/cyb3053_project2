#!/usr/bin/env bash
set -e
mkdir -p build
cd build
cmake ..
make
./cyb3053_project2
