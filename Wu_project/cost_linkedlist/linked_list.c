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

int _find_order(struct node *n, float x)
{
	int count = 0;
	
	while(n->next != NULL){
		if(n->x == x)   
			return count;
		count++;
		n = n->next;
	}
	if(count == 0)
		return 0;
	else
		return -1;
}

int _find_all_num(struct node *n)
{
	int count = 0;

	while(n->pre != NULL)
		n = n->pre;
	
	while(n->next != NULL){
		count++;
		n = n->next;
	}
	
	return count;
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
void _remove_node(struct node *n1)
{
	n1->pre->next = n1->next;
	n1->next->pre = n1->pre;
	free(n1);
}

/* remove all the node */
void remove_all(struct node *head)
{
	if(head->next != NULL)
		remove_all(head->next);
	
	free(head);
}

/* remove the duplicate node */
void remove_dup_node(struct node *head, int *node_num)
{
	for(;head->next!=NULL; head = head->next){
		if(head->x == head->next->x &&
			head->y == head->next->y)
		{
			_remove_node(head->next);
			*node_num -= 1;
		}
	}
}

void print_all(struct node *head)
{
	head = find_head(head);

	for(;head->next!=NULL; head = head->next){
		switch(head->color){
			case 1:
				TRACE("(%.1f, %.1f)<%s>---", 
					head->x, head->y, "R");
				break;
			case 2:
				TRACE("(%.1f,%.1f)<%s>---", 
					head->x, head->y, "G");
				break;
			case 3:
				TRACE("(%.1f,%.1f)<%s>---", 
					head->x, head->y, "Y");
				break;
			default:
				return;
		}
	}
	switch(head->color){
		case 1:
			TRACE("(%.1f,%.1f)<%s>", 
				head->x, head->y, "R");
			break;
		case 2:
			TRACE("(%.1f,%.1f)<%s>", 
				head->x, head->y, "G");
			break;
		case 3:
			TRACE("(%.1f,%.1f)<%s>", 
				head->x, head->y, "Y");
			break;
		default:
			return;
	}
	TRACE("\n");
}

void quick_sort(struct node *list, int start, int end)
{
	int i, l ,r, num;
	float pivot;
	struct node *base = list;
	struct node *tail = list;
	struct node *head = list;
	struct node *pHead;
	struct node *pTail;
	struct node *tmp;
	int sub_center;		// sub list center
	int main_center;	// main list center

	num = end - start;
	if(num < 2){	// <= 2 data, break condication
		if(num == 1){	// if 2 data, compare
			if(list->x > list->next->x)
				swap(list, list->next);
		}
		return;
	}

	// find the pivotal
	for(i = 0; i < num/2; i++){
		if(base->next != NULL)
			base = base->next;
	}
	pivot = base->x;

	// find tail
	for(i = 0; i < num; i++){
		if(tail->next != NULL){
			tail = tail->next;
		}
	}
	pHead = head;
	pTail = tail;

#ifdef DEBUG
	printf("before sorting:\n");
#endif
	print_all_m(list);
#ifdef DEBUG
	printf("head %.1f, Pivot %.1f, Tail %.1f\n", head->x, pivot, tail->x);
#endif
	do{
		// left side
		for(l = start; l <= start+(num/2); l++){
			if(pHead->x > pivot){
#ifdef DEBUG
				printf("left: %.1f > pivot(%.1f), l(%d), cond(%d)\n", pHead->x, pivot, l, start+(num/2));
#endif
				/*
				 * To relocate the `sub-list` head pointer,
				 * because insert node in front of head pointer
				 * will let the pointer lost !
				*/
				if(list->x == pHead->x) // change sub-list head, relocate `list` pointer !!
					list = pHead->next;
				tmp = pHead;
				pHead = pHead->next;
				_insert_node_left(base, tmp);
			}else{
				pHead = pHead->next;
			}
		}
		// right side
		for(r = end; r >= end-(num/2); r--){
			if(pTail->x < pivot){
#ifdef DEBUG
				printf("right: %.1f < pivot(%.1f), r(%d), cond(%d)\n", pTail->x, pivot, r, end-(num/2));
#endif
				tmp = pTail;
				pTail = pTail->pre;
				_insert_node_right(base->pre, tmp);
				/*
				 * To relocate the `sub-list` head pointer,
				 * because insert node in front of head pointer
				 * will let the pointer lost !
				*/
				if(list->x == pivot)
					list = tmp;
			}else{
				pTail = pTail->pre;
			}
		}
	}while(0);
#ifdef DEBUG
	printf("After sort:\n");	
#endif
	print_all_m(list);
	base = list;
	sub_center = _find_order(list, pivot);
	main_center = _find_order(find_head(list), pivot);
#ifdef DEBUG
	printf("list %.1f, sub-center %d, main-center %d\n", list->x, sub_center, main_center);
#endif
	
	for(i = 0; i < sub_center+1; i++){
		if(base->next != NULL){
			base = base->next;
		}
	}
	// left recursive
#ifdef DEBUG
	printf("left, base %.1f, start %d, end %d\n", list->x, start, main_center-1);
#endif
	quick_sort(list, start, main_center-1);	// pivot is cut node, so end = main_center-1
	// right recursive
#ifdef DEBUG
	printf("right, base %.1f, start %d, end %d\n", base->x, main_center+1, end);
#endif
	quick_sort(base, main_center+1, end);	// pivot is cut node, so start = main_center+1
}

