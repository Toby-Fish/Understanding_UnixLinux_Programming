/* pipedemo.c   * Demonstrate: how to create and use a pipe
 *              * Effect: creates a pipe, writes into writing
 *              end, then runs around and reads from reading
 *              end. A little weird, but demonstrates the idea.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int len, i, apipe[2];   /* two file descriptors */
    char buf[BUFSIZ];       /* for raeding end      */

    /* get a pipe */
    if ( pipe( apipe ) == -1 ) {
        perror("could not make pipe");
        exit(1);
    }
    printf("Got a pile! It is file descriptors: { %d, %d }\n",
            apipe[0], apipe[1]);

    /* read from stdin, write into pipe, read from pipe, print */

    while( fgets(buf, BUFSIZ, stdin) ) {
        len = strlen( buf );
        if ( write( apipe[1], buf, len) != len ) {      /* send */
            perror("writing to pipe");                  /* down */
            break;                                      /* pipe */
        }
        for (i = 0; i < len; i++)                       /* wipe */
            buf[i] = 'X';
        len = read( apipe[0], buf, BUFSIZ );            /* read */
        if ( len == -1 ) {                              /* from */
            perror("reading from pipe");                /* pipe */
            break;                                      /* pipe */
        }
        if ( write(1, buf, len ) != len ) {             /* send */
            perror("writing to pipe");                  /* to */
            break;                                      /* pipe */
        }
    }
}
