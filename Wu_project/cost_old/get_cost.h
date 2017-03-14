#ifndef _GET_COST_H
#define _GET_COST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// The number of point
#define NUM		2

// The maxium of possiable node 
#define NODE_MAX	4

// The area 
#define RED		1
#define GREEN	2
#define YELLOW	3

// The area bounding
#define G_x_L	51
#define G_x_H	70
#define G_y_L	61
#define G_y_H	80
#define Y_x_L	0
#define Y_x_H	50
#define Y_y_L	0
#define Y_y_H	60
#define R_x_L	10
#define R_x_H	30
#define R_y_L	10
#define R_y_H	40
#define BOUNDARY_x_L	0
#define BOUNDARY_x_H	70
#define BOUNDARY_y_L	0
#define BOUNDARY_y_H	80

int test;

// C_cost
#define R_cost	1
#define G_cost	1
#define Y_cost	1
	
/*
 * The point coordinate
*/
struct argu_table{
	int x1;
	int y1;
	int x2;
	int y2;
};

// The necessary infomation to find node
struct node_info{
	int x1;
	int y1;
	float m;
	int bound;
};
	
// The node result
struct node_result{
	float node_x[NODE_MAX];
	float node_y[NODE_MAX];
	int node_count;
};

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
__node(struct node_info *nInfo, char *axis)
{
	float x1 = (float)nInfo->x1;
	float y1 = (float)nInfo->y1;
	float m = nInfo->m;
	float bound = (float)nInfo->bound;

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

float get_cost(struct argu_table *table);
#endif
