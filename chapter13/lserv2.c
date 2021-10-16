/******************************************************************************
 * lserv2.c
 * License server server program version 2
 *
 * cc lserv2.c lserv_funcs2.c dgram.c -o server
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <sys/errno.h>

#define MSGLEN  128         /* Size of our datagrams */
#define RECLAIM_INTERVAL 60

extern int setup();
extern void narrate( char *, char *, struct sockaddr_in * );
extern void handle_request( char *, struct sockaddr_in *, socklen_t );
extern void ticket_reclaim();

int main( int ac, char *av[] )
{
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof ( client_addr );
    char    buf[MSGLEN];
    int     ret, sock;
    void ticket_reclaim();                  /* version 2 addition */
    unsigned time_left;

    sock = setup();
    signal( SIGALRM,  ticket_reclaim );     /* run ticket reclaimer */
    alarm( RECLAIM_INTERVAL );              /* after this delay */

    while ( 1 ) {
        addrlen = sizeof( client_addr );
        ret = recvfrom( sock, buf, MSGLEN, 0, 
                (struct sockaddr *)&client_addr, &addrlen );
        if ( ret != -1 ) {
            buf[ret] = '\0';
            narrate( "GOT:", buf, &client_addr );
            time_left = alarm( 0 );
            handle_request( buf, &client_addr, addrlen );
            alarm( time_left );
        }
        else if ( errno != EINTR )
            perror( "recvfrom" );
    }
}
