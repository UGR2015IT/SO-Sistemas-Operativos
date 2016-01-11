#include <stdio.h>	
#include <stdlib.h>	
#include <unistd.h>	
#include <string.h>	
#include <errno.h>	
int main(int argc, char *argv[ ])	
{int i;			/* numero de este proceso (comenzando en 1) */	
int childpid;	/* para recibir el resultado de fork */	
int nprocs;		/* nº  total de procesos en el anillo */	
int fd[2];		
int error;			
if ( (argc!= 2) || ((nprocs=atoi(argv[1])) <= 0) ) {	
	fprintf(stderr, "Error en %s Debe dar como argumento el nº de procesos nprocs\n",argv[0]);
	exit(1); }
if (pipe (fd) == -1) {	
	perror("No se puede crear pipe");
	exit(1);		
}		
if ((dup2(fd[0], STDIN_FILENO) == -1) ||		
	(dup2(fd[1], STDOUT_FILENO) == -1)) {	
	perror("Could not dup pipes");	
	exit(1);	
}		
if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {		
	perror("Could not close extra descriptors");	
	exit(1);	
}		
// crea los restantes procesos y sus conexiones	
for (i = 1; i <nprocs; i++) {		
	if (pipe (fd) == -1) {	
		fprintf(stderr,"Could not create pipe %d: %s\n",
		i,strerror(errno));
		exit(1);
	}	
	if ((childpid= fork()) == -1) {	
		fprintf(stderr, "Could not create child %d: %s\n",
		i,strerror(errno));
		exit(1);
	}	
	if (childpid> 0) 
		error = dup2(fd[1], STDOUT_FILENO);
	
	else	
		error = dup2(fd[0], STDIN_FILENO);
		
	if (error == -1) {	
		fprintf(stderr, "Could not dup pipes for iteration %d: %s\n",i,strerror(errno));
		exit(1);
	}	
	if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {	
		fprintf(stderr, "Could not close extra descriptors %d: %s\n",i,strerror(errno));
		exit(1);
	}	
	if (childpid)	
		break;
}// for		
fprintf(stderr,"Proceso con i= %d, pid %d y padre %d\n",i, 	(int)getpid(), (int)getppid());		
exit(0);
}
