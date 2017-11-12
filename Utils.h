/* Utils.h
 * ===========================
 * Exercise 3
 * ===========================
 * Name: Assaf Ben Yishay
 * =========================== */
#pragma once
#ifndef UTILS_H
#define UTILS_H

/*
 *   Description: Create new empty output file
 *	-------------------------------------------------------
 *   char* file_name:		string contains the name of the output file 
 *   FILE** file_handle:	pointer to FILE*
 *	-------------------------------------------------------
 *   returns: exit code-	OPEN_FILE_ERROR - if create new report file failed
 *							SUCCESS - no error occurred
 */
EXIT_CODE CreateLogFile(char* file_name, FILE** file_handle);

/*
 *   Description: Safely print to file using the given mutex to avoid collision between threads
 *	-------------------------------------------------------
 *   FILE* fp:				pointer to the file object
 *   HANDLE* mutex:			pointer to mutex
 *   BOOL console:			TRUE - to print to console as well.
 *   const char *format:	format of the line as defined in printf, fprintf functions
 *   ...					other args
 *	-------------------------------------------------------
 *   returns: vfprintf return values
 */
int SafelyLogLine(FILE* fp, HANDLE* mutex, BOOL console, const char *format, ...);

/*
 *   Description: Check if thread must end since error occurred in another thread
 *		using Event
 *	-------------------------------------------------------
 *	 no arguments
 *	-------------------------------------------------------
 *   returns: none
 */
BOOL CheckMustEnd();

/*
 *   Description: Set event for all threads to end and log error to file
 *	-------------------------------------------------------
 *	no arguments
 *	-------------------------------------------------------
 *   returns: none
 */
void SetMustEnd();

#endif