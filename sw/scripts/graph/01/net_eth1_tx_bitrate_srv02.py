#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/net_eth1_tx_bitrate_srv02.svg"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key outside right center box
plot \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MIN.U_net_eth1_tx_bitrate" using 1:2 title "01.U.MIN.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MIN.K_net_eth1_tx_bitrate" using 1:2 title "01.U.MIN.K" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MAX.U_net_eth1_tx_bitrate" using 1:2 title "01.U.MAX.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MAX.K_net_eth1_tx_bitrate" using 1:2 title "01.U.MAX.K" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MIN.U_net_eth1_tx_bitrate" using 1:2 title "01.T.MIN.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MIN.K_net_eth1_tx_bitrate" using 1:2 title "01.T.MIN.K" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MAX.U_net_eth1_tx_bitrate" using 1:2 title "01.T.MAX.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MAX.K_net_eth1_tx_bitrate" using 1:2 title "01.T.MAX.K" lw 3 with lines, \