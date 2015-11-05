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
#include<unistd.h> //to ignore the warnings

int main(int argc, char* argv[]){

    //Declaration & Initiation Section
    int f_write, f_read, dim, i=1;
    char block[80];
    char* output_file = "salida.txt";

    //"Catch" the error generated from not putting a pathname file
    if (argc < 2)	f_read=STDIN_FILENO;
    else {
        char* pathname = argv[1];
        //Opening the file to be read
        f_read=open(pathname,O_RDONLY);
    }
    //Opening the file to be written
    f_write=open(output_file,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);

    //"Catch" an error on opening the file to be read
    if (f_read<0) {
        printf("\nError %d in opening the file to be read.",errno);
        perror("\nError in opening the file to be read.");
        exit(-1);
    }

    //"Catch" an error on opening the file to be written
    if (f_write<0){
        printf("\nError %d in opening the file to be written.",errno);
        perror("\nError in opening the file to be written.");
        exit(-1);
    }

    //Reading 80 bytes from the first file (pathname)
    while ((dim = read(f_read, &block, 80))>0){

        //Generate the string for the block number
        char block_ID[12];
        sprintf(block_ID, "##Bloque %i##", i);
        write(f_write,&block_ID,sizeof(block_ID));
        write(f_write,"\n",1);

        //Writing down the 80 bytes block
        write(f_write, &block, dim);
        write(f_write,"\n", 1);
        write(f_write,"\n", 1);

        //increasing the counter for the number of blocks
        i++;

    }

    //==================//
    // MODIFICACION ADICIONAL//
    //==================//

    //Move the offset to the beginning of the file to be written
    if (lseek(f_write,0,SEEK_SET)<0){
        perror("\nError in setting the offset to 0 in the file to be written.");
        exit(-1);
    }

    //Create a new buffer, with known dimension of 27 bytes
    char block_number[27];
    sprintf(block_number, "El número de bloques es %i", i-1);

    //Write the new buffer
    write(fd2, &block_number, sizeof(block_number));
    write(fd2, "\n", 1);


    //Closing the files opened, not necessary
    close(f_write);
    close(f_read);

    return 0;
}