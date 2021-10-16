/* hello_single.c - a multi - threaded hello world program */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 5

int main()
{
    pthread_t t1, t2;           /* two threads */

    void *print_msg( void * );
    
    pthread_create( &t1, NULL, print_msg, (void *)"hello" );
    pthread_create( &t2, NULL, print_msg, (void *)"world\n" );
    pthread_join( t1, NULL );
    pthread_join( t2, NULL );
}

void *print_msg( void *m )
{
    int i;
    for ( i = 0; i < NUM; i++ ) {
        printf( "%s", (char *)m );
        fflush( stdout );
        sleep( 1 );
    }
    
    return NULL;
}
