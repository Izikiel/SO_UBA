#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <stdint.h>
#include <iostream>
#include "basesched.h"

using namespace std;

typedef uint32_t uint;
typedef uint CORE_LOAD;

typedef struct pcb_entry {
    
    bool operator == (const struct pcb_entry other) const
    {
        return pid == other.pid;
    }

    //necesario para indexar el map de waiting con la PK pid.
    int pid;
    //necerario para indexar la tabla de cores cuando se desbloquea de waiting y debe ser encolado
    //o cuando se modifica el load de un core
    uint core_affinity;
} PCB_ENTRY;

typedef struct core_entry {
    
    //sirve para obtener el core con menos carga con la funcion min_element de c++
    bool operator<(struct core_entry &other) const
    {
        return load < other.load;
    }

    //id del core, sirve para identificar el core al asignar afinidades a los procesos
    uint id;
    //quantum por default para cada proceso en este core
    uint default_quantum;
    //quantum restante en este core
    uint remaining_quantum;
    //cantidad de procesos entre BLOCKED + RUNNING + READY en este core
    CORE_LOAD load;
    //cola de procesos ready asociada al core
    queue<PCB_ENTRY> *ready_queue;
    //running process
    PCB_ENTRY running_process;
} CORE_ENTRY;

class SchedRR2 : public SchedBase
{
    public:
        //constructor y destructor
        SchedRR2(std::vector<int> argn);
        ~SchedRR2();
        //metodos publicos que deben implementarse por interfaz
        virtual void load(int pid);
        virtual void unblock(int pid);
        virtual int tick(int core, const enum Motivo m);
    private:
        //atributos
        vector<CORE_ENTRY> *core_table;  
        //tabla comun de procesos en espera  
        unordered_map<int, PCB_ENTRY> *waiting_table;//<pid, PCB_ENTRY>
        //IDLE_PCB static template
        static PCB_ENTRY IDLE_PCB;        
        
        //metodos privados
        void admitProcess(int pid, CORE_ENTRY assignedCore);
        void finalizeProcess(PCB_ENTRY targetProcess);
};

#endif
