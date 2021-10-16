/* hello_single.c - a single threaded hello world program */

#include <stdio.h>
#include <unistd.h>

#define NUM 5

int main()
{
    void print_msg( char * );

    print_msg( "hello" );
    print_msg( "world\n" );
}

void print_msg( char *m )
{
    int i;
    for ( i = 0; i < NUM; i++ ) {
        printf( "%s", m );
        fflush( stdout );
        sleep( 1 );
    }
}
