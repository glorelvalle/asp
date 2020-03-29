#!/bin/bash

rm "tiempos.dat"

min=0
max=32
incr=2

for i in $(seq $min $incr $max)
do
	echo "Tamaño $i..."
	t=$(mpirun -np 2 ./medida_latencia $i | grep "El valor de la latencia media es " | sed "s/El valor de la latencia media es //g")
	echo "Escribiendo valores en tiempos.dat..."
	echo "$i $t" >> "tiempos.dat"
done
echo "Dibujando graficas..."

gnuplot << EOF
set title "Grafica de tiempos Latencia"
set xlabel "Tamaño de mensaje"
set ylabel "Tiempo de ejecucion"
set term png
set output "graf_tiempos.png"
plot "tiempos.dat" u 1:2 w l title "latencia"
unset output
exit
EOF

echo "¡Hecho!"
