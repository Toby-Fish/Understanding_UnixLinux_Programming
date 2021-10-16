/*
 * cp3.c
 * version 3 of cp - if the target file already exists,
 *                   the user is prompted
 *                 - implemented by prompt_if_exist()
 * usage: cp3 src dest
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define BUFFERSIZE  4096
#define COPYMODE    0644

void oops(char*, char*);
int  prompt_if_exist(char[]);

int main(int argc, char* argv[])
{
    int in_fd, out_fd, n_chars, exist;
    char name_buf[64], tail[5] = ".tmp", y_or_n;
    char buf[BUFFERSIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* 
     * check if target file already exists
     * return 1 if exists, 0 otherwise
     */
    if(prompt_if_exist(argv[2])) {
        /* exists */
        do {
            printf("%s already exists, overdrive? y/n\n", argv[2]);
            scanf("%c", &y_or_n);
        } while (y_or_n != 'y' && y_or_n != 'n');
        if (y_or_n == 'n')
            return 0;
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
    
int prompt_if_exist(char filename[]) 
{
    struct stat stat_entry;
    int ret;
    
    /* ret = 0 if file exists */
    ret = stat(filename, &stat_entry);
    
    return !ret;
}
