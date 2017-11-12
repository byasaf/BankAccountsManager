/* List.c
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


account* CreateAccountNode(	ACCOUNTNUM		account_number,
							AMMOUNT			current_balance,
							AMMOUNT			total_deposit,
							AMMOUNT			total_withdrawal,
							int				num_of_deposits,
							int				num_of_withdrawals)
{
	account*	account_node=NULL;
	account_node=(account*)malloc(sizeof(account));
	if(NULL==account_node) return NULL; //memory allocation failed

	account_node->account_number=account_number;
	account_node->current_balance=current_balance;
	account_node->total_deposit=total_deposit;
	account_node->total_withdrawal=total_withdrawal;
	account_node->num_of_deposits=num_of_deposits;
	account_node->num_of_withdrawals=num_of_withdrawals;
	account_node->account_mutex=CreateMutex(NULL,FALSE,NULL);
    if (NULL == account_node->account_mutex) 
    {
			printf("CreateMutex error: %d\n", GetLastError()); 
			free(account_node);
			return NULL; 
	}
	account_node->next=NULL;
	return account_node;
}

account* PushSortedAccountNode(account* head,account* new_account)
{
	account* curr=NULL;
	account* prev=NULL;

	if(NULL==head) //this is empty list
	{
		return new_account;
	}
	if(new_account->account_number < head->account_number) //new account shuold be first
	{
		new_account->next=head;
		return new_account;
	}

	curr=head; //else, new account shold be somewhere in the middle, find where
	while(curr != NULL && curr->account_number <= new_account->account_number)
	{
		prev=curr;
        curr = curr->next;
    }

	new_account->next = curr;
	prev->next = new_account;
    
    return head;
}
account* RemoveAccountNode(account* head,ACCOUNTNUM account_num)
{
	account* temp_account=NULL;
	account* curr=FindAccountNode(head,account_num);
	if(NULL==curr) return head;	//not found, cannot delete
	if(head==curr) //we're about to delete the head
	{
		temp_account=curr->next;
		free(curr);
		return temp_account;
	}
	//node in the middle
	temp_account=FindPrev(head,curr);
	temp_account->next=curr->next;
	free(curr);

	return head;
}
account* FindAccountNode(account* head,ACCOUNTNUM account_number)
{
	account* curr=NULL;
	if(NULL==head) //list is empty
	{
		return NULL;
	}

	for(curr=head;NULL!=curr;curr=curr->next) 
	{
		if(curr->account_number==account_number) //return the node which pointing to the desired node
		{
			return curr;
		}
	}
	return NULL; //not found
}
account* FindPrev(account* head, account* curr)
{
	account* prev;
	if(NULL==head||NULL==curr) return NULL;
	for(prev=head;NULL!=prev->next,curr!=prev->next;prev=prev->next);
	return prev;
}
void FreeList(account* head)
{
	account* curr=NULL;
	while ((curr = head) != NULL) 
	{ 
		head = head->next; 
		free (curr);                
	}
}
