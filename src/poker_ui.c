//
// Created by bloo on 18/04/2016.
//

#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define POKER_COLOR_BACKGROUND    0
#define POKER_COLOR_RED           1
#define POKER_COLOR_BLUE          2
#define POKER_COLOR_TEXT          7

enum suit {HEARTS, DIAMONDS, CLUBS, SPADES, NONE};
typedef enum suit Suit;

enum rank {HighCard, OnePair, TwoPairs, Three, Four, Straight, Flush, FullHouse, StraightFlush, RoyalStraightFlush};
typedef enum rank Rank;

enum state {None, Called, Raised, Checked, Bets, Allins, Folded, SB, BB};
typedef enum state State;

enum option {Call, Raise, Check, Bet, Allin, Fold};
typedef enum option Option;

struct card {
    Suit suit;
    int rank;
};
typedef struct card Card;

struct player {
    char name[20];
    int money;
    int bet;
    int status;
    Card hand[2];
    Card *max_hand;
    Rank rank;
    State state;
    int isBigBlind;
    int isSmallBlind;
    Option option;
    int isTurn;
    int isWinner;
};
typedef struct player Player;

struct table {
    int pot_money;
    int ante;
    int highest_bet; //the largest amount of total bet in one round
    Card card[5];
    int card_idx;
    int last_bet;
};
typedef struct table Table;

char* getSuit(Suit s) {
    switch(s){
        case HEARTS: return "\u2665";
        case DIAMONDS: return "\u2666";
        case CLUBS: return "\u2663";
        case SPADES: return "\u2660";
        default: break;
    }
    return NULL;
}

Player* createPlayers(int num_player) {
    if (num_player >= 2 && num_player <= 10) {
        Player *players = malloc(sizeof(Player) * num_player);
        for (int i = 0; i < num_player; i++) {
            strcpy(players[i].name, "Player 0");
            players[i].status = 1;
            players[i].state = None;
            players[i].name[7] += i + 1;
            players[i].money = 5000;
            players[i].max_hand = malloc(sizeof(Card) * 5);
            players[i].bet = 0;
            players[i].isBigBlind = 0;
            players[i].isSmallBlind = 0;
        }
        return players;
    }
    return NULL;
}

Table * createTable() {
    Table * table = malloc(sizeof(Table));
    table->card_idx = 0;
    table->highest_bet = 0;
    return table;
}

static void init_screen();
static void finish(int sig);

void drawCard(Card card, int y, int x) {
    if (card.suit == SPADES) {
        mvaddstr(y, x, ".______."); y++;
        mvprintw(y, x, "|%d  .  |", card.suit); y++;
        mvaddstr(y, x, "|  / \\ |"); y++;
        mvaddstr(y, x, "| (_,_)|"); y++;
        mvaddstr(y, x, "|   I  |"); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == CLUBS) {
        mvaddstr(y, x, ".______."); y++;
        mvprintw(y, x, "|%d  _  |", card.suit); y++;
        mvaddstr(y, x, "|  ( ) |"); y++;
        mvaddstr(y, x, "| (_x_)|"); y++;
        mvaddstr(y, x, "|   I  |"); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == DIAMONDS) {
        mvaddstr(y, x, ".______."); y++;
        mvprintw(y, x, "|%d /\\  |", card.suit); y++;
        mvaddstr(y, x, "| /  \\ |"); y++;
        mvaddstr(y, x, "| \\  / |"); y++;
        mvaddstr(y, x, "|  \\/  "); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == HEARTS) {
        mvaddstr(y, x, ".______."); y++;
        mvprintw(y, x, "|%d_  _ |", card.suit); y++;
        mvaddstr(y, x, "|( \\/ )|"); y++;
        mvaddstr(y, x, "| \\  / |"); y++;
        mvaddstr(y, x, "|  \\/  |"); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == NONE) {
        mvaddstr(y, x, ".______."); y++;
        mvaddstr(y, x, "|______|"); y++;
        mvaddstr(y, x, "|______|"); y++;
        mvaddstr(y, x, "|______|"); y++;
        mvaddstr(y, x, "|______|"); y++;
        mvaddstr(y, x, ".______.");
    }

    move(0, 0);
    refresh();
}

void center(int row, char *title) {
    int len, start_point;
    for (len = 0; ; len++) {
        if (title[len] == '\0') {
            break;
        }
    }
    start_point = (COLS - len) / 2;
    mvprintw(row, start_point, title);
    refresh();
}

void drawGame(int num_player, int roundIdx, Player * players, Table * table) {
    Card card;
    card.suit = NONE;
    for (int i = 0; i < num_player; i++) {
        if (players[i].status == 0) {
            continue;
        }
        if (i == 0) {
            drawCard(players[i].hand[0], 13, 20);
            drawCard(players[i].hand[1], 13, 28);
        } else if (i == 1) {
            drawCard(card, 1, 20);
            drawCard(card, 1, 28);
        } else if (i == 2) {
            for (int j = 0; j < 2; j++) {
                drawCard(card, 1, 0);
                drawCard(card, 1, 8);
            }
        } else if (i == 3) {
            for (int j = 0; j < 2; j++) {
                drawCard(card, 1, 41);
                drawCard(card, 1, 49);
            }
        } else if (i == 4) {
            for (int j = 0; j < 2; j++) {
                drawCard(card, 13, 0);
                drawCard(card, 13, 8);
            }
        } else if (i == 5) {
            for (int j = 0; j < 2; j++) {
                drawCard(card, 13, 41);
                drawCard(card, 13, 49);
            }
        }
    }
    if (roundIdx > 0) {
        int x = 6;
        for (int i = 0; i < roundIdx + 2; i++) {
            drawCard(table->card[i], 7, x);
            x += 8;
        }
    }
    refresh();
}

void game() {
    int num_player = 6;
    Player * players = createPlayers(num_player);
    Table * table = createTable();
    for (int i = 0; i < 5; i++) {
        table->card[i].suit = HEARTS;
        table->card[i].rank = 5;
    }
    players[0].hand[0].rank = 4;
    players[0].hand[0].suit = CLUBS;
    players[0].hand[1].rank = 2;
    players[0].hand[1].suit = DIAMONDS;
    for (int i = 0; i < 4; i++) {
        drawGame(num_player, i, players, table);
        getch();
    }
    free(players);
    free(table);
}

void drawStartMenu(int item, int num_player) {
    int c;
    char menu[4][10] = {"<", ">", "Start", "Main Menu"};

    clear();
    center(5, "Number of Players:");
    for (c = 0; c < 4; c++) {
        if (c == item) {
            attron(A_REVERSE);
        }
        if (c == 0) {
            if (num_player != 2) {
                mvaddstr(7, COLS / 2 - 2, menu[c]);
            }
        } else if (c == 1) {
            if (num_player != 6) {
                mvaddstr(7, COLS / 2 + 2, menu[c]);
            }
        } else if (c == 2) {
            center(9, menu[c]);
        } else if (c == 3) {
            center(11, menu[c]);
        }
        attroff(A_REVERSE);
    }

    mvprintw(7, COLS / 2, "%d", num_player);
    move(0, 0);

    refresh();
}

int interactStartMenu(int num_player, int item) {
    int key = 0;

    keypad(stdscr, TRUE);

    while (key != 13) { //13 is Enter
        drawStartMenu(item, num_player);
        key = getch();
        switch (key) {
            case KEY_DOWN:
                if (item == 0) {
                    item = 2;
                    break;
                }
                item++;
                if (item > 3) {
                    if (num_player == 2) {
                        item = 1;
                    } else {
                        item = 0;
                    }
                }
                break;
            case KEY_UP:
                if (item == 1) {
                    item = 3;
                    break;
                }
                if (item == 2 && num_player == 6) {
                    item = 0;
                    break;
                }
                item--;
                if (item < 0) {
                    item = 3;
                }
                break;
            case KEY_LEFT:
                if (item == 1) {
                    if (num_player != 2) {
                        item = 0;
                    }
                    break;
                }
                break;
            case KEY_RIGHT:
                if (item == 0) {
                    if (num_player != 6) {
                        item = 1;
                    }
                    break;
                }
                break;
            default: break;
        }
    }
    return item;
}

void startMenu() {
    int endMenu = 0;
    int num_player = 2;
    int item;
    int choice = 0;

    while (!endMenu) {
        item = choice;
        choice = interactStartMenu(num_player, item);
        if (choice == 0) {
            if (num_player > 2) {
                num_player--;
            }
        } else if (choice == 1) {
            if (num_player < 6) {
                num_player++;
            }
        } else if (choice == 2) {
            clear();
            game();
            getch();
        } else if (choice == 3) {
            endMenu = 1;
        }
    }
}

void drawMainMenu(int item) {
    int c;
    char menu[5][20] = {"Start", "Load", "Options", "Highscore", "Exit"};

    clear();
    center(3, "POKER");
    center(6, "Main Menu");
    for (c = 0; c < 5; c++) {
        if (c == item) {
            attron(A_REVERSE);
        }
        center(8 + c, menu[c]);
        attroff(A_REVERSE);
    }
    mvaddstr(LINES - 2, COLS - 20, "Move: Arrow keys");
    mvaddstr(LINES - 1, COLS - 20, "Select: Enter");
    move(0, 0);

    refresh();
}

int interactMainMenu() {
    int key = 0;
    int item = 0;

    keypad(stdscr, TRUE);

    while (key != 13) { //13 is Enter
        drawMainMenu(item);
        key = getch();
        switch (key) {
            case KEY_DOWN:
                item++;
                if (item > 4) {
                    item = 0;
                }
                break;
            case KEY_UP:
                item--;
                if (item < 0) {
                    item = 4;
                }
                break;
            default: break;
        }
    }
    return item;
}

int main() {
    int endProgram = 0;
    init_screen();
    while (!endProgram) {
        struct timespec delay = {0, 500000000L},
                rem;
        int choice = interactMainMenu();
        clear();
        if (choice == 0) {
            startMenu();
        } else if (choice == 1) {
            center(1, "LOAD GAME");
            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
            refresh();
            getch();
        } else if (choice == 2) {
            center(1, "OPTIONS");
            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
            refresh();
            getch();
        } else if (choice == 3) {
            center(1, "HIGHSCORE");
            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
            refresh();
            getch();
        } else if (choice == 4) {
            endProgram = 1;
        }
    }
    finish(0);
}

static void init_screen() {
    (void) signal(SIGINT, finish);      /* arrange interrupts to terminate */
    (void) initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, don't wait for \n */
    (void) noecho();       /* do not echo input */
    //timeout(500);          /* wait maximum 500ms for a character */
    /* Use timeout(-1) for blocking mode */
    timeout(-1);

    if (has_colors()) {

        start_color();
        // initialise you color pairs (foreground, background)
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_RED);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
    }
    /* set default color pair */
    attrset(COLOR_PAIR(1));
    bkgd(COLOR_PAIR(1));
}

static void finish(int sig) {
    endwin();

    /* do your non-curses wrap up here, like freeing the memory allocated */


    exit(sig);
}

int xmain1() {
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
    return 0;
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
//void center(int row, char *title) {
//    int len, start_point;
//    for (len = 0; ; len++) {
//        if (title[len] == '\0') {
//            break;
//        }
//    }
//    start_point = (COLS - len) / 2;
//    mvprintw(row, start_point, title);
//    refresh();
//}
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
//    print_menu(menu_win, highlight);
//    refresh();
//    while(1) {
//        c = wgetch(menu_win);
//        switch(c) {
//            case KEY_UP:
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
//                break;
//        }
//        print_menu(menu_win, highlight);
//        clear();
//        if (choice == 1) {
//            center(1, "START GAME");
//            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
//            refresh();
//            getch();
//        } else if (choice == 2) {
//            center(1, "LOAD GAME");
//            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
//            refresh();
//            getch();
//        } else if (choice == 3) {
//            center(1, "OPTIONS");
//            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
//            refresh();
//            getch();
//        } else if (choice == 4) {
//            center(1, "HIGHSCORE");
//            mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
//            refresh();
//            getch();
//        } else if(choice == 5) {
//            break;
//        }
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
//    center(3, "POKER");
//    center(6, "Main menu");
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


