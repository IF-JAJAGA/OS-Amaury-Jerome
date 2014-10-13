#ifndef SCHED_H
#define SCHED_H

#define STACK_SIZE 1024 

typedef void (*func_t) (void);


/*struct ctx_s{
	unsigned int pc;
	unsigned int stackPointer;
};*/

//struct ctx_s* current_ctx;

enum state{NEW,READY,RUNNING,WAITING,DEAD};

struct pcb_s{
	unsigned int pid;
	func_t f;
	unsigned int pc;
	unsigned int stackPointer;
	void *args;
	enum state etat;
	struct pcb_s *next;
};


/*void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);*/
void __attribute__ ((naked)) ctx_switch();

void init_pcb(struct pcb_s *pcb, unsigned int pid, func_t f, void* args);
void start_current_process();
void elect();
void start_sched();
void function_idle();

void create_process(func_t f, void *args, unsigned int stack_size);
void __attribute__ ((naked)) ctx_switch();

#endif
