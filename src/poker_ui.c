//
// Created by bloo on 18/04/2016.
//

#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define POKER_COLOR_BACKGROUND    0
#define POKER_COLOR_RED           1
#define POKER_COLOR_BLUE          2
#define POKER_COLOR_TEXT          7

static void finish(int sig) {
    endwin();

    /* do your non-curses wrap up here, like freeing the memory allocated */


    exit(sig);
}

static void init_screen() {
    (void) signal(SIGINT, finish);      /* arrange interrupts to terminate */
    (void) initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, don't wait for \n */
    (void) noecho();       /* do not echo input */
    timeout(500);          /* wait maximum 500ms for a character */
    /* Use timeout(-1) for blocking mode */
    printw("Hello World !!!");
    if (has_colors()) {

        start_color();
        // initialise you color pairs (foreground, background)
        init_pair(1, POKER_COLOR_TEXT, POKER_COLOR_BACKGROUND);
        init_pair(2, POKER_COLOR_RED, POKER_COLOR_BACKGROUND);
        init_pair(3, POKER_COLOR_BLUE, POKER_COLOR_BACKGROUND);
    }
    /* set default color pair */
    attrset(COLOR_PAIR(1));
}

int main()
{
    chtype c;
    init_screen();
    struct timespec delay = {0, 500000000L},
            rem;
    int endProgram = 0, x = 0, y = 0, maxx, maxy;
    while (!endProgram) {
        c = (chtype) getch();
        if (c == 'q') {
            endProgram = 1;
        }
        getmaxyx(stdscr, maxy, maxx);
        raw();
        keypad(stdscr, TRUE);        /* Get keyboard	input	*/
        noecho();
        if (c == KEY_DOWN && y < maxy - 1) {
            y++;
        } else if (c == KEY_RIGHT && x < maxx - 1) {
            x++;
        } else if (c == ' ' || c == -1) {
            c = mvinch(y, x);
            mvaddch(y, x, c+1);
        }
        move(y, x);
    }
    finish(0);
}

//Menu
//static void init_screen();
//static void finish(int sig);
//
//#define WIDTH 30
//#define HEIGHT 10
//
//int startx = 0;
//int starty = 0;
//
//char *choices[] = {
//        "Start",
//        "Load",
//        "Options",
//        "Highscore",
//        "Exit",
//};
//int n_choices = sizeof(choices) / sizeof(char *);
//void print_menu(WINDOW *menu_win, int highlight);
//
//int main()
//{	WINDOW *menu_win;
//    int highlight = 1;
//    int choice = 0;
//    int c;
//
//    initscr();
//    clear();
//    noecho();
//    cbreak();	/* Line buffering disabled. pass on everything */
//    startx = (80 - WIDTH) / 2;
//    starty = (24 - HEIGHT) / 2;
//
//    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
//    keypad(menu_win, TRUE);
//    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
//    refresh();
//    print_menu(menu_win, highlight);
//    while(1)
//    {	c = wgetch(menu_win);
//        switch(c)
//        {	case KEY_UP:
//                if(highlight == 1)
//                    highlight = n_choices;
//                else
//                    --highlight;
//                break;
//            case KEY_DOWN:
//                if(highlight == n_choices)
//                    highlight = 1;
//                else
//                    ++highlight;
//                break;
//            case 10:
//                choice = highlight;
//                break;
//            default:
//                mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
//                refresh();
//                break;
//        }
//        print_menu(menu_win, highlight);
//        if(choice != 0)	/* User did a choice come out of the infinite loop */
//            break;
//    }
//    mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
//    clrtoeol();
//    refresh();
//    endwin();
//    return 0;
//}
//
//
//void print_menu(WINDOW *menu_win, int highlight)
//{
//    int x, y, i;
//
//    x = 2;
//    y = 2;
//    box(menu_win, 0, 0);
//    for(i = 0; i < n_choices; ++i)
//    {	if(highlight == i + 1) /* High light the present choice */
//        {	wattron(menu_win, A_REVERSE);
//            mvwprintw(menu_win, y, x, "%s", choices[i]);
//            wattroff(menu_win, A_REVERSE);
//        }
//        else
//            mvwprintw(menu_win, y, x, "%s", choices[i]);
//        ++y;
//    }
//    wrefresh(menu_win);
//}

//int main(int argc, char *argv[]) {
//
//    int x = 0, y = 0, maxx, maxy;
//    chtype c;
//
//    struct timespec delay = {0, 500000000L},
//            rem;
//
//    init_screen();
//
//    for (int i = 1; ; i++) {
//        c = (chtype) getch();     /* refresh, accept single keystroke of input */
//
//        /* process the command keystroke */
//        if (c == 'q') {
//            break;
//        }
//
//        getmaxyx(stdscr, maxy, maxx);
//        if (c == KEY_DOWN && y < maxy-1) {
//            y++;
//        } else if (c == KEY_RIGHT && x < maxx-1) {
//            x++;
//        } else if (c == ' ' || c == -1) {
//            attrset(COLOR_PAIR(i%3+1));
//            c = mvinch(y, x);
//            mvaddch(y, x, c+1);
//        }
//        move(y, x);
//
//        //nanosleep(&delay, &rem);
//    }
//
//    finish(0);               /* we're done */
//}
//
//
//static void init_screen() {
//    (void) signal(SIGINT, finish);      /* arrange interrupts to terminate */
//    (void) initscr();      /* initialize the curses library */
//    keypad(stdscr, TRUE);  /* enable keyboard mapping */
//    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
//    (void) cbreak();       /* take input chars one at a time, don't wait for \n */
//    (void) noecho();       /* do not echo input */
//    timeout(500);          /* wait maximum 500ms for a character */
//    /* Use timeout(-1) for blocking mode */
//    printw("Hello World !!!");
//    if (has_colors()) {
//
//        start_color();
//        // initialise you color pairs (foreground, background)
//        init_pair(1, POKER_COLOR_TEXT, POKER_COLOR_BACKGROUND);
//        init_pair(2, POKER_COLOR_RED, POKER_COLOR_BACKGROUND);
//        init_pair(3, POKER_COLOR_BLUE, POKER_COLOR_BACKGROUND);
//    }
//    /* set default color pair */
//    attrset(COLOR_PAIR(1));
//}
//
//static void finish(int sig) {
//    endwin();
//
//    /* do your non-curses wrap up here, like freeing the memory allocated */
//
//
//    exit(sig);
//}


