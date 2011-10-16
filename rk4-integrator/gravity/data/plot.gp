set title "RK4 Integrator: Ball falling under Earth's gravity"
set xlabel "time (t)"
set ylabel "position / velocity"
plot "output.dat" using 1:2 title "velocity" with lines, \
"output.dat" using 1:3 title "position" with lines
