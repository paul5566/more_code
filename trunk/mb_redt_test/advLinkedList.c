#include "shared.h"

int CheckIndex(int i, void * fdata, void * bdata)
{
	return (i == *(int *)fdata);
}

int LinkedList_Walk(int i, void * mData, void * brick)
{
	int tmpMsgLen = 0;
	struct host_param *monitor = (struct host_param *)brick;
	struct mbusWalkThroughData *mWalkData = (struct mbusWalkThroughData *)mData;
	int current = mWalkData->currentPointer;
	int remain_size = mWalkData->bufferSize - current;

	do{
		if(mWalkData->bufferSize <= 0 && 
			mWalkData->currentPointer >= mWalkData->bufferSize)
			break;

		tmpMsgLen += snprintf(&mWalkData->mString[current+tmpMsgLen], 
								remain_size, 
								"%d -> %d\n", 
								monitor->order,  
								monitor->val);

		if(tmpMsgLen < remain_size)
			 mWalkData->currentPointer += tmpMsgLen;
		else
			break;
	
		return 1;
	}while(0);

	return 0;
}

void AdvLinkedList_Remove(advLinkedList * listHeader, 
						CheckLinkedListFunc CheckFunction, 
						void * fdata)
{	
	int i = 0;
	int previous;

//	pthread_mutex_lock(&(listHeader->lock));
	do{
		/* 0 is always the pointer to the start; not a node*/
		previous = i;
		i = listHeader->List[i].mNext;
		if(CheckFunction(i, fdata, listHeader->List[i].pData)){
			if(listHeader->List[i].mNext == i){
				listHeader->List[previous].mNext = previous;
			}else{
				listHeader->List[previous].mNext = listHeader->List[i].mNext;
			}
			memset(&listHeader->List[i], 0, sizeof(ADVLINKEDLIST));
			break;
		}
	}while(listHeader->List[i].mNext != i);
//	pthread_mutex_unlock(&(listHeader->lock));
}
	
int AdvLinkedList_Add(advLinkedList * listHeader, void * data)
{
	int i = 0, j = 1, ret = 0;

//	pthread_mutex_lock(&(listHeader->lock));
	while(listHeader->List[i].mNext != i)
		i = listHeader->List[i].mNext;

	for(j = 1; listHeader->List[j].mNext != 0 && j < listHeader->TotalLength; j++);

	if(listHeader->List[j].mNext == 0){
		listHeader->List[i].mNext = listHeader->List[j].mNext = j;
		listHeader->List[j].pData = listHeader->DataBricks + (listHeader->DataBrickSize * (j -1));
		memcpy(listHeader->List[j].pData, data, listHeader->DataBrickSize);
		ret = j;
	}
//	pthread_mutex_unlock(&(listHeader->lock));

	return ret;
}

int AdvLinkedList_ForEach(advLinkedList * listHeader, 
							ForEachLinkedListFunc DoForEach, 
							void * fdata)
{
	int i = 0;
	int ret = 1;

//	pthread_mutex_lock(&(listHeader->lock));
	while(listHeader->List[i].mNext != i && ret)
	{
		i = listHeader->List[i].mNext;
		ret = DoForEach(i, fdata, listHeader->List[i].pData);
	}
//	pthread_mutex_unlock(&(listHeader->lock));

	return ret;
}

void AdvLinkedList_Init(advLinkedList * listHeader, 
						int size, 
						int brickSize)
{
//	pthread_mutex_init(&(listHeader->lock), NULL);
	listHeader->List = (ADVLINKEDLIST *)calloc(size+1, sizeof(ADVLINKEDLIST));
	listHeader->DataBricks = malloc(size * brickSize);
	listHeader->DataBrickSize = brickSize;
	listHeader->TotalLength = size + 1;
	listHeader->List[0].mNext = 0;
}

/*
int main(void)
{
	int ret;
	advLinkedList test_ll;
	char tmp[128];
	struct test data[SIZE];
	struct mbusWalkThroughData mWalkData;
	
	mWalkData.mString = tmp;
	mWalkData.bufferSize = 128;
	mWalkData.currentPointer = 0;	

	data[0].a = 1;
	data[1].a = 2;
	data[2].a = 3;
	data[3].a = 4;
	data[0].b = 11;
	data[1].b = 12;
	data[2].b = 13;
	data[3].b = 14;

	AdvLinkedList_Init(&test_ll, SIZE, sizeof(struct test));
	for(int i = 0; i < SIZE; ++i){
		ret = AdvLinkedList_Add(&test_ll, &data[i]);
	}
	
	printf("Add %d data into list\n", ret);
	printf("Data Walk:\n");
	AdvLinkedList_ForEach(&test_ll, PhilList_Walk, &mWalkData);
	printf("%s\n", mWalkData.mString);

	printf("Remove data[1]\n");	
	struct test *pData = &data[1];
	AdvLinkedList_Remove(&test_ll, MbusSlaveIPList_CheckIndex, (void *)pData);

	mWalkData.currentPointer = 0;
	printf("Data Walk:\n");
	AdvLinkedList_ForEach(&test_ll, PhilList_Walk, &mWalkData);
	printf("%s\n", mWalkData.mString);

	return 0;
}
*/
