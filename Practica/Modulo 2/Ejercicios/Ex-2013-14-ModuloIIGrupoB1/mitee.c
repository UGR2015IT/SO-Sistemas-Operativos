#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include<stdlib.h>
/////////// no hace falta redirigie nada
main (int argc, char *argv[]) {
	int dd , ret, valor;
	int dtub[2];
	char c[2]="\0\0";

	if (argc !=2 ){printf("Debe dar un argumento");
		return -1;}
		
	
	
	if ((dd = open(argv[1], O_WRONLY|O_CREAT, 0600)) < 0) {
				perror("\nError en open dd");
				exit(-1);
			}
	
	pipe(dtub);
	
	if (fork()== 0){///// hijo lee de dtub[0] y escribe en argv[1] y en sal.est.
	
		close(dtub[1]);// cierro lo que no necesito
		ret = read(dtub[0], c, 1);
		while (ret > 0){
			if (write(dd, c, 1)<0){
				perror("Write en hijo sobre dd ");
				exit(-1);
			}
			if (write(STDOUT_FILENO, c, 1)<0){
				perror("Write en hijo sobre STDOUT_FILENO");
				exit(-1);
			}
	
			ret = read(dtub[0], c, 1);
		}
		if (ret<0) {
			perror("Error al leer en el hijo");
		}
		exit(0);
	} else {// padre lee de ent.est.  y escribe en dtub[1] 
		close (dtub[0]); // cierro lo	que no necesito
		ret = read(STDIN_FILENO, c, 1);
		while (ret >0){
			if (write(dtub[1], c, 1)<0){
				perror("Write en padre");
				exit(-1);
			}
		ret=read(STDIN_FILENO, c, 1);
		}
		if (ret<0) {
			perror("Error al leer en el padre");
		}
	}
	
	
}