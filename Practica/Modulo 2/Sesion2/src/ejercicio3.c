// ================================================= //
//		    ejercicio3.c    		     //
//Orden recursivo para volver ejecutables para el    //
//grupo y para otros, numero de inodo e espacio	     //
//FORMATO: ./buscar <pathname>		             //
// ================================================= //

#include<sys/types.h>	
#include<unistd.h>	
#include<sys/stat.h>
#include<fcntl.h>	
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

int n_ficheros;
long unsigned tam = 0;

void analisi_recursiva(char*, char*);

int main(int argc, char *argv[]){
	
	printf("Los i-nodos son:\n");
    
        // INSERIRE FUNZIONE PER L'ANALISI DEGLI ATTRIBUTI DI OGNI FILE
	analisi_recursiva("",argv[1]);

        printf("Existen %d archivos regulares con permiso x para grupo y otros\n",n_ficheros);
        printf("El tamaño total ocupado por dichos archivos es %lu bytes\n",tam);
 
        return 0;

}

void analisi_recursiva(char* path, char* file_name){
	struct stat atributos;
	struct dirent *fichero;
        DIR *dirp;
	char temp_path[256], file_name_path[256];

	strcpy(file_name_path, file_name);
	strcpy(temp_path, path);
	strcat(temp_path, file_name_path);

	//printf ("Esamino la directory: %s\n", temp_path);
	//ABERTURA DEL DIRECTORIO
	dirp = opendir(file_name_path);
	if (dirp == NULL){
                 printf("Error: No se puede abrir el directorio: %s\n",temp_path);
                 exit(2);
        }

	while ((fichero = readdir(dirp)) != NULL){
		//CAMBIO DE DIRECTORIO DE TRABAJO
		chdir(file_name_path);

		//comprobamos si puedo acceder al dicho fichero (d_name) en el directorio y a su atributos (almacenados en &atributos)
	        if(stat(fichero->d_name,&atributos) < 0) {
	                printf("Error al intentar acceder a los atributos de %s%s\n", temp_path, fichero->d_name);

	        } else {
			//printf("Esamino il file: %s%s \n", temp_path,fichero->d_name);

	                if(fichero->d_type!=DT_DIR){
				if ((S_ISREG(atributos.st_mode)) && (atributos.st_mode & S_IXGRP) && (atributos.st_mode & S_IXOTH)) {
					//Los inodos son unsigned long int, necesito lu
	    				printf("./%s%s %lu\n",temp_path,fichero->d_name, atributos.st_ino);
					//printf("Debugging path: %s\n",temp_path);
					n_ficheros++;
					tam += atributos.st_ino;
				}
	                } else {
				//Reinicia el bucle por el nuevo directorio
				// PAsso la concatenazione dell'attuale dir e il precedente
				if (strcmp(fichero->d_name,".")!=0 && strcmp(fichero->d_name,"..")!=0){
					analisi_recursiva(temp_path,strcat(fichero->d_name,"/"));
					chdir("../");
				}
			}
	        }
        }
 
        //CERRO EL DIRECTORIO
        closedir(dirp);
}
