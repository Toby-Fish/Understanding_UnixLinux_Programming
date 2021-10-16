/*
 * cp2.c
 * version 2 of cp - works correctly even if the source file
 *                   and the destination file are the same file
 *
 * usage: cp2 src dest
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define BUFFERSIZE  4096
#define COPYMODE    0644

void oops(char*, char*);

int main(int argc, char* argv[])
{
    int in_fd, out_fd, n_chars;
    char name_buf[64], tail[5] = ".tmp";
    char buf[BUFFERSIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", argv[0]);
        exit(1);
    }
    
    /* to prevent the target file from having the
     * same name as the source file, add a .tmp suffix
     * to the target file name
     */
    strncpy(name_buf, argv[2], 50);
    if (strlen(argv[2]) > 50) 
        name_buf[50] = '\0';
    else 
        name_buf[strlen(argv[2])] = '\0';
    strcat(name_buf, tail);

    if ((in_fd = open(argv[1], O_RDONLY)) == -1)
            oops("Cannot open ", argv[1]);

    if ((out_fd = creat(name_buf, COPYMODE)) == -1)
            oops("Cannot creat ", name_buf);

    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
        if (write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to ", name_buf);

    if (n_chars == -1)
        oops("Read error from ", argv[1]);

    if (close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");

    /* rename target file name */
    if (rename(name_buf, argv[2]) == -1)
        oops("Error rename file", argv[2]);

    return 0;
}

void oops(char* s1, char* s2)
{
    fprintf(stderr, "Error: %s\n", s1);
    perror(s2);
    exit(1);
}
    
