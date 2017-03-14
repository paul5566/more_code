#include "get_cost.h"

int main(int argc, char **argv)
{
	float cost;
	struct argu_table table;

	if(argc != 5){
		printf("Wrong\n");
		return -1;
	}	
	table.x1 = (float)atof(argv[1]);
	table.y1 = (float)atof(argv[2]);
	table.x2 = (float)atof(argv[3]);
	table.y2 = (float)atof(argv[4]);
	
	printf("EndPoint: P1(%.2f. %.2f)  P2(%.2f, %.2f)\n",
		table.x1, table.y1, table.x2, table.y2);

	cost = get_cost(&table);
	
	printf("Cost = %f\n", cost);

	return 0;
}
