#!/bin/bash

cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Release && \
make -C ./build -j4 
