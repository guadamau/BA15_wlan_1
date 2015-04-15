#!/bin/bash

echo "#============================================================================"
echo "# BA15_wlan_1"
echo "#============================================================================"
echo ""
echo "-> Create files for usage as histogram-data in LyX-documentation"
echo ""
echo "*****************************************************************************"

if [ -z "$1" ]
then
    echo "Usage: $0 meas_interval_file"
    echo "USE ONLY OUTPUT INTERVAL FILE FROM MEAS!"
    echo "*****************************************************************************"
    exit
fi

PRP_NAME=$(cat test/meas_interval_results_1428888790 | grep "prp (" | head -n 1 | cut -d' ' -f3 | cut -d'(' -f2 | cut -d')' -f1)
IF0_NAME=$(cat test/meas_interval_results_1428888790 | grep "if0 (" | head -n 1 | cut -d' ' -f3 | cut -d'(' -f2 | cut -d')' -f1)
IF1_NAME=$(cat test/meas_interval_results_1428888790 | grep "if1 (" | head -n 1 | cut -d' ' -f3 | cut -d'(' -f2 | cut -d')' -f1)

OUTPUT_FOLDER="./out"
CPU_LOAD_PER_INT="cpu_load_per_interval.hist"
USER_TIME_PER_INT="cpu_usertime_per_interval.hist"
SYSTEM_TIME_PER_INT="cpu_systemtime_per_interval.hist"
PRP_RX_BYTES_PER_INT="net_${PRP_NAME}_rx_bytes_per_interval.hist"
PRP_TX_BYTES_PER_INT="net_${PRP_NAME}_tx_bytes_per_interval.hist"
PRP_RX_BITRATE_PER_INT="net_${PRP_NAME}_rx_bitrate_per_interval.hist"
PRP_TX_BITRATE_PER_INT="net_${PRP_NAME}_tx_bitrate_per_interval.hist"
IF0_RX_BYTES_PER_INT="net_${IF0_NAME}_rx_bytes_per_interval.hist"
IF0_TX_BYTES_PER_INT="net_${IF0_NAME}_tx_bytes_per_interval.hist"
IF0_RX_BITRATE_PER_INT="net_${IF0_NAME}_rx_bitrate_per_interval.hist"
IF0_TX_BITRATE_PER_INT="net_${IF0_NAME}_tx_bitrate_per_interval.hist"
IF1_RX_BYTES_PER_INT="net_${IF1_NAME}_rx_bytes_per_interval.hist"
IF1_TX_BYTES_PER_INT="net_${IF1_NAME}_tx_bytes_per_interval.hist"
IF1_RX_BITRATE_PER_INT="net_${IF1_NAME}_rx_bitrate_per_interval.hist"
IF1_TX_BITRATE_PER_INT="net_${IF1_NAME}_tx_bitrate_per_interval.hist"

echo -n "Parsing CPU-load per interval to ${OUTPUT_FOLDER}/${CPU_LOAD_PER_INT}..."
cat $1 | grep CPU | cut -d' ' -f6 | cut -d'%' -f1 > ${OUTPUT_FOLDER}/${CPU_LOAD_PER_INT}
echo "done"

echo -n "Parsing CPU-usertime per interval to ${OUTPUT_FOLDER}/${USER_TIME_PER_INT}..."
cat $1 | grep "user" | cut -d' ' -f6 | cut -d's' -f1  > ${OUTPUT_FOLDER}/${USER_TIME_PER_INT}
echo "done"

echo -n "Parsing CPU-systemtime per interval to ${OUTPUT_FOLDER}/${SYSTEM_TIME_PER_INT}..."
cat $1 | grep "sys" | cut -d' ' -f8 | cut -d's' -f1 > ${OUTPUT_FOLDER}/${SYSTEM_TIME_PER_INT}
echo "done"

echo -n "Parsing ${PRP_NAME} - RX Bytes per interval to ${OUTPUT_FOLDER}/${PRP_RX_BYTES_PER_INT}..."
cat $1 | grep "prp" | cut -d' ' -f10 > ${OUTPUT_FOLDER}/${PRP_RX_BYTES_PER_INT}
echo "done"

echo -n "Parsing ${PRP_NAME} - TX Bytes per interval to ${OUTPUT_FOLDER}/${PRP_TX_BYTES_PER_INT}..."
cat $1 | grep "prp" | cut -d' ' -f16 > ${OUTPUT_FOLDER}/${PRP_TX_BYTES_PER_INT}
echo "done"

echo -n "Parsing ${PRP_NAME} - RX Bitrate per interval to ${OUTPUT_FOLDER}/${PRP_RX_BITRATE_PER_INT}..."
cat $1 | grep "prp" | cut -d' ' -f30 > ${OUTPUT_FOLDER}/${PRP_RX_BITRATE_PER_INT}
echo "done"

echo -n "Parsing ${PRP_NAME} - TX Bitrate per interval to ${OUTPUT_FOLDER}/${PRP_TX_BITRATE_PER_INT}..."
cat $1 | grep "prp" | cut -d' ' -f43 > ${OUTPUT_FOLDER}/${PRP_TX_BITRATE_PER_INT}
echo "done"

echo -n "Parsing ${IF0_NAME} - RX Bytes per interval to ${OUTPUT_FOLDER}/${IF0_RX_BYTES_PER_INT}..."
cat $1 | grep "if0" | cut -d' ' -f10 > ${OUTPUT_FOLDER}/${IF0_RX_BYTES_PER_INT}
echo "done"

echo -n "Parsing ${IF0_NAME} - TX Bytes per interval per interval to ${OUTPUT_FOLDER}/${IF0_TX_BYTES_PER_INT}..."
cat $1 | grep "if0" | cut -d' ' -f16 > ${OUTPUT_FOLDER}/${IF0_TX_BYTES_PER_INT}
echo "done"

echo -n "Parsing ${IF0_NAME} - RX Bitrate per interval to ${OUTPUT_FOLDER}/${IF0_RX_BITRATE_PER_INT}..."
cat $1 | grep "if0" | cut -d' ' -f30 > ${OUTPUT_FOLDER}/${IF0_RX_BITRATE_PER_INT}
echo "done"

echo -n "Parsing ${IF0_NAME} - TX Bitrate per interval to ${OUTPUT_FOLDER}/${IF0_TX_BITRATE_PER_INT}..."
cat $1 | grep "if0" | cut -d' ' -f43 > ${OUTPUT_FOLDER}/${IF0_TX_BITRATE_PER_INT}
echo "done"

echo -n "Parsing ${IF1_NAME} - RX Bytes per interval to ${OUTPUT_FOLDER}/${IF1_RX_BYTES_PER_INT}..."
cat $1 | grep "if1" | cut -d' ' -f10 > ${OUTPUT_FOLDER}/${IF1_RX_BYTES_PER_INT}
echo "done"

echo -n "Parsing ${IF1_NAME} - TX Bytes per interval per interval to ${OUTPUT_FOLDER}/${IF1_TX_BYTES_PER_INT}..."
cat $1 | grep "if1" | cut -d' ' -f16 > ${OUTPUT_FOLDER}/${IF1_TX_BYTES_PER_INT}
echo "done"

echo -n "Parsing ${IF1_NAME} - RX Bitrate per interval to ${OUTPUT_FOLDER}/${IF1_RX_BITRATE_PER_INT}..."
cat $1 | grep "if1" | cut -d' ' -f30 > ${OUTPUT_FOLDER}/${IF1_RX_BITRATE_PER_INT}
echo "done"

echo -n "Parsing ${IF1_NAME} - TX Bitrate per interval to ${OUTPUT_FOLDER}/${IF1_TX_BITRATE_PER_INT}..."
cat $1 | grep "if1" | cut -d' ' -f43 > ${OUTPUT_FOLDER}/${IF1_TX_BITRATE_PER_INT}
echo "done"

echo "*****************************************************************************"
