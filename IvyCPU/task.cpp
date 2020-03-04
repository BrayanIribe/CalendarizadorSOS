#include "task.h"

Task::Task(int id, int time, int size) {
	Task::id = id;
	Task::time = time;
	Task::size = size;
	Task::status = TASK_STATUS::STATUS_WAITING;
}

int Task::getId() {
	return id;
}

int Task::getTime() {
	return time;
}

int Task::getSize() {
	return size;
}

TASK_STATUS Task::getStatus() {
	return status;
}

string Task::getStatusString() {
	string rtn;
	switch (status) {
	case STATUS_WAITING: 
		rtn = "Esperando";
		break;
	case _STATUS_PENDING:
		rtn = "Pendiente";
		break;
	case STATUS_EXECUTING:
		rtn = "Ejecutando";
		break;
	case STATUS_FINISHED:
		rtn = "Finalizado";
	}
	return rtn;
}

void Task::setStatus(TASK_STATUS status) {
	Task::status = status;
}