#ifndef __SCHED_EDF__
#define __SCHED_EDF__

#include "basesched.h"


class SchedEDF : public SchedBase {
  public:
    SchedEDF(std::vector<int> argn);
        ~SchedEDF();
    virtual void load(int pid);
    virtual void load(int pid, int deadline);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);

};

#endif
