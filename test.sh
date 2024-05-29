#!/bin/bash

clear

mkdir -p poze
rm -rf poze/*

for i in {1..100}
do
    cp car.jpeg poze/car$i.jpeg
done

mkdir -p poze_jmk
rm -rf poze_jmk/*

for i in {1..99}
do
    ./build/client --image poze/car$i.jpeg --resize 500x500 --output poze_jmk/new$i.jpeg &
done

exit 0