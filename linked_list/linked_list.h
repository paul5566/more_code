#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <unistd.h>

//#define DEBUG

#ifdef DEBUG
#define print_all_m(n) print_all(n)
#else
#define print_all_m(n) do{}while(0);
#endif

struct node{
	int data;
	struct node *next;
	struct node *pre;
};

struct node * find_head(struct node *n);
struct node * create_node(int data);
void add_node(struct node *n1, struct node *n2);
void insert_node_left(struct node *n1, struct node *n2);
void insert_node_right(struct node *n1, struct node *n2);
void be_head_node(struct node *n1, struct node *n2);
void swap(struct node *n1, struct node *n2);
void print_all(struct node *head);
int find_order(struct node *n, int data);
struct node * find_head(struct node *n);
int find_all_num(struct node *n);
void quick_sort(struct node *list, int start, int end);

#endif
