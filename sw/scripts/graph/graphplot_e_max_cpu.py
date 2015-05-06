#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "graph_e_max_cpu.svg"
set ylabel "CPU-Last [%]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key center top box
set arrow from 0.6,graph(0,0) to 0.6,graph(1,1) nohead front
set arrow from 12.6,graph(0,0) to 12.6,graph(1,1) nohead front
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
plot [0:13] [0:100] 20 lc rgb"#eee8aa" lw 97 notitle with lines, \
    17.51 title "Average" lc rgb"#9932cc" with lines, \
    "data/eth/cpu_max" using 1:2 title "E.MAX CPU" lc rgb"red" with lines


#plot [0:13] [0:100] "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
