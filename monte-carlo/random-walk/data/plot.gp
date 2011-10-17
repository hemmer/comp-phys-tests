set title "Random Walks on 2D Lattice"
set xlabel "N (number of steps)"
set ylabel "<R> Mean end-to-end distance"
plot 	"output.dat" using 1:2 title "simulation" with points, \
	"output.dat" using 1:3 title "sqrt(N)" with lines
#	"output.dat" using 1:3 title "<x>" with lines, 
#	"output.dat" using 1:4 title "<y>" with lines

