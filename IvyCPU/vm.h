#pragma once
#include "main.h"
#include "memory.h"
#include "block.h"

using namespace std;

class VM {
public:

	VM();
	bool run();

private:

	Memory memory;
	vector<Task> tasks;
	int firstOut;

	vector<Task*> getPendingProcesses();
	void printMemory();
	void firstFit();
	bool isFinish();
};