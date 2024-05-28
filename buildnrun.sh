#!/bin/bash

mkdir -p build
rm -rf build/*
cd build
cmake ..
make


cp -r ../frontend/build ./static
mv ./static/static/* ./static

./server