#include "sched.h"
#include "phyAlloc.h"

#define NULL 0

static unsigned int pidCurrent = 0;
static struct pcb_s *current_process = NULL;
static struct pcb_s *first_process = NULL;
static struct pcb_s *idle = NULL;




void init_pcb(struct pcb_s *pcb, unsigned int pid, func_t f, void* args){
	pcb->pid = pid;
	pcb->f = f;
	pcb->args = args;
	pcb->etat = NEW;
	pcb->next = first_process;
	
	//Init de la pile pour le process
	pcb->stackPointer = (unsigned int) phyAlloc_alloc(STACK_SIZE);
	pcb->stackPointer += STACK_SIZE;
	pcb->stackPointer -= 4;
	pcb->stackPointer -= (13 * 4);
	
	pcb->pc = (int) start_current_process;
	pcb->etat = READY;
}

void create_process(func_t f, void *args, unsigned int stack_size){
	struct pcb_s *pcb = (unsigned int) (phyAlloc_alloc(sizeof(struct pcb_s)));
	init_pcb(pcb, pidCurrent++, f, args);
	
	if(pidCurrent==1){
		first_process = pcb;
	} else {
		current_process->next = pcb;
	}
	current_process = pcb;	
}

void start_current_process(){
	current_process->etat = RUNNING;
	current_process->f();
	current_process->etat = DEAD;
	ctx_switch();
}

void elect(){
	//TODO : choisir le process et changer current_process.
	
	//Pour l'instant, on prend le prochain élément dans la liste.
	//current_process = current_process->next;
	while(current_process->next->etat == DEAD){
		//Supprimer de la liste chainée.
		struct pcb_s *toDel = current_process->next;
		if(current_process == current_process->next){
			current_process->next = idle;
			idle->next = current_process;
			idle->pc=start_current_process;
		}else{
			current_process->next = current_process->next->next;
			//Libérer mémoire : la pile du process et son PCB
			toDel->stackPointer += (13 * 4);
			toDel->stackPointer += 4;
			toDel->stackPointer -= STACK_SIZE;
			phyAlloc_free((void *)toDel->stackPointer, STACK_SIZE);
		
			toDel = toDel - sizeof(struct pcb_s) + 4;
			phyAlloc_free(toDel , sizeof(struct pcb_s));
		}
	}
	if(current_process->etat != DEAD){
		current_process->etat = WAITING;
	}
	current_process = current_process->next;
	current_process->etat = RUNNING;
}

void start_sched(){
	//Initialisation de idle
	idle = (unsigned int) phyAlloc_alloc(sizeof(struct pcb_s));
	init_pcb(idle, -1, function_idle, NULL);
	current_process = idle;
}
void function_idle(){
	ctx_switch();
	while(1){}
}

void __attribute__ ((naked)) ctx_switch(){
	//	####	Sauvegarde du processus courant.
	//Sauvegarde des variables locales
	//Copie de tous les registres dans la pile d'exécution
	__asm("push {R0-R12}");
	
	//Sauvegarde du contexte
	__asm("mov %0, sp" : "=r"(current_process->stackPointer));
	__asm("mov %0, lr" : "=r"(current_process->pc));
	
	//	####	Election d'un nouveau process
	elect();
	
	//	####	Restaurer le process qui vient d'être choisi 
	//Restauration du nouveau contexte
	__asm("mov sp, %0" : : "r"(current_process->stackPointer));
	__asm("mov lr, %0" : : "r"(current_process->pc));
	
	//Restauration des variables locales
	__asm("pop {R0-R12}");
	
	//Branch sur le nouveau pc
	__asm("bx lr"); 
}

/*void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size){
	ctx->stackPointer = (unsigned int) phyAlloc_alloc(stack_size);
	ctx->stackPointer += stack_size;
	ctx->stackPointer -= 4;
	ctx->stackPointer -= (13 * 4);
	
	ctx->pc = (int) f;
}*/

/*void __attribute__ ((naked)) switch_to(struct ctx_s* ctx){
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
}*/
