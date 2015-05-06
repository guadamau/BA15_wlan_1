#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "graph_t_min_cpu.svg"
set ylabel "CPU-Last [%]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key center top box
set arrow from 0.5,graph(0,0) to 0.5,graph(1,1) nohead front
set arrow from 6.5,graph(0,0) to 6.5,graph(1,1) nohead front
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
plot [0:7] [0:100] 30 lc rgb"#eee8aa" lw 97 notitle with lines, \
    33.25 title "Average" lc rgb"#9932cc" with lines, \
    "data/tcp/cpu_min" using 1:2 title "T.MIN CPU" lc rgb"#006400" with lines

#plot [0:7] [0:100] "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
