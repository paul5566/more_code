#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "lla.h"

int Lla_init(struct lla_head **self, int *array)
{
	*self = (struct lla_head *)malloc(sizeof(struct lla_head));
	if(*self == NULL){
		printf("%s failed (%s)\n", __func__, strerror(errno));
		return -1;
	}

	memcpy((*self)->array, array, sizeof(int)*MAX_ARRAY);
	(*self)->tail = 0;
	for(int i = 0; i < MAX_NODE; ++i){
		(*self)->node[i].data_index = EMPTY;
	}
	
	return 0;
}

static int __findTailWithNoGuide(struct lla_head *head, int targ)
{
	return (head->node[targ].next == 0 && targ != head->tail);
}

static int _redefine_tail(struct lla_head *head)
{
	int i;
	
	for(i = 0; i < MAX_ARRAY; ++i){
		if(__findTailWithNoGuide(head, i)){
			head->node[head->tail].next = i;
			head->tail = i;
			return 0;
		}
	}
	
	return -1; //means this is tail node
}

static int _isLinkedListFull(struct lla_head *head)
{
	return (head->tail == -1);
}

int Lla_add(struct lla_head *head, int index)
{
	if(_isLinkedListFull(head)){
		printf("The linked-list is full!\n");
		return -1;
	}

	head->node[head->tail].data_index = index;
	
	if(_redefine_tail(head) == -1){
		head->node[head->tail].next = 0;
		head->tail = -1;
	}

	return 0;
}

static void _lla_swap(struct lla_node *a, struct lla_node *b)
{
	// move b to a
	a->next = b->next;
	a->data_index = b->data_index;
	// clean b
	b->next = 0;
	b->data_index = EMPTY;
}

static int _redefine_tail_path(struct lla_head *head)
{
	for(int i = 0; i < MAX_NODE; ++i){
		if(__findTailWithNoGuide(head, i)){
			head->node[i].next = head->tail;
			return 0;
		}
	}
	return -1;
}

static int _isRemoveTail(struct lla_head *head, int targ)
{
	return (head->node[targ].next == head->tail);
}

static int _findTargetData(struct lla_head *head, int i, int targ)
{
	return (head->array+head->node[i].data_index == head->array+targ);
}

int Lla_remove(struct lla_head *head, int targ)
{
	int i;

	for(i = 0; i < MAX_NODE; ++i){
		if(_findTargetData(head, i, targ)){
			if(_isLinkedListFull(head)){
				/*
				 * remove tail(not empty)
				 * while linked-list is full
				*/
				if(head->node[i].next == 0){
					head->node[i].next = 0;
					head->node[i].data_index = EMPTY;
					head->tail = i;
				/*
				 * normal remove
				 * while linked-list is full
				*/
				}else{
					head->tail = head->node[i].next;
					_lla_swap(&head->node[i],
						&head->node[head->node[i].next]);
					//need to fix the tail's path 
					int ret = _redefine_tail_path(head);
					assert(ret == 0);
				}
			/*
			 * remove tail(not empty)
			 * while linked-list is not full
			*/
			}else if(_isRemoveTail(head, i)){
				head->tail = i;
				_lla_swap(&head->node[i],
						&head->node[head->node[i].next]);
			/* normal remove */
			}else{
				_lla_swap(&head->node[i],
						&head->node[head->node[i].next]);
			}		
		}
	}
	
	return 0;
}

void Lla_display(struct lla_head *head, char *msg)
{
	int i = 0;

	printf("DISPLAY(%s)[tail %d]:\n", msg, head->tail);
	do{
		if(i == head->tail) {printf("#tb");break;}
	
		printf("<%d>:%d[%d]->%d\t", 
							i,
							head->array[head->node[i].data_index],
							head->node[i].data_index,
							head->node[i].next);
		i = head->node[i].next;

		if(head->tail == -1){
			if(i == 0) {printf("#fb");break;}
		}
	}while(1);
	printf("\n");
}
