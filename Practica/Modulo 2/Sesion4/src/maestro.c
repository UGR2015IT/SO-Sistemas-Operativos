// COMPILE WITH: gcc maestro.c -o maestro -lm

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

void esclavo(int inicio, int fin, int *fd){
	int n;
        for (n = inicio; n < fin; n++) {
		int k;
		//Método rapido con la raiz cuadrada
		for (k = 2;k < (int)sqrt(n); k++) {
			//printf("%d\t%d\t%d\n",n,k,(int)sqrt(n));
		        if ( n % k == 0 ) break; //No es primo, paramos la iteracion
		}
		if ((k == (int)sqrt(n) && n%k != 0) || n==2 || n==3) {
			printf("El numero %d es primo.\n", n);			
			write(fd[1],&n,sizeof(int));
		}
        }
	int senalFin = -1;
	write(fd[1],&senalFin,sizeof(int));
        exit(0);
}


int main(int argc, char *argv[]) {
        int inicio, fin, mitad, numero, fd1[2], fd2[2];
        char params[2], buffer[80];
        pid_t PID[2];

        //Comprobamos si se le ha pasado dos enteros como parámetro
        if(argc!=3) {
		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <inicio> <fin>\n\n", argv[0]);
		exit(1);
        }

        //Convertimos el argumento a int y calculo el punto de inicio por esclavo 2
        inicio=atoi(argv[1]);
        fin=atoi(argv[2]);
        mitad = inicio + ((fin - inicio) / 2);

	//Creamos los pipe
        pipe(fd1);
        pipe(fd2);

        if ( (PID[0] = fork())<0) {
                perror("Error al hacer fork");
                exit(1);
        }
        if (PID[0] == 0) {
                //Cierre del descriptor de lectura en el proceso hijo
                close(fd1[0]);
                params[0] = inicio;
                params[1] = mitad;
                // Enviar el mensaje a través del cauce usando el descriptor de escritura
                printf("paso 1\n");
                dup2(fd1[1],STDOUT_FILENO);
		//execlp("esclavo","esclavo", params[0], params[1], NULL);
		esclavo(params[0],params[1], fd1);   
        } else { 
		// Estoy en el proceso padre porque PID != 0
                if ( (PID[1]= fork())<0) {
                        perror("Error al hacer fork");
                        exit(1);
                }
                if (PID[1] == 0) {
                        close(fd2[0]);

                        params[0] = mitad + 1;
                        params[1] = fin;
 
                        printf("paso 2\n");
                        dup2(fd2[1],STDOUT_FILENO);
                        //execlp("esclavo","esclavo", params[0],params[1],NULL);
			esclavo(params[0],params[1], fd2);

                        exit(0);

                }
	}

	if (waitpid(PID[0],0,0) > 0 && waitpid(PID[1],0,0) > 0){            
    		//Cerramos el descriptor de escritura en el proceso padre
                close(fd1[1]);
		close(fd2[1]);
        
                //Leimos datos desde el cauce.
                dup2(fd1[0],STDIN_FILENO);
                dup2(fd2[0],STDIN_FILENO);

		numero = 0;

                printf("paso 3\n");
                //Vamos a leer los risultados!
                while (numero > 0) {
			read(fd1[0], &numero, 4);
                        printf("Numero primo de esclavo 1: %d",numero);
                }
		close(fd1[0]);
		while (numero > 0) {
			read(fd2[0], &numero, 4);
                        printf("Numero primo de esclavo 2: %d",numero);
                }
		close(fd2[0]);
        }        
        exit(0);

}
