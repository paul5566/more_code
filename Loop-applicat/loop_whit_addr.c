/*
 *	In this case, initial x = 1 (main(int argc), cmd argc = 1, so x = 1)
 *	&exit - &main = (main func addr) shift to (exit func addr),
 *	x/100 when (x < 100) = 0 (because the type of x is int)
 *	if x = 100, x/100 = 1, &main + main addr shift to exit addr = &exit
 *	So call exit function, and exit the program
 *
 *	!!!!! WARRNING !!!!
 *	This maybe case stack overflow, casue the way is continue call main
 *	function without close it, so when stack full and cover to heap,
 * 	This will be cause segemation fault
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(int x)
{
	printf("Hello, World, x = %d\n", x);
	printf("main func addr : %p, exit func addr : %p, printf func addr : %p\n", 
			&main, &exit, &printf);	

	sleep(1);
	(&main+(&exit-&main)*(x/100))(++x);	// goto main function addr to do () thing
}
