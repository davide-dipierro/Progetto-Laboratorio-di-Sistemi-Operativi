for i in $(seq 1 $1);
do
	$(./a.out autopilota >> log$i)&
	sleep 0.1
done
