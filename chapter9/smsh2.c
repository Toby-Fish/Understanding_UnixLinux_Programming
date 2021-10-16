/*
 *  smsh2.c small - shell version 2
 *      small shell that supports command line parsing
 *      and if .. then .. else. fi logic (by calling process())
 *
 *  cc -o smsh2 smsh2.c splitline.c execute.c process.c controlflow.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT "> "

int main()
{
    char *cmdline, *prompt, **arglist;
    int result, process(char **);
    void setup();

    prompt = DFL_PROMPT;
    setup();

    while((cmdline = next_cmd(prompt, stdin)) != NULL) {
        if ((arglist = splitline(cmdline)) != NULL) {
            result = process(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }

    return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. call fatal() if trouble
 */
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}

