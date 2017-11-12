/* Errors.h
 * ===========================
 * Exercise 3
 * ===========================
 * Name: Assaf Ben Yishay
 * =========================== */
#pragma once
#ifndef ERRORS_H
#define ERRORS_H

//exit and error codes
typedef enum{
			SUCCESS,
			COMMAND_ERROR,					//one or more commands returned error
			UNKNOWN_ARGUMENTS,
			FATAL_ERROR,
			CREATE_THREAD_FAILED,
			THREAD_BRUTALLY_CLOSED,
			MEMORY_ALLOCATION_FAILED,
			OPEN_FILE_ERROR,
			CREATE_MUTEX_ERROR,
			ACCOUNT_ALREDAY_EXIST,
			ACCOUNT_NOT_FOUND,
			LIST_IS_EMPTY
}EXIT_CODE;

#endif