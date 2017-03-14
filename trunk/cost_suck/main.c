#include "get_cost.h"

#ifdef TESTING_LOOP
#include <bsd/stdlib.h>
int main(void)
{
	int i = 0;
	int randval;
	float cost;
	struct argu_table table;
	
	printf("Start testing loop !!!\n");

	do{
		randval = arc4random() % 98 + 1;
		table.x1 = (float)randval;
		randval = arc4random() % 98 + 1;
		table.y1 = (float)randval;
		randval = arc4random() % 98 + 1;
		table.x2 = (float)randval;
		randval = arc4random() % 98 + 1;
		table.y2 = (float)randval;

		if(table.x1 == table.x2 &&
			table.y1 == table.y2)
		{
			continue;
		}

		i++;
		printf("EndPoint: P1(%.2f. %.2f)  P2(%.2f, %.2f)\n",
		table.x1, table.y1, table.x2, table.y2);

		cost = get_cost(&table);
		printf("Cost = %f, << %d round passing >>\n", cost, i);
		usleep(400*1000);
	}while(1);

	return 0;
}
#else
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
#endif
