/*
 * echostate.c
 * reports current state of echo bit in tty driver for fd 0
 * shows how to read attributes from driver and test a bit
 */
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

int main()
{
    struct termios info;
    int rv;

    rv = tcgetattr(STDIN_FILENO, &info);   /* read values from driver */
    if (rv == -1) {
        perror("tcgetattr");
        exit(1);
    }
    
    if (info.c_lflag & ECHO)
        printf("echo is on, since its bit is 1\n");
    else
        printf("echo is off, since its bit is 0\n");

    return 0;
}
