#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc < 3) {
		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Sintaxi: %s <programa> <archivo>\n\n", argv[0]);
		return -1;
	} else {
		//Declaracion de variables
		struct flock cerrojo;
		char* filename = argv[1];
		int fd, i;

		//Abrimos el archivo
		if ((fd=open(filename, O_RDWR)) == -1 ){
			perror("Fallo al abrir el archivo");
			return -1;
		}
		//Definicion del cerrojo: bloqueo de escritura de todo el archivo
		cerrojo.l_type=F_WRLCK;
                cerrojo.l_whence=SEEK_SET;
                cerrojo.l_start=0;
                cerrojo.l_len=0;
		//Bloqueo de escritura!
		if (fcntl (fd, F_SETLKW, &cerrojo) == EDEADLK) printf ("%s ha dado un EDEADLK\n", filename); 
		printf("Ahora el archivo %s esta bloqueado en escritura. \n", filename);
		//Qualcuier otro proceso
		sleep(20);
		//Desbloqueo
		cerrojo.l_type=F_UNLCK;
		cerrojo.l_whence=SEEK_SET;
		cerrojo.l_start=0;
		cerrojo.l_len=0;
		if (fcntl (fd, F_SETLKW, &cerrojo) == -1) {
			perror ("Error al desbloquear el archivo");
		}
		return 0;
	}
}
