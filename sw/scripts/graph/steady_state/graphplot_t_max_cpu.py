#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "graph_t_max_cpu.svg"
set ylabel "CPU-Last [%]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key center top box
set arrow from 0.7,graph(0,0) to 0.7,graph(1,1) nohead front
set arrow from 12.4,graph(0,0) to 12.4,graph(1,1) nohead front
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
plot [0:13] [0:100] 40 lc rgb"#eee8aa" lw 97 notitle with lines, \
    43.1 title "Average" lc rgb"#9932cc" with lines, \
    "data/tcp/cpu_max" using 1:2 title "T.MAX CPU" lc rgb"#006400" with lines

#plot [0:13] [0:100] "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
