#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/cpu_load_srv02.04.T.MAX.U.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_A" using 1:2:0 title "04.T.MAX.U.A" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_9" using 1:2:0 title "04.T.MAX.U.9" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_8" using 1:2:0 title "04.T.MAX.U.8" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_7" using 1:2:0 title "04.T.MAX.U.7" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_6" using 1:2:0 title "04.T.MAX.U.6" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_5" using 1:2:0 title "04.T.MAX.U.5" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_4" using 1:2:0 title "04.T.MAX.U.4" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_3" using 1:2:0 title "04.T.MAX.U.3" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_2" using 1:2:0 title "04.T.MAX.U.2" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/04/srv02_04.T.MAX.U_cpu_load_1" using 1:2:0 title "04.T.MAX.U.1" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \
