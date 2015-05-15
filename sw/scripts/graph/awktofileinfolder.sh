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

for i in $(find $1 | sort); do
    INT_FILE_NAME=$(echo "$i" | rev | cut -d'/' -f1 | rev)
    INT_FILE=$i
    DIR_NAME=$(dirname $i)
    DIR_OUT=$(echo ${DIR_NAME}/out/${INT_FILE_NAME})
    awk '{printf("%010f %s\n", NR, $0)}' ${INT_FILE} > "${INT_FILE}.awk"
    rm ${INT_FILE}
    mv "${INT_FILE}.awk" ${INT_FILE}
done
