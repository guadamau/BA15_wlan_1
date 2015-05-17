#!/bin/bash
if [ -z $1 ]
then
    echo "usage: $0 PDFFILE.pdf"
    echo "will create PDFFILE.1.pdf in same directory as PDFFILE.pdf"
    exit
fi

DIR=$(dirname $i)
FILENAME=$(echo $i | rev | cut -c 4- | rev)
gs -o ${FILENAME}.ps -dNOCACHE -sDEVICE=ps2write $1
ps2pdf13 ${FILENAME}.ps ${FILENAME}.1.pdf
rm ${FILENAME}.ps
