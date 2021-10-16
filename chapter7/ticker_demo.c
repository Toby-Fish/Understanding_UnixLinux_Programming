/*
 * ticker_demo.c
 *      demonstrates use of interval timer to generate regular
 *      signals, which are in turn caught and used to count down
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<signal.h>

int main()
{
    int set_ticker(int);
    void countdown(int);

    signal(SIGALRM, countdown);
    if (set_ticker(500) == -1)
        perror("set_ticker");
    else
        while(1)
            pause();
    return 0;
}

void countdown(int signum)
{
    static int num = 10;
    printf("%d ..", num--);
    fflush(stdout);
    if (num < 0) {
        printf("DONE! \n");
        exit(0);
    }
}

/* [from set_ticker.c]
 * set_ticker( number_of_milliseconds )
 * arranges for interval timer to issue SIGALRMs at regular intervals
 * returns -1 on error, 0 for ok
 * arg in milliseconds, converted into whole seconds and microseconds
 * note: set_ticker(0) turns off ticker
 */


/*
 * struct itimerval
 * {
 *    struct timeval it_value;      ; time to next timer expiration
 *    struct timeval it_interval;   ; reload it_value with this 
 * }
 *
 * struct timeval {
 *    time_t tv_sec;                ; seconds
 *    suseconds_t tv_usec;          ; and microseconds
 * }
 */
 
int set_ticker( int n_msecs )
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;                             /* int part */
    n_usecs = ( n_msecs % 1000 ) * 1000L;               /* remainder */

    new_timeset.it_interval.tv_sec  = n_sec;             /* set reload */
    new_timeset.it_interval.tv_usec = n_usecs;           /* new ticker value */

    new_timeset.it_value.tv_sec     = n_sec;            /* store this */
    new_timeset.it_value.tv_usec    = n_usecs;          /* and this */

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
