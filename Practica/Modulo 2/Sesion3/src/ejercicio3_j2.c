#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define NUM_PROC 20

void main(int argc, char *argv[]){
	int i=0;
	pid_t pid1, pid2;

	// Jerarquia de procesos tipo 2
	// Solo el padre inicial puede generar hijos

	printf("Initial PID: %d \n\n",getpid());

	for (i=0;i<NUM_PROC;i++){
		if ((pid1 = fork()) == 0){
			printf("Hola soy el hijo %d, con PID %d y padre de PID %d \n",i,getpid(),getppid());
			break;
		}
		else if (pid1 < 0) {
			printf("Could not create child in hierarchy 1.");
			return;			
		} else if (pid2=(waitpid(pid1),0,0) > 0) {}
	}

}
