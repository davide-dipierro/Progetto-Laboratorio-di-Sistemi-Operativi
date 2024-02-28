#!/bin/bash

for i in $(seq 1 $1);
do
	sleep 0.3791
	$(./autopilota > log$i)&
done

wait
