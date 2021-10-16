/*
 * setecho.c
 *  usage: setecho [y/n]
 *  shows: how to read, change, reset tty attributes
 */
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

#define oops(s,x) { perror(s); exit(x); }

int main(int ac, char *av[])
{
    struct termios info;

    if (ac == 1)
        exit(0);

    if (tcgetattr(STDIN_FILENO, &info) == -1)       /* get attribs */
        oops("tcgetattr", 1);

    if (av[1][0] == 'y')
        info.c_lflag |= ECHO;                       /* turn on bit */
    else
        info.c_lflag &= ~ECHO;                      /* turn off bit */

    if (tcsetattr(STDIN_FILENO, TCSANOW, &info) == -1)       /* set attribs */
        oops("tcsetattr", 1);

    return 0;
}
