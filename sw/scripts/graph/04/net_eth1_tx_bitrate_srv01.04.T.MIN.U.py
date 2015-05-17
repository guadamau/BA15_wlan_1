#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/net_eth1_tx_bitrate_srv01.04.T.MIN.U.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_A" using 1:2:0 title "04.T.MIN.U.A" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_9" using 1:2:0 title "04.T.MIN.U.9" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_8" using 1:2:0 title "04.T.MIN.U.8" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_7" using 1:2:0 title "04.T.MIN.U.7" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_6" using 1:2:0 title "04.T.MIN.U.6" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_5" using 1:2:0 title "04.T.MIN.U.5" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_4" using 1:2:0 title "04.T.MIN.U.4" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_3" using 1:2:0 title "04.T.MIN.U.3" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_2" using 1:2:0 title "04.T.MIN.U.2" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.T.MIN.U_net_eth1_tx_bitrate_1" using 1:2:0 title "04.T.MIN.U.1" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
