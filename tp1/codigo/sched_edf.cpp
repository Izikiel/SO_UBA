#include "sched_edf.h"

using namespace std;

const Proc free_proc;

SchedEDF::SchedEDF(vector<int> argn)
{
    // EDF recibe la cantidad de cores.
    pending_processes.clear();
    core_process.clear();
    make_heap(pending_processes.begin(), pending_processes.end());
    total_cores = argn[0];
    core_process.resize(total_cores);

    for (int i = 0; i < total_cores; ++i) {
        core_process[i] = free_proc;
        free_cores.push(i);
    }
}

SchedEDF::~SchedEDF() {}

void SchedEDF::load(int pid)
{
    load(pid, 0);
}

int SchedEDF::find_greater_running(Proc p)
{
    for (int i = 0; i < total_cores; ++i) {
        if (core_process[i] < p) {
            return i;
        }
    }

    return -1;
}

void SchedEDF::push_pending(Proc p)
{
    pending_processes.push_back(p);
    push_heap(pending_processes.begin(), pending_processes.end());
}

void SchedEDF::pop_pending()
{
    pop_heap(pending_processes.begin(), pending_processes.end());
    pending_processes.pop_back();
}

void SchedEDF::load(int pid, int deadline)
{
    critical_safe.lock();
    Proc new_task(deadline, pid);

    if (!free_cores.empty()) {
        int core = free_cores.front();
        free_cores.pop();
        core_process[core] = new_task;
        critical_safe.unlock();
        return;
    }

    int greater_proc = find_greater_running(new_task);

    if (greater_proc >= 0) {
        push_pending(core_process[greater_proc]);
        core_process[greater_proc] = new_task;

    } else {
        push_pending(new_task);
    }

    critical_safe.unlock();
    return;
}

void SchedEDF::unblock(int pid)
{
    critical_safe.lock();
    push_pending(blocked_process[pid]);
    blocked_process.erase(pid);
    critical_safe.unlock();
}

void SchedEDF::change_processes()
{
    if (!pending_processes.empty()) {
        for (int i = 0; i < total_cores && !pending_processes.empty(); ++i) {
            Proc pend_proc = pending_processes.front();
            int available_core = find_greater_running(pend_proc);

            if (available_core >= 0) {
                pop_pending();
                push_pending(core_process[available_core]);
                core_process[available_core] = pend_proc;

            } else {
                break;
            }
        }
    }
}

int SchedEDF::tick(int cpu, const enum Motivo m)
{
    critical_safe.lock();
    int pid = core_process[cpu].get_pid();

    switch (m) {
        case TICK:
            change_processes();
            break;

        case BLOCK:
            blocked_process[pid] = core_process[cpu];

            if (!pending_processes.empty()) {
                core_process[cpu] = pending_processes.front();
                pid = core_process[cpu].get_pid();
                pop_pending();

            } else {
                core_process[cpu] = free_proc;
                free_cores.push(cpu);
                pid = -1;
            }

            break;

        case EXIT:
            if (!pending_processes.empty()) {
                core_process[cpu] = pending_processes.front();
                pid = core_process[cpu].get_pid();
                pop_pending();

            } else {
                core_process[cpu] = free_proc;
                free_cores.push(cpu);
                pid = -1;
            }

            break;

        default:
            break;
    }

    critical_safe.unlock();

    return pid;
}
