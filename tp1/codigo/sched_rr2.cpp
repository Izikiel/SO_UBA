#include "sched_rr2.h"
#include "basesched.h"

using namespace std;

// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
SchedRR2::SchedRR2(vector<int> argn)
{
    //inicializo tablas
    this->cpu_table = new vector<CPU_ENTRY>();
    this->cpu_table->reserve(argn.size());
    this->process_table = new deque<PCB_ENTRY>();

    //me armo tabla de cpus
    for (uint j = 0; j < argn.size(); j++) {
        CPU_ENTRY cpu_entry;
        cpu_entry.default_quantum = argn[j];
        cpu_entry.id = j;
        cpu_entry.remaining_quantum = 0;
        cpu_entry.status = CPU_IDLE_STATUS;
        cpu_entry.load = 0;
        cpu_table->push_back(cpu_entry);
    }
}

SchedRR2::~SchedRR2()
{
    delete this->cpu_table;
    delete this->process_table;
}

void SchedRR2::exitProcess(int pid)
{
    PCB_ENTRY process_entry;
    process_entry.pid = pid;
    deque<PCB_ENTRY>::iterator it = find(process_table->begin(),
                                         process_table->end(), process_entry);

    if (it != process_table->end()) {
        //obtengo el pcb_entry
        process_entry = (*it);

        //decremento carga en el cpu asociado al proceso a eliminar
        ((*cpu_table)[process_entry.cpu_affinity_id]).load--;

        //  //remuevo el elemento de la tabla de procesos
        //  process_table->erase(it);

    } else {
        cerr << "[exitProcess - SchedRR2 - el pid " << pid << " no esta en la lista]" <<
             endl;
    }
}

void SchedRR2::dispatchProcess(int pid, CPU_ENTRY assignedCore)
{
    //me armo el pcb
    PCB_ENTRY process_entry;
    process_entry.pid = pid;
    process_entry.status = PROCESS_READY_STATUS;
    process_entry.cpu_affinity_id = assignedCore.id;
    //aumento carga en el cpu asociado al nuevo proceso
    ((*cpu_table)[process_entry.cpu_affinity_id]).load++;
    //lo agrego a la lista de procesos al final
    process_table->push_back(process_entry);
}

void SchedRR2::load(int pid)
{
    //cae un nuevo proceso...
    //hay que mandarlo al core donde haya menos carga...
    //esta el operator < en el .h en el struct, funcion comparadora por puntero era un dolor de...dientes...terrible de C++
    CPU_ENTRY assignedCore = *(min_element(cpu_table->begin(), cpu_table->end()));
    this->dispatchProcess(pid,
                          assignedCore);//la dispatcheo a ese core(y queda para siempre en ese core).
}

void SchedRR2::changeProcessStatus(int pid, PROCESS_STATUS newStatus)
{
    PCB_ENTRY process_entry;
    process_entry.pid = pid;
    deque<PCB_ENTRY>::iterator it = find(process_table->begin(),
                                         process_table->end(), process_entry);

    if (it != process_table->end()) {
        it->status = newStatus;

    } else {
        cerr << "[changeProcessStatus - SchedRR2 - el pid " << pid <<
             " no esta en la lista]" << endl;
    }
}

void SchedRR2::unblock(int pid)
{
    //switchear el estado de ese PCB a ready
    changeProcessStatus(pid, PROCESS_READY_STATUS);
}

int SchedRR2::tick(int cpuid, const enum Motivo m)
{
    //flag para determinar si termino el quantum en este cpu, asumo que no hasta hacer la cuenta.
    bool terminoQuantum = false;

    //resto uno y luego evaluo por igualdad con cero...
    if (--((*cpu_table)[cpuid].remaining_quantum) == 0) {
        //termino el quantum, reseteo el valor a default_quantum.
        terminoQuantum = true;
        (*cpu_table)[cpuid].remaining_quantum = ((*cpu_table)[cpuid].default_quantum);
    }

    //flag para determinar si hay que devolver un nuevo proceso, o sigue el mismo actual
    //esto va a valer true en todos los casos salvo que haya sido motivo TICK y todavia tenga quantum disponible
    bool nuevoProceso = true;

    //obtengo el proceso actual en el cpu pasado por parametro
    int currentpid = current_pid(cpuid);

    //evaluo el motivo
    switch (m) {
        case TICK:
            if (terminoQuantum) {
                //desalojar el proceso actual del cpu (mandarlo a ready)
                changeProcessStatus(currentpid, PROCESS_READY_STATUS);

            } else {
                //no hacer cambio de procesos, que siga corriendo en el cpu hasta que se termine el quantum
                nuevoProceso = false;
            }

            break;

        case BLOCK:
            //reseteo el quantum(el que se fue a esperar a I/O perdio su turno ;)
            (*cpu_table)[cpuid].remaining_quantum = ((*cpu_table)[cpuid].default_quantum);
            //pongo el proceso en estado waiting
            changeProcessStatus(currentpid, PROCESS_WAITING_STATUS);
            //TODO: mandarlo al final de la queue
            break;

        case EXIT:
            //aca se elimina de la process table y se le resta al cpu un proceso de la carga total
            exitProcess(currentpid);
            break;
    }

    //si es necesario un nuevo proceso, el flag nuevoProceso es true
    if (nuevoProceso) {
        //obtener el nuevo proceso ( con estado ready y afinidad para este cpu) de la tabla de procesos
        PCB_ENTRY process_entry;
        process_entry.pid = currentpid;
        deque<PCB_ENTRY>::iterator it = find(process_table->begin(),
                                             process_table->end(), process_entry);
        int newPid = getNextPIDReadyForCpu(cpuid, it);
        //marcar el nuevo proceso como running
        changeProcessStatus(newPid, PROCESS_RUNNING_STATUS);
        return newPid;

    } else {
        return currentpid;
    }
}

int SchedRR2::getNextPIDReadyForCpu(int cpuid,
                                    deque<PCB_ENTRY>::iterator currentProcess)
{
    //devolver siguiente, el mas cercano hacia adelante a currentProcess, que tenga afinidad cpuid y estado ready
    int processCount = process_table->size();
    int i = 0;
    bool foundReadyForCpu = false;

    while ((i < processCount) && (!foundReadyForCpu)) {
        currentProcess++;

        if (currentProcess == process_table->end()) {
            currentProcess = process_table->begin();
        }

        if ((currentProcess->cpu_affinity_id == cpuid)
                && (currentProcess->status == PROCESS_READY_STATUS)) {
            foundReadyForCpu = true;
        }

        i++;
    }

    //devuelvo IDLE_TASK en caso que no haya mas procesos ready para este cpu
    if (foundReadyForCpu) {
        return currentProcess->pid;

    } else {
        return IDLE_TASK;
    }
}

void SchedRR2::printProcessTable()
{
    deque<PCB_ENTRY>::iterator start = process_table->begin();
    deque<PCB_ENTRY>::iterator end = process_table->end();
    cout << endl << "Processes table" << endl;
    cout  << "|" << "pid" << "|" << "status" << "|" << "cpu_affinity_id" << "|" <<
          endl;

    while (start != end) {
        cout  << "|" << (*start).pid << "|" << (*start).status << "|" <<
              (*start).cpu_affinity_id << endl;
        start++;
    }
}

void SchedRR2::printCpusTable()
{
    vector<CPU_ENTRY>::iterator start = cpu_table->begin();
    vector<CPU_ENTRY>::iterator end = cpu_table->end();
    cout << endl << "Cpus table" << endl;
    cout  << "|" << "default_quantum" << "|" << "id" << "|" << "remaining_quantum"
          << "|" << "status" << "|" << "load" << "|" << endl;

    while (start != end) {
        cout << "|" << (*start).default_quantum << "|" << (*start).id << "|" <<
             (*start).remaining_quantum << "|" << (*start).status << "|" <<
             (*start).load << endl;
        start++;
    }
}
