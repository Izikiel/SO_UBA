#include "tasks.h"
#include <random>
#include <chrono>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params) { // params: n, bmin, bmax, ...
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed1);
	uniform_int_distribution<int> distribution(params[1], params[2]);
	for (int i = 0; i < params[0]; i++) {
		int uso = distribution(generator);
		uso_IO(pid, uso);
	}
}

void TaskBatch(int pid, vector<int> params) { // params: total_cpu, cant_bloqueos
	int total_cpu = params[0], cant_bloqueos = params[1], i = 0;
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed1);
	uniform_int_distribution<int> distribution(0, total_cpu-1);
	vector<int> moments;
	set<int> moments_used;
	while(i < cant_bloqueos) {
		int m = distribution(generator);
		if (moments_used.find(m) == moments_used.end()) {
			moments_used.insert(m);
			moments.push_back(m);
			i++;
		}
	}
	sort(moments.begin(), moments.end());
	int act_moment = 0;
	for (i = 0; i < cant_bloqueos; i++) {
		cout << moments[i] << endl;
		if (act_moment != moments[i]) {
			uso_CPU(pid, moments[i]-act_moment);
		}
		uso_IO(pid, 1);
		act_moment = moments[i] + 1;
	}
	if (act_moment < total_cpu-1) {
		uso_CPU(pid, total_cpu-1 - act_moment);
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}
