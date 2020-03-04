#pragma once
#include "main.h"

enum TASK_STATUS {
	STATUS_WAITING,
	_STATUS_PENDING,
	STATUS_EXECUTING,
	STATUS_FINISHED
};

class Task {
private:
	int id;                                     //Identifier of the task
	int time;                                   //Time of task
	int size;                                   //Size of task
	TASK_STATUS status;                         //Actual status of the task
public:
	Task(int id, int time, int size);
	int getId();
	int getTime();
	int getSize();
	TASK_STATUS getStatus();
	string getStatusString();
	void setStatus(TASK_STATUS status);
};