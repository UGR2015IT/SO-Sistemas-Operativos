#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include<stdlib.h>
main (int argc, char *argv[]) {
	int df,dd , ret, valor;
	int dpadreAhijo[2];
	char c[2]="\0\0";

	if (argc !=3 ){printf("Debe dar dos argumento");
		exit(-1);}
		
	if ( (strcmp(argv[1],"-") ==0) ) {
			df=STDIN_FILENO;}
			else if ((df = open(argv[1], O_RDONLY|O_CREAT, 0600)) < 0) {
				perror("\nError en open argv[1]");
				exit(-1);
			}
	
	if ( (strcmp(argv[2],"-") ==0) ) {
			dd= STDOUT_FILENO;}
			else if ((dd =open(argv[2], O_WRONLY|O_CREAT, 0600) ) < 0) {
				perror("\nError en open argv[2]");
				exit(-1);
			}
			
	pipe(dpadreAhijo);
	
	if (fork()== 0){///// hijo lee de pipe y escribe en dd
		close(dpadreAhijo[1]);// cierro lo que no necesito
		
		dup2 (dpadreAhijo[0], STDIN_FILENO);
		dup2 (dd , STDOUT_FILENO);
		
		execlp ("leede0Escribeen1", "leede0Escribeen1",NULL);
		
	} else {// padre lee de df y escribe en dtub[1]
		close(dpadreAhijo[0]);//cierro lo que no necesito
		
		dup2 (df , STDIN_FILENO);
		dup2(dpadreAhijo[1], STDOUT_FILENO);
		
		execlp ("leede0Escribeen1", "leede0Escribeen1",NULL);

		
		
	}
	
	
}