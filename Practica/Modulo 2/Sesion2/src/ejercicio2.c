// ================================================= //
//		    ejercicio2.c    		     //
//Realiza un programa en C por el cambio de permisos //
//- pathname como argumento per il fichero	     //
//- octal de 4 digitos per los permisos              //
// ================================================= //

#include<sys/types.h>	
#include<unistd.h>	
#include<sys/stat.h>
#include<fcntl.h>	
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>


int main(int argc, char *argv[]){
	struct stat atributos;
	
	struct dirent *fichero;
        DIR *dirp;

        int chmod_result, permisos_antiguos, permisos_nuevos;


	//ABERTURA DEL DIRECTORIO
	dirp = opendir(argv[1]);
	if (dirp == NULL){
                 printf("Error: No se puede abrir el directorio: %s\n",argv[1]);
                 exit(2);
        }

	while ((fichero = readdir(dirp)) != NULL){
		//CAMBIO DE DIRECTORIO DE TRABAJO
		chdir(argv[1]);
		
		//comprobamos si puedo acceder al dicho fichero (d_name) en el directorio y a su atributos (almacenados en &atributos)
                if(stat(fichero->d_name,&atributos) < 0) {
                        printf("Error al intentar acceder a los atributos de %s\n",fichero->d_name);

                } else {

                        //Si se puede, comprobamos que no es un directorio (tampoco . y ..)
                        if(!S_ISDIR(atributos.st_mode)){

                                //Asignamos permisos_nuevos
                                permisos_nuevos = strtol(argv[2], 0, 8) & 0777;

                                //Guardamos permisos_antiguos
                                permisos_antiguos = atributos.st_mode & 0777;

                                //Cambio de permisos!
                                chmod_result = chmod(fichero->d_name, permisos_nuevos);

				//Comprobamos si el cambio se ha realizado
                                if(chmod_result < 0) {

                                        //Error nel cambio de permisos
                                        printf("%s : \t%d \t%o\n", fichero->d_name,chmod_result, permisos_antiguos);
                                        exit(-1); 

                                } else {
                                        //Se han cambiado los permisos, usamos %o para que muestre el valor octal
                                        printf("%s : \t%o \t%o\n", fichero->d_name, permisos_antiguos, permisos_nuevos);

                                }
                        };
                }
        }
 
        //CERRO EL DIRECTORIO
        closedir(dirp);

        return 0; 
}

// ================================================= //
//		    Fine ejercicio2.c  		     //
// ================================================= //
