#!/usr/bin/env gnuplot
binwidth=10
bin(x,width)=width*floor(x/width)
set terminal svg size 1000,750 enhanced font "Helvetica,20"
set output "hist.svg"
set ylabel "Häufigkeit"
set xlabel "Framegrösse [Bytes]"
set style histogram cluster gap 1
set style fill solid noborder
set grid x
set auto x
set grid y
set auto y
set nokey
plot "../shck/random_framesizes.txt" using (bin($1,binwidth)):(1.0) smooth freq with boxes lc rgb"blue"
