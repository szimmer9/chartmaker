#!/bin/bash

# This script runs chartmaker with the specified arguments, converts it to a jgraph, 
# and converts the jgraph to the specified format.

# Currently only works for PPM->PDF conversion

if [ "$#" -ne 2 ]; then
    echo "usage: convert.sh <input file name> <output file name>"
    exit
fi

./bin/chartmaker $1 p > temp
./jgraph/jgraph -P temp | ps2pdf - $2

