#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/net_eth1_tx_bitrate_srv01.07.T.MAX.K.svg"
set ylabel "Bitrate [MBit/s]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key outside right center box
unset colorbox
set palette file '/home/guadagnini/BA15_wlan_1/sw/scripts/graph/sst.gpf'
plot n=0, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/srv01_07.T.MAX.K_net_eth1_tx_bitrate_T" using 1:2:0 title "07.T.MAX.K.T" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/srv01_07.T.MAX.K_net_eth1_tx_bitrate_S" using 1:2:0 title "07.T.MAX.K.S" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/srv01_07.T.MAX.K_net_eth1_tx_bitrate_R" using 1:2:0 title "07.T.MAX.K.R" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \