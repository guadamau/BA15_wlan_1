#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/net_eth0_rx_bitrate_srv01.04.T.MIN.K.svg"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key outside right center box
plot \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_9" using 1:2 title "04.T.MIN.K.9" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_8" using 1:2 title "04.T.MIN.K.8" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_7" using 1:2 title "04.T.MIN.K.7" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_6" using 1:2 title "04.T.MIN.K.6" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_5" using 1:2 title "04.T.MIN.K.5" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_4" using 1:2 title "04.T.MIN.K.4" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_3" using 1:2 title "04.T.MIN.K.3" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_2" using 1:2 title "04.T.MIN.K.2" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.K_net_eth0_rx_bitrate_0" using 1:2 title "04.T.MIN.K.0" lw 3 with lines, \
