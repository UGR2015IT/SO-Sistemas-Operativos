///   LEE DE STDIN_FILENO Y ESCRIBE EN STDOUT_FILENO

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include<stdlib.h>
main (int argc, char *argv[]) {
	int ret;
	char c[2]="\0\0";

		ret = read(0, c, 1);
		while (ret > 0){
			if (write(1, c, 1)<0){
				perror("Write en hijo");
				exit(-1);
			}
			ret = read(0, c, 1);
		}
		if (ret<0) {
			perror("Error al leer en el hijo");
		}
}