#!/bin/bash

clear

echo "" > out.txt 
echo "" > err.txt

rm -rf pics/*

for i in {1..10}
do
    ./build/client --image ./build/car.jpeg --resize 500x500 --output pics/new$i.jpeg >> out.txt 2>> err.txt &
    sleep 0.1
done

exit 0