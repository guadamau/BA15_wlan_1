#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03/cpu_load_srv01.03.T.MIN.U.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03/srv01_03.T.MIN.U_cpu_load_E" using 1:2:0 title "03.T.MIN.U.E" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03/srv01_03.T.MIN.U_cpu_load_D" using 1:2:0 title "03.T.MIN.U.D" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03/srv01_03.T.MIN.U_cpu_load_C" using 1:2:0 title "03.T.MIN.U.C" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03/srv01_03.T.MIN.U_cpu_load_B" using 1:2:0 title "03.T.MIN.U.B" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/03/srv01_03.T.MIN.U_cpu_load_A" using 1:2:0 title "03.T.MIN.U.A" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \