#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
echo $DIR
sync_results.sh fresh
rm -rfv ~/BA15_wlan_1/doc/_tex/0*
${DIR}/textablegen.sh ~/BA15_wlan_1/doc/_tex
${DIR}/meas_interval_parse/parsefolder.sh ~/BA_result_inbox
rm -rfv ${HOME}/BA15_wlan_1/sw/scripts/graph/0*
${DIR}/graph/copyresultstograph.sh
cp -rv ${HOME}/BA15_wlan_1/sw/scripts/graph/0* ${HOME}/BA15_wlan_1/doc/_img/graph/
