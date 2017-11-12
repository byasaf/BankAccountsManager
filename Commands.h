/* Commands.h
 * ===========================
 * Exercise 3
 * ===========================
 * Name: Assaf Ben Yishay
 * =========================== */
#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H

//commands strings
#define CREATE_ACCOUNT_CMDSTR		"CreateAccount"
#define CLOSE_ACCOUNT_CMDSTR		"CloseAccount"
#define PRINT_BALANCES_CMDSTR		"PrintBalances"
#define DEPOSIT_CMDSTR				"Deposit"
#define WITHDRAWAL_CMDSTR			"Withdrawal"

//commands types
typedef enum 
{
	CREATE_ACCOUNT	=1,
	CLOSE_ACCOUNT	=2,
	PRINT_BALANCES	=3,
	DEPOSIT			=4,
	WITHDRAWAL		=5
}CMD_TYPE;

typedef unsigned long int	ACCOUNTNUM;
typedef long double			AMMOUNT;

//command object
typedef struct _CMD
{
	CMD_TYPE				type;
	ACCOUNTNUM				account_number;
	AMMOUNT					amount;
	LPTHREAD_START_ROUTINE	thread_routine;
}CMD;

/*
 *   Description: Reads commands from file line by line 
 *	-------------------------------------------------------
 *   char* file_name:				string contains the name of the file 
 *   CMD commands[]:				array to hold the commands
 *   int* num_of_commands_pointer:	pointer to integer to the num of files
 *	-------------------------------------------------------
 *   returns: pointer to the commands array 
 */
CMD* GetCommandsFromFile(char* file_name,CMD commands[],int* num_of_commands_pointer);

/*
 *   Description: gets single command string and initilize CMD object 
 *	-------------------------------------------------------
 *   char* command_str: string contains the whole command
 *   CMD* command:		pointer to CMD object to initilize
 *   CMD_TYPE type:		command's type
 *	-------------------------------------------------------
 *   returns: pointer to the commands array 
 */
void ExtractCommand(char* command_str,CMD* command,CMD_TYPE type);

/*
 *   Description: Run each command from array in a new thread
 *	-------------------------------------------------------
 *   CMD commands[]:		array holds all the commands
 *   int num_of_commands:	size of the commands array
 *	-------------------------------------------------------
 *   returns: exit code if errors occurred
 */
EXIT_CODE CommandsLauncer(CMD commands[], int num_of_commands);

#endif
