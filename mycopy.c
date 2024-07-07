#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>



#define sizeof_data 4096

// Assignment #1 - Joseph Vaughn
// This program (mycopy) copies the contents of a file of any-
// size or type, from a source location to a destination


int main(int argc, char *argv[]) {


    int source_file = open(argv[1], O_RDONLY);

    if (argc != 3) {

        printf("%s: invalid number of arguments\nusage: mycopy <sourcefile> <destinationfile>\n", argv[0]);

        return -1;
    }
    if (source_file < 0) {

        printf("mycopy: Could not open file %s: %s\n", argv[1], strerror(errno));

        return -2;
    }

    int destination_file = creat(argv[2], S_IWUSR | S_IRUSR | S_IROTH | S_IRGRP);

    if (destination_file < 0) {

        close(source_file);
        printf("could not open %s: %s\n", argv[2], strerror(errno));

        return -3;
    }

    char data[sizeof_data];
    ssize_t numbytes_read;
    size_t numbytes_copy = 0;

    while ((numbytes_read = read(source_file, data, sizeof_data)) > 0) {

        ssize_t numbytes_write = write(destination_file, data, numbytes_read);

        if (numbytes_write < numbytes_read) {

            close(source_file);
            close(destination_file);

            printf("could not write to file %s: %s\n", argv[2], strerror(errno));

            return -4;
        }
        numbytes_copy += numbytes_write;
    }

    if (numbytes_read < 0) {

        close(source_file);
        close(destination_file);
        printf("could not read from %s: %s\n", argv[1], strerror(errno));

        return -5;
    }

    if (close(source_file) < 0) {

        close(destination_file);
        printf("could not close file %s: %s\n", argv[1], strerror(errno));

        return -6;
    }

    if (close(destination_file) < 0) {

        printf("could not close file %s: %s\n", argv[2], strerror(errno));

        return -7;
    }

    printf("Copied %zu bytes from %s to %s.\n", numbytes_copy, argv[1], argv[2]);

    close(source_file);
    close(destination_file);

    return 0;

}
