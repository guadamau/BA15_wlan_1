#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_pkt/cpu_load_srv01.03.U.MAX.K.svg"
set ylabel "CPU-Last [%]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key outside right center box
unset colorbox
set palette file '/home/guadagnini/BA15_wlan_1/sw/scripts/graph/sst.gpf'
plot n=0, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_pkt/srv01_03.U.MAX.K_cpu_load_S" using 1:2:0 title "03.U.MAX.K.S" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_pkt/srv01_03.U.MAX.K_cpu_load_R" using 1:2:0 title "03.U.MAX.K.R" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_pkt/srv01_03.U.MAX.K_cpu_load_Q" using 1:2:0 title "03.U.MAX.K.Q" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03_pkt/srv01_03.U.MAX.K_cpu_load_P" using 1:2:0 title "03.U.MAX.K.P" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \