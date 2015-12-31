#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){

	if (argc != 4){
		printf("Error: necesito de 3 argumentos. \n nombreprog orden \"</>\" archivo \n");
		return -1;
	} else {
		// Declaracion y incializacion de variables
		char* orden = argv[1];
		char* redireccion = argv[2];
		char* filename = argv[3];
		int fd;
		fd = open(filename, O_RDWR|O_TRUNC);
		
		if (strcmp(redireccion,"<")==0){
			//rediccionar desde el archivo
			close (STDIN_FILENO);
			if (fcntl(fd, F_DUPFD, STDIN_FILENO) == -1){
				perror("Fallo en fcntl desde el archivo");
				return -1;
			}
		} else if (strcmp(redireccion,">")==0){
			//redireccionar hacia el archivo
			close(STDOUT_FILENO);
			if (fcntl(fd, F_DUPFD, STDOUT_FILENO) == -1){
				perror("Fallo en fcntl hacia el archivo");
				return -1;
			}
		} else {
			//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
			printf("El segundo argumento tiene que ser < (desde) o > (hacia). Reiniciar el programa.\n");
			return -1;
		}
		//Ejecutar la orden
		if (execlp(orden, "", NULL) < 0) {
			perror("Error en el execl de el orden");
			return -1;
		}
		// Cerrar el fichero y devolver 0
		close(fd);
		return 0;
	}

}
