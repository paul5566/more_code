#include "get_cost.h"

struct node * create_node(float x, float y, int color)
{
	struct node *n = (struct node *)malloc(sizeof(struct node));
	if(n != NULL){
		n->x = x;
		n->y = y;
		n->color = color;
		n->next = NULL;
		n->pre = NULL;
	}else{
		printf("%s failed !!", __func__);
		exit(0);
	}
	
	return n;
}

/* n2 add after n1 */
void add_node(struct node *n1, struct node *n2)
{
	n1->next = n2;
	n2->pre = n1;
	n2->next = NULL;
}

/* n2 insert after n1, n2 is on left side*/
void _insert_node_left(struct node *n1, struct node *n2)
{
	// means n2 is not head
	if(n2->pre != NULL)
		n2->pre->next = n2->next;
	n2->next->pre = n2->pre;
	n1->next->pre = n2;
	n2->pre = n1;
	n2->next = n1->next;
	n1->next = n2;
}
/* n2 insert after n1, n1 is on right side */
void _insert_node_right(struct node *n1, struct node *n2)
{
	// means n2 is not tail
	if(n2->next != NULL)
		n2->next->pre = n2->pre;
	n2->pre->next = n2->next;
	n1->next->pre = n2;
	n2->pre = n1;
	n2->next = n1->next;
	n1->next = n2;
}

struct node * find_tail(struct node *n)
{
	while(n->next != NULL)
		n = n->next;
	
	return n;
}
	
struct node * find_head(struct node *n)
{
	while(n->pre != NULL)
		n = n->pre;
	
	return n;
}

/* n2 be new head, n1 is orginal head */
void _be_head_node(struct node *n1, struct node *n2)
{
	n2->pre->next = n2->next;
	n2->pre = NULL;
	n2->next = n1;
	n1->pre = n2;
}

/* swap n1<->n2 */
void swap(struct node *n1, struct node *n2)
{
	float tmp_x;
	float tmp_y;
	int tmp_color;

	tmp_x = n1->x;
	tmp_y = n1->y;
	tmp_color = n1->color;
	n1->x = n2->x;
	n1->y = n2->y;
	n1->color = n2->color;
	n2->x = tmp_x;
	n2->y = tmp_y;
	n2->color = tmp_color;
}

/* remove n1 next node */
void remove_node(struct node *n1)
{
	n1->next = n1->next->next;
	n1->next->next->pre = n1;
}

/* remove all the node */
void remove_all(struct node *head)
{
	if(head->next != NULL){
		remove_all(head->next);
	}
	
	free(head);
}

int _find_order(struct node *n, float data)
{
	int count = 0;

	while(n->next != NULL){
		if(n->x == data)
			return count;
		count++;
		n = n->next;
	}

	return -1;
}

int _find_all_num(struct node *n)
{
	int count = 0;
	
	while(n->next != NULL){
		count++;
		n = n->next;
	}
	
	return count;
}

void print_all(struct node *head)
{
	for(;head->next!=NULL; head = head->next){
		switch(head->color){
			case 1:
				printf("(%.1f, %.1f)<%s>---", 
					head->x, head->y, "R");
				break;
			case 2:
				printf("(%.1f,%.1f)<%s>---", 
					head->x, head->y, "G");
				break;
			case 3:
				printf("(%.1f,%.1f)<%s>---", 
					head->x, head->y, "Y");
				break;
			default:
				return;
		}
	}
	switch(head->color){
		case 1:
			printf("(%.1f,%.1f)<%s>", 
				head->x, head->y, "R");
			break;
		case 2:
			printf("(%.1f,%.1f)<%s>", 
				head->x, head->y, "G");
			break;
		case 3:
			printf("(%.1f,%.1f)<%s>", 
				head->x, head->y, "Y");
			break;
		default:
			return;
	}
	printf("\n");
}

void quick_sort(struct node *list, int start, int end)
{
	int i, l ,r;
	float pivot;
	struct node *base = list;
	struct node *tail = list;
	struct node *head = list;
	struct node *pHead;
	struct node *pTail;
	struct node *tmp;
	int center;

	if(end - start < 3)	// break condition
		return;
	
	// find the pivotal
	for(i = 0; i < end/2; i++){
		base = base->next;
	}
	pivot = base->x;
	// find tail
	for(i = 0; i < end; i++){
		tail = tail->next;
	}
	pHead = head;
	pTail = tail;	
	do{
		// left side
		for(l = start; l < end/2; l++){
			if(pHead->x > pivot){
				tmp = pHead;
				pHead = pHead->next;
				_insert_node_left(base, tmp);
			}else{
				pHead = pHead->next;
			}
		}
		// right side
		for(r = end; r > end/2; r--){
			if(pTail->x < pivot){
				tmp = pTail;
				pTail = pTail->pre;
				if(base->pre == NULL){
					_be_head_node(base, tmp);
					base->pre = tmp;
				}else{
					_insert_node_right(base->pre, tmp);
				}
			}else{
				pTail = pTail->pre;
			}
		}
	}while(0);

	center = _find_order(head, pivot);

	print_all(find_head(list));
	quick_sort(list, 0, center);	
	quick_sort(list, center+1, _find_all_num(list));
}

