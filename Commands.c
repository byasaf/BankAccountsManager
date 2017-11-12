/* Commands.c
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
//==================


CMD* GetCommandsFromFile(char* file_name,CMD commands[],int* num_of_commands_pointer)
{
	FILE			*input_file=NULL;
	char*			temp_cmd=NULL;
	char			c='.';
	int				i=0,j,
					num_of_commands=0,
					max_command_length=0;

	input_file = fopen(file_name,"r");
	if(!input_file)
	{
		printf("Could not open commands file: %s\n",file_name);
		return NULL;
	}

	//counting the commands (lines number) and finding the max command length for later allocating memory
	while(EOF!=c)
	{
		c=fgetc(input_file);
		i++;
		if('\n'==c||EOF==c)
		{
			if(i>1) num_of_commands++;
			if(i>max_command_length)
			{
				max_command_length=i;
			}
			i=0;
		}
	};

	max_command_length++;
	rewind(input_file);

	commands=(CMD*)malloc((num_of_commands)*sizeof(CMD));
	if( NULL==commands)
	{
		free(temp_cmd);
		fclose(input_file);
		printf("Memory allocation failed. Exiting.\n");
		return NULL;
	}
	temp_cmd=(char*)malloc(max_command_length);
	if(NULL==temp_cmd)
	{
		free(commands);
		fclose(input_file);
		printf("Memory allocation failed. Exiting.\n");
		return NULL;
	}

	i=0;
	j=0;
	c='.';
	while(EOF!=c)
	{
		c=fgetc(input_file);
		if(c!='\n' && c!=EOF)
		{
			temp_cmd[i]=c;
			i++;
		}
		else //end of command
		{
			temp_cmd[i]='\0';
			if(i>0)
			{
				if(NULL!=strstr(temp_cmd,CREATE_ACCOUNT_CMDSTR))
				{
					ExtractCommand(temp_cmd,&commands[j],CREATE_ACCOUNT);		
				}
				else if(NULL!=strstr(temp_cmd,CLOSE_ACCOUNT_CMDSTR))
				{
					ExtractCommand(temp_cmd,&commands[j],CLOSE_ACCOUNT);	
				}
				else if(NULL!=strstr(temp_cmd,PRINT_BALANCES_CMDSTR))
				{
					ExtractCommand(temp_cmd,&commands[j],PRINT_BALANCES);	
				}
				else if(NULL!=strstr(temp_cmd,DEPOSIT_CMDSTR))
				{
					ExtractCommand(temp_cmd,&commands[j],DEPOSIT);	
				}
				else if(NULL!=strstr(temp_cmd,WITHDRAWAL_CMDSTR))
				{
					ExtractCommand(temp_cmd,&commands[j],WITHDRAWAL);	
				}
				else 
				{
					j--;
					num_of_commands--;
				}
				j++;
				i=0;
			}
		}
		
	}

	free(temp_cmd);
	fclose(input_file);
	*num_of_commands_pointer=num_of_commands;
	return commands;
}
void ExtractCommand(char* command_str,CMD* command,CMD_TYPE type)
{
	command->type=type;
	switch(type)
	{
		case CREATE_ACCOUNT:
			sscanf(command_str,"%*s %lu %lf",&command->account_number,&command->amount);
			command->thread_routine=(LPTHREAD_START_ROUTINE)CreateAccountRoutine;
		break;

		case CLOSE_ACCOUNT:
			sscanf(command_str,"%*s %lu",&command->account_number);
			command->amount=0;
			command->thread_routine=(LPTHREAD_START_ROUTINE)CloseAccountRoutine;
		break;

		case PRINT_BALANCES:
			command->account_number=0;
			command->amount=0;
			command->thread_routine=(LPTHREAD_START_ROUTINE)PrintBalancesRoutine;
		break;

		case DEPOSIT:
			sscanf(command_str,"%*s %lu %lf",&command->account_number,&command->amount);
			command->thread_routine=(LPTHREAD_START_ROUTINE)DepositRoutine;
		break;

		case WITHDRAWAL:
			sscanf(command_str,"%*s %lu %lf",&command->account_number,&command->amount);
			command->thread_routine=(LPTHREAD_START_ROUTINE)WithdrawalRoutine;
		break;

		default:
			command->type=(CMD_TYPE)0;
			command->account_number=0;
			command->amount=0;
	}
}
EXIT_CODE CommandsLauncer(CMD commands[], int num_of_commands)
{	
	ThreadArguments*		args;
	HANDLE*					thread_handles=NULL;
	DWORD*					thread_IDs=NULL;
	DWORD					exit_code;
	int						i;
	
	thread_handles	=(HANDLE*)malloc(num_of_commands*sizeof(HANDLE));
	thread_IDs		=(DWORD*)malloc(num_of_commands*sizeof(DWORD));

	if(NULL==thread_handles || NULL==thread_IDs)
	{
		printf("Memory allocation failed. Exiting.\n");
		free(thread_handles);
		free(thread_IDs);
		return MEMORY_ALLOCATION_FAILED;
	}
		
	for(i=0;i<num_of_commands;i++) //main launcher's loop
	{
		args=(ThreadArguments*)malloc(sizeof(ThreadArguments));
		if(NULL==args)
		{
			printf("Memory allocation failed. Exiting.\n");
			free(thread_handles);
			free(thread_IDs);
			SetMustEnd(); //close all currently running threads
			return MEMORY_ALLOCATION_FAILED;
		}
		args->command=commands[i];
		if( i>0 && (  CREATE_ACCOUNT==commands[i].type 
					||CLOSE_ACCOUNT==commands[i].type 
					||PRINT_BALANCES==commands[i].type ) )
				{	
					//for accounting commands, wait for all previous thread to end
					WaitForMultipleObjects(i,thread_handles,TRUE,INFINITE);
					thread_handles[i]=CreateThread(
									NULL,
									0,
									commands[i].thread_routine,
									args,
									0,
									&thread_IDs[i]);
					//if create thread failed - close all threads
					if(NULL==thread_handles[i]) SetMustEnd();
					//also wait for this command to end before continue
					WaitForSingleObject(thread_handles[i], INFINITE);
				}
		else
		{
					//no need to wait
					thread_handles[i]=CreateThread(
										NULL,
										0,
										commands[i].thread_routine,
										args,
										0,
										&thread_IDs[i]);
					//if create thread failed - close all threads
					if(NULL==thread_handles[i]) SetMustEnd(); 
		}

	}
	//wait for all threads to end before exiting the function
	WaitForMultipleObjects(num_of_commands,thread_handles,TRUE,INFINITE);

	for(i=0;i<num_of_commands;i++) //check if one or more threads returned error
	{
		if(GetExitCodeThread(thread_handles[i],&exit_code))
			if(SUCCESS!=exit_code)
			{
				free(thread_handles);
				free(thread_IDs);
				return COMMAND_ERROR;
			}
	}
	free(thread_handles);
	free(thread_IDs);
	return SUCCESS;
}
