#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <memoriaI.h>
#include <semaforoI.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#define MAX_T_LANZAR 3
#define COCHES 30
#define MUTEXN "mutexn"
#define MUTEXS "mutexs"
#define COCHESNORTE "cn"
#define COCHESSUR "cs"
#define PUENTE "puente"

pid_t pids[COCHES];
void liberarecursos (); void finalizarprocesos (); void controlador (int senhal);
int main (int argc, char *argv[]) {
pid_t pid_hijo; int i;

srand((int)getpid());
// Creación de semáforos y segmentos de memoria compartida.
crear_sem(PUENTE, 1); crear_sem(MUTEXN, 1); crear_sem(MUTEXS, 1);
crear_var(COCHESNORTE, 0); crear_var(COCHESSUR, 0);
if (signal(SIGINT, controlador) == SIG_ERR) {
fprintf(stderr, "Abrupt termination.\n"); exit(EXIT_FAILURE);
}
for (i = 0; i < COCHES; i++) {
switch (pid_hijo = fork()) {
// Se lanzan los coches...
case 0:

if ((i % 2) == 0) execl("./exec/coche", "coche", "N", PUENTE, MUTEXN, COCHESNORTE, NULL);
else
break;
execl("./exec/coche", "coche", "S", PUENTE, MUTEXS, COCHESSUR, NULL);
}
sleep(rand() % MAX_T_LANZAR);
}
for (i = 0; i < COCHES; i++) waitpid(pids[i], 0, 0);
liberarecursos(); return EXIT_SUCCESS;

}
void liberarecursos () {
destruir_sem(PUENTE); destruir_sem(MUTEXN); destruir_sem(MUTEXS);
destruir_var(COCHESNORTE); destruir_var(COCHESSUR);
}
void finalizarprocesos () {
int i; printf ("\n--- Finalizando procesos --- \n");
for (i = 0; i < COCHES; i++)
if (pids[i]) {

printf ("Finalizando proceso [ %d]... ", pids[i]);

kill(pids[i], SIGINT); printf ("<Ok>\n");

}


}


void controlador (int senhal) {
finalizarprocesos(); liberarecursos();
printf ("\nFin del programa (Ctrol + C)\n"); exit(EXIT_SUCCESS);
}