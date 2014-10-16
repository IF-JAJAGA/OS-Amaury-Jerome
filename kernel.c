#include "sched.h"
#include "hw.h"

#include <stdlib.h> 


void funcA()
{
	int cptA = 0;
	while ( cptA < 3 ) {
		cptA ++;
		ctx_switch();
	}
}
void funcB()
{
	int cptB = 1;
	while ( cptB < 6 ) {
		cptB += 2 ;
		ctx_switch();
	}
}
//------------------------------------------------------------------------
int kmain ( void )
{
	init_hw();
	create_process(funcB, NULL, STACK_SIZE);
	create_process(funcA, NULL, STACK_SIZE);
	start_sched();
	ctx_switch();
	/* Pas atteignable vu nos 2 fonctions */
	return 0;
}
