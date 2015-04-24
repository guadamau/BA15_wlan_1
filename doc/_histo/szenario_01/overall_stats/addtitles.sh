#!/bin/bash

for i in $(find . | grep "meas_ove"); do
    INT_FILE_NAME=$(echo "$i" | rev | cut -d'/' -f1 | rev)
    INT_FILE=$i
    DIR_NAME=$(dirname $i)
    DIR_OUT=$(echo ${DIR_NAME}/out/${INT_FILE_NAME})
    sed -i -e '1i'${DIR_NAME}'\' $i
done
