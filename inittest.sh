#!/bin/bash


mkdir -p poze
rm -rf poze/*

for i in {1..100}
do
    cp audi.jpg poze/car$i.jpeg
    sleep 0.01
done
