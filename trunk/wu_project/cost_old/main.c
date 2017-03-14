#include "get_cost.h"

int main(int argc, char **argv)
{
	float total_cost;
	struct argu_table table;
	
	if(argc != 5){
		printf("Usage: ");
		printf("./cost [Point1 x] [Point1 y] [Point2 x] [Point y]\n");
		return -1;
	}

	printf("int: ?");
	scanf("%d", &test);

	table.x1 = atoi(argv[1]);
	table.y1 = atoi(argv[2]);
	table.x2 = atoi(argv[3]);
	table.y2 = atoi(argv[4]);
	
	total_cost = get_cost(&table);
	printf("Cost %f\n", total_cost);	
	
	return 0;
}
	
	
