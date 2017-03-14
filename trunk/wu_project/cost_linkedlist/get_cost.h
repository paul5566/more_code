#ifndef _GET_COST_H
#define _GET_COST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SOURCE_NUM	2
#define BOUND_NUM	8

// The maxium of possiable node 
#define NODE_MAX	4

// The area 
#define RED		1
#define GREEN	2
#define YELLOW	3

// The area bounding
#define G_x_LB	51
#define G_x_UB	70
#define G_y_LB	61
#define G_y_UB	80
#define Y_x_LB	0
#define Y_x_UB	50
#define Y_y_LB	0
#define Y_y_UB	60
#define R_x_LB	10
#define R_x_UB	30
#define R_y_LB	10
#define R_y_UB	40
#define BOUNDARY_x_LB	0
#define BOUNDARY_x_UB	70
#define BOUNDARY_y_LB	0
#define BOUNDARY_y_UB	80

// C_cost
#define R_cost	60
#define Y_cost	40
#define G_cost	20

/*
 * The linked-list node
*/
struct node{
	float x;
	float y;
	int color;
	struct node *next;
	struct node *pre;
};
	
/*
 * The point coordinate
*/
struct argu_table{
	float x1;
	float y1;
	float x2;
	float y2;
};

/*
 * The necessary infomation 
 * to find intersection node
*/
struct line_info{
	float x1;
	float y1;	
	float x2;
	float y2;
	float m;
	float bound;
};

static inline int
line_seg(float bound, float coord1, float coord2)
{
	if((coord1 <= bound && bound <= coord2) ||
		(coord2 <= bound && bound <= coord1))
		return 1; else return 0;
}

/* 
 * To calculate distance
 * s_x -> soruce X axis
 * s_y -> source Y axis
 * d_x -> destination X axis
 * d_y -> destination Y axis
*/
static inline float 
__distance(float s_x, float s_y, float d_x, float d_y)
{
	return (float)pow(pow((d_x-s_x), 2) + pow((d_y-s_y), 2), 0.5);
}

/*
 * To calculate node 
 * X -> find x node
 * 		X = Y/m + x1 - y1/m
 * Y -> find Y node
 *		Y = m*X - m*x1 + y1
*/
static inline float 
__intersection(struct line_info *lInfo, char *axis)
{
	float x1 = lInfo->x1;
	float y1 = lInfo->y1;
	float m = lInfo->m;
	float bound = lInfo->bound;

	if(strcmp(axis, "X") == 0)
		return (bound/m)+x1-(y1/m);
	else if(strcmp(axis, "Y") == 0)
		return (m*bound)-(m*x1)+y1;
	else
		printf("%s the parameter of axis must be X or Y\n", __func__);
	
	return -1;	// means error
}

/* 
 * To calculate slope(m)
*/
static inline float 
_slope(struct argu_table *table)
{
	float x1 = (float)table->x1;
	float y1 = (float)table->y1;
	float x2 = (float)table->x2;
	float y2 = (float)table->y2;

	return ((y1 - y2)/(x1 - x2));
}

struct node * create_node(float x, float y, int color);
struct node * find_tail(struct node *n);
struct node * find_head(struct node *n);
void add_node(struct node *n1, struct node *n2);
void print_all(struct node *head);
void quick_sort(struct node *list, int start, int end);
void swap(struct node *n1, struct node *n2);

float get_cost(struct argu_table *table);
#endif
