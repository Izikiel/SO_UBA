#/bin/sh
../../simusched tests_roundrobin.tsk 2 1 1 SchedRR2 2 2 > res2.txt
../../simusched tests_roundrobin.tsk 2 1 1 SchedRR 2 2 > res1.txt
../../graphsched.py res2.txt 
../../graphsched.py res1.txt 