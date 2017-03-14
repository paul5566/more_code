#include "get_cost.h"

void _find_area(struct argu_table *table, int *mark)
{
	int i;
	float x[2];
	float y[2];

	x[0] = table->x1;
	x[1] = table->x2;
	y[0] = table->y1;
	y[1] = table->y2;
	
	for(i = 0; i < SOURCE_NUM; i++){
		// To check boundary
		if(x[i] > (float)BOUNDARY_x_UB || x[i] < (float)BOUNDARY_x_LB){
			printf("p%d x must 0 < x < 100\n", i+1);
			exit(0);
		}
		if(y[i] > (float)BOUNDARY_y_UB || y[i] < (float)BOUNDARY_y_LB){
			printf("p%d y must 0 < y < 100\n", i+1);
			exit(0);
		}
		// the edge, only 1 part (green)
		if((x[i] >= (float)G_x_LB && x[i] < (float)Y_x_LB) || 
			(x[i] > (float)Y_x_UB && x[i] <= (float)G_x_UB))
		{
			*(mark+i) = GREEN;
		}
		// the little edge, 2 part (green, yellow)
		else if((x[i] >= (float)Y_x_LB && x[i] < (float)R_x_LB) ||
			(x[i] > (float)R_x_UB && x[i] <= (float)Y_x_UB))
		{
			if((y[i] > (float)Y_y_UB && y[i] <= (float)G_y_UB) ||
				(y[i] >= (float)G_y_LB && y[i] < (float)Y_y_LB))
			{
				*(mark+i) = GREEN;
			}
			else{
				*(mark+i) = YELLOW;
			}
		}
		// the center, 3 part (green, yellow, red)
		else if(x[i] >= (float)R_x_LB && x[i] <= (float)R_x_UB){
			if((y[i] > (float)Y_y_UB && y[i] <= (float)G_y_UB) ||
				(y[i] >= (float)G_y_LB && y[i] < (float)Y_y_LB))
			{
				*(mark+i) = GREEN;
			}
			else if((y[i] >= (float)Y_y_LB && y[i] < (float)R_y_LB) ||
					(y[i] > (float)R_y_UB && y[i] <= (float)Y_y_UB))
			{
				*(mark+i) = YELLOW;
			}
			else if(y[i] >= (float)R_y_LB && y[i] <= (float)R_y_UB)
			{
				*(mark+i) = RED;
			}else{ printf("Error ! cannot find area \n"); exit(0); }
		}else{ printf("Error ! cannot find area \n"); exit(0); }
	
		switch(*(mark+i)){
			case RED:
				TRACE("Point %d in Red region\n", i+1);
				break;
			case GREEN:
				TRACE("Point %d in Green region\n", i+1);
				break;
			case YELLOW:
				TRACE("Ponit %d in Yellow region\n", i+1);
				break;
			default:
				TRACE("Point %d UnKnow area!\n", i+1);
				exit(0);
		}
	}
}

struct node * chk_R_Y_LB(struct line_info *lInfo, struct node *head)
{
	float x_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;
	
	lInfo->bound = (float)R_y_LB;
	if(lInfo->y1 == lInfo->bound ||
		lInfo->y2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		if(x_tmp == -2) return NULL;
		// check the node in boundary or not
		if(x_tmp >= R_x_LB && x_tmp <= R_x_UB){
			// ------------- XXX:DO not use _remove_node --------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == x_tmp && tmp_tail->y == lInfo->bound)
				return NULL;
			// ------------- end -------------------
			TRACE("find node(%.1f, %.1f) on Red-Y-LowerBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, RED);
		}
	}
	
	return find_node;
}

struct node * chk_R_Y_UB(struct line_info *lInfo, struct node *head)
{
	float x_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;

	lInfo->bound = (float)R_y_UB;
	if(lInfo->y1 == lInfo->bound ||
		lInfo->y2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		if(x_tmp == -2) return NULL;
		// check the node in boundary or not
		if(x_tmp >= R_x_LB && x_tmp <= R_x_UB){
			// ------------- XXX:DO not use _remove_node --------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == x_tmp && tmp_tail->y == lInfo->bound)
				return NULL;
			// -------------------------------------------------------------
			TRACE("find node(%.1f, %.1f) on Red-Y-UpperBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, RED);
		}
	}
	
	return find_node;
}

struct node * chk_R_X_LB(struct line_info *lInfo, struct node *head)
{
	float y_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;

	lInfo->bound = (float)R_x_LB;
	if(lInfo->x1 == lInfo->bound ||
		lInfo->x2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		if(y_tmp == -2) return NULL;
		// check the node in boundary or not
		if(y_tmp >= R_y_LB && y_tmp <= R_y_UB){
			// ----------- XXX: Do not use _remove_node ---------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == lInfo->bound && tmp_tail->y == y_tmp)
				return NULL;
			// -------------------------------------------------------------
			TRACE("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, RED);
		}
	}
	
	return find_node;
}

struct node * chk_R_X_UB(struct line_info *lInfo, struct node *head)
{
	float y_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;

	lInfo->bound = (float)R_x_UB;
	if(lInfo->x1 == lInfo->bound ||
		lInfo->x2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		if(y_tmp == -2) return NULL;
		// check the node in boundary or not
		if(y_tmp >= R_y_LB && y_tmp <= R_y_UB){
			// ----------- XXX: Do not use _remove_node ---------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == lInfo->bound && tmp_tail->y == y_tmp)
				return NULL;
			// --------------------------------------------------------------
			TRACE("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, RED);
		}
	}
	
	return find_node;
}

struct node * chk_Y_Y_LB(struct line_info *lInfo, struct node *head)
{
	float x_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;
	
	lInfo->bound = (float)Y_y_LB;
	if(lInfo->y1 == lInfo->bound ||
		lInfo->y2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		if(x_tmp == -2) return NULL;
		// check the node in boundary or not
		if(x_tmp >= Y_x_LB && x_tmp <= Y_x_UB){
			// ----------- XXX: Do not use _remove_node ---------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == x_tmp && tmp_tail->y == lInfo->bound)
				return NULL;
			// -------------------------------------------------------------	
			TRACE("find node(%.1f, %.1f) on Red-Y-LowerBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, YELLOW);
		}
	}
	
	return find_node;
}

struct node * chk_Y_Y_UB(struct line_info *lInfo, struct node *head)
{
	float x_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;

	lInfo->bound = (float)Y_y_UB;
	if(lInfo->y1 == lInfo->bound ||
		lInfo->y2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->y1, lInfo->y2)){
		x_tmp = __intersection(lInfo, "X");
		if(x_tmp == -2) return NULL;
		// check the node in boundary or not
		if(x_tmp >= Y_x_LB && x_tmp <= Y_x_UB){
			// ----------- XXX: Do not use _remove_node ---------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == x_tmp && tmp_tail->y == lInfo->bound)
				return NULL;
			// --------------------------------------------------------------
			TRACE("find node(%.1f, %.1f) on Red-Y-UpperBound\n", 
					x_tmp, lInfo->bound);
			find_node = create_node(x_tmp, lInfo->bound, YELLOW);
		}
	}
	
	return find_node;
}

struct node * chk_Y_X_LB(struct line_info *lInfo, struct node *head)
{
	float y_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;

	lInfo->bound = (float)Y_x_LB;
	if(lInfo->x1 == lInfo->bound ||
		lInfo->x2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		if(y_tmp == -2) return NULL;
		// check the node in boundary or not
		if(y_tmp >= Y_y_LB && y_tmp <= Y_y_UB){
			// ----------- XXX: Do not use _remove_node ---------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == lInfo->bound && tmp_tail->y == y_tmp)
				return NULL;
			// --------------------------------------------------------------
			TRACE("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, YELLOW);
		}
	}
	
	return find_node;
}

struct node * chk_Y_X_UB(struct line_info *lInfo, struct node *head)
{
	float y_tmp;
	struct node *find_node = NULL;
	struct node *tmp_tail;

	lInfo->bound = (float)Y_x_UB;
	if(lInfo->x1 == lInfo->bound ||
		lInfo->x2 == lInfo->bound)
	{
		return NULL;
	}
	// set the line segment boundary
	if(line_seg(lInfo->bound, lInfo->x1, lInfo->x2)){
		y_tmp = __intersection(lInfo, "Y");
		if(y_tmp == -2) return NULL;
		// check the node in boundary or not
		if(y_tmp >= Y_y_LB && y_tmp <= Y_y_UB){
			// ----------- XXX: Do not use _remove_node ---------------------
			tmp_tail = find_tail(head);
			if(tmp_tail->x == lInfo->bound && tmp_tail->y == y_tmp)
				return NULL;
			// --------------------------------------------------------------
			TRACE("find node(%.1f, %.1f) on Red-X-LowerBound\n", 
					lInfo->bound, y_tmp);
			find_node = create_node(lInfo->bound, y_tmp, YELLOW);
		}
	}
	
	return find_node;
}

static struct node *
(*chk_intersection[])(struct line_info *lInfo, struct node *head) =
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
	
	_find_area(table, mark);
	
	head = create_node(table->x1, table->y1, mark[0]);
	tail = create_node(table->x2, table->y2, mark[1]);

	lInfo.x1 = table->x1;
	lInfo.y1 = table->y1;
	lInfo.x2 = table->x2;
	lInfo.y2 = table->y2;
	if(lInfo.x1 == lInfo.x2)
		lInfo.m = 0.0;
	else
		lInfo.m = _slope(table);
	TRACE("Slope : %.3f\n", lInfo.m);
	// for now, check closs-node with every boundary
	for(i = 0; i < BOUND_NUM; i++){
		tmp = chk_intersection[i](&lInfo, head);
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

	TRACE("All node without sorting:\n");
	print_all(find_head(head));
//	TRACE("All node after remove dup node:\n");
//	remove_dup_node(find_head(head), &node_num);
//	print_all(find_head(head));
	// if m = 0 or m = infinity, DONOT use <quick_sort>
	if(lInfo.m == 0.0){
		if(lInfo.x1 == lInfo.x2){
			if(lInfo.y1 > lInfo.y2){
				swap(head, tail);
			}
		}else if(lInfo.y1 == lInfo.y2){
			if(lInfo.x1 > lInfo.x2){
				swap(head, tail);
			}
		}
	}else{
		//0 means the first node, all shift 1
		quick_sort(find_head(head), 0, node_num+SOURCE_NUM-1);
	}
	TRACE("All node with sorting:\n");
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
				// G->G, G->Y only G_cost
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
