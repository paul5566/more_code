#include <stdio.h>
#include <stdlib.h>

struct node{
	int value;
	struct node *next;
};

void show_list(struct node *head)
{
	struct node *tmp = head;

	while(tmp){
		printf("%d -> ", tmp->value);
		if(tmp->next == NULL)
			printf("NULL");
		tmp = tmp->next;
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	int remove;

	if(argc != 2){
		printf("Please enter a number you want to remove in linkedlist\n");
		exit(0);
	}

	remove = atoi(argv[1]);
	
	struct node *head = &((struct node){1, 
							&(struct node){2,
							&(struct node){3, 
							&(struct node){4,
							&(struct node){5,
							NULL}}}}});
	printf("Initial:\n");
	show_list(head);	
	
	struct node **ppHead = &head;
	while(*ppHead){
		if((*ppHead)->value == remove){
			(*ppHead)->value = 0;
			/*
			 * Substitution：
			 * *ppHead = head->next->....(based on passing)
			 * (head->next->...) = (head->next->next...)->next
			 * thus jump over the remove node
			 *
			 * eg: remove 3
			 *
			 * -----    -----    -----    -----    -----
			 * | 1 | -> | 2 | -> | 3 | -> | 4 | -> | 5 |
			 * -----    -----    -----    -----    -----
			 *                     ^
			 *                     |
			 * @(*ppHead)----------|
			 *
			 * *ppHead is point to node(3), which means *ppHead = head->next->next(2)->next(3),
			 * then susbsitute to node(4), such like copy, copy node(4) to node(3), so the
			 * next of node(2) is node(4)
			*/
			(*ppHead) = (*ppHead)->next;
			/*
			 * After that:
			 * -----    -----    -----    
			 * | 1 | -> | 2 | _  | 3 |  
			 * -----    ----- |  -----    
			 *                |        
			 *                |  -----    -----
			 *                |->| 4 | -> | 5 |
			 *                   -----    -----
			 *                     ^
			 *                     |
			 * @(*ppHead)----------|
			 */
			break;
		}else{
			ppHead = &((*ppHead)->next);
		}
	}
	
	printf("Remove %d:\n", remove);
	show_list(head);
	
	return 0;
}
