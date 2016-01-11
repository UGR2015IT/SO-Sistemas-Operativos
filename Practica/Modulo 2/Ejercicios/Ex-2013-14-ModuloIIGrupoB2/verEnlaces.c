// primer argumento: ruta de un archivo
// segundo argumento: ruta de un directorio

// Recorre el directorio argv[2] en profundidad,
// muestra aquellos enlaces duros a argv[1] que encuentra

#include<unistd.h>	
#include<sys/stat.h>
#include<fcntl.h>		
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include <dirent.h>
#include <limits.h>
#include<string.h>
ino_t inodo;
int nEnlaces;

void recorreDir(char * p);
int main(int argc, char *argv[])
{
	struct stat atributos, a ;
	DIR *dir;
	if(setvbuf(stdout,NULL,_IONBF,0)) {
		perror("\nError en setvbuf");
	}
	if (argc!=3){
		printf("\nError ha introducido %d argumentos",argc);
		exit(-1);
	}
	// habría que comprobar que argv[2] existe y es de tipo directorio
	
	//Obtiene los atributos del archivo
	if (stat(argv[1], &a)) {
		(void) fprintf(stderr,"No se puede obtener los atributos de <%s>\n", argv[1]);
		closedir(dir) ;            
		exit(-1);
	}
	inodo=a.st_ino;
	nEnlaces=((int)a.st_nlink);
	recorreDir(argv[2]);
	
}

void recorreDir(char * p){
	char nombreCompleto[256];
	DIR *dir;
	struct dirent *archivo;
	struct stat atributos;
	if ((dir = opendir(p)) == NULL) {
			perror("\nError en opendir\n");
	    	exit(-1);
  	}
	while ((archivo=readdir(dir)) != NULL && nEnlaces>0) {
		//Se almacena el nombre completo del archivo actual
		if (strcmp(archivo->d_name,".")==0 || strcmp(archivo->d_name,"..")==0 ) continue;
		sprintf(nombreCompleto, "%s/%s", p, archivo->d_name);
		//Obtiene los atributos del archivo

		if (stat(nombreCompleto, &atributos)) {
			(void) fprintf(stderr,"Error en stat de <%s>\n", nombreCompleto);
			closedir(dir) ;                  
			exit(-1);
  		}
		if(S_ISDIR(atributos.st_mode)) recorreDir(nombreCompleto);
		else{
			if(((int)inodo) == ((int)atributos.st_ino)){
				printf("%s\n",nombreCompleto);
				nEnlaces--;
			}
		}
	}
	

}
