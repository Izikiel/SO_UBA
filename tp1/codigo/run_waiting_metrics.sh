#!/bin/bash
# rm -f data7.txt
# for (( i = 1; i < 11; i++ )); do
# 	for (( j = 0; j < 10; j++ )); do
# 		./simusched ej7.tsk 2 1 2 SchedRR $i $i > ej7.txt
# 		ipython waiting_time_metric.py >> data7.txt
# 		echo "Hice el caso con 2 cores:D"
# 	done
# done

rm -f data7_quad.txt
for (( i = 1; i < 11; i++ )); do
	for (( j = 0; j < 10; j++ )); do
		./simusched ej7.tsk 4 1 2 SchedRR $i $i $i $i > ej7.txt
		ipython waiting_time_metric.py >> data7_quad.txt
		echo "Hice el caso con 4 cores :O"
	done
done