/* Threads.h
 * ===========================
 * Exercise 3
 * ===========================
 * Name: Assaf Ben Yishay
 * =========================== */
#pragma once
#ifndef THREADS_H
#define THREADS_H

#define DEFUALT_SLEEP_TIME	10
#define LOG_TO_CONSOLE		FALSE //set to TRUE if you want to see the runtime log on console screen

typedef struct {
	CMD				command;
	//add arguments here...
}ThreadArguments;

//Global memory used by all threads and shared with other .c modules
extern 	HANDLE		AllThreadsMustEnd;
extern	FILE*		file_handle;
extern	HANDLE		file_mutex;

/*
 *   Description: Main thread
 *	-------------------------------------------------------
 *   char* command_file: string holds the commands' file name
 *   char* balance_files: string holds the balances report file name
 *   char* runtime_file: string holds the runtime log file name
 *	-------------------------------------------------------
 *   returns: exit code-	COMMAND_ERROR - if one or more command where failed
 *							CREATE_MUTEX_ERROR - if create mutex failed
 *							MEMORY_ALLOCATION_FAILED - if memory allocation failed
 *							OPEN_FILE_ERROR - if create a new log file failed
 *							FATAL_ERROR - if fatal error occured in a secondary thread (memory allocation failed, open handle failed etc)
 *							SUCCESS - no errors occurred  including in the secondaries threads
 */
EXIT_CODE MainThread(char* command_file,char* balance_files, char* runtime_file);

/*
 *   Description: Create Account thread routine
 *	-------------------------------------------------------
 *   ThreadArguments* args: arguments for the routine, holds a CMD object to run
 *							args->command.account_number - account number to open
 *							args->command.amount - initial balance
 *	-------------------------------------------------------
 *   returns: exit code-	THREAD_BRUTALLY_CLOSED - if thread was closed by another thread in the middle of the routine
 *							ACCOUNT_ALREDAY_EXIST - if the specified account number is already exists in the list
 *							SUCCESS - no errors occurred
 */
EXIT_CODE CreateAccountRoutine(ThreadArguments* args);

/*
 *   Description: Close Account thread routine
 *	-------------------------------------------------------
 *   ThreadArguments* args: arguments for the routine, holds a CMD object to run
 *							args->command.account_number - account number to close
 *	-------------------------------------------------------
 *   returns: exit code-	THREAD_BRUTALLY_CLOSED - if thread was closed by another thread in the middle of the routine
 *							ACCOUNT_NOT_FOUND - if the specified account number is not exists in the list
 *							SUCCESS - no errors occurred
 */
EXIT_CODE CloseAccountRoutine(ThreadArguments* args);

/*
 *   Description: Print Balances thread routine
 *	-------------------------------------------------------
 *   ThreadArguments* args: arguments for the routine, holds a CMD object to run
 *							Not used during the routine
 *	-------------------------------------------------------
 *   returns: exit code-	THREAD_BRUTALLY_CLOSED - if thread was closed by another thread in the middle of the routine
 *							SUCCESS - no errors occurred
 */
EXIT_CODE PrintBalancesRoutine(ThreadArguments* args);

/*
 *   Description: Deposit thread routine
 *	-------------------------------------------------------
 *   ThreadArguments* args: arguments for the routine, holds a CMD object to run
 *							args->command.account_number - account number to deposit into
 *							args->command.amount - amount to deposit
 *	-------------------------------------------------------
 *   returns: exit code-	THREAD_BRUTALLY_CLOSED - if thread was closed by another thread in the middle of the routine
 *							ACCOUNT_NOT_FOUND - if the specified account number is not exists in the list
 *							SUCCESS - no errors occurred
 */
EXIT_CODE DepositRoutine(ThreadArguments* args);

/*
 *   Description: Withdrawal thread routine
 *	-------------------------------------------------------
 *   ThreadArguments* args: arguments for the routine, holds a CMD object to run
 *							args->command.account_number - account number to withdrawal from
 *							args->command.amount - amount to withdrawal
 *	-------------------------------------------------------
 *   returns: exit code-	THREAD_BRUTALLY_CLOSED - if thread was closed by another thread in the middle of the routine
 *							ACCOUNT_NOT_FOUND - if the specified account number is not exists in the list
 *							SUCCESS - no errors occurred
 */
EXIT_CODE WithdrawalRoutine(ThreadArguments* args);

/*
 *   Description: Generate Balance Report File - used by PrintBalancesRoutine
 *	-------------------------------------------------------
 *   char* file_name: balances report's output file name
 *	 account* accounts_list - pointer to the first node on the list holds the accounts
 *	-------------------------------------------------------
 *   returns: exit code-	OPEN_FILE_ERROR - if create new report file failed
 *							SUCCESS - no errors occurred
 *	
 */
EXIT_CODE GenerateBalanceReportFile(char* file_name, account* accounts_list);

#endif
