reset
f(x)=0.6 * 3600
set xrange[0:133]
set xlabel 'Density (v/km)'
set ylabel 'Flow (v/h)'
set grid

plot 'Default-1CAR.txt' u 5:4 w p t'Left lane', 'Default-1CAR.txt' u 10:9 w p t'Left lane'
