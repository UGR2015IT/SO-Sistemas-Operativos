#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
// programa que quiere asegurarse
// de que esta solo.
// bloque el archivo "unic" con F_SETLK

int main (int argc, char * argv[]) 
{
char s[10];
struct flock cerrojo;
int fd;
umask(0);
if ((fd=open("unic", O_RDWR|O_CREAT,0777)) == -1 ){
		perror("Error en open");
		exit (-1);
}
cerrojo.l_type=F_WRLCK;
cerrojo.l_whence=SEEK_SET;
cerrojo.l_start=0;
cerrojo.l_len=0;
printf("%s:intento un bloqueo de escritura del archivo %s \n",argv[0],"unic");
while (fcntl (fd, F_SETLK, &cerrojo) == -1) {
	// si es EAGAIN vuelvo
	if (errno != EAGAIN) {
		perror ("Error en fcntl");
		exit(-1);
	}
}// while
printf(" Solo estoy yo");
printf(" escriba cualquier cadena");
scanf("%s",s);
return 0;
}
