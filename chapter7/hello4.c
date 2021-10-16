/*
 * hello4.c
 * perpose      show how to use erase, time, and draw for animation
 */
#include<stdio.h>
#include<curses.h>
#include<unistd.h>

int main()
{
    int i;

    initscr();              
    clear();    
    for (i = 0; i < LINES; i++) {
        move(i, i + 1);
        if (i % 2 == 1)
            standout();
        addstr("Hello world");
        if (i % 2 == 1)
            standend();
        sleep(1);
        refresh();
        move(i ,i + 1);                 /* move back */
        addstr("                ");     /* erase line */
    }
    getch();                            
    endwin();                   
}
