#!/bin/bash
echo "#============================================================================"
echo "# BA15_wlan_1"
echo "#============================================================================"
echo ""
echo "-> Create histogramfiles for all interval results in a specific folder"
echo "   If the files are in /tmp/results/meas, /tmp as parameter would be ok"
echo ""
echo "*****************************************************************************"

if [ -z "$1" ]
then
    echo "Usage: $0 FOLDER_WHERE_RESULTS_ARE"
    echo "*****************************************************************************"
    exit
fi

SCRIPT_DIR=$(dirname $0)
echo $SCRIPT_DIR

for i in $(find $1 | grep "meas_interval" | sort); do
    INT_FILE_NAME=$(echo "$i" | rev | cut -d'/' -f1 | rev)
    INT_FILE=$i
    DIR_NAME=$(dirname $i)
    DIR_OUT=$(echo ${DIR_NAME}/out/${INT_FILE_NAME})
    echo "-------------------${INT_FILE_NAME}"
    echo "Interval result: ${INT_FILE}"
    echo "Creating histogram files in ${DIR_OUT}"
    mkdir -p ${DIR_OUT}
    ${SCRIPT_DIR}/include/histogram.sh ${INT_FILE} ${DIR_OUT} $(dirname ${INT_FILE} | rev | cut -d'/' -f1 | rev) $1
done
