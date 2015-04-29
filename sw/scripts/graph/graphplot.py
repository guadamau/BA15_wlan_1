#!/usr/bin/env gnuplot
set terminal svg size 1000,750 enhanced font "Helvetica,20"
set output "graph_eth.svg"
set ylabel "Bitrate [MBit/s]"
set xlabel "Zeit [s]"
set grid x
set auto x
set grid y
set auto y
set key right box
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 3 ps 0.5   # --- blue
#plot "data/eth/tx_max_rate" using 1:2 title "MAX" lc rgb"red", \
#     "data/eth/tx_rnd_rate" using 1:2 title "RANDOM" lc rgb"green", \
#     "data/eth/tx_min_rate" using 1:2 title "MIN" lc rgb"blue"

plot "test_with_10ms_in_s.txt" using 1:2 title "MAX" with linespoints ls 1
