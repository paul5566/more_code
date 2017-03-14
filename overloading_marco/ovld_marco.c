#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define in1(a, b)	printf("in1 : %d %d\n",a ,b)
#define in2(a, b, c)	printf("in2 : %d %d %d\n", a, b, c)
#define mid(x, y, z, func, ...)	func
#define out(args...)	mid(args, in2, in1,...)(args)

int main(int argc, char **argv)
{
	if(argc < 2){
		printf("Please enter argument\n");
		printf("%s [0/1]\n", argv[0]);
		return -1;
	}
	if(*argv[1] == '0')	{
		printf("passing 2 argument ...\n");
		out(1, 2);
	} else if(*argv[1] == '1') {
		printf("passing 3 argument ...\n");
		out(1, 2, 3);
	}

	return 0;
}
/********************************************************************
 *																	*			
 *	out(1, 2) -> mid(1, 2, in2, in1)(1, 2)							*
 *	mid(x, y, z, func, ...) func -> mid(1, 2, in2, in1) in1			*
 *  func = in1														*
 *  run : in1														*
 *																	*	
 ********************************************************************/

/********************************************************************
 *																	*
 *	out(1, 2, 3) -> mid(1, 2, 3, in2, in1)(1, 2, 3)					*
 *	mid(x, y, z, func, ...) func -> mid(1, 2, 3, in2, in1) in2		*
 *	func = in2														*
 *	run : in2														*
 *																	*
 ********************************************************************/
