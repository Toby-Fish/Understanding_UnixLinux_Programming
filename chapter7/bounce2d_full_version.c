/*
 * bounce2d 2.0
 * bounce a character (default is 'o') around the screen
 * defined by some parameters
 *
 * user input: s: slow down x component, S: slow y component
 *             f: speed up x component, F: speed y component
 *             Q: quit
 *
 * blocks on read, but timer tick sends SIGALRM caught by ball_move
 * build: cc bounce2d.c set_ticker.c -lncurses -o bounce2d
 */
#include<curses.h>
#include<signal.h>
#include<stdlib.h>
#include"bounce.h"

#define BAFFLE_LEN 5

struct ppball the_ball;

extern int set_ticker(int);

void draw_baffle_wall();
void clear_baffle_wall();
void check_if_lose(struct ppball *);
int bounce_or_lose(struct ppball *);
void game_over();

int baffle_pos, up_or_down;
char LONG_EDGE[] =  "------------------------------------------------------------";

/* * the main loop * */

void set_up();
void wrap_up();

int main()
{
    int c;

    set_up();   

    while((c = getchar()) != 'Q') {
        if (c == 'f')       the_ball.x_ttm--;
        else if (c == 's')  the_ball.x_ttm++;
        else if (c == 'F')  the_ball.y_ttm--;
        else if (c == 'S')  the_ball.y_ttm++;
        else if (c == '5')   up_or_down++;
        else if (c == '8')   up_or_down--;
    }

    wrap_up();
}

void set_up()
/* 
 * init structure an other stuff
 */
{
    int i;
    void ball_move(int);

    baffle_pos = 10;
    up_or_down = 0;

    the_ball.y_pos = Y_INIT;
    the_ball.x_pos = X_INIT;
    the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
    the_ball.x_ttg = the_ball.x_ttm = X_TTM;
    the_ball.y_dir = 1;
    the_ball.x_dir = 1;
    the_ball.symbol = DFL_SYMBOL;

    initscr();
    noecho();
    crmode();

    signal( SIGINT, SIG_IGN );
    move( TOP_ROW - 1, LEFT_EDGE );         /* build top wall */
    addstr( LONG_EDGE );
    move( BOT_ROW + 1, LEFT_EDGE );         /* build bottom wall */
    addstr( LONG_EDGE );
    for (i = TOP_ROW; i <= BOT_ROW; i++) {  /* build left wall */
        mvaddch( i, LEFT_EDGE - 1, '|' );
    }
    draw_baffle_wall();                     /* build buffle wall */
    mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol );
    refresh();

    signal( SIGALRM, ball_move );
    set_ticker( 1000 / TICKS_PER_SEC );     /* send millisecs per tick */
}

void wrap_up()
{
    set_ticker( 0 );                        /* put back to normal */
    endwin();
}

void ball_move(int signum)
{
    int y_cur, x_cur, moved;

    signal( SIGALRM, SIG_IGN );              /* dont get caught now */
    y_cur = the_ball.y_pos;                  /* old spot */
    x_cur = the_ball.x_pos;
    moved = 0;

    if ( the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
        the_ball.y_pos += the_ball.y_dir;   /* move */
        the_ball.y_ttg = the_ball.y_ttm;    /* reset */
        moved = 1;
    }

    if ( the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
        the_ball.x_pos += the_ball.x_dir;   /* move */
        the_ball.x_ttg = the_ball.x_ttm;    /* reset */
        moved = 1;
    }

    if ( moved ) {
        mvaddch( y_cur, x_cur, BLANK );
        mvaddch( y_cur, x_cur, BLANK );
        mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol );
        bounce_or_lose( &the_ball );
        move(LINES - 1, COLS - 1);
        refresh();
    }

    if (up_or_down != 0) {
        if (up_or_down < 0 && baffle_pos + up_or_down >= TOP_ROW - BAFFLE_LEN)
            baffle_pos += up_or_down;
        else if (up_or_down > 0 && baffle_pos + up_or_down <= BOT_ROW)
            baffle_pos += up_or_down;
        clear_baffle_wall();
        draw_baffle_wall();
        up_or_down = 0;
    }

    signal( SIGALRM, ball_move);            /* for unreliable systems */
}

int bounce_or_lose(struct ppball *bp)
{
    int return_val = 0;

    if ( bp->y_pos == TOP_ROW ){
        bp->y_dir = 1;
        return_val = 1;
    }
    else if ( bp->y_pos == BOT_ROW){
        bp->y_dir = -1;
        return_val = 1;
    }

    if ( bp->x_pos == LEFT_EDGE ){
        bp->x_dir = 1;
        return_val = 1;
    }
    else if ( bp->x_pos == RIGHT_EDGE) {
        if (bp->y_pos >= baffle_pos && bp->y_pos <= baffle_pos + BAFFLE_LEN) {
            bp->x_dir = -1;
            return_val = 1;
        }
        else game_over();
    }

    return return_val;
}

inline void clear_baffle_wall()
{
    int i;
    for (i = 0; i <= LINES; i++)
        mvaddch(i, RIGHT_EDGE + 1, ' ');
}

inline void draw_baffle_wall()
{
    int i;
    for (i = BAFFLE_LEN; i > 0; i--) {
        mvaddch(baffle_pos + i, RIGHT_EDGE + 1, '|');
    }
}

void game_over()
{
    move( (TOP_ROW + BOT_ROW) / 2, (LEFT_EDGE + RIGHT_EDGE) / 2 - 4 );
    addstr("GAME OVER!");
    refresh();
    set_ticker(0);
}
