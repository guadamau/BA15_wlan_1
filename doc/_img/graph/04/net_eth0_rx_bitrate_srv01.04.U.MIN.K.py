#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/net_eth0_rx_bitrate_srv01.04.U.MIN.K.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_A" using 1:2:0 title "04.U.MIN.K.A" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_9" using 1:2:0 title "04.U.MIN.K.9" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_8" using 1:2:0 title "04.U.MIN.K.8" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_7" using 1:2:0 title "04.U.MIN.K.7" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_6" using 1:2:0 title "04.U.MIN.K.6" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_5" using 1:2:0 title "04.U.MIN.K.5" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_4" using 1:2:0 title "04.U.MIN.K.4" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_3" using 1:2:0 title "04.U.MIN.K.3" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_2" using 1:2:0 title "04.U.MIN.K.2" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv01_04.U.MIN.K_net_eth0_rx_bitrate_1" using 1:2:0 title "04.U.MIN.K.1" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
