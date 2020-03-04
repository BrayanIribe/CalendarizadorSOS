#include "vm.h"
#include "utils.h"

VM::VM() {

	VM::firstOut = 1;
	// create tasks

	int taskTime[] = { 5,4,8,2,2,6,8,10,7,6,5,8,9,10,10,7,3,1,9,3,7,2,8,5,10 };
	int taskSize[] = { 5760,4190,3290,2030,2550,6990,8940,740,3930,6890,6580,3820,9140,420,220,7540,3210,1380,9350,3610,7540,2710,8390,5950,60 };

	if (size(taskTime) != size(taskSize)) {
		Utils::Log("Task size and time doesnt contain the same number of elements.", error);
		return;
	}

	for (int i = 0; i < size(taskTime); i++) {
		//int id, int time, int size
		Task task(tasks.size() + 1, taskTime[i], taskSize[i]);
		// task.setStatus(TASK_STATUS::STATUS_WAITING);
		tasks.push_back(task);
	}

	// create memory blocks

	int blockSize[] = { 9500, 7000, 4500, 8500, 3000, 9000, 1000, 5500, 1500, 500 };
	for (int i = 0; i < size(blockSize); i++) {
		Block block(memory.blocks.size() + 1, blockSize[i]);
		memory.blocks.push_back(block);
		char buf[100];
		sprintf_s(buf, "Memory block of %d bytes created at %d", blockSize[i], block.getId());
		Utils::Log(buf, warn);
	}

	Utils::Log("Memory mode...", warn);
}

bool VM::run() {
	while (true) {

		// check if program end
		if (VM::isFinish()) {
			MessageBoxA(NULL, "El programa ha terminado, todos los procesos han finalizado satisfactoriamente.", "Fin", MB_TOPMOST | MB_ICONINFORMATION);
			break;
		}

		if (firstOut > memory.getTotalBlocks()) {
			firstOut = memory.getIdFirstBlockWithTask();
		}
		// print memory status on display
		VM::printMemory();
		// first setting allocation
		VM::firstFit();
		cin.ignore();
		system("cls");
		Sleep(1);
	}
	return true;
}

void VM::firstFit() {
	if (firstOut > 0 && memory.getBlockById(firstOut)->getTask() != nullptr) {
		memory.getBlockById(firstOut)->getTask()->setStatus(TASK_STATUS::STATUS_FINISHED);
		memory.getBlockById(firstOut)->setTask(nullptr);
	}

	// priorize pending proccesses
	vector<Task*> waiting = VM::getPendingProcesses();
	if (waiting.size() > 0) {
		for (size_t i = 0; i < waiting.size(); i++) {
			bool change = false;
			Task* task = waiting[i];
			if (memory.isProcessOnBlocks(task->getId()))
				continue;

			for (size_t w = 0; w < memory.blocks.size(); w++) {
				if (memory.blocks[w].getTask() != nullptr &&
					memory.blocks[w].getTask()->getStatus() == TASK_STATUS::STATUS_EXECUTING)
					continue;

				if (memory.blocks[w].getSize() >= task->getSize() && firstOut == memory.blocks[w].getId()) {
					task->setStatus(TASK_STATUS::STATUS_EXECUTING);
					memory.blocks[w].setTask(task);
					change = true;
					break;
				}
			}

			if (change)
				break;
		}
	}

	
	// logic first fit aka primer ajuste
	
	for (size_t i = 0; i < tasks.size(); i++) {
		bool insert = false;
		Task* task = (Task*)&tasks[i];
		int lastSize = memory.getBlockById(1)->getSize();

		// skip if proccess has end
		if (task->getStatus() == TASK_STATUS::STATUS_FINISHED ||
			task->getStatus() == TASK_STATUS::STATUS_EXECUTING)
			continue;

		for (size_t w = 0; w < memory.blocks.size(); w++) {
			Task* blockTask = memory.blocks[w].getTask();

			if (blockTask != nullptr) {
				continue;
			}

			if (memory.blocks[w].getSize() >= task->getSize()) {
				task->setStatus(TASK_STATUS::STATUS_EXECUTING);
				memory.blocks[w].setTask(task);
				insert = true;
				break;
			}
			else if (!blockTask) {
				task->setStatus(TASK_STATUS::_STATUS_PENDING);
			}

			lastSize = memory.blocks[w].getSize();
		}

		if (insert)
			break;
	}
	
	VM::firstOut++;
}

void VM::printMemory() {
	char buffer[4000];
	sprintf_s(buffer, "\n%s", " TABLA DE TAREAS                         TABLA DE MEMORIA\n\n");
	sprintf_s(buffer, "%s%s", buffer, " | Id | Tiempo | Tamano | Estado     |   | Bloque | Tamano | Proceso | Tiempo |  FI  |\n");
	for (size_t i = 0; i < tasks.size(); i++) {
		string id = to_string(tasks[i].getId());
		string time = to_string(tasks[i].getTime());
		string size = to_string(tasks[i].getSize());
		string status = tasks[i].getStatusString();
		Utils::padString(id, 2, '0', false);
		Utils::padString(time, 2, '0', false);
		Utils::padString(size, 4, ' ', false);
		Utils::padString(status, 10, ' ');
		sprintf_s(
			buffer,
			"%s | %s |   %s   |  %s  | %s |",
			buffer,
			id.c_str(),
			time.c_str(),
			size.c_str(),
			status.c_str()
		);
		if (i <= memory.blocks.size() - 1) {
			Block* block = memory.getBlockById(i + 1);
			string block_id = to_string(block->getId());
			string block_size = to_string(block->getSize());
			string block_task = "       ";
			string block_time = "  ";
			string block_fragmentation = "    ";
			if (block->getTask() != nullptr) {
				Task* task = block->getTask();
				block_task = to_string(task->getId());
				block_time = to_string(task->getTime());
				block_fragmentation = to_string(block->getSize() - task->getSize());
				Utils::padString(block_task, 2, '0', false);
				Utils::padString(block_task, 7, ' ');
				Utils::padString(block_time, 2, '0', false);
				Utils::padString(block_fragmentation, 4, ' ', false);
			}
			Utils::padString(block_id, 2, '0', false);
			Utils::padString(block_size, 4, '0', false);
			sprintf_s(
				buffer,
				"%s   |   %s   |  %s  | %s |   %s   | %s |",
				buffer,
				block_id.c_str(),
				block_size.c_str(),
				block_task.c_str(),
				block_time.c_str(),
				block_fragmentation.c_str()
			);
			if (firstOut == block->getId()) {
				sprintf_s(buffer, "%s <--", buffer);
			}
		}

		if (i == memory.blocks.size() + 1) {
			int totalFragmentation = memory.getTotalFragmentation();
			int totalMemory = memory.getTotalMemory();
			float percentage = ((float)totalFragmentation / (float)totalMemory) * 100;
			string fragmentation = to_string(totalFragmentation);
			sprintf_s(
				buffer,
				"%s    Total de fragmentacion interna: %s (%.0f%%)",
				buffer,
				fragmentation.c_str(),
				percentage
			);
		}
		sprintf_s(buffer, "%s\n", buffer);
	}
	printf("%s", buffer);
}

vector<Task*> VM::getPendingProcesses() {
	vector<Task*> waiting;
	for (size_t i = 0; i < tasks.size(); i++) {
		Task* task = &tasks[i];
		if (task == nullptr || task->getStatus() == TASK_STATUS::STATUS_EXECUTING)
			continue;
		if (task->getStatus() == TASK_STATUS::_STATUS_PENDING)
			waiting.push_back(task);
	}
	return waiting;
}

bool VM::isFinish() {
	vector<Task*> finish;
	for (size_t i = 0; i < tasks.size(); i++) {
		Task* task = &tasks[i];
		if (task == nullptr || task->getStatus() != TASK_STATUS::STATUS_FINISHED)
			continue;
			finish.push_back(task);
	}
	return finish.size() == tasks.size();
}