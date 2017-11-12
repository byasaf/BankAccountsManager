/* main.c
 * ===========================
 * Exercise 3
 * ===========================
 * Name: Assaf Ben Yishay
 * =========================== */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//==================
#include "Errors.h"
#include "List.h"
#include "Commands.h"
#include "Threads.h"


EXIT_CODE main(int argc, char **argv)
{
	if(argc!=4)
	{
		printf("Unknown arguments. Exiting");
		return UNKNOWN_ARGUMENTS;
	}
	return MainThread(argv[1],argv[2],argv[3]);
}
