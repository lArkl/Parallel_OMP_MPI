#set terminal gif animate

set yrange[-1:1]
set xrange[0:1]
set output 'plot.gif'
while(1>0){
do for [t=1:500] {
outfile = sprintf('%04.0f.dat',t)
plot outfile using 1:2 title 'data '.t w l
pause 0.005
}

}
