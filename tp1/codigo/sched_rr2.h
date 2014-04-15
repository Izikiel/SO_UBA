#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <deque>
#include <algorithm>
#include <stdint.h>
#include <iostream>
#include "basesched.h"

using namespace std;

typedef uint32_t uint;

enum PROCESS_STATUS { PROCESS_READY_STATUS = 0, PROCESS_WAITING_STATUS = 1, PROCESS_RUNNING_STATUS = 2};

enum CPU_STATUS { CPU_IDLE_STATUS = 0, CPU_WORKING_STATUS = 1 };

typedef uint CPU_LOAD;

typedef struct pcb_entry {
    //para funcion stl::find
    bool operator == (const struct pcb_entry &other) const
    {
        return pid == other.pid;
    }

    uint pid;
    enum PROCESS_STATUS status;
    uint cpu_affinity_id;
} PCB_ENTRY;

typedef struct cpu_entry {
    //para funcion stl::min_element
    bool operator<(struct cpu_entry other) const
    {
        return load < other.load;
    }

    uint default_quantum;
    uint id;
    uint remaining_quantum;
    enum CPU_STATUS status;
    CPU_LOAD load;
} CPU_ENTRY;


class SchedRR2 : public SchedBase
{
    public:
        SchedRR2(std::vector<int> argn);
        ~SchedRR2();
        void load(int pid);
        void unblock(int pid);
        int tick(int cpu, const enum Motivo m);
        void dispatchProcess(int pid, CPU_ENTRY assignedCore);
        void exitProcess(int pid);
        void changeProcessStatus(int pid, PROCESS_STATUS newStatus);
        int getNextPIDReadyForCpu(int cpuid, deque<PCB_ENTRY>::iterator currentProcess);
        void printProcessTable();
        void printCpusTable();

    private:
        vector<CPU_ENTRY> *cpu_table;
        deque<PCB_ENTRY> *process_table;
};

#endif
