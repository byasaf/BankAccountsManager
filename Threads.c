/* Threads.c
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
#include "Utils.h"
#include "List.h"
#include "Commands.h"
#include "Threads.h"

//Global memory used by all threads
	account*		accounts_list=NULL;
	HANDLE			list_mutex;
	HANDLE			file_mutex;
	HANDLE			AllThreadsMustEnd;
	FILE*			file_handle=NULL;
	char*			log_filename=NULL;

EXIT_CODE MainThread(char* command_file,char* balance_files, char* runtime_file)
{
	CMD*				commands=NULL;
	int					num_of_commands;
	EXIT_CODE			ret;
	
	list_mutex = CreateMutex(NULL,FALSE,NULL);
	file_mutex = CreateMutex(NULL,FALSE,NULL);
	AllThreadsMustEnd = CreateEvent(NULL, TRUE, FALSE, NULL);

	if(NULL==list_mutex || NULL==file_mutex || NULL==AllThreadsMustEnd)
	{
			printf("CreateMutex error: %d. Exiting.\n", GetLastError()); 
			ret = CREATE_MUTEX_ERROR; 
			goto End;
	}

	log_filename=(char*)malloc(strlen(runtime_file)+1);
	if(NULL == log_filename)
	{
			printf("Memory allocation faild. Exiting.\n", GetLastError()); 
			ret = MEMORY_ALLOCATION_FAILED;
			goto End;
	}
	log_filename=strcpy(log_filename,runtime_file);

	if(SUCCESS!=CreateLogFile(runtime_file,&file_handle)) //create empty log file
	{
			ret = OPEN_FILE_ERROR;
			goto Main_Cleanup_1;
	}

	commands = GetCommandsFromFile(command_file,commands,&num_of_commands);
	if (NULL==commands)
	{
			ret = OPEN_FILE_ERROR;
			goto Main_Cleanup_2;
	}
	ret = CommandsLauncer(commands,num_of_commands);
	GenerateBalanceReportFile(balance_files,accounts_list);
	if(FALSE == CheckMustEnd())
	{
		SafelyLogLine(file_handle,NULL,LOG_TO_CONSOLE,"Program successfully finished running. Exiting.\n");
	}
	else
	{
		SafelyLogLine(file_handle,NULL,LOG_TO_CONSOLE,"Program ended with fatal errors. Exiting.\n");
		ret = FATAL_ERROR;
	}


	free(commands);
	FreeList(accounts_list);

Main_Cleanup_2:
	fclose(file_handle);

Main_Cleanup_1:
	free(log_filename);

End:
	system("pause");
	return ret;
}

EXIT_CODE CreateAccountRoutine(ThreadArguments* args)
{
	ThreadArguments		curr_arguments=*args;
	account*			new_account=NULL;
	
	Sleep(DEFUALT_SLEEP_TIME);
	free(args);
	//=====================================================
	//printf("Create account started. Account num %d\n",curr_arguments.command.account_number);

	if(CheckMustEnd()) return THREAD_BRUTALLY_CLOSED;

	if(NULL!=FindAccountNode(accounts_list,curr_arguments.command.account_number))
	{	
		SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"!!! Account number %lu already exists. Can't create account. Skipping command. !!!\n",
			curr_arguments.command.account_number);
		return ACCOUNT_ALREDAY_EXIST;
	}
	new_account=CreateAccountNode(curr_arguments.command.account_number,curr_arguments.command.amount,0,0,0,0);
	if(new_account==NULL) SetMustEnd(); //close all currently running threads since create new node failed

	WaitForSingleObject(list_mutex, INFINITE);
	//==============Critical Section Start==============//
	accounts_list=PushSortedAccountNode(accounts_list,new_account);
	//===============Critical Section End===============//
	ReleaseMutex(list_mutex);

	SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"Successfully created bank account number %lu with current balance of %.2lf.\n",
		curr_arguments.command.account_number,
		curr_arguments.command.amount);
	//printf("Create account stopped. Account num %d\n",curr_arguments.command.account_number);
	return SUCCESS;
}
EXIT_CODE CloseAccountRoutine(ThreadArguments* args)
{
	ThreadArguments		curr_arguments=*args;
	
	Sleep(DEFUALT_SLEEP_TIME);
	free(args);
	//=====================================================//

	if(CheckMustEnd()) return THREAD_BRUTALLY_CLOSED;

	//printf("Close account started. Account num %d\n",curr_arguments.command.account_number);
	if(NULL==FindAccountNode(accounts_list,curr_arguments.command.account_number))
	{
		SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"!!! Account number %lu doesn't exist. Can't close account. Skipping command. !!!\n",
			curr_arguments.command.account_number);
		return ACCOUNT_NOT_FOUND;
	}

	WaitForSingleObject(list_mutex, INFINITE);
	//==============Critical Section Start==============//
	accounts_list=RemoveAccountNode(accounts_list,curr_arguments.command.account_number);
	//===============Critical Section End===============//
	ReleaseMutex(list_mutex);

	SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"Successfully closed bank account number %lu.\n",
		curr_arguments.command.account_number);
	//printf("Close account stopped. Account num %d\n",curr_arguments.command.account_number);
	return SUCCESS;
}
EXIT_CODE PrintBalancesRoutine(ThreadArguments* args)
{
	account*			curr;
	
	Sleep(DEFUALT_SLEEP_TIME);
	free(args);
	//=====================================================
	//printf("Print balances started. Account num %d\n",curr_arguments.command.account_number);
	
	if(CheckMustEnd()) return THREAD_BRUTALLY_CLOSED;

	WaitForSingleObject(list_mutex, INFINITE);
	//==============Critical Section Start==============//
	SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"Current balances in bank accounts are:\nBank Account #,Current Balance\n");
	for(curr=accounts_list;curr!=NULL;curr=curr->next)
	{
		SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"%lu,%.2lf\n",
			curr->account_number,
			curr->current_balance);
	}
	//===============Critical Section End===============//
	ReleaseMutex(list_mutex);

	//printf("Print balances stopped. Account num %d\n",curr_arguments.command.account_number);
	return SUCCESS;
}
EXIT_CODE DepositRoutine(ThreadArguments* args)
{
	ThreadArguments		curr_arguments=*args;
	account*			curr=NULL;
	
	Sleep(DEFUALT_SLEEP_TIME);
	free(args);
	//=====================================================
	//printf("Deposit started. Account num %d\n",curr_arguments.command.account_number);

	if(CheckMustEnd()) return THREAD_BRUTALLY_CLOSED;

	if(NULL==(curr=FindAccountNode(accounts_list,curr_arguments.command.account_number)))
	{
		SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"!!! Unable to deposited %.2lf to account number %lu. Account doesn't exist. Skipping command. !!!\n",
			curr_arguments.command.amount,
			curr_arguments.command.account_number);
		return ACCOUNT_NOT_FOUND;
	}
	
	WaitForSingleObject( curr->account_mutex, INFINITE );
	//==============Critical Section Start==============//
	curr->current_balance+=curr_arguments.command.amount;				
	curr->total_deposit+=curr_arguments.command.amount;					
	curr->num_of_deposits++;							
	//===============Critical Section End===============//
	ReleaseMutex( curr->account_mutex );

	SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"Successfully deposited %.2lf to account number %lu.\n",
		curr_arguments.command.amount,
		curr_arguments.command.account_number);

	//printf("Depositt stopped. Account num %d\n",curr_arguments.command.account_number);
	return SUCCESS;
}
EXIT_CODE WithdrawalRoutine(ThreadArguments* args)
{
	ThreadArguments		curr_arguments=*args;
	account*			curr=NULL;
	
	Sleep(DEFUALT_SLEEP_TIME);
	free(args);
	//=====================================================
	//printf("Withdrawal started. Account num %d\n",curr_arguments.command.account_number);

	if(CheckMustEnd()) return THREAD_BRUTALLY_CLOSED;

	if(NULL==(curr=FindAccountNode(accounts_list,curr_arguments.command.account_number)))
	{
		SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"!!! Unable to withdrew %.2lf from account number %lu. Account doesn't exist. Skipping command. !!!\n",
			curr_arguments.command.amount,
			curr_arguments.command.account_number);
		return ACCOUNT_NOT_FOUND;
	}

	WaitForSingleObject( curr->account_mutex, INFINITE );
	//==============Critical Section Start==============//
	curr->current_balance-=curr_arguments.command.amount;		
	curr->total_withdrawal+=curr_arguments.command.amount;		
	curr->num_of_withdrawals++;							
	//===============Critical Section End===============//
	ReleaseMutex( curr->account_mutex );

	SafelyLogLine(file_handle,&file_mutex,LOG_TO_CONSOLE,"Successfully withdrew %.2lf from account number %lu.\n",
		curr_arguments.command.amount,
		curr_arguments.command.account_number);

	//printf("Withdrawal stopped. Account num %d\n",curr_arguments.command.account_number);
	return SUCCESS;
}

EXIT_CODE GenerateBalanceReportFile(char* file_name, account* accounts_list)
{	
	FILE*			fp=NULL;
	account*		curr;
	EXIT_CODE		ret;

	ret = CreateLogFile(file_name,&fp);
	if(SUCCESS!=ret)
	{
		return ret;
	}

	fprintf(fp,"Summary of balances in bank accounts:\nBank Account #,Current Balance,Initial Balance,Total Deposited,Total Withdrawal,# of Deposits,# of Withdrawals\n");
	for(curr=accounts_list;curr!=NULL;curr=curr->next)
	{
		fprintf(fp,"%lu,%.2lf,%.2lf,%.2lf,%.2lf,%d,%d\n",
			curr->account_number,
			curr->current_balance,
			curr->current_balance-curr->total_deposit+curr->total_withdrawal,
			curr->total_deposit,
			curr->total_withdrawal,
			curr->num_of_deposits,
			curr->num_of_withdrawals);
	}
	fclose(fp);
	return SUCCESS;
}



