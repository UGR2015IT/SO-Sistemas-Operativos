#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include<stdlib.h>
// en esta version se han creado más procesos.
// manteniendo solo 2 procesos, entonces solo el padre llamaria a exec
// y el hijo lee y escribe "a mano".
main (int argc, char *argv[]) {
	int dd , ret, valor;
	int dtub[2];
	char c[2]="\0\0";

	if (argc !=2 ){printf("Debe dar un argumento");
		return -1;}
		
	
	pipe(dtub);
	
	if (fork()== 0){ // hijo que creara otro hijo, es necesario si queremos
					///	que todas las lecturas y escrituras se hagan con exec de cat
					// manteniendo que se use un cauce 
					// hay que tener en cuenta que las lecturas de un cauce son 
					//destructivas!!!
			if (fork()==0) {///// proceso que lee de dtub[0] y escribe en argv[1] 
				if ((dd = open(argv[1], O_WRONLY|O_CREAT, 0600)) < 0) {
					perror("\nError en open dd");
					exit(-1);
				}
				close(dtub[1]);//cierro lo que no necesito
				dup2(dtub[0], STDIN_FILENO);
				dup2(dd, STDOUT_FILENO);
				execlp("cat","cat",NULL); // leera del cauce y escribira en argv[1]
				perror("error en exec de cat");	
				}		
			else { // proceso que lee de argv[1] y escribe en sal.est	
				if ((dd = open(argv[1], O_RDONLY|O_CREAT, 0600)) < 0) {
					perror("\nError en open dd");
					exit(-1);
				}
				close(dtub[0]);//cierro lo que no necesit
				close(dtub[1]);//cierro lo que no necesito
				dup2(dd, STDIN_FILENO);
				execlp("cat","cat",NULL); // leera de argv[1] y escribira en sal.est.
				perror("error en exec de cat");	
				}
		
	} 
	else {// padre lee de ent.est.  y escribe en dtub[1] 
		close(dtub[0]);// cierro lo que no necesito
		// solo hace falta redirigir la salida estandar
		dup2(dtub[1],STDOUT_FILENO);
		
		execlp("cat","cat",NULL);
	}
	
	
}