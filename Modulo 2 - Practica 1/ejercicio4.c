//================================//
// Created by dave95 on 05/11/15. //
// Source Code for exercise 2     //
// of the Sistemas Operativos     //
// course of UGR, year 2015/16    //
//================================//
//      COPY WITH CAUTION!        //
//================================//

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
//to ignore the warnings
#include<unistd.h>
//for working on strings, functions like strcpy() and strcat()
#include<string.h>

// Defining the macro
// It checks on the IFMT field (filetype) if it's equal to the regular flag
#define S_ISREG2(mode)      (((mode) & S_IFMT) == 0100000)

int main(int argc, char *argv[])
{
    int i;
    struct stat atributos;
    char tipoArchivo[30];

    if(argc<2) {
        printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
        exit(-1);
    }
    for(i=1;i<argc;i++) {
        printf("%s: ", argv[i]);
        if(lstat(argv[i],&atributos) < 0) {
            printf("\nError al intentar acceder a los atributos de %s",argv[i]);
            perror("\nError en lstat");
        }
        else {
            //Here we call the newly-defined macro
            if(S_ISREG2(atributos.st_mode)) strcpy(tipoArchivo,"Regular");
            else if(S_ISDIR(atributos.st_mode)) strcpy(tipoArchivo,"Directorio");
            else if(S_ISCHR(atributos.st_mode)) strcpy(tipoArchivo,"Especial de caracteres");
            else if(S_ISBLK(atributos.st_mode)) strcpy(tipoArchivo,"Especial de bloques");
            else if(S_ISFIFO(atributos.st_mode)) strcpy(tipoArchivo,"Tubería con nombre (FIFO)");
            else if(S_ISLNK(atributos.st_mode)) strcpy(tipoArchivo,"Enlace relativo (soft)");
            else if(S_ISSOCK(atributos.st_mode)) strcpy(tipoArchivo,"Socket");
            else strcpy(tipoArchivo,"Tipo de archivo desconocido");
            printf("%s\n",tipoArchivo);
        }
    }

    return 0;
}