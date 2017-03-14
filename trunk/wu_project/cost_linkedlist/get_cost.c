#include "get_cost.h"

int _find_area(struct argu_table *table, int *mark)
{
	int i;
	int x[2];
	int y[2];

	x[0] = table->x1;
	x[1] = table->x2;
	y[0] = table->y1;
	y[1] = table->y2;
	
	for(i = 0; i < SOURCE_NUM; i++){
		// To check boundary
		if(x[i] > BOUNDARY_x_UB || x[i] < BOUNDARY_x_LB){
			printf("p%d x must 0 < x < 70\n", i+1);
			exit(0);
		}
		if(y[i] > BOUNDARY_y_UB || y[i] < BOUNDARY_y_LB){
			printf("p%d y must 0 < y < 80\n", i+1);
			exit(0);
		}
		// 0 < x < 10
		if(x[i] >= BOUNDARY_x_LB && x[i] < R_x_LB){
			if(y[i] >= G_y_LB){
				*(mark+i) = GREEN;
			}else{
				*(mark+i) = YELLOW;
			}
		// 10 < x < 30
		}else if(x[i] >= R_x_LB && x[i] <= R_x_UB){
			if((y[i] >=Y_y_LB && y[i] < R_x_LB) ||
				(y[i] > R_y_UB && y[i] < G_y_LB)){
				*(mark+i) = YELLOW;
			}else if(y[i] >= R_y_LB && y[i] <= R_y_UB){
				*(mark+i) = RED;
			}else{
				*(mark+i) = GREEN;
			}
		// 31 < x < 50	
		}else if(x[i] > R_x_UB && x[i] <= Y_x_UB){
			if(y[i] <= Y_y_UB){
				*(mark+i) = YELLOW;
			}else{
				*(mark+i) = GREEN;
			}
		// 51 < x < 70
		}else{
			*(mark+i) = GREEN;
		}
	
		switch(*(mark+i)){
			case RED:
				printf("Point %d in Red region\n", i+1);
				break;
			case GREEN:
				printf("Point %d in Green region\n", i+1);
				break;
			case YELLOW:
				printf("Ponit %d in Yellow region\n", i+1);
				break;
			default:
				printf("Point %d UnKnow area!\n", i+1);
				return -1;
		}
	}
	
	return 0;
}

struct node * chk_R_Y_LB(struct line_info *lInfo)
{
	float x_tmp;
	struct node *find_node = NULL;
	
	lInfo->bound = (float)R_y_LB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		// check the node in boundary or not
		if(x_tmp >= R_x_LB && x_tmp <= R_x_UB){
			printf("find node(%.1f, %.1f) on Red-Y-LowerBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, RED);
		}
	}
	
	return find_node;
}

struct node * chk_R_Y_UB(struct line_info *lInfo)
{
	float x_tmp;
	struct node *find_node = NULL;

	lInfo->bound = (float)R_y_UB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		// check the node in boundary or not
		if(x_tmp >= R_x_LB && x_tmp <= R_x_UB){
			printf("find node(%.1f, %.1f) on Red-Y-UpperBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, RED);
		}
	}
	
	return find_node;
}

struct node * chk_R_X_LB(struct line_info *lInfo)
{
	float y_tmp;
	struct node *find_node = NULL;

	lInfo->bound = (float)R_x_LB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		// check the node in boundary or not
		if(y_tmp >= R_y_LB && y_tmp <= R_y_UB){
			printf("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, RED);
		}
	}
	
	return find_node;
}

struct node * chk_R_X_UB(struct line_info *lInfo)
{
	float y_tmp;
	struct node *find_node = NULL;

	lInfo->bound = (float)R_x_UB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		// check the node in boundary or not
		if(y_tmp >= R_y_LB && y_tmp <= R_y_UB){
			printf("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, RED);
		}
	}
	
	return find_node;
}

struct node * chk_Y_Y_LB(struct line_info *lInfo)
{
	float x_tmp;
	struct node *find_node = NULL;
	
	lInfo->bound = (float)Y_y_LB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		// check the node in boundary or not
		if(x_tmp >= Y_x_LB && x_tmp <= Y_x_UB){
			printf("find node(%.1f, %.1f) on Red-Y-LowerBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, YELLOW);
		}
	}
	
	return find_node;
}

struct node * chk_Y_Y_UB(struct line_info *lInfo)
{
	float x_tmp;
	struct node *find_node = NULL;

	lInfo->bound = (float)Y_y_UB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		// check the node in boundary or not
		if(x_tmp >= Y_x_LB && x_tmp <= Y_x_UB){
			printf("find node(%.1f, %.1f) on Red-Y-UpperBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, YELLOW);
		}
	}
	
	return find_node;
}

struct node * chk_Y_X_LB(struct line_info *lInfo)
{
	float y_tmp;
	struct node *find_node = NULL;

	lInfo->bound = (float)Y_x_LB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		// check the node in boundary or not
		if(y_tmp >= Y_y_LB && y_tmp <= Y_y_UB){
			printf("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, YELLOW);
		}
	}
	
	return find_node;
}

struct node * chk_Y_X_UB(struct line_info *lInfo)
{
	float y_tmp;
	struct node *find_node = NULL;

	lInfo->bound = (float)Y_x_UB;
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		// check the node in boundary or not
		if(y_tmp >= Y_y_LB && y_tmp <= Y_y_UB){
			printf("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, YELLOW);
		}
	}
	
	return find_node;
}

static struct node *
(*chk_intersection[])(struct line_info *lInfo) =
{
	chk_R_X_LB,
	chk_R_X_UB,
	chk_R_Y_LB,
	chk_R_Y_UB,
	chk_Y_X_LB,
	chk_Y_X_UB,
	chk_Y_Y_LB,
	chk_Y_Y_UB
};	

float get_cost(struct argu_table *table)
{
	float cost = 0.0;
	int i;
	int mark[2];
	int node_num = 0;
	void *tmp;
	struct line_info lInfo;
	struct node *pList;
	struct node *head;
	struct node *tail;
	struct node *n1 = NULL;
	struct node *n2 = NULL;
	struct node *n3 = NULL;
	struct node *n4 = NULL;
	
	if(_find_area(table, mark) == -1)
		return -1;
	
	head = create_node(table->x1, table->y1, mark[0]);
	tail = create_node(table->x2, table->y2, mark[1]);

	lInfo.x1 = table->x1;
	lInfo.y1 = table->y1;
	lInfo.x2 = table->x2;
	lInfo.y2 = table->y2;
	lInfo.m = _slope(table);
	printf("Slope : %.3f\n", lInfo.m);
	// for now, check closs-node with every boundary
	for(i = 0; i < BOUND_NUM; i++){
		tmp = chk_intersection[i](&lInfo);
		if(tmp != NULL){
			switch(node_num){
				case 0:
					n1 = (struct node *)tmp;
					add_node(head, n1);
					break;
				case 1:
					n2 = (struct node *)tmp;
					add_node(n1, n2);
					break;
				case 2:
					n3 = (struct node *)tmp;
					add_node(n2, n3);
					break;
				case 3:
					n4 = (struct node *)tmp;
					add_node(n3, n4);
					break;
				default:
					printf("Find node cannot grater than 4 !!\n");
					return -1;
			}
			node_num++;
		}
	}
	add_node(find_tail(head), tail);

	if(head->x > tail->x)
		swap(head, tail);

	swap(head->next->next, head->next->next->next);
	printf("All node without sorting:\n");
	print_all(find_head(head));
	quick_sort(find_head(head), 0, node_num+SOURCE_NUM-1);
	printf("All node with sorting:\n");
	print_all(find_head(head));
	pList = find_head(head); 
	for(i = 0; i < node_num+1; i++){
		switch(pList->color){
			case RED:
				if(pList->next->color == RED)// R->R
					cost += __distance(pList->x, pList->y, pList->next->x, pList->next->y) * (float)R_cost;
				else if(pList->next->color == YELLOW)	// R->Y
					cost += __distance(pList->x, pList->y, pList->next->x, pList->next->y) * (float)Y_cost;
				else
					printf("ERROR ! only R->R or R->Y, next node color %d\n", pList->next->color);
				break;
			case YELLOW:
				if(pList->next->color == RED || pList->next->color == YELLOW)// Y->R, Y->Y
					cost += __distance(pList->x, pList->y, pList->next->x, pList->next->y) * (float)Y_cost;
				else if(pList->next->color == GREEN)// Y->G
					cost += __distance(pList->x, pList->y, pList->next->x, pList->next->y) * (float)G_cost;
				else
					printf("ERROR ! only Y->Y, Y->R, Y->G, next node color %d\n", pList->next->color);
				break;
			case GREEN:
				// only G_cost
				cost += __distance(pList->x, pList->y, pList->next->x, pList->next->y) * (float)G_cost;
				break;
			default:
				printf("Error color (%d)\n", pList->color);
				return -1;
		}
		pList = pList->next;
	}

	return cost;
}

				

