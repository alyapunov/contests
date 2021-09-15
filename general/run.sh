#!/bin/bash

make -j

FILES="./data*.txt"
for f in $FILES; do
    #echo "./a.out \"$f\""
    ./a.out "$f"
done