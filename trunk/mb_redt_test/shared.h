#ifndef _SHARED_H
#define _SHARED_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>

#define errBreak(msg, args...) \
	{printf(""msg, ##args); break;}
#define errContinue(msg, args...) \
	{printf(""msg, ##args); continue;}
#define errReturn(msg, args...) \
	{printf(""msg, ##args); return -1;}
#define errReturn_en(en, msg)	\
	do{errno = en; perror(msg); exit(EXIT_FAILURE);}while(0);

#define LOCK(st)	\
	do{if(shared_test_lock(st) == -1) exit(0);}while(0);
#define UNLOCK(st, order) \
	do{if(shared_test_unlock(st) == -1) exit(0);}while(0);

#define CLEAR -2
#define SHOW -1
#define HOST 0
#define GUEST 1
#define SIZE 4

struct host_param{
	int val;
	int order;
};

struct shared_member{
	int val;
	pthread_mutex_t lock;
};

struct shared_test{
	int ctrl;
	struct shared_member sm[SIZE];
};

static inline int shared_mutex_init(pthread_mutex_t *lock)
{
	int ret = -1;
	pthread_mutexattr_t attr;
	
	do{
		if(pthread_mutexattr_init(&attr) != 0)
			errBreak("pthread_mutexaddr_init failed!\n");

		if(pthread_mutexattr_setpshared(&attr, 
					PTHREAD_PROCESS_SHARED) != 0)
			errBreak("pthread_mutexattr_setpshared failed!\n");
		
		if(pthread_mutexattr_setrobust(&attr, 
						PTHREAD_MUTEX_ROBUST) != 0)
			errBreak("pthread_mutexattr_setrobust failed!\n");
	
		if(pthread_mutex_init(lock, &attr) != 0)
			errBreak("pthread_mutex_init failed!\n");
	
		ret = 0;
	}while(0);
	
	pthread_mutexattr_destroy(&attr);
	
	return ret;
}	

static inline int 
shared_test_alloc(int role, struct shared_test **self)
{
	int id;
	key_t key = 0x4a899451;

	if(role == HOST){
		if((id = shmget(key, 1024, 
						IPC_CREAT|IPC_EXCL|key)) == -1)
		{
			errReturn("Host getting SHM id failed(%s)\n", 
										strerror(errno));
		}
	}else{
		if((id = shmget(key, 1024, key)) == -1)
			errReturn("Guest getting SHM id failed(%s)\n", 
										strerror(errno));
	}

	*self = shmat(id, (void *)0, 0);
	if(*self == MAP_FAILED)
		errReturn("shmat failed (%s)\n", strerror(errno));

	if(role == HOST){
		for(int i = 0; i < SIZE; ++i){
			if(shared_mutex_init(&(*self)->sm[i].lock) == -1)
				return -1;
			(*self)->sm[i].val = i+1;
		}
		(*self)->ctrl = 0;
	}
	
	return 0;
}

static inline int 
shared_test_lock(struct shared_member *sm)
{
	int ret;
	pthread_mutex_t *lock = sm->lock;

//	assert(i > 0 && i <= SIZE);	
	
	ret = pthread_mutex_lock(lock);
	if(ret == EOWNERDEAD){
		printf("A process dead while holding lock!\n");
		pthread_mutex_consistent(lock);
		ret = 0;
	}
	
	return ret;
}

static inline int 
shared_test_unlock(struct shared_member *sm)
{
	pthread_mutex_t *lock = sm->lock;
	
//	assert(i > 0 && i <= SIZE);
	
	return pthread_mutex_unlock(lock);
}

static inline int 
shared_test_detach(struct shared_test *st)
{
	return shmdt(st);
}

/* Advantech LinkedList */

struct mbusWalkThroughData{
	char *mString;
	int bufferSize;
	int currentPointer;
};

typedef struct __ADV_LINKEDLIST{
	int mNext;
	void *pData;
}ADVLINKEDLIST;

typedef struct{
	ADVLINKEDLIST *List;
	int TotalLength;
	void *DataBricks;
	int DataBrickSize;
	pthread_mutex_t lock;
}advLinkedList;

typedef int (*CheckLinkedListFunc)(int i,
									void *fdata,
									void *bdata);
typedef int (*ForEachLinkedListFunc)(int i,
									void *fdata,
									void *bdata); 

int CheckIndex(int i, void * fdata, void * bdata);
int LinkedList_Walk(int i, void * mData, void * brick);
void AdvLinkedList_Remove(advLinkedList * listHeader,
							CheckLinkedListFunc CheckFunction,
							void * fdata);
int AdvLinkedList_Add(advLinkedList * listHeader, void * data);
int AdvLinkedList_ForEach(advLinkedList * listHeader,
							ForEachLinkedListFunc DoForEach,
							void * fdata);
void AdvLinkedList_Init(advLinkedList * listHeader,
						int size,
						int brickSize);

#endif
