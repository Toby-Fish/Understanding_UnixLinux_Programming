/* 
 * utmplib.c - functions to buffer reads from utmp file
 *
 *  functions are
 *      utmp_open(filename) -   open file
 *          returns -1 on error
 *      utmp_next()         -   return pointer to next struct
 *          return NULL on eof
 *      utmp_reload()       -   reload elements from file
 *          return numbers of newly reloaded elements
 *      utmp_close()        -   close file
 *
 *  reads NRECS per read and then doles them out from the buffer
 */
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp*)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];            /* storage */
static int  num_recs;                           /* num stored */
static int  cur_rec;                            /* next to go */
static int fd_utmp = -1;                        /* read from */

int utmp_open(char*);
struct utmp* utmp_next();
int utmp_reload();
void utmp_close();

int utmp_open(char* filename)
{
    fd_utmp = open(filename, O_RDONLY);         /* open it */
    cur_rec = num_recs = 0;                     /* no recs yet */
    return fd_utmp;                             /* report */
}

struct utmp* utmp_next()
{
    struct utmp* recp;
    if (fd_utmp == -1)
        return NULLUT;                              /* error? */
    if (cur_rec == num_recs || utmp_reload() == 0)  /* any more? */
        return NULLUT;

    recp = (struct utmp*)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}

int utmp_reload()
/* 
 * read next branch of records into buffer
 */
{
    int amt_read;
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);  /* read them in */
    num_recs = amt_read / UTSIZE;                       /* how many did we get? */
    cur_rec = 0;                                        /* reset pointer */
    return num_recs;
}

void utmp_close()
{
    if (fd_utmp != 0)                                   /* don't close if not */
        close(fd_utmp);
}
