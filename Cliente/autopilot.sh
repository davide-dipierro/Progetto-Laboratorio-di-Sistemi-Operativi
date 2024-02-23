for i in $(seq 1 $1);
do
	$(./a.out autopilot >> log$i)&
	sleep 0.1
done
