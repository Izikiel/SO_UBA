#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"
#include <vector>
#include <map>
#include <mutex>
using namespace std;

struct circNode {
	int pid;
	circNode* next;
	circNode* prev;
};

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

private:
		int cores;
		int cant_processes;
		circNode* first_process;
		vector<int> cores_quantums;
		vector<int> cores_quantums_left;
		map<circNode*, int> core_per_process;
		map<int, circNode*> id_to_process;
		vector<circNode*> process_per_core;
		mutex critical_safe;
		int next(int cpu);
		virtual void assign_next (int cpu, circNode* process);
};

#endif
