#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/net_eth0_rx_bitrate_srv01.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.U.MIN.U_net_eth0_rx_bitrate" using 1:2:0 title "01.U.MIN.U" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.U.MIN.K_net_eth0_rx_bitrate" using 1:2:0 title "01.U.MIN.K" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.U.MAX.U_net_eth0_rx_bitrate" using 1:2:0 title "01.U.MAX.U" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.U.MAX.K_net_eth0_rx_bitrate" using 1:2:0 title "01.U.MAX.K" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.T.MIN.U_net_eth0_rx_bitrate" using 1:2:0 title "01.T.MIN.U" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.T.MIN.K_net_eth0_rx_bitrate" using 1:2:0 title "01.T.MIN.K" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.T.MAX.U_net_eth0_rx_bitrate" using 1:2:0 title "01.T.MAX.U" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv01_01.T.MAX.K_net_eth0_rx_bitrate" using 1:2:0 title "01.T.MAX.K" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
