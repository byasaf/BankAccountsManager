/* Utils.c
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

EXIT_CODE CreateLogFile(char* file_name, FILE** file_handle)
{
	*file_handle=fopen(file_name,"w");
	if(NULL==*file_handle)
	{
		printf("Could not create file: %s\n",file_name);
		return OPEN_FILE_ERROR;
	}
	return SUCCESS;
}
int SafelyLogLine(FILE* fp, HANDLE* mutex, BOOL console, const char *format, ...)
{
	int ret;
    va_list args;
	if(NULL == fp) return 1;
	va_start(args, format);
    if(NULL != mutex) WaitForSingleObject(*mutex, INFINITE);
	//==============Critical Section Start==============// 
	ret = vfprintf(fp,format,args);
	if (console) vprintf(format,args);
	//===============Critical Section End===============//
    if(NULL != mutex) ReleaseMutex(*mutex);
    va_end(args);
	return ret;
}
BOOL CheckMustEnd()
{
	DWORD exit_code = WaitForSingleObject(AllThreadsMustEnd,0);
	if(WAIT_OBJECT_0==exit_code)
		return TRUE;
	else return FALSE;
}
void SetMustEnd()
{
	SetEvent(AllThreadsMustEnd);
	SafelyLogLine(file_handle,&file_mutex,TRUE,"!!! Fatal error occurred. All threads will be brutally closed. !!!\n");
}