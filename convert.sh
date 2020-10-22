#!/bin/bash

# This script runs chartmaker with the specified arguments, converts it to a jgraph, 
# and converts the jgraph to the specified format.

if [ "$#" -le 4 ]; then
    echo "usage: convert.sh <input file name> <output file name> <ppm/pgm/pbm/pdf> <args>"
    exit
fi

OUTPUT=$2
TYPE=$3
INPUT=$1

shift 3

./bin/chartmaker $INPUT p $@ > temp

if [ $TYPE == "pdf" ]; then
    ./jgraph/jgraph -P temp | ps2pdf - $OUTPUT 
elif [ $TYPE == "ppm" ]; then
    # ./jgraph/jgraph temp | 
    echo "$TYPE not yet implemented"
elif [ $TYPE == "pgm" ]; then
    echo "$TYPE not yet implemented"
elif [ $TYPE == "pbm" ]; then
    echo "$TYPE not yet implemented"
fi;
