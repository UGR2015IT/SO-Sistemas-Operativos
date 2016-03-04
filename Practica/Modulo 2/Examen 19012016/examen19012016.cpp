// Gallitelli
// Davide

// Codigo fuente del examen del 19012016, modulo II

#include <stdio>
#include <stdlib>
#include <stderr.h>
#include<sys/types.h>	
#include<unistd.h>	
#include<sys/stat.h>
#include<dirent.h>
#include <string.h>

#define ERROR -1

int main (int argc, char *argv[]) {

	struct stat atributos;
	struct dirent *fichero;
        DIR *dirp;
	char* ruta = argv[1];
	char* cauche = argv[2];
	char accion;
	int fd,leer_accion;
	
	// COmprobacion inicial sobre la ejecucion del programa
	if (argc != 3){
		printf("Error de sintaxis! \n Sintaxi correcta: ./program <path> <FIFO> \n");
		return ERROR;
	} else {
		if (stat(argv[1],&atributos) < 0){
			printf ("Error para intentar a acceder a las informacciones del directorio \n");
			return ERROR;
		} else {
			if (!S_ISDIR(atributos.st_mode)) {
				printf("La ruta posta como primero argumento no es un directorio! \n");
				return ERROR;
			}
		}
	}
	// Creaccion del FIFO
	umask(0);
	mknod(cauche,S_IFIFO|0666,0);

	// Abrir directorio (ruta)
	dirp = opendir(argv[1]);
	if (dirp == NULL){
                 printf("Error: No se puede abrir el directorio: %s\n",argv[1]);
                 exit(2);
        }

	do {
		fichero = readdir(dirp);
	} while ((strcmp(fichero->d_name,".") != 0) && (strcmp(fichero->d_name,"..") != 0));
	
	// Abrir el FIFO para leer que hacer
	if ( (fd=open(cauche,O_RDWR)) <0) {
		printf("Error en abrir el archivo FIFO! \n");
		return ERROR;
	} else {
		do {			
			struct stat atr_fichero;
			leer_accion = read(fd,accion,1);
			char my_string[1024];
		        sprintf(my_string,"Leida por el FIFO una %c",accion);	
			//printf("Leida por el FIFO una %c",accion);
			write(STDOUT_FILENO,my_string,sizeof(my_string));
			if (strcmp(accion,"I"==0) {
				my_string = "";
				// Accion I
				// Mostrar en salida estandar NOMBRE INODO Y PERMISOS
				stat(argv[1],&atr_fichero);
				sprintf (my_string,"Informacciones sobre el fichero \n");
				sprintf (my_string, "Nombre: %s \n",fichero->d_name);
				sprintf (my_string,"Inodo: %d \n", (int)atr_fichero.st_ino);
				sprintf (my_string,"Permisos: %d \n\n", atr_fichero.st_mode & 0777);
				write(STDOUT_FILENO,my_string,sizeof(my_string));
			}
			else if (strcmp(accion,"S")==0) {
				// Accion S
				// lee siguiente entrada, si no puede imprime error
				if (readdir(dirp) == NULL) printf ("No hay otro ficheros en esto directorio! \n");
			}
			else if (strcmp(accion,"D")==0) {
				// Accion D
				// si entrada actual es directorio, abre y lee primera entrada, si no error
				if (stat(fichero->d_name,&atr_fichero) < 0){
					printf ("Error para intentar a acceder a las informacciones del directorio \n");
					return ERROR;
				} else {
					if (!S_ISDIR(atr_fichero.st_mode)) printf("La entrada seleccionada no es un directorio! \n");
					else {
						chdir(fichero->d_name);
						do {
							fichero = readdir(dirp);
						} while ((strcmp(fichero->d_name,".") != 0) && (strcmp(fichero->d_name,"..") != 0));
					}
				}
			}
			else if (strcmp(accion,"U")==0) {
				// Accion U
				// Va al dir padre y lee primera entrada
				if (strcmp(dirp->d_name,"/")==0) printf("Estoy deja en el directorio raiz!");
				else {
					chdir("..");
					do {
						fichero = readdir(dirp);
					} while ((strcmp(fichero->d_name,".") != 0) && (strcmp(fichero->d_name,"..") != 0));
				}
			}
			else if (strcmp(accion,"G")==0) {
				// Accion G
				// si entrada=archivo_regular
				if (stat(fichero->d_name,&atr_fichero) < 0){
					printf ("Error para intentar a acceder a las informacciones del directorio \n");
					return ERROR;
				} else {
					if (S_ISREG2(atr_fichero.st_mode)){
						// leer de STDIN un literal
						char literalLeido[256],buffer[1024];
						pid_t pid;
						int fd[2];
						pipe(fd);
						do{
							int read_literal = read(STDIN_FILENO,literalLeido,1);
						} while (read_literal != 0);
						// hijo que ejecuta "grep literalLeido rutaActual"
						if ( (pid=fork())==0){
							close(fd[0]);
							close(STDOUT_FILENO);
							dup(fd[1],STDOUT_FILENO);
							execlp("grep","grep", &literalLeido, fichero->d_name);
						} else if (pid < 0) {
							printf ("Error en crear el hijo!");
						} else {
						// Espera por el hijo y mostra mensaje con codigo de fin
							if (waitpid(pid[i],0,0) > 0) {
								printf("Hijo acaba.");
								close(fd[1]);
								read(fd[0],buffer,strlen(buffer)+1);
							}
						}
					}
				}
			}
			else if (strcmp(accion,"P")==0) // Accion P
			else if (strcmp(accion,"F")!=0){
				printf("Orden no reconoscido, error!");
			}
		} while (strcmp(accion,"F")!=0);
	}
	return 0;
}
