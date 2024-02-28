#!/bin/bash

for i in $(seq 1 $1);
do
	# Aspetta un tempo random da 0.1 a 2 secondi
	sleep $(awk -v min=0.1 -v max=2 'BEGIN{srand(); print rand()*(max-min)+min}')
	$(./autopilota > log$i)&
done

wait
