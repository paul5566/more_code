#include "linked_list.h"

struct node * create_node(int data)
{
	struct node *n = (struct node *)malloc(sizeof(struct node));
	if(n != NULL){
		n->data = data;
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
}

/* n2 insert after n1, n2 is on left side*/
void insert_node_left(struct node *n1, struct node *n2)
{
	n2->pre->next = n2->next;
	n2->next->pre = n2->pre;
	n1->next->pre = n2;
	n2->pre = n1;
	n2->next = n1->next;
	n1->next = n2;
}
/* n2 insert after n1, n1 is on right side */
void insert_node_right(struct node *n1, struct node *n2)
{
	n2->next->pre = n2->pre;
	n2->pre->next = n2->next;
	n1->next->pre = n2;
	n2->pre = n1;
	n2->next = n1->next;
	n1->next = n2;
}

/* swap n1<->n2 */
void swap(struct node *n1, struct node *n2)
{
	int tmp;
	
	tmp = n1->data;
	n1->data = n2->data;
	n2->data = tmp;
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

void print_all(struct node *head)
{
	head = find_head(head);

	for(;head->next!=NULL; head = head->next){
		printf(" %d |", head->data);
	}
	printf(" %d", head->data);
	printf("\n");
}

int find_order(struct node *n, int data)
{
	int count = 0;

	while(n->next != NULL){
		if(n->data == data)
			return count;
		count++;
		n = n->next;
	}
	if(count == 0)
		return 0;
	else
		return -1;
}

struct node * find_head(struct node *n)
{
	while(n->pre != NULL)
		n = n->pre;
	
	return n;
}

int find_all_num(struct node *n)
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

void quick_sort(struct node *list, int start, int end)
{
	int i, l ,r;
	int pivot, num;
	struct node *base = list;
	struct node *tail = list;
	struct node *head = list;
	struct node *pHead;
	struct node *pTail;
	struct node *tmp;
	int sub_center;		// sub list center;
	int main_center;	// main list center

	num = end - start;
	if(num < 2){	// <= 2 data, break condication
		if(num == 1){	// if 2 data, compare
			if(list->data > list->next->data)
				swap(list, list->next);
		}
		return;
	}
	
	// find the pivotal
	for(i = 0; i < num/2; i++){
		if(base->next != NULL){
			base = base->next;
		}
	}
	pivot = base->data;

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
	printf("head %d, Pivot %d, Tail %d\n", head->data, pivot, tail->data);
#endif
	do{
		// left side
		for(l = start; l <= start+(num/2); l++){
			if(pHead->data > pivot){
#ifdef DEBUG
				printf("left: %d > pivot(%d), l(%d), cond(%d)\n", pHead->data, pivot, l, start+(num/2));
#endif
				/*
				 * To relocate the `sub-list` head pointer,
				 * because insert node in front of head pointer
				 * will let the pointer lost !
				*/
				if(list->data == pHead->data)// change sub-list head, relocate `list` pointer !!
					list = pHead->next;
				tmp = pHead;
				pHead = pHead->next;
				insert_node_left(base, tmp);
			}else{
				pHead = pHead->next;
			}
		}
		// right side
		for(r = end; r >= end-(num/2); r--){
			if(pTail->data < pivot){
#ifdef DEBUG
				printf("right: %d < pivot(%d), r(%d), cond(%d)\n", pTail->data, pivot, r, end-(num/2));
#endif
				tmp = pTail;
				pTail = pTail->pre;
				insert_node_right(base->pre, tmp);
				/*
				 * To relocate the `sub-list` head pointer,
				 * because insert node in front of head pointer
				 * will let the pointer lost !
				*/
				if(list->data == pivot)
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
	sub_center = find_order(list, pivot);
	main_center = find_order(find_head(list), pivot);
#ifdef DEBUG
	printf("list %d, sub-center %d, main-center %d\n", list->data, sub_center, main_center);
#endif
	
	for(i = 0; i < sub_center+1; i++){
		if(base->next != NULL){
			base = base->next;
		}
	}
	
	// left recursive
#ifdef DEBUG
	printf("left, base %d, start %d, end %d\n", list->data, start, main_center-1);
#endif
	quick_sort(list, start, main_center-1);	// pivot is cut node, so end = main_center-1
	// right recursive
#ifdef DEBUG
	printf("right, base %d, start %d, end %d\n", base->data, main_center+1, end);
#endif
	quick_sort(base, main_center+1, end);	// pivot is cut node, so start = main_center+1
}

/* 
 * arc4rand : a better random number
*/
/*
int urandom_fd = -2;

void urandom_init(void)
{
	printf("arc4rand init....\n");
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if(urandom_fd == -1){
		int errsv = urandom_fd;
		printf("Error opening [/dev/urandom]: %i\n", errsv);
		exit(1);
	}
}

unsigned long urandom(void)
{
	unsigned long buf_impl;
	unsigned long *buf = &buf_impl;

	if(urandom_fd == -2){
		printf("You have to call urandom_init() before urandom()");
		exit(1);
	}
	// Read 4 bytes, or 32 bits into *buf, which points to buf_impl 
	read(urandom_fd, buf, sizeof(long));

	return buf_impl;
}
*/

int check_res(struct node *n)
{
	int d1, d2, d3, d4, d5, d6, d7, d8;
	
	d1 = n->data;
	d2 = n->next->data;
	d3 = n->next->next->data;
	d4 = n->next->next->next->data;
	d5 = n->next->next->next->next->data;
	d6 = n->next->next->next->next->next->data;
	d7 = n->next->next->next->next->next->next->data;
	d8 = n->next->next->next->next->next->next->next->data;

	if((d1>d2) || (d1>d3) || (d1>d4) || (d1>d5) 
		|| (d1>d6) || (d1>d7) || (d1>d8))
		return -1;

	if((d2>d3) || (d2>d4) || (d2>d5) || (d2>d6) || (d2>d7) || (d2>d8))
		return -2;
	
	if((d3>d4) || (d3>d5) || (d3>d6) || (d3>d7) || (d3>d8))
		return -3;

	if((d4>d5) || (d4>d6) || (d4>d7) || (d4>d8))
		return -4;

	if((d5>d6) || (d5>d7) || (d5>d8))
		return -5;

	if((d6>d7) || (d6>d8))
		return -6;
	
	if(d7>d8)
		return -7;

	return 0;
}
	
#ifdef DEBUG
int main(void)
{
	struct node *A = create_node(5);
	struct node *H = create_node(99);
	
	struct node *B = create_node(85);
	struct node *C = create_node(7);
	struct node *D = create_node(89);
	struct node *E = create_node(77);
	struct node *F = create_node(33);
	struct node *G = create_node(82);
		
	add_node(A, B);
	add_node(B, C);
	add_node(C, D);
	add_node(D, E);
	add_node(E, F);
	add_node(F, G);
	add_node(G, H);

	quick_sort(A, 0, 7);
	printf("Final :\n");
	print_all(find_head(A));

	return 0;
}

#else

int main(int argc, char **argv)
{
	int res;
	int i, count = 1;
	int src[6] = {0};
	int tmp;

	struct node *A = create_node(5);
	struct node *H = create_node(99);

	do{
		for(i = 0; i < 6; i++){
			do{
				tmp = arc4random()%90;
				if(tmp <= 5)
					continue;
				if(tmp == src[0])
					continue;
				if(tmp == src[1])
					continue;
				if(tmp == src[2])
					continue;
				if(tmp == src[3])
					continue;
				if(tmp == src[4])
					continue;
				if(tmp == src[5])
					continue;
				break;
			}while(1);
			src[i] = tmp;
		}
		struct node *B = create_node(src[0]);
		struct node *C = create_node(src[1]);
		struct node *D = create_node(src[2]);
		struct node *E = create_node(src[3]);
		struct node *F = create_node(src[4]);
		struct node *G = create_node(src[5]);
		add_node(A, B);
		add_node(B, C);
		add_node(C, D);
		add_node(D, E);
		add_node(E, F);
		add_node(F, G);
		add_node(G, H);

//		printf("Befor(%d):\n", count);
//		print_all(find_head(A));
		quick_sort(A, 0, 7);
//		printf("Final (%d):\n", count);
//		print_all(find_head(A));
		
		res = check_res(A);
		if(res != 0){
			printf("res %d\n", res);
			printf("Error(%d):\n", count);
			print_all(find_head(A));
			printf("SRC:");
			for(i = 0; i < 6; i++){
				printf(" %d | ", src[i]);
			}
			printf("\n");
			break;
		}else{
			printf("Test round %d pass !!\n", count);
		}
		usleep(300*1000);
		count++;
	}while(1);

	return 0;
}
#endif
