/* List.h
 * ===========================
 * Exercise 3
 * ===========================
 * Name: Assaf Ben Yishay
 * =========================== */
#pragma once
#ifndef LIST_H
#define LIST_H

typedef unsigned long int	ACCOUNTNUM;
typedef long double			AMMOUNT;

typedef struct node{
	ACCOUNTNUM		account_number;
	AMMOUNT			current_balance;
	AMMOUNT			total_deposit;
	AMMOUNT			total_withdrawal;
	int				num_of_deposits;
	int				num_of_withdrawals;
	HANDLE			account_mutex;
	struct node*	next;
}account;
 
/*
 *  Description: Create new node for accounts linked list
 *	-------------------------------------------------------
 *	ACCOUNTNUM	account_number: unsigned long int holds the new account's number
 *	AMMOUNT	current_balance:	long double holds the current balance
 *	AMMOUNT	total_deposit:		long double holds the total deposits amount
 *	AMMOUNT	total_withdrawal:	long double holds the total withdrawals amount
 *	int	num_of_deposits:		holds the number of depostis
 *	int	num_of_withdrawals:		holds the number of num_of_withdrawals
 *	-------------------------------------------------------
 *  returns: pointer to the new node
 */
account* CreateAccountNode(	ACCOUNTNUM		account_number,
							AMMOUNT			current_balance,
							AMMOUNT			total_deposit,
							AMMOUNT			total_withdrawal,
							int				num_of_deposits,
							int				num_of_withdrawals);
/*
 *  Description: Push a new node to list, sorted in ascending order by account's number 
 *	-------------------------------------------------------
 *	account* head:			pointer to the first node in the linked list
 *	account* new_account:	pointer to the new node
 *	-------------------------------------------------------
 *  returns: pointer to the head of the list
 */
account* PushSortedAccountNode(account* head,account* new_account);

/*
 *  Description: Remove node with the given account number 
 *	-------------------------------------------------------
 *	account* head:			pointer to the first node in the linked list
 *	ACCOUNTNUM account_num:	node's account number
 *	-------------------------------------------------------
 *  returns: pointer to the head of the list
 */
account* RemoveAccountNode(account* head,ACCOUNTNUM account_num);

/*
 *  Description: Find node with the given account number 
 *	-------------------------------------------------------
 *	account* head:			pointer to the first node in the linked list
 *	ACCOUNTNUM account_num:	node's account number
 *	-------------------------------------------------------
 *  returns: pointer to the node or NULL if not found
 */
account* FindAccountNode(account* head,ACCOUNTNUM account_number);

/*
 *  Description: Find the previous node to the given node
 *	-------------------------------------------------------
 *	account* head:	pointer to the first node in the linked list
 *	account* curr:	node
 *	-------------------------------------------------------
 *  returns: pointer to the previous node
 */
account* FindPrev(account* head, account* curr);

/*
 *  Description: Free all the memory which was allocated for the linked list
 *	-------------------------------------------------------
 *	account* head:	pointer to the first node in the linked list
 *	-------------------------------------------------------
 *  returns: none
 */
void FreeList(account* head);

#endif