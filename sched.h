// STACK_SIZE doit être suffisament grand pour contenir les éléments de contexte,
// les éventuels paramètres des différentes fonctions ainsi que le code lui-même
// Puisqu'on y trouve des boucles infinies, on choisit une variable assez grande 
// pour contenir un bon nombre d'informations et pas trop grande pour pouvoir tourner
// sur le Raspberry Pi.
#define STACK_SIZE 512

typedef void (*func_t) (void);

struct ctx_s {
// pointeur de pile
int sp;
// adresse de l'instruction en cours
int lr;
//fonction associée au contexte
func_t f_ctx;
};

struct ctx_s* current_ctx;

void __attribute__ ((naked)) switch_to (struct ctx_s* ctx);

