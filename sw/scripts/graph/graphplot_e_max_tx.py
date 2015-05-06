#!/usr/bin/env gnuplot
set terminal svg size 1000,600 enhanced font "Helvetica,20"
set output "graph_e_max_tx.svg"
set ylabel "Bitrate [MBit/s]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key center top box
set arrow from 0.6,graph(0,0) to 0.6,graph(1,1) nohead front
set arrow from 12.6,graph(0,0) to 12.6,graph(1,1) nohead front
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
plot [0:13] [0:130] 99 lc rgb"#eee8aa" lw 25 notitle with lines, \
    98.07 title "Average" lc rgb"#9932cc" with lines, \
    "data/eth/tx_max_rate" using 1:2 title "E.MAX TX" lc rgb"red" with lines


#plot [0:13] "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
