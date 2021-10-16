/* popen_ex3.c
 *      shows how to use popen to write to a process that
 *      reads from stdin. This program writes email to 
 *      tow users. Note how easy it is to use fprintf
 *      to format the data to send.
 */

#include <stdio.h>

int main()
{
    FILE *fp;

    fp = popen( "mail admin backup", "w" );
    fprintf( fp, "Error with backup!\n" );
    pclose(fp);
}
