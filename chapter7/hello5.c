/*
 * hello5.c
 * perpose      bounce a message back and forth across the screen
 * compile cc hello5.c -o hello5 -lncurses
 */
#include<stdio.h>
#include<curses.h>
#include<unistd.h>

#define LEFTEDGE    10
#define RIGHTEDGE   30
#define ROW         10

int main()
{
    char *message = "Hello";
    char *blank   = "     ";
    int dir = +1;
    int pos = LEFTEDGE;

    initscr();              
    clear();
    while (1) {
        move(ROW, pos);                 
        addstr(message);                /* draw string */
        move (LINES - 1, COLS - 1);     /* park the cursor */
        refresh();
        sleep(1);
        move(ROW, pos);                 /* erase string */
        addstr(blank);
        pos += dir;                     /* advance position */
        if (pos >= RIGHTEDGE)           /* check for bouncce */
            dir = -1;
        if (pos <= LEFTEDGE)
            dir = +1;
    }

    endwin();                   
}
