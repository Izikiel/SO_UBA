#ifndef __SCHED_EDF__
#define __SCHED_EDF__

#include "basesched.h"
#include "proc.h"
#include <queue>
#include <algorithm>
#include <mutex>
#include <map>

using namespace std;

class SchedEDF : public SchedBase
{
    public:
        SchedEDF(std::vector<int> argn);
        ~SchedEDF();
        virtual void load(int pid);
        virtual void load(int pid, int deadline);
        virtual void unblock(int pid);
        virtual int tick(int cpu, const enum Motivo m);

    private:
        mutex critical_safe;
        queue<int> free_cores;
        vector<Proc> pending_processes;
        vector<Proc> core_process;
        map<int, int> pid_start;
        map<int, Proc> blocked_process;
        unsigned int time;
        int total_cores;

        virtual int find_greater_running(Proc p);
        virtual void change_processes();
        virtual void push_pending(Proc p);
        virtual void pop_pending();
};



#endif
