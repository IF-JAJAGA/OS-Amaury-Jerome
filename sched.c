#include "sched.h"
#include "phyAlloc.h"

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size){
	ctx->stackPointer = phyAlloc_alloc(stack_size);
	ctx->stackPointer -= (stack_size -1 - 13);
	ctx->pc = *f;
}

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx){
	//Sauvegarde des variables locales
	//Copie de tous les registres dans la pile d'exécution
	__asm("push {R0-R12}");
	
	//Sauvegarde du contexte
	__asm("mov %0, sp" : "=r"(current_ctx->stackPointer));
	__asm("mov %0, lr" : "=r"(current_ctx->pc));
	
	
	//Changement du pointeur sur le nouveau contexte
	current_ctx = ctx;
	
	//Restauration du nouveau contexte
	__asm("mov sp, %0" : : "r"(current_ctx->stackPointer));
	__asm("mov lr, %0" : : "r"(current_ctx->pc));
	
	//Restauration des variables locales
	__asm("pop {R0-R12}");
	
	//Branch sur le nouveau pc
	__asm("bx lr"); 
}
