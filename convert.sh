#!/bin/bash

# This script runs chartmaker with the specified arguments, converts it to a jgraph, 
# and converts the jgraph to the specified format.

if [ "$#" -lt 2 ]; then
    echo "usage: convert.sh <input file name> <output file name (with extension)> <args>"
    exit
fi

INPUT=$1
OUTPUT=$2
TYPE=${OUTPUT##*.}

shift 2

./bin/chartmaker $INPUT p $@ > temp

if [ $TYPE == "pdf" ]; then
    ./jgraph/jgraph -P temp | ps2pdf - $OUTPUT 
else
    ./jgraph/jgraph -P temp | ps2pdf - | convert -density 300 - -quality 100 $OUTPUT
fi;
