#!/bin/bash

for i in $(seq 1 $1);
do
	#waitTime=$(awk 'BEGIN { srand(); printf("%.1f\n", rand() * 1.9 + 0.1) }')
	#echo "Aspetto $waitTime secondi"
	#sleep $waitTime
	$(./a.out > log$i)&
done

wait
