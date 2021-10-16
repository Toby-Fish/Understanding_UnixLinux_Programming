/******************************************************************************
 * lclnt1.c
 * License server client version 1
 * link with lclnt_funcs1.o dgram.o
 *
 * cc lclnt1.c lclnt_funcs1.c dgram.o -o lclnt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern void setup();
extern int get_ticket();
extern int release_ticket();
extern void shut_down();
extern void ticket_validate();

void do_regular_work();

int main( int ac, char *av[] )
{
    setup();
    if ( get_ticket() != 0 )
        exit( 0 );

    do_regular_work();

    release_ticket();
    shut_down();

}

/******************************************************************************
 * do_regular_work the main work of the application goes here
 */
void do_regular_work()
{
    printf( "SuperSleep version 1.0 Running - Licensed Software\n" );
    sleep( 15 ); /* our patented sleep algorithm */
    ticket_validate();
    sleep( 15 );
}
