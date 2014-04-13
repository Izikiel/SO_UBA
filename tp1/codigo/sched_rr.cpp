#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// cout << endl;
	// cout << "QUIERE CONSTRUIR!" << endl;
	first_process = new circNode;
	cores = argn[0];
	cant_processes = 0;
	cores_quantums.clear();
	cores_quantums_left.clear();
	core_per_process.clear();
	process_per_core.clear();
	id_to_process.clear();
	cores_quantums.resize(cores);
	cores_quantums_left.resize(cores);
	process_per_core.resize(cores);
	for (int i = 0; i < cores; i++) {cores_quantums[i] = argn[i+1]; cores_quantums_left[i] = cores_quantums[i];}
	// cout << "CONSTRUYO!" << endl;

}

SchedRR::~SchedRR() {
	// cout << "QUIERE DESTRUIR" << endl;
	// cout << "DESTRUYO!" << endl;
}


void SchedRR::load(int pid) {
	critical_safe.lock();
	// cout << "BUSCA LOADEAR" << endl;
	circNode* process;
	if (cant_processes == 0)  {
		// cout << "FIRST!" << endl;
		first_process->pid = pid;
		first_process->prev = first_process;
		first_process->next = first_process;
		id_to_process[pid] = first_process;
		core_per_process[first_process] = -1;
		process = first_process;
	}
	else {
		// cout << "NOT FIRST!" << endl;
		process = new circNode;
		process->pid = pid;
		process->prev = first_process;
		process->next = first_process->next;
		first_process->next->prev = process;
		first_process->next = process;
		id_to_process[pid] = process;
		core_per_process[process] = -1;
	}
	for (int i = 0; i < cores; i++) {
		if (process_per_core[i] == 0) {
			process_per_core[i] = process;
			core_per_process[process] = i;
			break;
		}
	}
	cant_processes++;
	// cout << "LOADEA" << ' ' << core_per_process[process] << ' ' << process->pid << endl;
	critical_safe.unlock();
}

void SchedRR::unblock(int pid) {
	critical_safe.lock();
	core_per_process[id_to_process[pid]] = -1;
	critical_safe.unlock();
}

void SchedRR::assign_next (int cpu, circNode* process) {
	// cout << "QUIERE ASIGNAR" << endl;
	cores_quantums_left[cpu] = cores_quantums[cpu];
	for(circNode* p = process->next; ;p = p->next) {
		if (core_per_process[p] == -1) {
			process_per_core[cpu] = p;
			core_per_process[p] = cpu;
			break;
		}
		if (p == process && core_per_process[p] != -1) {
			process_per_core[cpu] = 0;
			break;
		}
	}
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	// cout << "QUIERE TICKEAR " << m << endl;
	critical_safe.lock();
	circNode* process = process_per_core[cpu];
	if (process == 0) {
		assign_next(cpu, first_process);
	}
	else {
		switch(m) {
			case TICK:
				// cout <<"QUANTUM LEFT " << cores_quantums_left[cpu] << endl;
				if (cores_quantums_left[cpu] == 0) {
					core_per_process[process] = -1;
					assign_next(cpu, process);
				}
				cores_quantums_left[cpu]--;
				break;
			case BLOCK:
				core_per_process[process] = -2;
				assign_next(cpu, process);
				break;
			case EXIT:
				process->next->prev = process->prev;
				process->prev->next = process->next;
				assign_next(cpu, process->next);
				if (process == first_process) {
					first_process = first_process->next;
				}
				id_to_process.erase(process->pid);
				delete process;
				cant_processes--;
				break;
			default:
				break;
		}
	}
	int ret_pid;
	// cout << "TICKEO!" << endl;
	if (process_per_core[cpu] == 0) {
		ret_pid = IDLE_TASK;
	}
	else {
		// cout << "CPUIDSIGUITENTE" << process_per_core[cpu]->pid << endl;
		ret_pid = process_per_core[cpu]->pid;
	}
	critical_safe.unlock();
	return ret_pid;
}
