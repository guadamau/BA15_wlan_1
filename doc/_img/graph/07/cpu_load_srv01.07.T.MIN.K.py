#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/cpu_load_srv01.07.T.MIN.K.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/srv01_07.T.MIN.K_cpu_load_T" using 1:2:0 title "07.T.MIN.K.T" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/srv01_07.T.MIN.K_cpu_load_S" using 1:2:0 title "07.T.MIN.K.S" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/07/srv01_07.T.MIN.K_cpu_load_R" using 1:2:0 title "07.T.MIN.K.R" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
