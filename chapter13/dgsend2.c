/******************************************************************************
 * dgsend2.c - datagram sender
 *          usage: dgsend hostname portnum "message"
 *          action: sends message to hostname: portnum
 *
 *          cc dgsend2.c dgram.c -o dgsend
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define oops( m, x )    { perror( m ); exit( x ); }

extern int make_dgram_client_socket();
extern int make_internet_address( char *, int, struct sockaddr_in * );

int main( int ac, char *av[] )
{
    int     sock;               /* use this socket to send */
    char    *msg;               /* send this message */
    struct sockaddr_in  saddr;  /* put sender's address here */
    char    buf[BUFSIZ];        /* to receive data here */
    size_t  msglen;             /* store its length here */

    if ( ac != 4 ) {
        fprintf( stderr, "usage: dgsend host port 'message'\n" );
        exit( 1 );
    }
    msg = av[3];

    /* get a datagram socket */

    if ( ( sock = make_dgram_client_socket() ) == -1 )
        oops( "cannot make socket", 2 );

    /* combine hostname and portnumber of destination into an address */

    make_internet_address( av[1], atoi( av[2] ), &saddr );

    /* send a string through the socket to that address */

    if ( sendto( sock, msg, strlen( msg ), 0,
                (struct sockaddr *)&saddr, sizeof( saddr ) ) == -1 )
        oops( "sendto failed", 3 );
    
    msglen = recvfrom( sock, buf, BUFSIZ, 0, NULL, NULL );
    buf[msglen] = '\0';
    printf( "reply from receiver: %s", buf );

    return 0;
}
