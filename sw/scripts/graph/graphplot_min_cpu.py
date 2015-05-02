#!/usr/bin/env gnuplot
set terminal svg size 1000,750 enhanced font "Helvetica,20"
set output "graph_min_cpu.svg"
set ylabel "CPU-Last [%]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key right box
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
plot "data/eth/cpu_min" using 1:2 title "ETH" lc rgb"red" with lines, \
     "data/tcp/cpu_min" using 1:2 title "TCP" lc rgb"green" with lines, \
     "data/udp/cpu_min" using 1:2 title "UDP" lc rgb"blue" with lines

#plot "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
