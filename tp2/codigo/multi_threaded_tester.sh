#!/bin/bash
reset
CLI_PER_THREAD="25"
CANT_THREADS=10
for i in $(seq 1 $CANT_THREADS); do
	python server_tester.py "$CLI_PER_THREAD" &
	sleep 5
done