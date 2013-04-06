reset

set terminal pngcairo size 1920,1152 enhanced font "Verdana,20"
set output "renderingTimes.png"

#set terminal wxt persist

#set key spacing 5 font ",30"

set xlabel "Game Time(s)"
set ylabel "Rendering Time(ms)"

set grid 

set ytics 1
set xtics 10
set xrange [0:20]

set style line 1 lc rgb "red"
set style line 2 lc rgb "orange" 
set style line 3 lc rgb "brown"
set style line 4 lc rgb "green"
set style line 5 lc rgb "black"
set style line 6 lc rgb "purple"
set style line 7 lc rgb "blue" 

plot\
		"GFX_COMP_Average_Rendering.log" 	w lines ls 2 t 'Compose', \
		"GFX_LO-RES_Average_Rendering.log" 	w lines ls 1 t "Generate Low Res", \
		"GFX_MESH_Average_Rendering.log" 	w lines ls 6 t "Mesh", \
		"GFX_LIGH_Average_Rendering.log" 	w lines ls 5 t "Light", \
		"GFX_EFFECTS_Average_Rendering.log" w lines ls 3 t 'DoF', \
		"GFX_PART_Average_Rendering.log" 	w lines ls 7 t "Particles", \
		"GFX_GUI_Average_Rendering.log" 	w lines ls 4 t "GUI"
		
set output "temp.txt"