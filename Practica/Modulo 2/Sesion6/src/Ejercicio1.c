#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){

	if (argc != 4){
		printf("Error: necesito de 3 argumentos. \n nombreprog orden \"</>\" archivo \n");
		return -1;
	} else {
		//exec del orden insertado por pantalla
		char* orden = argv[1];
		char* redireccion = argv[2];
		char* filename = argv[3];
		int fd;
		fd = open(filename, O_RDWR|O_TRUNC);
		
		if (strcmp(redireccion,"<")==0){
			//rediccionar desde el archivo
			close (1);
			if (fcntl(fd, F_DUPFD, 1) == -1){
				perror("Fallo en fcntl desde el archivo");
				return -1;
			}
		} else if (strcmp(redireccion,">")==0){
			//redireccionar hacia el archivo
			close(fd);
			if (fcntl(1, F_DUPFD, fd) == -1){
				perror("Fallo en fcntl hacia el archivo");
				return -1;
			}
		} else {
			printf("El segundo argumento tiene que ser < (desde) o > (hacia). Reiniciar el programa.\n");
			return -1;
		}
		printf ("Redireccionamiento hecho, vuelvo a ejecutar el orden %s \n",orden);
		if (execl(orden, "", NULL) == -1) {
			perror("Error en el execl de el orden");
			return -1;
		}
		return 0;
	}

}
