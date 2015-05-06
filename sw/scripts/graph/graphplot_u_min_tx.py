#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "graph_u_min_tx.svg"
set ylabel "Bitrate [MBit/s]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key center top box
set arrow from 0.5,graph(0,0) to 0.5,graph(1,1) nohead front
set arrow from 8.5,graph(0,0) to 8.5,graph(1,1) nohead front
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
plot [0:9] [0:8] 5.82 lc rgb"#eee8aa" lw 14 notitle with lines, \
    5.82 title "Average" lc rgb"#9932cc" with lines, \
    "data/udp/tx_min_rate" using 1:2 title "U.MIN TX" lc rgb"blue" with lines

#plot [0:9] "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
