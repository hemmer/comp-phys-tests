set title "Diffusion in 1D"
set xlabel "position (x)"
set ylabel "time (t)"
set zlabel "concentration (phi)"
set nokey
splot 	"output.dat" using 1:2:3 with lines
