#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_err/net_eth1_rx_bitrate_srv02.03.T.MAX.U.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_err/srv02_03.T.MAX.U_net_eth1_rx_bitrate_X" using 1:2:0 title "03.T.MAX.U.X" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_err/srv02_03.T.MAX.U_net_eth1_rx_bitrate_W" using 1:2:0 title "03.T.MAX.U.W" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_err/srv02_03.T.MAX.U_net_eth1_rx_bitrate_V" using 1:2:0 title "03.T.MAX.U.V" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_err/srv02_03.T.MAX.U_net_eth1_rx_bitrate_U" using 1:2:0 title "03.T.MAX.U.U" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
