#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/06/net_prp1_tx_bitrate_srv02.06.U.MAX.U.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/06/srv02_06.U.MAX.U_net_prp1_tx_bitrate_D" using 1:2:0 title "06.U.MAX.U.D" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/06/srv02_06.U.MAX.U_net_prp1_tx_bitrate_C" using 1:2:0 title "06.U.MAX.U.C" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/06/srv02_06.U.MAX.U_net_prp1_tx_bitrate_B" using 1:2:0 title "06.U.MAX.U.B" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/06/srv02_06.U.MAX.U_net_prp1_tx_bitrate_A" using 1:2:0 title "06.U.MAX.U.A" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \