/* 
 * more02.c - version 0.2 of more
 * read and print 24 lines then pause for a few special commands
 * feature of version 0.2: reads from /dev/tty for commands
 */
#include<stdio.h>
#include<stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE*);
int see_more();

int main(int argc, char* argv[]) {
    FILE* fp;

    if (argc == 1)
        do_more(stdin);
    else
        while (--argc)
            if ((fp = fopen(* ++argv, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            }
            else 
                exit(1);

    return 0;
}

void do_more(FILE* fp)
/*
 * read PAGELEN lines, then call see_more() for further instructions
 */
{
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(), reply;
    FILE* fp_tty;
    fp_tty = fopen("/dev/tty", "r");

    if (fp_tty == NULL)                 /* if open fails */
        exit(1);

    while (fgets(line, LINELEN, fp)) {
        if (num_of_lines == PAGELEN) {
            reply = see_more(fp_tty);
            if (reply == 0)
                break;
            num_of_lines -= reply;
        }
    if (fputs(line, stdout) == EOF)
        exit(1);
    num_of_lines++;
    }
}

int see_more(FILE* cmd)
/* 
 * print message, wait for response, return # of lines to advance
 * q means no, space means yes, CR means one line
 */
{
    int c;
    printf("\033[7m more?\033[m");      /* reverse on a vt100 */
    while ((c = getc(cmd)) != EOF) {
        if (c == 'q')                   /* q -> N */
            return 0;
        if (c == ' ')                   /* ' ' => next page */
            return PAGELEN;
        if (c == '\n')                  /* Enter key => 1 line */
            return 1;
    }

    return 0;
}

