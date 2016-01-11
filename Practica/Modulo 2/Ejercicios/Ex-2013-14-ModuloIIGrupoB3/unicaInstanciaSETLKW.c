#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
// programa que quiere asegurarse
// de que esta solo.
// bloque el archivo "unic" con F_SETLKW

int main (int argc, char * argv[]) 
{
	char s[10];
	struct flock cerrojo;
	int fd;

	if ((fd=open("unic", O_RDWR|O_CREAT,0777)) == -1 ){
		perror("Error en open");
		exit (-1);
	}
	cerrojo.l_type=F_WRLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	printf("%s:intento un bloqueo de escritura del archivo %s \n",argv[0],"unic");
	if (fcntl (fd, F_SETLKW, &cerrojo) == -1) {
			perror ("Error en fcntl"); // tratamiento estandar de errores
			exit(-1);
	}
	printf(" Solo estoy yo");
	printf(" escriba cualquier cadena");
	scanf("%s",s);
	return 0;
}
