/*
 * who3.c - who with buffered reads
 *      - surpresses empty records
 *      - formats time nicely
 *      - buffers input(using utmplib.c)
 */
#include<unistd.h>
#include<time.h>
#include"./utmplib.c"

#define SHOWHOST                        /* include remote machine on output */

void showtime(long);
void show_info(struct utmp*);

int main()
{
    struct utmp* utbufp;                /* holds pointer to next rec */
    //struct utmp* utmp_next();           /* returns pointer to next */

    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE);              /* UTMP_FILE is in utmp.h */
        exit(1);
    }
    
    while ((utbufp = utmp_next()) != (struct utmp*)NULL)
        show_info(utbufp);
    utmp_close();
    
    return 0;
}

/*
 * show_info()
 *          displays contents of the utmp struct
 *          in human readable form
 *          *displays nothing if record has no user name
 */
void show_info(struct utmp* utbufp)
{
    if (utbufp->ut_type != USER_PROCESS)
        return;

    printf("%-8.8s", utbufp->ut_name);    /* the logname */
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);    /* the tty */
    printf(" ");
    printf("%-8.8s", utbufp->ut_name);    /* login time */
    printf(" ");
#ifdef SHOWHOST
    printf("%-8.8s", utbufp->ut_host);    /* the host */
#endif
    printf("\n");
}

void showtime(long timeval)
/*
 * displays time in a format fit for human consumption
 * uses ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITS
 * it to 12 chars.
 */
{
    char* cp;                   /* to hold address of time */
    cp = ctime(&timeval);       /* convert time to string */
                                /* string looks like */
                                /* Mon Feb 4 00:46:40 EST 1991 */
                                /* 0123456789012345. */
    printf("%12.12s", cp + 4);  /* pick 12 chars from pos 4 */
}
