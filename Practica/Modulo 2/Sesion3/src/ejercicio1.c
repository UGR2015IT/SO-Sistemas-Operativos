#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

void check_even(int value){
	if (value%2==0) printf("Hola, yo soy el hijo. El numero es par. \n");
	else printf("Hola, yo soy el hijo. El numero es impar. \n");
}

void check_4(int value){
	if (value%4==0) printf("Hola, yo soy el padre. El numero es divisible por 4. \n");
	else printf("Hola, yo soy el padre. El numero no es divisible por 4. \n");
}

void main(int argc, char *argv[]){
	
	if (argc==2){
		pid_t pid = fork();

		if (pid == 0){
			check_even(atoi(argv[1]));
		}
		else if (pid > 0){
			check_4(atoi(argv[1]));
		}
	} else printf("Not the right amount of arguments. Sintax: ./eje1 numero \n");

}
