#include "sched_edf.h"

using namespace std;

SchedEDF::SchedEDF(vector<int> argn) {
  // FCFS recibe la cantidad de cores.
}

SchedEDF::~SchedEDF() {
}

void SchedEDF::load(int pid) {
  load(pid,0);
}

void SchedEDF::load(int pid,int deadline) {
}

void SchedEDF::unblock(int pid) {
}

int SchedEDF::tick(int cpu, const enum Motivo m) {
  return 0;
}
