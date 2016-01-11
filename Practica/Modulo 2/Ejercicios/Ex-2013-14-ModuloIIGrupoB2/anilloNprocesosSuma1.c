//////// CADA PROCESO LEE 1 CARACTER, LE SUMA 1 Y LO ESCRIBE

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
int error, ret, n,p ;
char c[2]="\0\0";			
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

if (write(1, "0", 1)<0){
		fprintf(stderr, "Write en hijo: %d\n",errno);
		exit(-1);
}

// crea los restantes procesos y sus conexiones	
for (i = 1; i <nprocs; i++) {		
	if (pipe (fd) == -1) {	
		fprintf(stderr,"Could not create pipe %d: %s\n",i,strerror(errno));
		exit(1);
	}	
	if ((childpid= fork()) == -1) {	
		fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
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
////  ACTUACION DE CADA PROCESO   ///////////////////
p=getpid();
fprintf(stderr,"Proceso con i= %d, pid %d y padre %d\n",i, 	(int)p, (int)getppid());		
ret = read(0, c, 1);
//fprintf(stderr, "Acabo de leer: %s\n",c );
		while ( (ret > 0) && (strcmp(c,"9")!=0)) {
		
			sscanf (c,"%i", &n );
			fprintf(stderr, "Soy pid %d, me llega numero %d\n",(int) p , n  );
			sprintf(c,"%d",n+1);			
			if (write(1, c, 1)<0){
				fprintf(stderr, "Write en hijo: %d\n",errno);
				exit(-1);
			}
			fprintf(stderr, "Soy pid %d, acabo de escribir: %s\n", (int) p, c );

			ret = read(0, c, 1);
		}
		if (ret<0) {
			fprintf(stderr, "Error al leer en el hijo %d\n",errno);
			exit(-1);
		}


exit(0);/////////
}
