//jisoo Heo	B115233
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_LINELENGTH	1024
#define MAX_PARAMETER	64

//真 真 真真真.

typedef unsigned char uchar;

typedef struct _Node
{
	char* command;
	struct _Node* link;
	struct _Node* parent;	
}Node ;

typedef struct _List
{
	int count;
	Node* head;
	Node* rear;
}List;


//Function Module
void readCommandLines();
void printCommands();
void executeCommands();

//Set of Sub Procedure
uchar terminationCheck(char*);
void getTokens(char ***, char **);
void free_Tokens(char**);

//Linked List Library
void list_Init();
void list_headPop();
void list_Insert(char*);
void createNode(Node**, char*);

//Global List
List list;

int main()
{
	readCommandLines();
	printCommands();
	executeCommands();
	return(0);
}

/********************************************* function module ******************************************/

void readCommandLines()
{
	char str[MAX_LINELENGTH + 1];

	list_Init();

	while (1)
	{
		fgets(str, sizeof(str), stdin);
		str[strlen(str) - 1] = '\0';		

		if(strlen(str) > 0) list_Insert(str);
	
		if(terminationCheck(str)) 
		{
			if(list.count > 0) return;
			else exit(0);		
		}
		str[0] = '\0';
	}
}

void printCommands()
{
	Node* nodeIter = list.rear;

	while (nodeIter->parent != 0)
	{
		printf("Command line : %s\n", nodeIter->command);
		nodeIter = nodeIter->parent;
	}
	printf("Command line : %s\n", nodeIter->command);
}

void executeCommands()
{
	pid_t child_pid;
	int status;		
	char** tokens;
	char* commandPtr = list.head->command;
	
	getTokens(&tokens, &commandPtr);

	//Parent
	if ((child_pid = fork()) != 0)
	{
		waitpid(child_pid,&status,0);	
		printf("*** Process %d Child process %d DONE with status %x\n\n",getpid(), child_pid, status);
		if(list.count > 1)
		{	
			list_headPop();
			executeCommands();
		}
		free_Tokens(tokens);	
	}
	//Child	
	else
	{
		printf("*** Process %d Executing Command %s\n",getpid(),list.head->command);
		execve(tokens[0], tokens, NULL);
		printf("ERROR - not executing command \"%s\"\n", tokens[0]);
	}
}

/******************************* SUB PROCEDURE  **************************************/

uchar terminationCheck(char* str)
{
	if(str[0] == '\0') return 1;
	else	return 0;
}

void getTokens(char ***tokens, char **commandPtr)
{	
	int i, j;				//for loop indexing
	int tokenLength;			//length of token
	int tokenNum = 0;			//count of token
	char* tokenPtr = 0;
	char* TKPtrTBL[MAX_PARAMETER] = { 0 };  //container which has adddress of each token
	int TKLengthTBL[MAX_PARAMETER] = { 0 }; //container which has length of each token

	while( *(*commandPtr) != '\0')
	{	
		if(*(*commandPtr) != ' ')
		{
			tokenLength = 0;
			
			TKPtrTBL[tokenNum] = *commandPtr;

			while( *(*commandPtr) != ' ' && *(*commandPtr) != '\0') { (*commandPtr)++; tokenLength++; }
		
			TKLengthTBL[tokenNum++] = tokenLength;
		}
		
		if(*(*commandPtr) != '\0') (*commandPtr)++;
	}

	*tokens = (char**)malloc(sizeof(char*)*tokenNum);
	for(i=0; i<tokenNum ; i++) (*tokens)[i] = (char*)malloc(sizeof(char)*(TKLengthTBL[i] + 1));	

	for(i= 0; i < tokenNum; i++)
	{
		tokenPtr = TKPtrTBL[i];
		for (j= 0; j < TKLengthTBL[i]; j++)
		{
			(*tokens)[i][j] = *tokenPtr;		
			tokenPtr++;				
		}
		(*tokens)[i][j] = '\0';
	}
}

void free_Tokens(char ** tokens)
{
	int i;
	int tokenNum = sizeof(tokens)/sizeof(char*);
	for(i = 0; i < tokenNum ; i++) free(tokens[i]);
	free(tokens);
}

/******************************* Linked List Library **********************************/

void list_Init()
{
	list.count = 0;
	list.head = NULL;
	list.rear = NULL;
}

void list_headPop()
{
	Node* nodePtr = 0;

	if(list.count > 1)
	{
		nodePtr = list.head -> link;
		(list.head -> link)->parent = NULL;
		free(list.head->command);
		free(list.head);
		list.head = nodePtr;
		(list.count)--;	
	}
	else if(list.count == 1)
	{
		free(list.head->command);
		free(list.head);
		list.head = 0;
		list.rear = 0;
		list.count = 0;
	}	
	else printf("List has no node\n");
}

void createNode(Node** nodePtr, char* str)
{
	char* _command;
	int strLength = strlen(str) + 1;
	
	*nodePtr = (Node*)malloc(sizeof(Node));
	(*nodePtr) -> command = (char*)malloc(strLength * sizeof(char));
	_command = (*nodePtr) -> command;
	(*nodePtr) -> parent = NULL;
	(*nodePtr) -> link = NULL;
	while(*str != '\0')
	{
		*_command = *str;
		_command++;
		str++;
	}
	*_command = '\0';	
}


void list_Insert(char* str)
{
	Node* nodePtr;
	createNode(&nodePtr, str);	

	if (list.head)					
	{
		list.rear->link = nodePtr;
		nodePtr->parent = list.rear;
		list.rear = list.rear->link;
	}

	else
	{
		list.head = nodePtr;
		list.rear = nodePtr;
	}

	(list.count)++;
}
