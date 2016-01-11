#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include<stdlib.h>
main (int argc, char *argv[]) {
char memoriapadre[40]="archivosproc";
int dd, ret , p;
int dtub[2];
char c[2]="\0\0";


// este programa procesa el mapa de memoria del proceso padre
printf ("Procesando el mapa de memoria del proceso con pid %d\n",getppid());
printf("Para cada región con soporte, se muestra la ruta del archivo asociado");
sprintf (memoriapadre, "/proc/%d/maps",getppid());
printf ("Ruta del archivo que contiene el mapa de memoria: %s\n",memoriapadre);

	dd= STDOUT_FILENO; // aqui escribe el padre
	pipe(dtub);
	
	if ((p=fork())>0){///// padre lee de dtub[0] y escribe en salida est
		close(dtub[1]);// cierro lo que no necesito
		ret = read(dtub[0], c, 1);
		int deboescribir=0;
		while (ret > 0){ // while no fin de archivo
			if (strcmp(c,"/")==0) deboescribir=1;
			if (deboescribir) if (write(dd, c, 1)<0){
					perror("Write en hijo");
					exit(-1);
					}
			if (strcmp(c,"\n")==0) deboescribir=0;

			ret = read(dtub[0], c, 1);
		} // while no fin de archivo
		if (ret<0) {
			perror("Error al leer en el hijo");
			exit(-1);
		}
		exit(0);
		
	} else if (p==0){// hijo  escribe en dtub[1]
		close (dtub[0]); // cierro lo	que no necesito
		dup2(dtub[1], STDOUT_FILENO); // redirijo la salida estandar
		execlp("cat","cat",memoriapadre,NULL);
	}
	else {
		perror("Error en fork");
		exit(-1);
	}

exit(0);
}