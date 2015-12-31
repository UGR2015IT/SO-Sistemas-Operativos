#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
        if(argc != 3) {
                printf("Sintaxi: %s <programa> <origen> <destino>\n\n", argv[0]);
                return -1;
        } else {
                //Declaracion de variables
        	struct stat sb;
                char* start_file = argv[1];
                char* dest_file = argv[2];
                
                int fd_orig, fd_dest;
                char *start_mem, *dest_mem;
                int filesize;

                //Abrimos el fichero de origen
                fd_orig = open(start_file, O_RDONLY);
                if (fd_orig == -1) {
                        perror("Fallo al abrir el archivo de origen\n");
                        return -2;
                }
                //Obtenemos su stat, para comprobar si es regular y obtener su tamaño
        if (fstat (fd_orig, &sb) == -1) {
                printf("Error al hacer stat en el fichero de origen\n");
                return 1;
        }
        if (!S_ISREG (sb.st_mode)) {
                printf ("El fichero de origen no es un archivo regular\n");
                return 1;
        }
        //Guardamos el tamaño en una variable (por comodidad)
        filesize = sb.st_size;
        //Creamos el archivo de destino
        umask(0);
                fd_dest = open(dest_file, O_RDWR|O_CREAT|O_EXCL, S_IRWXU);
                if (fd_dest == -1) {
                        perror("Fallo al crear el archivo de salida");
                        return -2;
                }
                //Asignamos el espacion en el fichero de destino
                ftruncate(fd_dest, filesize);
                //Creamos el mapa de memoria del fichero de origen
                start_mem = (char *) mmap(0, filesize, PROT_READ, MAP_SHARED, fd_orig, 0);
                if(start_mem == MAP_FAILED) {
                        perror("Fallo mapeando el archivo de entrada");
                        return -2;
                }
                //Creamos el mapa de memoria del fichero de destino
                dest_mem = (char *) mmap(0, filesize, PROT_WRITE, MAP_SHARED, fd_dest, 0);
                if(dest_mem == MAP_FAILED) {
                        perror("Fallo mapeando el archivo de salida");
                        return -2;
                }
                //Copiamos un mapa de memoria en otro
                memcpy(dest_mem, start_mem, filesize);
                //Liberamos los mapas de memoria
                munmap(start_mem, filesize);
                munmap(dest_mem, filesize);
                //Cerramos los descriptores de fichero
                close(fd_orig);
                close(fd_dest);
                //Terminamos la ejecución del programa
                return 0;
        }
}
