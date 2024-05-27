#!/bin/bash

clear

for i in {1..10}
do
    ./build/client --image ./build/car.jpeg --resize 500x500 --output new.jpeg &
done

exit 0