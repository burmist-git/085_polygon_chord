#!/bin/bash

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d  : default"
    echo " -c  : compile"
}

if [ $# -eq 0 ]; then
    printHelp
else
    if [ "$1" = "-d" ]; then
	./fourier_series
    elif [ "$1" = "-c" ]; then
	make clean ; make;
    elif [ "$1" = "-h" ]; then
	printHelp
    else
        printHelp
    fi
fi
