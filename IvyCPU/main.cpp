/*==============================================
# CalendarizadorSOS

Calendarizador de procesos que simula las tareas
administrativas de un sistema operativo para asignar
procesos y manejar tareas.

Desarrollado por Brayan Iribe para la asignatura
Sistemas Operativos en el segundo semestre
enero 2020 de ISW en el ITSON.

Este software es de código abierto, si utilizas
alg�n fragmento o te fue de ayuda no olvides dejar
una estrella y dar créditos.

Repositorio original:

https://github.com/BrayanIribe/CalendarizadorSOS
===============================================*/

#include "main.h"
#include "vm.h"
#include "utils.h"

int main(int argc, char ** argv) {
	VM * vm = new VM();
	vm->run();
	SetConsoleTextAttribute(Utils::hConsole, Utils::CONSOLE_DEFAULT);
	return 1;
}