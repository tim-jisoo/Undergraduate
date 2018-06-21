//Ji Soo Heo
//B115233
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define ENDMARK	-1
#define MAX_STAGE_NUM 2048

struct autoPart{
	int partNumber;
	struct autoPart *next;
};

struct autoPartBox{
	int bid;
	int size;
	int count;
	struct autoPart *lastPart;
	struct autoPart *firstPart;
	pthread_mutex_t mutex;
	pthread_cond_t full;
	pthread_cond_t empty;
};

struct stageArg{
	int sid;
	int defectNumber;
};

//standard procedures
void sendAutoPart(int id, struct autoPart *ap, struct autoPartBox *apBox);
struct autoPart* receiveAutoPart(int id, struct autoPartBox *apBox);
void* startThread(void *arg);
void* endThread(void* id);
void* stageThread(void * ptr);

//additional procedures
void createAndInit_apBox(int nstages, int boxSize);
void createAndInit_AutoPart(struct autoPart** apPtr, int nPart);
void eliminate_AutoPart(struct autoPart* apPtr);
int defectClassifier(int partNum, int defectNum);

//global variables
struct autoPartBox * AutoBox;
struct autoPart* head;
pthread_mutex_t headMutex;
pthread_barrier_t barrier;

int main(int argc, char*argv[])
{	
	pthread_t startTid, endTid, stageTid[MAX_STAGE_NUM];
	void * status;
	int i, nStages, boxSize, nPart, lastStage;
	struct stageArg* arg;
	long int startThreadSum = 0;
	long int endThreadSum = 0;
	long int stageThreadSum = 0;

	if(argc <5) exit(0);	
	
	/*********************  initialize system parameters  ***************/
	
	nStages = atoi(argv[1]);
	if(argc-4 != nStages) exit(0);
	boxSize = atoi(argv[2]);
	nPart = atoi(argv[3]);
	lastStage = nStages+1;
	arg = (struct stageArg*)malloc(sizeof(struct stageArg)*nStages);
	for(i=0; i<nStages; i++){ arg[i].sid = i+1; arg[i].defectNumber = atoi(argv[i+4]);}
	startThreadSum = endThreadSum = stageThreadSum = 0;
	pthread_mutex_init(&headMutex,NULL);
	pthread_barrier_init(&barrier,NULL,nStages+3);
	srand(100);
	
	/************************    Start simulation	 ************************/
	
	//1. Create AutoPartBox
	createAndInit_apBox(nStages, boxSize);	
	
	//2. Create stage Threads
	pthread_create(&startTid, NULL, startThread,(void*)&nPart);
	pthread_create(&endTid, NULL, endThread,(void*)&lastStage);
	for(i=0; i<nStages; i++) pthread_create(&stageTid[i], NULL, stageThread, (void*)&arg[i]);

	//3. <Barrier> Main Thread & Start Thread & Stage threads & End Thread
	pthread_barrier_wait(&barrier);

	//***********************   Termination simulation   *********************/

	//Free heap data
	free(AutoBox);
	free(arg);

	//initialize total staus and print out total status to console
	printf("*** Part Sum Information ***\n");
	
	pthread_join(startTid, &status);
	printf("startThread sum %ld\n", status);
	startThreadSum = (long int)status;
	
	pthread_join(endTid, &status); 
	printf("endThread sum %ld\n", status);
	endThreadSum = (long int)status;
	
	for(i=0; i < nStages; i++)
	{
		pthread_join(stageTid[i], &status);
		printf("Stage %d sum %ld\n",i, status);	
		stageThreadSum += (long int)status;
	}

	//system error check
	assert(startThreadSum == (endThreadSum + stageThreadSum));
	pthread_exit(0);
}

void* startThread(void * arg)
{
	int myid = 0;
	int afterBox = myid, i;
	int nPart = *((int *)arg);
	long int weightedID = 0;
	struct autoPart* item;


	//create & initialize AutoPart
	createAndInit_AutoPart(&item, nPart+1);

	//initialize list head (global variable)
	head = item;

	//send AutoPart to AutoPartBox
	while(1)
	{
		if((item->partNumber) != ENDMARK)
		{
			weightedID += item -> partNumber;
			printf("Start Thread Stage %d sending autoPart %d to autoPartBox %d\n", myid, item->partNumber, afterBox);
			sendAutoPart(myid,item, &AutoBox[afterBox]);
			item = item -> next;
		}
		else
		{
			printf("Start Thread Stage %d sending ENDMARK to autoPartBox %d\n",myid,afterBox);
			sendAutoPart(myid,item,&AutoBox[afterBox]);
			break;
		}
	}

	//terminate transaction
	pthread_barrier_wait(&barrier);
	pthread_exit((void*)weightedID);
}

void* endThread(void * arg)
{
	int myid = *((int*)arg);
	int beforeBox = myid-1;
	long int weightedID=0;
	struct autoPart* item;

	//receive AutoPart from AutoPartBox and eliminate (free) it	
	while(1)
	{	
		item = receiveAutoPart(myid,&AutoBox[beforeBox]);
		if( (item->partNumber) == ENDMARK )
		{
			printf("End Thread Stage %d receiving ENDMARK from autoPartBox %d\n",myid,beforeBox);
			eliminate_AutoPart(item);
			break;
		}
		printf("End Thread Stage %d receiving autoPart %d from autoPartBox %d\n",myid,item->partNumber,beforeBox);
		weightedID += item -> partNumber;
		eliminate_AutoPart(item);
	}

	//terminate transaction
	pthread_barrier_wait(&barrier);
	pthread_exit((void*)weightedID);
}

void* stageThread(void* arg)
{
	struct stageArg param = *((struct stageArg*)arg);
	int defectNumber = param.defectNumber;
	int myid = param.sid;
	int beforeBox = myid-1;
	int afterBox = myid;
	long int weightedID = 0;
	struct autoPart* item;
	
	//autoPart transaction
	while(1) 
	{
		//receive AutoPart from AutoPartBox 
		item = receiveAutoPart(myid, &AutoBox[beforeBox]);
		if(item->partNumber == ENDMARK) printf("Stage %d receiving ENDMARK from autoPartBox %d\n",myid,beforeBox);	
		else 				printf("Stage %d receiving autoPart %d from autoPartBox %d\n",myid,item->partNumber,beforeBox);

		//check if AutoPart has defect or not
		if(defectClassifier(item -> partNumber, defectNumber))
		{
			weightedID += item -> partNumber;
			printf("Stage %d deleting autoPart %d\n",myid,item->partNumber);
			eliminate_AutoPart(item);
			continue;
		}
		
		//send AutoPart to AutoPartBox
		sendAutoPart(myid,item,&AutoBox[afterBox]);
		if(item->partNumber == ENDMARK)
		{
			printf("Stage %d sending ENDMARK to autoPartBox %d\n",myid,afterBox);
			break;
		}
		else printf("Stage %d sending autoPart %d to autoPartbox %d\n",myid,item->partNumber,afterBox);
	}

	//terminate transaction
	pthread_barrier_wait(&barrier);
	pthread_exit((void*)weightedID);
}

struct autoPart* receiveAutoPart(int id, struct autoPartBox *apBox)
{

	pthread_mutex_t * mutex = &(*apBox).mutex;
	pthread_cond_t * full = &(*apBox).full;
	pthread_cond_t * empty = &(*apBox).empty;
	struct autoPart* item;

	//Check if autoPartBox is empty if not empty
	pthread_mutex_lock(mutex);
	while((*apBox).count == 0)
	{
		printf("RECEIVE:: Stage %d waiting on autoPartBox %d empty\n", id, apBox->bid);
		pthread_cond_wait(empty,mutex);	
	}

	//Cause autoPartBox is not empty, the stage can receive a autoPart from Box
	item = (*apBox).firstPart;
	if( (*apBox).count == 1)
	{
		(*apBox).firstPart = NULL;
		(*apBox).lastPart = NULL;
	}
	else (*apBox).firstPart = item -> next;
	(*apBox).count--;
	printf("RECEIVE:: Stage %d receiving autoPart %d from autoPartBox %d\n", id, item->partNumber, apBox->bid);
	
	//Signals to before stage
	printf("RECEIVE:: Stage %d signals autoPartBox %d NOT full\n", id, apBox->bid);
	pthread_cond_signal(full);
	pthread_mutex_unlock(mutex);	

	//return val
	return item;
}

void sendAutoPart(int id, struct autoPart *item, struct autoPartBox *apBox)
{
	pthread_mutex_t * mutex = &(*apBox).mutex;
	pthread_cond_t * full = &(*apBox).full;
	pthread_cond_t * empty = &(*apBox).empty;

	//Check if autoPartBox is full
	pthread_mutex_lock(mutex);	
	while((*apBox).count == (*apBox).size)
	{	
		printf("SEND:: Stage %d thread waiting on autoPartBox %d full \n",id,apBox->bid);
		pthread_cond_wait(full,mutex);
	}

	//Cause autoPartBox is not full, the stage can send a autoPart to Box
	if( (*apBox).count == 0)
	{
		(*apBox).firstPart = item;
		(*apBox).lastPart = item;
	}
	else (*apBox).lastPart = item;	
	(*apBox).count++;
	printf("SEND:: Stage %d sending autoPart %d to autoPartBox %d\n",id,apBox->lastPart->partNumber, apBox->bid);
	
	//Signal to next stages
	printf("SEND:: Stage %d signals autoPartBox %d NOT empty\n",id,apBox->bid);
	pthread_cond_signal(empty);
	pthread_mutex_unlock(mutex);
}

int defectClassifier(int partNum, int defectNum)
{
	if(partNum == ENDMARK) return 0;
	if(partNum % defectNum == 0) return 1;
	else return 0;
}

void createAndInit_AutoPart(struct autoPart** apPtr, int nPart)
{
	if(nPart)
	{
		*apPtr = (struct autoPart*)malloc(sizeof(struct autoPart));
		if(nPart>1) (*apPtr)-> partNumber = abs(rand()); 
		else (*apPtr)->partNumber = ENDMARK;

		(*apPtr)-> next = NULL;

		nPart--;
		createAndInit_AutoPart(&(*apPtr)->next,nPart);
	}
}

void eliminate_AutoPart(struct autoPart* apPtr)
{
	struct autoPart* iter;

	pthread_mutex_lock(&headMutex);	
	iter = head;
	if(apPtr == iter)
	{
		head = apPtr->next;
		free(apPtr);	
	}
	else
	{
		while(iter->next != apPtr) iter = iter -> next;			
		iter -> next = (apPtr -> next);
		free(apPtr);
	}
	pthread_mutex_unlock(&headMutex);		
}

void createAndInit_apBox(int nStages, int boxSize)
{
	int i;
	int boxNum = nStages+1;

	AutoBox = (struct autoPartBox*)malloc(sizeof(struct autoPartBox)* boxNum);
	
	for(i=0 ; i<boxNum ; i++)
	{
		AutoBox[i].bid = i;
		AutoBox[i].size = boxSize;
		AutoBox[i].count = 0;
		AutoBox[i].firstPart = NULL;
		AutoBox[i].lastPart = NULL;
		pthread_mutex_init(&(AutoBox[i].mutex),NULL);
		pthread_cond_init(&(AutoBox[i].full),NULL);
		pthread_cond_init(&(AutoBox[i].empty),NULL);
	}
}

