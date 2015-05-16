#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/cpu_load_srv02.svg"
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
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MIN.U_cpu_load" using 1:2:0 title "01.U.MIN.U" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MIN.K_cpu_load" using 1:2:0 title "01.U.MIN.K" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MAX.U_cpu_load" using 1:2:0 title "01.U.MAX.U" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.U.MAX.K_cpu_load" using 1:2:0 title "01.U.MAX.K" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MIN.U_cpu_load" using 1:2:0 title "01.T.MIN.U" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MIN.K_cpu_load" using 1:2:0 title "01.T.MIN.K" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MAX.U_cpu_load" using 1:2:0 title "01.T.MAX.U" lc palette frac (n-1)/10.0 lw 3 with lines, \
n=n+1, "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/01/srv02_01.T.MAX.K_cpu_load" using 1:2:0 title "01.T.MAX.K" lc palette frac (n-1)/10.0 lw 3 with lines, \
