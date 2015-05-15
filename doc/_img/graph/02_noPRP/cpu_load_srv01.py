#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/cpu_load_srv01.svg"
set ylabel "CPU-Last [%]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key outside right center box
plot \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.U.MIN.U_cpu_load" using 1:2 title "02.U.MIN.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.U.MIN.K_cpu_load" using 1:2 title "02.U.MIN.K" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.U.MAX.U_cpu_load" using 1:2 title "02.U.MAX.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.U.MAX.K_cpu_load" using 1:2 title "02.U.MAX.K" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.T.MIN.U_cpu_load" using 1:2 title "02.T.MIN.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.T.MIN.K_cpu_load" using 1:2 title "02.T.MIN.K" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.T.MAX.U_cpu_load" using 1:2 title "02.T.MAX.U" lw 3 with lines, \
"/home/guadagnini/BA15_wlan_1/sw/scripts/graph/02_noPRP/srv01_02.T.MAX.K_cpu_load" using 1:2 title "02.T.MAX.K" lw 3 with lines, \
