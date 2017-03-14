#include <stdio.h>
#include <stdlib.h>

struct node{
	int value;
	struct node *next;
};

void show_list(struct node *head)
{
	while(head){
		printf("%d -> ", head->value);
		if(head->next == NULL)
			printf("NULL");
		head = head->next;
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
			*/
			(*ppHead) = (*ppHead)->next;
		}else{
			ppHead = &((*ppHead)->next);
		}
	}
	
	printf("Remove %d:\n", remove);
	show_list(head);
	
	return 0;
}
