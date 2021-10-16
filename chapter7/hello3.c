/*
 * hello3.c
 * perpose      using refersh and sleep for animated effects
 * outline      initialize, draw stuff, wrap up
 */
#include<stdio.h>
#include<curses.h>
#include<unistd.h>

int main()
{
    int i;

    initscr();                          /* turn on curses */
    clear();                            /* draw some stuff */
    for (i = 0; i < LINES; i++) {       /* in a loop */
        move(i, i + 1);
        if (i % 2 == 1)
            standout();
        addstr("Hello world");
        if (i % 2 == 1)
            standend();
        sleep(1);
        refresh();
    }
    getch();                            /* wait for user input */
    endwin();                           /* reset the tty etc */
}
