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
	
	for(i = 0; i < NUM; i++){
		// To check boundary
		if(x[i] > BOUNDARY_x_H || x[i] < BOUNDARY_x_L){
			printf("p%d x must 0 < x < 70\n", i+1);
			exit(0);
		}
		if(y[i] > BOUNDARY_y_H || y[i] < BOUNDARY_y_L){
			printf("p%d y must 0 < y < 80\n", i+1);
			exit(0);
		}
		// 0 < x < 10
		if(x[i] >= BOUNDARY_x_L && x[i] < R_x_L){
			if(y[i] >= G_y_L){
				*(mark+i) = GREEN;
			}else{
				*(mark+i) = YELLOW;
			}
		// 10 < x < 30
		}else if(x[i] >= R_x_L && x[i] <= R_x_H){
			if((y[i] >=Y_y_L && y[i] < R_x_L) ||
				(y[i] > R_y_H && y[i] < G_y_L)){
				*(mark+i) = YELLOW;
			}else if(y[i] >= R_y_L && y[i] <= R_y_H){
				*(mark+i) = RED;
			}else{
				*(mark+i) = GREEN;
			}
		// 31 < x < 50	
		}else if(x[i] > R_x_H && x[i] <= Y_x_H){
			if(y[i] <= Y_y_H){
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

/*
 * find the end-point
 * the end-point must be 
 * outside buondary!
*/
float 
__get_part_dist(struct argu_table *table, int bound, float coor_tmp, char *part)
{
	float _dist;
	int x1 = table->x1;
	int x2 = table->x2;
	int y1 = table->y1;
	int y2 = table->y2;
	
	/* find the Y-axis lower bound endpoint */
	if(strcmp(part, "Y-Lower") == 0){
		/*
		 * take point 1 to endpoint
		 * the point 1 Y-coordinate
		 * is less than bound
		*/
		if(y1 < bound && y2 > bound){
			_dist = __distance(coor_tmp, bound, x1, y1);
			printf("Get distance %f with p1\n", _dist);
		/*
		 * take point 2 to endpoint
		 * the point 2 Y-coordinate
		 * is less than bound
		*/
		}else if(y2 < bound && y1 > bound){
			_dist = __distance(coor_tmp, bound, x2, y2);
			printf("Get distance %f with p2\n", _dist);
		/*
		 * means the endpoint is on 
		 * the boundary line
		*/
		}else{
			return 0.0;
		}
	/* find the Y-axis upper bound endpoint */
	}else if(strcmp(part, "Y-Upper") == 0){
		/* 
		 * take point 1 to endpoint
		 * the point 1 Y-coordinate
		 * is gratter than bound
		*/
		if(y1 > bound && y2 < bound){
			_dist = __distance(coor_tmp, bound, x1, y1);
			printf("Get distance %f with P1\n", _dist);
		/*
		 * take point 2 to endpoint
		 * the point 2 Y-coordinate
		 * is gratter than bound
		*/
		}else if(y2 > bound && y1 < bound){
			_dist = __distance(coor_tmp, bound, x2, y2);
			printf("Get distance %f with P2\n", _dist);
		/*
		 * means the end point is on
		 * the bound line
		*/	
		}else{
			return 0.0;
		}
	/* find the X-aixs lower bound endpoint */
	}else if(strcmp(part, "X-Lower") == 0){
		/* 
		 * take point 1 to endpoint
		 * the point 1 X-coordinate
		 * is less than bound
		*/
		if(x1 < bound && x2 > bound){
			_dist = __distance(coor_tmp, bound, x1, y1);
			printf("Get distance %f with P1\n", _dist);
		/* 
		 * take point 2 to endpoint
		 * the point 2 X-coordinate
		 * is less than bound
		*/
		}else if(x2 < bound && x1 > bound){
			_dist = __distance(coor_tmp, bound, x2, y2);
			printf("Get distance %f with P2\n", _dist);
		/*
		 * means the endpoint is on 
		 * the bound line
		*/
		}else{
			return 0.0;
		}
	/* find the X-axis upper boound endpoint */
	}else if(strcmp(part, "X-Upper") == 0){
		/* 
		 * take point 1 to endpoint
		 * the point 1 X-coordinate
		 * is gratter than bound
		*/
		if(x1 > bound && x2 < bound){
			_dist = __distance(bound, coor_tmp, x1, y1);
			printf("Get distance %f with P1\n", _dist);
		/* 
		 * take point 2 to endpoint
		 * the point 2 X-coordinate
		 * is gratter than bound
		*/
		}else if(x2 > bound && x1 < bound){
			_dist = __distance(bound, coor_tmp, x2, y2);
			printf("Get distance %f with P2\n", _dist);
		/* 
		 * means the endpoint is on
		 * the bounding line
		*/
		}else{
			return 0.0;
		}
	}else{
		printf("%s error parameter\n", __func__);
		exit(0);
	}
	
	return _dist;
}
		
float 
_get_dist(struct argu_table *table, int bound, float node,char *bd_line)
{
	float dist = 0.0;

	if(strcmp(bd_line, "R-Y-LB") == 0){
		dist = __get_part_dist(table, bound, node, "Y-Lower");
	}else if(strcmp(bd_line, "R-Y-UB") == 0){
		dist = __get_part_dist(table, bound, node, "Y-Upper");
	}else if(strcmp(bd_line, "R-X-LB") == 0){
		dist = __get_part_dist(table, bound, node, "X-Lower");
	}else if(strcmp(bd_line, "R-X-UB") == 0){
		dist = __get_part_dist(table, bound, node, "X-Upper");
	}else if(strcmp(bd_line, "Y-Y-UB") == 0){
		dist = __get_part_dist(table, bound, node, "Y-Upper");
	}else if(strcmp(bd_line, "Y-X-UB") == 0){
		dist = __get_part_dist(table, bound, node, "X-Upper");
	}else{
		printf("%s Error parameter\n", __func__);
		return -1;
	}

	return dist;
}

float _get_dist_in_area(struct node_result *nRes, int start, int des)
{
	return __distance(nRes->node_x[start-1], nRes->node_y[start-1],
					nRes->node_x[des-1], nRes->node_y[des-1]);
}

float 
_chk_node(struct argu_table *table, struct node_info *nInfo, char *bd_line)
{
	float x_tmp;
	float y_tmp;

	if(strcmp(bd_line, "R-Y-LB") == 0){
		if((table->y1 <= R_y_L && R_y_L <= table->y2) ||
			(table->y2 <= R_y_L && R_y_L <= table->y1)){
			x_tmp = __node(nInfo, "X");
			if(x_tmp >= R_x_L && x_tmp <= R_x_H){// 10 < node_x < 30 
				printf("node(%f, %d)\n", x_tmp, R_y_L);
				return x_tmp;
			}
		}
	}else if(strcmp(bd_line, "R-Y-UB") == 0){
		if((table->y1 <= R_y_H && R_y_H <= table->y2) ||
			(table->y2 <= R_y_H && R_y_H <= table->y1)){
			x_tmp = __node(nInfo, "X");
			if(x_tmp >= R_x_L && x_tmp <= R_x_H){// 10 < node_x < 30
				printf("node(%f, %d)\n", x_tmp, R_y_H);
				return x_tmp;
			}
		}
	}else if(strcmp(bd_line, "R-X-LB") == 0){
		if((table->x1 <= R_x_L && R_x_L <= table->x2) ||
			(table->x2 <= R_x_L && R_x_L <= table->x1)){
			y_tmp = __node(nInfo, "Y");
			if(y_tmp >= R_y_L && y_tmp <= R_y_H){// 10 < node_y < 40
				printf("node(%d, %f)\n", R_x_L, y_tmp);
				return y_tmp;
			}
		}
	}else if(strcmp(bd_line, "R-X-UB") == 0){
		if((table->x1 <= R_x_H && R_x_H <= table->x2) ||
			(table->x2 <= R_x_H && R_x_H <= table->x1)){
			y_tmp = __node(nInfo, "Y");
			if(y_tmp >= R_y_L && y_tmp <= R_y_H){// 10 < node_y < 40
				printf("node(%d, %f)\n", R_x_H, y_tmp);
				return y_tmp;
			}
		}
	}else if(strcmp(bd_line, "Y-Y-UB") == 0){
		if((table->y1 <= Y_y_H && Y_y_H <= table->y2) ||
			(table->y2 <= Y_y_H && Y_y_H <= table->y1)){
			x_tmp = __node(nInfo, "X");
			if(x_tmp >= Y_x_L && x_tmp <= Y_x_H){// 0 < node_x < 50
				printf("node(%f, %d)\n", x_tmp, Y_y_H);
				return x_tmp;
			}
		}
	}else if(strcmp(bd_line, "Y-X-UB") == 0){
		if((table->x1 <= Y_x_H && Y_x_H <= table->x2) ||
			(table->x2 <= Y_x_H && Y_x_H <= table->x1)){
			y_tmp = __node(nInfo, "Y");
			if(y_tmp >= Y_y_L && y_tmp <= Y_y_H){// 0 < node_y < 60
				printf("node(%d, %f)\n", Y_x_H, y_tmp);
				return y_tmp;
			}
		}
	}else{
		printf("%s error parameter\n", __func__);
		exit(0);
	}
	
	return 0.0;
}

float get_cost(struct argu_table *table)
{
	int i;
	int mark[2];
	float cost = 0.0;
	float tmp_x;
	float tmp_y;
	struct node_info nInfo;
	struct node_result nRes;
	int x1 = table->x1;
	int x2 = table->x2;
	int y1 = table->y1;
	int y2 = table->y2;

	printf("test %d\n", test);

	for(i = 0; i < NODE_MAX; i++){
		nRes.node_x[i] = 0.0;
		nRes.node_y[i] = 0.0;
	}	
	nRes.node_count = 0;


	if(_find_area(table, mark) == -1)
		return -1;

	nInfo.x1 = table->x1;
	nInfo.y1 = table->y1;
	nInfo.m = _slope(table);
	printf("Slope : %f\n", nInfo.m);

	// Point 1 area == Point 2 area
	if(mark[0] == mark[1]){
		switch(mark[0]){
			case RED:
				cost = __distance(x1, y1, x2, y2) * R_cost;
				break;
			case YELLOW:	/* case: Y->Y or Y->R->Y */
				// check with Red-Y-LB(y = 10), find X
				nInfo.bound = R_y_L;
				tmp_x = _chk_node(table, &nInfo, "R-Y-LB");
				if(tmp_x != 0.0){
					nRes.node_x[nRes.node_count] = tmp_x;
					nRes.node_y[nRes.node_count] = R_y_L;	//node(x, 10)
					nRes.node_count++;
					cost += _get_dist(table, nInfo.bound, tmp_x, "R-Y-LB");
				}
				// check with Red-Y-UB(y = 40), find X
				nInfo.bound = R_y_H;
				tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
				if(tmp_x != 0.0){
					nRes.node_x[nRes.node_count] = tmp_x;
					nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
					nRes.node_count++;
					cost += _get_dist(table, nInfo.bound, tmp_x, "R-Y-UB");
				}
				// check with Red-X-LB(x = 10), find y
				nInfo.bound = R_x_L;	
				tmp_y = _chk_node(table, &nInfo, "R-X-LB");
				if(tmp_y != 0.0){
					nRes.node_x[nRes.node_count] = R_x_L;
					nRes.node_y[nRes.node_count] = tmp_y;	//node(10, y)
					nRes.node_count++;
					cost += _get_dist(table, nInfo.bound, tmp_y, "R-X-LB");
				}
				// check with Red-X-UB(x = 30), find y
				nInfo.bound = R_x_H;
				tmp_y = _chk_node(table, &nInfo, "R-X-UB");
				if(tmp_y != 0.0){
					nRes.node_x[nRes.node_count] = R_x_H;
					nRes.node_y[nRes.node_count] = tmp_y;
					nRes.node_count++;
					cost += _get_dist(table, nInfo.bound, tmp_y, "R-X-UB");
				}
				printf("Total %d nodes\n", nRes.node_count);
				// means Yellow->Yellow, no other area crossed
				if(nRes.node_count == 0){
					cost = __distance(x1, y1, x2, y2);
				// means Yellow->Red->Yellow
				}else if(nRes.node_count == 2){	
					//add the cost of red area
					cost += _get_dist_in_area(&nRes, 1, 2);
				}else{	
					printf("Wrong !!! In bost of Yellow case, only 0/2 node is possiable !! ");
					printf("But there are %d nodes\n", nRes.node_count);
					return -1;
				}
				break;
			case GREEN:	/* CASE: G->G or G->Y->G or G->Y->R->G */
				// check with Yellow-Y-UB(y = 60), find x
				nInfo.bound = Y_y_H;
				tmp_x = _chk_node(table, &nInfo, "Y-Y-UB");
				if(tmp_x != 0.0){
					nRes.node_x[nRes.node_count] = tmp_x;
					nRes.node_y[nRes.node_count] = Y_y_H;	//node(x, 60)
					nRes.node_count++;
					cost += _get_dist(table, nInfo.bound, tmp_x, "Y-Y-UB");
				}
				//check with Yellow-X-UB(x = 50), find y
				nInfo.bound = Y_x_H;
				tmp_y = _chk_node(table, &nInfo, "Y-X-UB");
				if(tmp_y != 0.0){
					nRes.node_x[nRes.node_count] = Y_x_H;
					nRes.node_y[nRes.node_count] = tmp_y;
					nRes.node_count++;
					cost += _get_dist(table, nInfo.bound, tmp_y, "Y-X-UB");
				}
				// check with Red-Y-UB(y = 40), find x
				nInfo.bound = R_y_H;
				tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
				if(tmp_x != 0.0){
					nRes.node_x[nRes.node_count] = tmp_x;
					nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
					nRes.node_count++;
					/* 
					 * means G->Y->R->G case, 
					 * the Red-Y-UB must link with Yellow-Y-UB
					 * in node_result array, 	
					 * array 0 stored node with Yellow-Y-UB,
					 * array 2 stored node with Red-Y-UB.
					*/
					cost += _get_dist_in_area(&nRes, 1, 3);
				}
				nInfo.bound = R_x_H;
				tmp_y = _chk_node(table, &nInfo, "R-X-UB");
				if(tmp_y != 0.0){
					nRes.node_x[nRes.node_count] = R_x_H;
					nRes.node_y[nRes.node_count] = tmp_y;
					nRes.node_count++;
					/*
					 * means G->Y->R->G case, 
					 * the Red-X-UB must link with Yellow-X-UB
					 * in node_result array, 	
					 * array 1 stored node with Yellow-X-UB,
					 * array 3 stored node with Red-X-UB.
					*/
					cost += _get_dist_in_area(&nRes, 2, 4);
				}
				printf("Total %d nodes\n", nRes.node_count);
				// means Green->Green, no other area crossed
				if(nRes.node_count == 0){
					cost = __distance(x1, y1, x2, y2);
				// means Green->Yellow->Green
				}else if(nRes.node_count == 2){
					cost += _get_dist_in_area(&nRes, 1, 2);
				// means Green->Yellow->Red->Green 
				}else if(nRes.node_count == 4){
					// the Red area cost
					cost += _get_dist_in_area(&nRes, 3, 4);
				}else{
					printf("Wrong !!! In both of Green case, ");
					printf(" only 0/2/4 node is possible ! ");
					printf("But there are %d nodes\n", nRes.node_count);
					return -1;
				}
				break;
			default:
				printf("Impossible region!\n");
				exit(0);
			}
		/* Point1 = R, Point2 = Y or Point1 = Y, Point2 = R */
		}else if(mark[0] != GREEN && mark[1] != GREEN && 
				mark[0] != mark[1]){
			/* Only one case, R<->Y */
			// check with Red-Y-LB(y = 10), find X
			nInfo.bound = R_y_L;
			tmp_x = _chk_node(table, &nInfo, "R-Y-LB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_x[nRes.node_count] = R_y_L;	//node(x, 10)
				nRes.node_count++;
				cost += _get_dist(table, nInfo.bound, tmp_x, "R-Y-LB");
			}
			// check with Red-Y-UB(y = 40), find X
			nInfo.bound = R_y_H;
			tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
				nRes.node_count++;
				cost += _get_dist(table, nInfo.bound, tmp_x, "R-Y-UB");
			}
			// check with Red-X-LB(x = 10), find y
			nInfo.bound = R_x_L;	
			tmp_y = _chk_node(table, &nInfo, "R-X-LB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_L;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(10, y)
				nRes.node_count++;
				cost += _get_dist(table, nInfo.bound, tmp_y, "R-X-LB");
			}
			// check with Red-X-UB(x = 30), find y
			nInfo.bound = R_x_H;
			tmp_y = _chk_node(table, &nInfo, "R-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(30, y)
				nRes.node_count++;
				cost += _get_dist(table, nInfo.bound, tmp_y, "R-X-UB");
			}
			printf("Total %d nodes\n", nRes.node_count);
			// In this case, must have one corss node!
			if(nRes.node_count == 1){
				if(mark[0] == RED){
					cost += __distance(nRes.node_x[nRes.node_count-1], 
								nRes.node_y[nRes.node_count-1], x1, y1);
				}else if(mark[1] == RED){
					cost += __distance(nRes.node_x[nRes.node_count-1], 
							nRes.node_y[nRes.node_count-1], x2, y2); 
				}else{
					printf("Error! in R<->Y case ");
					printf("one of mark must be RED!\n");
					return -1;
				}
			}else{
				printf("Error! in R<->Y case, must have 1 cross node!\n");
				return -1;
			}
		/* Point1 = R, Point2 = G */
		}else if(mark[0] == RED && mark[1] == GREEN){ 
			// only R->Y->G case
			// First, check Red boundary
			// check with Red-Y-LB(y = 10), find X
			nInfo.bound = R_y_L;
			tmp_x = _chk_node(table, &nInfo, "R-Y-LB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_x[nRes.node_count] = R_y_L;	//node(x, 10)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// check with Red-Y-UB(y = 40), find X
			nInfo.bound = R_y_H;
			tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// check with Red-X-LB(x = 10), find y
			nInfo.bound = R_x_L;	
			tmp_y = _chk_node(table, &nInfo, "R-X-LB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_L;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(10, y)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// check with Red-X-UB(x = 30), find y
			nInfo.bound = R_x_H;
			tmp_y = _chk_node(table, &nInfo, "R-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(30, y)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// find the node cross with Yellow
			// check with Yellow-Y-UB(y = 60), find x
			nInfo.bound = Y_y_H;
			tmp_x = _chk_node(table, &nInfo, "Y-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = Y_y_H;	//node(x, 60)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, 1, 2);
			}
			//check with Yellow-X-UB(x = 50), find y
			nInfo.bound = Y_x_H;
			tmp_y = _chk_node(table, &nInfo, "Y-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = Y_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(50, y)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, 1, 2);
			}
			// in R->Y->G case, must have 2 node
			printf("Total %d node\n", nRes.node_count);
			if(nRes.node_count == 2){
				cost += __distance(x2, y2, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}else{
				printf("In R->Y->G case, there are 2 corss node, but only %d node\n", nRes.node_count);
				return -1;
			}
		/* point1 = G, point2 = R */
		}else if(mark[0] == GREEN && mark[1] == RED){
			// only G->Y->R case
			// First check Yellow boundary
			// check with Yellow-Y-UB(y = 60), find x
			nInfo.bound = Y_y_H;
			tmp_x = _chk_node(table, &nInfo, "Y-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = Y_y_H;	//node(x, 60)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// check with Yellow-X-UB(x = 50), find y
			nInfo.bound = Y_x_H;
			tmp_y = _chk_node(table, &nInfo, "Y-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = Y_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(50, y)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// Secondly, check with Red bound
			// check with Red-Y-LB(y = 10), find X
			nInfo.bound = R_y_L;
			tmp_x = _chk_node(table, &nInfo, "R-Y-LB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_x[nRes.node_count] = R_y_L;	//node(x, 10)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1 ,nRes.node_count);
			}
			// check with Red-Y-UB(y = 40), find X
			nInfo.bound = R_y_H;
			tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);
			}
			// check with Red-X-LB(x = 10), find y
			nInfo.bound = R_x_L;	
			tmp_y = _chk_node(table, &nInfo, "R-X-LB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_L;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(10, y)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);
			}
			// check with Red-X-UB(x = 30), find y
			nInfo.bound = R_x_H;
			tmp_y = _chk_node(table, &nInfo, "R-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(30, y)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);
			}
			// in G->Y->R case , must have 2 node
			printf("Total %d node\n", nRes.node_count);
			if(nRes.node_count == 2){
				cost += __distance(x2, y2, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}else{
				printf("In G->Y->R case, there are 2 cross node, but only %d node\n", nRes.node_count);
				return -1;
			}
		/* point1 = G, point2 = Y */
		}else if(mark[0] == GREEN && mark[1] == YELLOW){
			// G->Y->R->Y & G->Y case!
			// First, check with Yellow boundary
			/* !!! <Check Upper bound FIRST> !!!! */
			// check with Yellow-Y-UB(y = 60), find x
			nInfo.bound = Y_y_H;
			tmp_x = _chk_node(table, &nInfo, "Y-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = Y_y_H;	//node(x, 60)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// check with Yellow-X-UB(x = 50), find y
			nInfo.bound = Y_x_H;
			tmp_y = _chk_node(table, &nInfo, "Y-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = Y_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(50, y)
				nRes.node_count++;
				cost += __distance(x1, y1, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}
			// Secondly, check with Red boundary
			// check with Red-Y-UB(y = 40), find X
			nInfo.bound = R_y_H;
			tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);
			}
			// check with Red-X-UB(x = 30), find y
			nInfo.bound = R_x_H;
			tmp_y = _chk_node(table, &nInfo, "R-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(30, y)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);
			}
			// check with Red-Y-LB(y = 10), find X
			nInfo.bound = R_y_L;
			tmp_x = _chk_node(table, &nInfo, "R-Y-LB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_x[nRes.node_count] = R_y_L;	//node(x, 10)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1 ,nRes.node_count);
			}
			// check with Red-X-LB(x = 10), find y
			nInfo.bound = R_x_L;	
			tmp_y = _chk_node(table, &nInfo, "R-X-LB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_L;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(10, y)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);
			}
			printf("Total %d node\n", nRes.node_count);
			// must have 1 or 3 node
			if(nRes.node_count == 3 || nRes.node_count == 1){
				cost += __distance(x2, y2, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}else{
				printf("In G->Y/G->Y->R->Y case, must have 1 or 3 node!\n");
				return -1;
			}
		// point1 = Y, point2 = G
		}else if(mark[0] == YELLOW && mark[1] == GREEN){
			// First, check with Red boundary
			/* !!! <Check Lower bound FIRST> !!! */
			// check with Red-Y-LB(y = 10), find X
			nInfo.bound = R_y_L;
			tmp_x = _chk_node(table, &nInfo, "R-Y-LB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_x[nRes.node_count] = R_y_L;	//node(x, 10)
				nRes.node_count++;
				// first node need to calculate cost with endpoint
				if(nRes.node_count == 1){
					cost += __distance(x1, y1, 
							nRes.node_x[nRes.node_count-1], nRes.node_y[nRes.node_count-1]);
				}else{
					cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);	
				}

			}
			// check with Red-X-LB(x = 10), find y
			nInfo.bound = R_x_L;	
			tmp_y = _chk_node(table, &nInfo, "R-X-LB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_L;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(10, y)
				nRes.node_count++;
				// first node need to calculate cost with endpoint
				if(nRes.node_count == 1){
					cost += __distance(x1, y1, 
							nRes.node_x[nRes.node_count-1], nRes.node_y[nRes.node_count-1]);
				}else{
					cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);	
				}
			}
			// check with Red-Y-UB(y = 40), find x
			nInfo.bound = R_y_H;
			tmp_x = _chk_node(table, &nInfo, "R-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = R_y_H;	//node(x, 40)
				nRes.node_count++;
				// first node need to calculate cost with endpoint
				if(nRes.node_count == 1){
					cost += __distance(x1, y1, 
							nRes.node_x[nRes.node_count-1], nRes.node_y[nRes.node_count-1]);
				}else{
					cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);	
				}
			}
			// check with Red-X-UB(x = 30), find y
			nInfo.bound = R_x_H;
			tmp_y = _chk_node(table, &nInfo, "R-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = R_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(30, y)
				nRes.node_count++;
				// first node need to calculate cost with endpoint
				if(nRes.node_count == 1){
					cost += __distance(x1, y1, 
							nRes.node_x[nRes.node_count-1], nRes.node_y[nRes.node_count-1]);
				}else{
					cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);	
				}
			}
			// Secondly, check Yellow boundary
			// check with Yellow-Y-UB(y = 60), find x
			nInfo.bound = Y_y_H;
			tmp_x = _chk_node(table, &nInfo, "Y-Y-UB");
			if(tmp_x != 0.0){
				nRes.node_x[nRes.node_count] = tmp_x;
				nRes.node_y[nRes.node_count] = Y_y_H;	//node(x, 60)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);	
			}
			// check with Yellow-X-UB(x = 50), find y
			nInfo.bound = Y_x_H;
			tmp_y = _chk_node(table, &nInfo, "Y-X-UB");
			if(tmp_y != 0.0){
				nRes.node_x[nRes.node_count] = Y_x_H;
				nRes.node_y[nRes.node_count] = tmp_y;	//node(50, y)
				nRes.node_count++;
				cost += _get_dist_in_area(&nRes, nRes.node_count-1, nRes.node_count);	
			}
			printf("Total %d node\n", nRes.node_count);
			// must have 1 or 3 node
			if(nRes.node_count == 3 || nRes.node_count == 1){
				cost += __distance(x2, y2, nRes.node_x[nRes.node_count-1],
									nRes.node_y[nRes.node_count-1]);
			}else{
				printf("In Y->G/Y->R->Y->G case, must have 1 or 3 node!\n");
				return -1;
			}
		}else{
			printf("Fuckin other case ???? FUCK!!!\n");
			return -1;
		}

	return cost;
}

				

