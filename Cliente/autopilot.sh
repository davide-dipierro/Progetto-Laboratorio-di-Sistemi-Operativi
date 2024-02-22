for i in $(seq 1 4);
do
	$(./a.out autopilot >> log$i)&
	sleep 0.1
done
