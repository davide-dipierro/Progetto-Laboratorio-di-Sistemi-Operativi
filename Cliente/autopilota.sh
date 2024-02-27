#!/bin/bash

for i in $(seq 1 $1);
do
	./cliente autopilota
	sleep 0.1
done
