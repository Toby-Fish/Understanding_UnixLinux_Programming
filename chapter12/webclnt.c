#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

extern int connect_to_server( char *hostname, int portnum );

int main( int ac, char *av[] )
{
    int     sock, fd;
    FILE    *fpin, *fp;
    char    request[BUFSIZ];

    sock = connect_to_server( av[1], atoi( av[2] ) );

    while ( 1 ) {
        fgets( request, BUFSIZ, stdin );
        write( sock, request, BUFSIZ );
        write( sock, "\n", 1 );
        while ( read( sock, request, BUFSIZ ) > 0 )
            write( 1, request, BUFSIZ );
    }
}    
