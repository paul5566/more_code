#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lla.h"

#define TEST_LOGO(msg)\
	do{printf("\n*****************************\n");\
	printf("\n\tStart %s\n", msg);\
	printf("\n*****************************\n");}while(0);


int table[MAX_ARRAY] = {2, 4, 6, 8, 10, 12, 14, 16};

/*
 * add: 0, 1, 2, 3, 4, 5, 6, 7
 * remove: 7, 6, 5, 4, 3, 2, 1 ,0
*/
void test1(struct lla_head *head)
{
	TEST_LOGO(__func__);
	
	for(int i = 0; i < 8; ++i)
		ADD(head, i);
	
	for(int i = 7; i >= 0; --i)
		REMOVE(head, i);
}

/*
 * add: 7, 6, 5, 4, 3, 2, 1, 0
 * remove 6, 4, 3, 1
 * remove 7, 5, 2, 0
*/
void test2(struct lla_head *head)
{
	TEST_LOGO(__func__);
	
	for(int i = 7; i >= 0; --i)
		ADD(head, i);

	REMOVE(head, 6);	
	REMOVE(head, 4);	
	REMOVE(head, 3);	
	REMOVE(head, 1);	
	REMOVE(head, 7);	
	REMOVE(head, 5);	
	REMOVE(head, 2);
	REMOVE(head, 0);
}
/*
 * add: 0, 1, 2
 * remove: 1, 2
 * add: 1, 2, 3
 * remove: 2, 3
 * add: 2, 3, 4
 * remove: 3, 4
 * add: 3, 4, 5
 * remove: 4, 5
 * add: 4, 5, 6
 * remove: 5, 6
 * add: 5, 6, 7
 * remove all
*/
void test3(struct lla_head *head)
{
	TEST_LOGO(__func__);
	
	ADD(head, 0);
	ADD(head, 1);
	ADD(head, 2);
	REMOVE(head, 1);
	REMOVE(head, 2);
	printf("\n");
	
	ADD(head, 1);
	ADD(head, 2);
	ADD(head, 3);
	REMOVE(head, 2);
	REMOVE(head, 3);
	printf("\n");

	ADD(head, 2);
	ADD(head, 3);
	ADD(head, 4);
	REMOVE(head, 3);
	REMOVE(head, 4);
	printf("\n");

	ADD(head, 3);
	ADD(head, 4);
	ADD(head, 5);
	REMOVE(head, 4);
	REMOVE(head, 5);
	printf("\n");

	ADD(head, 4);
	ADD(head, 5);
	ADD(head, 6);
	REMOVE(head, 5);
	REMOVE(head, 6);
	printf("\n");
	
	ADD(head, 5);
	ADD(head, 6);
	ADD(head, 7);
	REMOVE(head, 6);
	REMOVE(head, 7);
	
	REMOVE(head, 0);
	REMOVE(head, 1);
	REMOVE(head, 2);
	REMOVE(head, 3);
	REMOVE(head, 4);
	REMOVE(head, 5);
}

/*
 * add: 0, 1
 * remove: 0
 * add: 2, 3
 * remove: 1
 * add: 4, 5
 * remove: 2
 * add: 6, 7
 * remove: 3, 4, 5, 6, 7
*/
void test4(struct lla_head *head)
{
	TEST_LOGO(__func__);
	
	ADD(head, 0);
	ADD(head, 1);
	REMOVE(head, 0);
	printf("\n");
	
	ADD(head, 2);
	ADD(head, 3);
	REMOVE(head, 1);
	printf("\n");

	ADD(head, 4);
	ADD(head, 5);
	REMOVE(head, 2);
	printf("\n");

	ADD(head, 6);
	ADD(head, 7);
	REMOVE(head, 3);
	printf("\n");

	REMOVE(head, 4);
	REMOVE(head, 5);
	REMOVE(head, 6);
	REMOVE(head, 7);
	printf("\n");
}

/*
 * Add: 0~7
 * rm: 7
 * Add: 7
 * rm: 6
 * Add: 6
 * rm: 5
 * add: 5
 * rm: 4
 * add: 4
 * rm: 0~7
 */ 
void test5(struct lla_head *head)
{
	TEST_LOGO(__func__);
	
	for(int i = 0; i < MAX_NODE; ++i)
		ADD(head, i);
	printf("\n");
	REMOVE(head, 7);
	printf("\n");
	ADD(head, 7);
	REMOVE(head, 6);
	printf("\n");
	ADD(head, 6);
	REMOVE(head, 5);
	printf("\n");
	ADD(head, 5);
	REMOVE(head, 4);
	ADD(head, 4);
	for(int i = 0; i < MAX_NODE; ++i)
		REMOVE(head, i);
}	

int main(void)
{
	struct lla_head *head;

	if(Lla_init(&head, table) == -1) return -1;

	test1(head);

	test2(head);

	test3(head);

	test4(head);

	test5(head);
	
	free(head);

	return 0;
}
