#include "sched_rr2.h"
#include "basesched.h"

using namespace std;

//inicializador de atributo estatico
PCB_ENTRY createIdlePCB(){
    PCB_ENTRY pcb;
    pcb.pid = IDLE_TASK;
    pcb.core_affinity = 0;
    return pcb;
}
//inicializo atributos estaticos
PCB_ENTRY SchedRR2::IDLE_PCB = createIdlePCB();

// Round robin recibe la cantidad de cores y sus core_quantum por parámetro
SchedRR2::SchedRR2(vector<int> argn)
{
    //tabla de cores
    this->core_table = new vector<CORE_ENTRY>();
    this->core_table->reserve(argn.size());//evito redimensionamientos.

    for (uint j = 0; j < argn.size(); j++) {
		//inicializo el core_entry
		CORE_ENTRY core_entry;
	        //notar que puedo indexar los cores por el id en la tabla de cores.
		    core_entry.id = j;
		    core_entry.default_quantum = argn[j];
		    core_entry.remaining_quantum = 0;
		    core_entry.load = 0;
            core_entry.running_process = IDLE_PCB;
		    //cola de procesos ready por core
		    core_entry.ready_queue = new queue<PCB_ENTRY>();
		//agrego el core a la tabla
        core_table->push_back(core_entry);
    }

    //tabla global de procesos waiting
    this->waiting_table = new unordered_map<uint, PCB_ENTRY>();//<pid, PCB_ENTRY>
}

SchedRR2::~SchedRR2()
{
	//destruyo todas las colas ready del core table
	for(uint j=0;j<core_table->size();j++){
		delete (*core_table)[j].ready_queue;
	}

	//destruyo tablas
    delete this->core_table;
	delete this->waiting_table;
}

void SchedRR2::finalizeProcess(PCB_ENTRY targetProcess)
{
	//decremento carga en el core asociado al proceso que finaliza
    ((*core_table)[targetProcess.core_affinity]).load--;
}

void SchedRR2::dispatchProcess(int pid, CORE_ENTRY assignedCore)
{
	//inicializo la PCB
    PCB_ENTRY process_entry;
    process_entry.pid = pid;
    process_entry.core_affinity = assignedCore.id;

    //aumento carga en el core asociado al nuevo proceso
    ((*core_table)[process_entry.core_affinity]).load++;
    
    //lo agrego a la cola de procesos ready del core elegido
    ((*core_table)[process_entry.core_affinity]).ready_queue->push(process_entry);
}

void SchedRR2::load(int pid)
{
    //hay que buscar el core menos cargado
    //pasar la funcion comparadora por puntero era un dolor de...dientes...terrible de C++
    //asi que esta el operator < que compara por el atributo load en el .h en el struct,
    CORE_ENTRY assignedCore = *(min_element(core_table->begin(), core_table->end()));//min_element me devuelve el iterador, tengo que desreferenciar
    this->dispatchProcess(pid, assignedCore);//la dispatcheo con afinidad unica a el core asignado
}

void SchedRR2::unblock(int pid)
{
    //mueve el proceso identificado con pid de la tabla global waiting a la cola ready del core correspondiente
    //tupla <pid, PCB_ENTRY>, entonces lo de aca abajo me devuelve un PCB_ENTRY
    PCB_ENTRY process_entry = (*waiting_table)[pid];
    waiting_table->erase(pid);
    ((*core_table)[process_entry.core_affinity]).ready_queue->push(process_entry);
}

int SchedRR2::tick(int coreid, const enum Motivo motivo)
{
    //flag para determinar si termino el quantum en este core, asumo que no hasta hacer la cuenta.
    bool terminoQuantum = false;

    //resto uno y luego evaluo por igualdad con cero...
    if (--((*core_table)[coreid].remaining_quantum) == 0) {
        //termino el quantum, reseteo el valor a default_quantum.
        terminoQuantum = true;
        (*core_table)[coreid].remaining_quantum = ((*core_table)[coreid].default_quantum);
    }

    //flag para determinar si hay que devolver un nuevo proceso, o sigue el mismo actual
    //esto va a valer true en todos los casos salvo que haya sido motivo TICK y todavia tenga quantum disponible el core
    bool nuevoProceso = true;

    //obtengo el proceso actual en el core pasado por parametro
    int currentpid = current_pid(coreid);
    PCB_ENTRY running_process = ((*core_table)[coreid]).running_process;

    switch (motivo) {
        case TICK:
            if (terminoQuantum) {
                //mando el proceso a la cola ready
                ((*core_table)[coreid]).ready_queue->push(running_process);
            } else {
                //que siga corriendo en el core hasta que se termine el quantum
                nuevoProceso = false;
            }
            break;
        case BLOCK:
            //reseteo el quantum(el que se fue a esperar a I/O perdio su turno ;)
            (*core_table)[coreid].remaining_quantum = ((*core_table)[coreid].default_quantum);
            //desalojo el running_process y lo pongo en la tabla global waiting
            (*waiting_table)[currentpid] = running_process;
            break;
        case EXIT:
            //decrementa el load en el core
            finalizeProcess(running_process);
            break;
    }

    //si es necesario un nuevo proceso, el flag nuevoProceso es true
    if (nuevoProceso) {
        //asumo que no hay ninguno pendiente en la cola ready y devuelvo la idle
        PCB_ENTRY new_process = SchedRR2::IDLE_PCB;
        //elegir un nuevo proceso
        if(!((*core_table)[coreid]).ready_queue->empty()){
            new_process = ((*core_table)[coreid]).ready_queue->front();
        }        

        //actualizar el proceso actual
        ((*core_table)[coreid]).running_process = new_process;
        return new_process.pid;
    } else {
        return currentpid;
    }
}