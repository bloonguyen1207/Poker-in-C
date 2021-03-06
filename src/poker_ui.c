//CREDITS
//COSC2174 - Programming Techniques
//Semester A - 2016
//Lecturer's Name: Denis Rinfret
//Student's ID - name:
//s3558475 - Nguyen Ha Phuong Mai
//s3515305 - Nguyen Thi Mai Thao

#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include "poker.h"
#include "poker_ui.h"
#define POKER_COLOR_BACKGROUND    6
#define POKER_COLOR_RED           1
#define POKER_COLOR_YELLOW        3
#define POKER_COLOR_WHITE         7

static void init_screen();
static void finish(int sig);

void drawCard(Card card, int y, int x) {
    if (card.suit == SPADES) {
        mvaddstr(y, x, ".______."); y++;
        if (card.rank == 10) {
            mvprintw(y, x, "|%s .  |", getCardRank(card.rank)); y++;
        } else {
            mvprintw(y, x, "|%s  .  |", getCardRank(card.rank));
            y++;
        }
        mvaddstr(y, x, "|  / \\ |"); y++;
        mvaddstr(y, x, "| (_,_)|"); y++;
        mvaddstr(y, x, "|   I  |"); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == CLUBS) {
        mvaddstr(y, x, ".______."); y++;
        if (card.rank == 10) {
            mvprintw(y, x, "|%s _  |", getCardRank(card.rank)); y++;
        } else {
            mvprintw(y, x, "|%s  _  |", getCardRank(card.rank));
            y++;
        }
        mvaddstr(y, x, "|  ( ) |"); y++;
        mvaddstr(y, x, "| (_x_)|"); y++;
        mvaddstr(y, x, "|   I  |"); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == DIAMONDS) {
        mvaddstr(y, x, ".______."); y++;
        if (card.rank == 10) {
            mvprintw(y, x, "|%s/\\  |", getCardRank(card.rank)); y++;
        } else {
            mvprintw(y, x, "|%s /\\  |", getCardRank(card.rank));
            y++;
        }
        mvaddstr(y, x, "| /  \\ |"); y++;
        mvaddstr(y, x, "| \\  / |"); y++;
        mvaddstr(y, x, "|  \\/  |"); y++;
        mvaddstr(y, x, ".______.");
    } else if (card.suit == HEARTS) {
        mvaddstr(y, x, ".______."); y++;
        if (card.rank == 10) {
            mvprintw(y, x, "|%s_ _ |", getCardRank(card.rank)); y++;
        } else {
            mvprintw(y, x, "|%s_  _ |", getCardRank(card.rank)); y++;
        }
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

void credit() {
    clear();
    center(1, "CREDIT");
    center(4, "COSC2451 - Programming Techniques");
    center(5, "Semester A - 2016");
    center(7, "Lecturer's name: Denis Rinfret");
    mvaddstr(10, COLS / 2 - 20, "Nguyen Ha Phuong Mai");
    mvaddstr(10, COLS / 2 + 10, "s3558475");
    mvaddstr(12, COLS / 2 - 20, "Nguyen Thi Mai Thao");
    mvaddstr(12, COLS / 2 + 10, "s3515305");
    mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
    move(0, 0);
    refresh();
}

void drawPlayerCards(Player * players, int num_player, Table table) {
    Card card;
    card.suit = NONE;

    int players_posyx[5][2] = {{25, 30}, {5, 50}, {5, 30}, {5, 10}, {25, 10}};
    if (num_player == 2) {
        players_posyx[1][0] = 5; players_posyx[1][1] = 30;
    }
    if (num_player == 3) {
        players_posyx[2][0] = 5; players_posyx[2][1] = 10;
    }

    for (int i = 0; i < num_player; i++) {
        if (i < 4 && i > 0 ) {
            mvprintw(players_posyx[i][0] - 1, players_posyx[i][1], "%s", players[i].name);
            if (players[i].isBigBlind) {
                mvaddstr(players_posyx[i][0] - 1, players_posyx[i][1] + 13, "BB");
            } else if (players[i].isSmallBlind) {
                mvaddstr(players_posyx[i][0] - 1, players_posyx[i][1] + 13, "SB");
            }
        } else {
            mvprintw(players_posyx[i][0] + 6, players_posyx[i][1], "%s", players[i].name);
            if (players[i].isBigBlind) {
                mvaddstr(players_posyx[i][0] + 6, players_posyx[i][1] + 13, "BB");
            } else if (players[i]. isSmallBlind) {
                mvaddstr(players_posyx[i][0] + 6, players_posyx[i][1] + 13, "SB");
            }
        }
        if (players[i].status == 0) {
            continue;
        }
        if (i == 0 || (table.showCard == 1 && players[i].state != Folded)) {
            drawCard(players[i].hand[0], players_posyx[i][0], players_posyx[i][1]);
            drawCard(players[i].hand[1], players_posyx[i][0], players_posyx[i][1] + 7);
        } else {
            drawCard(card, players_posyx[i][0], players_posyx[i][1]);
            drawCard(card, players_posyx[i][0], players_posyx[i][1] + 7);
        }
    }
    refresh();
}

void drawPlayerInfo(Player * players, int num_player, int playerIdx) {
    int info_y = 0;
    mvaddstr(info_y, 100, "Money   Bet   State"); info_y += 2;

    for (int i = 0; i < num_player; i++) {
        mvaddstr(info_y, 114, "              ");
        mvprintw(info_y, 90, "%s", players[i].name);
        mvprintw(info_y, 100, "%d", players[i].money);
        mvprintw(info_y, 107, "%d", players[i].bet);
        mvprintw(info_y, 114, "%s", getState(players[i].state));
        info_y += 2;
    }
    mvprintw(20, 90, "%s 's Turn", players[playerIdx].name);
    move(0, 0);
    refresh();
}

void drawWinner(Player * players, int num_player, Table table, int roundIdx) {
    drawPlayerCards(players, num_player, table);
    int players_posyx[5][2] = {{25, 30}, {5, 50}, {5, 30}, {5, 10}, {25, 10}};
    if (num_player == 2) {
        players_posyx[1][0] = 5;
        players_posyx[1][1] = 30;
    }
    if (num_player == 3) {
        players_posyx[2][0] = 5;
        players_posyx[2][1] = 10;
    }
    int y = 22;
    for (int i = 0; i < num_player; i++) {
        if (players[i].status == 1 && roundIdx == 4) {
            attron(A_REVERSE);
            mvprintw(players_posyx[i][0] + 7, players_posyx[i][1], "%s", getRank(players[i].rank));
            attroff(A_REVERSE);
        }
        if (players[i].isWinner) {
            mvprintw(y, 90, "%s is a winner", players[i].name);
            y += 2;
        }
    }
    mvaddstr(y, 90, "Press any key to play again");

    move(0, 0);
    refresh();
}

void drawPot(Table table) {
    mvaddstr(12, 90, "----------------------");
    mvprintw(14, 90, "Pot: %d", table.pot_money);
    move(0, 0);
    refresh();}

void drawSharedCard(Table table, int roundIdx) {
    if (roundIdx > 0) {
        int x = 20;
        for (int i = 0; i < roundIdx + 2; i++) {
            drawCard(table.card[i], 15, x);
            x += 7;
        }
    }
    move(0, 0);
    refresh();}

void drawRangeMoney(int min, int max, int item, int money) {
    int c;
    char menu[4][10] = {"<", ">", "OK", "Cancel"};

    mvaddstr(28, 70, "             ");

    for (c = 0; c < 4; c++) {
        if (c == item) {
            attron(A_REVERSE);
        }
        if (c == 0) {
            if (money != min) {
                mvaddstr(28, 70, menu[c]);
            }
        } else if (c == 1) {
            if (money != max) {
                mvaddstr(28, 80, menu[c]);
            }
        } else if (c == 2) {
            mvaddstr(29, 70, menu[c]);
        } else if (c == 3) {
            mvaddstr(29, 75, menu[c]);
        }
        attroff(A_REVERSE);
    }
    mvaddstr(27, 71, "How much?");
    mvprintw(28, 74, "%d", money);


    move(0, 0);
    refresh();
}

int interactRangeMoney(int min, int max, int money, int item) {
    int key = 0;

    keypad(stdscr, TRUE);

    while (key != 13) { //13 is Enter
        drawRangeMoney(min, max, item, money);
        key = getch();
        switch (key) {
            case KEY_DOWN:
                if (item == 0 || item == 1) {
                    item += 2;
                } else if (item < 3){
                    item++;
                }
                break;
            case KEY_UP:
                if (item == 2 || item == 3) {
                    if (money == min && item == 2) {
                        item--;
                    } else if (money == max && item == 3) {
                        item++;
                    } else {
                        item -= 2;
                    }
                }
                break;
            case KEY_RIGHT:
                if ((item == 0 && money != max) || item == 2) {
                    item++;
                }
                break;
            case KEY_LEFT:
                if ((item == 1 && money != min) || item == 3) {
                    item--;
                }
                break;
            default: break;
        }
    }
    return item;
}

int rangeMoney(int min, int max) {
    int money = min;
    int choice = 1;
    int endMenu = 0;
    int item;

    while (!endMenu) {
        item = choice;
        choice = interactRangeMoney(min, max, money, item);
        if (choice == 0) {
            if (money > min) {
                money--;
            }
        } else if (choice == 1) {
            if (money < max) {
                money++;
            }
        } else if (choice == 2 || choice == 3) {
            if (choice == 3) {
                money = -1;
            }
            endMenu = 1;
        }
    }
    return money;
}

void updateInfo(Player * players, int num_player, Table table, int roundIdx, int playerIdx) {
    drawPlayerCards(players, num_player, table);
    drawSharedCard(table, roundIdx);
    drawPlayerInfo(players, num_player, playerIdx);
    drawPot(table);
}

void drawGame(int num_player, int roundIdx, Player * players, Table * table, int playerIdx, int item) {
    clear();
    updateInfo(players, num_player, *table, roundIdx, playerIdx);

    //draw Options menu
    int c;
    char menu[8][15] = {"Call", "Raise", "Check", "Bet", "Allin", "Fold", "Save", "Exit"};

    for (c = 0; c < 8; c++) {
        if (c == item) {
            attron(A_REVERSE);
        }
        if (playerIdx == 0 && players[playerIdx].status != 0) {
            if (isCallRaise(players[0], *table)) {
                if (isAllin(players[0], *table)) {
                    if (c == 4) {
                        mvaddstr(26, 55, menu[c]);
                    }
                } else if (players[0].money < table->highest_bet - players[0].bet + table->last_bet) {
                    if (c == 0) {
                        mvaddstr(26, 55, menu[c]);
                    } else if (c == 4) {
                        mvaddstr(28, 55, menu[c]);
                    }
                } else {
                    if (c == 0) {
                        mvaddstr(26, 55, menu[c]);
                    } else if (c == 1) {
                        mvaddstr(28, 55, menu[c]);
                    }
                }
            } else if (isCheckBet(players[0], *table)) {
                if (c == 2) {
                    mvaddstr(26, 55, menu[c]);
                }
                if (players[0].money >= table->ante * 2) {
                    if (c == 3) {
                        mvaddstr(28, 55, menu[c]);
                    }
                } else {
                    if (c == 4) {
                        mvaddstr(28, 55, menu[c]);
                    }
                }
            }
            if (c == 5) {
                mvaddstr(30, 55, menu[c]);
            }
        }
        if (c == 6) {
            mvaddstr(28, 100, menu[c]);
        } else if (c == 7) {
            mvaddstr(30, 100, menu[c]);
        }
        attroff(A_REVERSE);
    }

    move(0, 0);
    refresh();
}

int interactGame(int num_player, int roundIdx, Player * players, Table * table, int playerIdx) {
    int key = 0;
    int item = 0;

    //initialize the first option that player can choose
    if (playerIdx == 0 && players[playerIdx].status != 0) {
        if (isCallRaise(players[0], *table)) {
            if (isAllin(players[0], *table)) {
                item = 4;
            } else {
                item = 0;
            }
        } else if (isCheckBet(players[0], *table)) {
            item = 2;

        } else {
            item = 5;
        }
    } else {
        item = 6;
    }

    keypad(stdscr, TRUE);

    while (key != 13) { //13 is Enter
        drawGame(num_player, roundIdx, players, table, playerIdx, item);
        key = getch();
        if (playerIdx == 0 && players[playerIdx].status != 0) {
            if (isCallRaise(players[0], *table)) {
                if (isAllin(players[0], *table)) {
                    switch (key) {
                        case KEY_DOWN:
                            item++;
                            if (item > 7) {
                                item = 4;
                            }
                            break;
                        case KEY_UP:
                            item--;
                            if (item < 4) {
                                item = 7;
                            }
                            break;
                        case KEY_RIGHT:
                            if (item == 5) {
                                item = 7;
                            } else {
                                item = 6;
                            }
                            break;
                        case KEY_LEFT:
                            item = 5;
                            break;
                        default: break;
                    }
                } else if (players[0].money < table->highest_bet - players[0].bet + table->last_bet) {
                    switch (key) {
                        case KEY_DOWN:
                            if (item == 0) {
                                item = 4;
                            } else {
                                item++;
                            }
                            if (item > 7) {
                                item = 0;
                            }
                            break;
                        case KEY_UP:
                            if (item == 4) {
                                item = 0;
                            } else {
                                item--;
                            }
                            if (item < 0) {
                                item = 7;
                            }
                            break;
                        case KEY_RIGHT:
                            if (item == 5) {
                                item = 7;
                            } else {
                                item = 6;
                            }
                            break;
                        case KEY_LEFT:
                            item = 5;
                            break;
                        default: break;
                    }
                } else {
                    switch (key) {
                        case KEY_DOWN:
                            if (item == 1) {
                                item = 5;
                            } else {
                                item++;
                            }
                            if (item > 7) {
                                item = 0;
                            }
                            break;
                        case KEY_UP:
                            if (item == 5) {
                                item = 1;
                            } else {
                                item--;
                            }
                            if (item < 0) {
                                item = 7;
                            }
                            break;
                        case KEY_RIGHT:
                            if (item == 5) {
                                item = 7;
                            } else {
                                item = 6;
                            }
                            break;
                        case KEY_LEFT:
                            item = 5;
                            break;
                        default: break;
                    }
                }
            } else if (isCheckBet(players[0], *table)) {
                if (players[0].money >= table->ante * 2) {
                    switch (key) {
                        case KEY_DOWN:
                            if (item == 3) {
                                item = 5;
                            } else {
                                item++;
                            }
                            if (item > 7) {
                                item = 2;
                            }
                            break;
                        case KEY_UP:
                            if (item == 5) {
                                item = 3;
                            } else {
                                item--;
                            }
                            if (item < 2) {
                                item = 7;
                            }
                            break;
                        case KEY_RIGHT:
                            item = 7;
                            break;
                        case KEY_LEFT:
                            item = 5;
                            break;
                        default: break;
                    }
                } else {
                    switch (key) {
                        case KEY_DOWN:
                            if (item == 2) {
                                item = 4;
                            } else {
                                item++;
                            }
                            if (item > 7) {
                                item = 2;
                            }
                            break;
                        case KEY_UP:
                            if (item == 4) {
                                item = 2;
                            } else {
                                item--;
                            }
                            if (item < 2) {
                                item = 7;
                            }
                            break;
                        case KEY_RIGHT:
                            if (item == 5) {
                                item = 7;
                            } else {
                                item = 6;
                            }
                            break;
                        case KEY_LEFT:
                            item = 5;
                            break;
                        default: break;
                    }
                }
            }
        }
    }
    return item;
}

int turn(Player * players, Table * table, int roundIdx, int playerIdx, int num_player, Deck * deck,
         int countCheck, int countAllin, int countCall, int is_1st_bet) {
    int input = 6;

    if (playerIdx != 0) {
        if (playerIdx % 2  == 0) {
            input = consAI(&players[playerIdx], table, roundIdx);
        } else {
            input = aggrAI(&players[playerIdx], table, roundIdx);
        }
        clear();
        updateInfo(players, num_player, *table, roundIdx, playerIdx);
        if (!(players[0].status == 0 || (players[0].money == 0 && players[0].state != Allins))) {
            napms(500);
        }
    } else {
        //let user choose option
        int money = -1;
        table->isLoad = 0;
        while (money < 0) {
            input = interactGame(num_player, roundIdx, players, table, playerIdx);
            if (input == 0 || input == 2) {
                if (input == 0) {
                    call(&players[playerIdx], table);
                } else {
                    check(&players[playerIdx]);
                }
                money = 0;
            } else if (input == 1 || input == 3) {
                int min = minMoney(players[0], *table);
                money = rangeMoney(min, players[0].money);
                if (input == 1) {
                    raisePoker(&players[playerIdx], table, money);
                } else {
                    bet(&players[playerIdx], table, money);
                }
                table->last_bet = money;
            } else if (input == 4) {
                money = players[0].money;
                allin(&players[playerIdx], table);
                table->last_bet = players[playerIdx].money;
            } else if (input == 5) {
                fold(&players[playerIdx]);
                money = 0;
            } else if (input == 6) {
                if (save(players, table, deck, num_player, roundIdx, playerIdx,
                         countCheck, countAllin, countCall, is_1st_bet) == 1) {
                    mvaddstr(26, 95, "Save complete");
                } else {
                    mvaddstr(26, 92, "Error saving game");
                }
                move(0, 0);
                refresh();
                napms(1000);
            } else if (input == 7) {
                break;
            }
        }
        if (input == 7) {
            return -1;
        }
    }

    //Update highest_bet
    if (players[playerIdx].bet > table->highest_bet) {
        table->highest_bet = players[playerIdx].bet;
    }
    updateInfo(players, num_player, *table, roundIdx, playerIdx);
    return input;
}

int roundPoker(Player *players, Table *table, Deck *deck, int num_player, int roundIdx, int countActivePlayer) {
    int playerIdx  = 0, countCheck = 0, countAllin = 0, countCall = 0, end_round = 0, is_1st_bet = 0, count = 0;
    State lastState = None;

    if (table->isLoad == 0) {
        // Deal shared cards
        if (roundIdx > 0) {
            dealSharedCards(table, deck, roundIdx);
            table->last_bet = 0;
        }

        // Check first bet, first round
        if (roundIdx == 0) {
            for (int b = 0; b < num_player; b++) {
                if (players[b].state != BB && players[b].state != SB) {
                    count++;
                }
            }
            if (count == 0 && players[playerIdx].state == BB) {
                if (num_player == 2) {
                    is_1st_bet = 1;
                } else {
                    is_1st_bet = 0;
                }
            } else {
                lastState = BB;
                is_1st_bet = 1;
            }
            // If first bet, start with person after Big Blind
            if (is_1st_bet) {
                for (int i = 0; i < num_player; i++) {
                    if (players[i].state == BB || players[playerIdx].state == SB) {
                        if (players[i].state == BB) {
                            playerIdx = i + 1;
                            if (playerIdx >= num_player) {
                                playerIdx = 0;
                            }
                            table->last_bet = players[i].bet;
                        }
                        continue;
                    }
                }
                table->highest_bet = table->ante * 2;
            }
        } else {
            // If not first round, start with Big blind, small blind, if not available, start with the one who is still active
            for (int e = 0; e < num_player; e++) {
                if (players[e].isBigBlind && players[e].status == 1) {
                    playerIdx = e;
                } else if (players[e].isSmallBlind && players[e].status == 1) {
                    playerIdx = e;
                }
            }
        }
        for (int f = 0; f < num_player; f++) {
            if (players[f].state == Allins) {
                countAllin++;
            }
        }
        // If all player allin, show cards and check hands
        if (countAllin == countActivePlayer && is_1st_bet == 0) {
            table->showCard = 1;
            return countActivePlayer;
        }
    } else {
        loadRoundInfo(&countCheck, &countAllin, &countCall, &is_1st_bet);
    }

    while (!end_round) {
        if (table->isLoad == 0) {
            if (playerIdx >= num_player) {
                playerIdx = 0;
            }
            if (countAllin == countActivePlayer - 1 && players[playerIdx].bet == table->highest_bet) {
                table->showCard = 1;
                break;
            }
            // If player not fold or out of money
        }
        if (players[playerIdx].state != Folded && players[playerIdx].state != Allins
            && players[playerIdx].money > 0 && players[playerIdx].status == 1) {
            if (players[playerIdx].state == BB && table->isLoad == 0) {
                is_1st_bet = 0;
            }
            if (turn(players, table, roundIdx, playerIdx, num_player, deck,
                     countCheck, countAllin, countCall, is_1st_bet) == -1) {
                return -1;
            }
            if (is_1st_bet) {
                // if is first bet, round end when return to Big blind
                if (players[playerIdx].isBigBlind && players[playerIdx].state == Checked) {
                    end_round = 1;
                }
            } else {
                if (players[playerIdx].state == Checked) {
                    countCheck++;
                }
            }
            if (players[playerIdx].state == Allins) {
                countAllin++;
            }
            if (players[playerIdx].state == Called) {
                // If player money is just or not enough to call
                if (players[playerIdx].money == 0) {
                    players[playerIdx].state = Allins;
                    countAllin++;
                } else {
                    countCall++;
                }
            }
            if (players[playerIdx].state == Folded) {
                countActivePlayer--;
            }
            if (players[playerIdx].state == Raised || players[playerIdx].state == Bets) {
                // If player raised or bet all of their money
                if (players[playerIdx].money == 0) {
                    players[playerIdx].state = Allins;
                    countAllin++;
                }
                countCall = 0;
            }
            // Check conditions to break round
            if (countActivePlayer == 1 || countAllin == countActivePlayer || countCheck == countActivePlayer ||
                (countCall == countActivePlayer - 1 && !is_1st_bet) ||
                (players[playerIdx].isBigBlind && lastState == BB && players[playerIdx].state == Folded) ||
                (countAllin + countCall == countActivePlayer - 1 && (players[playerIdx].state == Called ||
                        players[playerIdx].state == Checked)) ||
                        (countAllin + countCall || countAllin + countCheck) == countActivePlayer) {
                lastState = None;
                end_round = 1;
            }
        }
        playerIdx++;
    }
    return countActivePlayer;
}

int game (Player * players, Table * table, Deck * deck, int num_player, int nextSB, int roundIdx) {
    int prevPlayer = nextSB - 1, nextBB = nextSB + 1;
    int countActivePlayer = num_player;

    if (table->isLoad == 0) {
        // Find next person to be small blind
        if (players[nextSB].money == 0) {
            for (int i = 0; i < num_player; i++) {
                nextSB++;
                if (nextSB == num_player) {
                    nextSB = 0;
                }
                if (players[nextSB].money > 0) {
                    nextBB = nextSB + 1;
                    break;
                }
            }
        }

        // Find next person to be big blind
        if (players[nextBB].money == 0) {
            for (int i = 0; i < num_player; i++) {
                nextBB++;
                if (nextBB == num_player) {
                    nextBB = 0;
                }
                if (players[nextBB].money > 0) {
                    break;
                }
            }
        }

        if (nextSB == num_player - 1) {
            nextBB = 0;
        }
        if (nextSB == num_player) {
            nextSB = 0;
            nextBB = 1;
        }

        // Reset previous big blind and small blind
        for (int i = 0; i < num_player; i++) {
            if (i != nextSB) {
                players[i].isSmallBlind = 0;
            }
            if (players[i].money == 0) {
                players[i].isBigBlind = 0;
                players[i].isSmallBlind = 0;
            }
        }

        // Set big blind, small blind and reset previous SB, BB
        table->ante = 2;
        reset(players, table, num_player, deck);
        players[nextSB].isSmallBlind = 1;
        players[nextSB].state = SB;
        if (players[nextSB].money <= table->ante) {
            players[nextSB].bet = players[nextSB].money;
        } else {
            players[nextSB].bet = table->ante;
        }
        players[nextSB].money = players[nextSB].money - players[nextSB].bet;
        players[nextBB].isBigBlind = 1;
        players[nextBB].state = BB;
        if (players[nextBB].money <= table->ante * 2) {
            players[nextBB].bet = players[nextBB].money;
        } else {
            players[nextBB].bet = table->ante * 2;
        }
        players[nextBB].money = players[nextBB].money - players[nextBB].bet;
        players[prevPlayer].isSmallBlind = 0;
        players[nextSB].isBigBlind = 0;
        table->pot_money = players[nextSB].bet + players[nextBB].bet;
    }

    nextSB++;
    if (nextSB == num_player) {
        nextSB = 0;
    }

    if (table->isLoad == 0) {
        // Deal cards for players
        dealStartingHand(players, deck, num_player);
        for (int i = 0; i < num_player; i++) {
            if (players[i].money <= 0) {
                players[i].status = 0;
                countActivePlayer--;
            }
        }
    }

    int countFold = 0;

    // Begin game
    for (; roundIdx < 4; roundIdx++) {
        countActivePlayer = roundPoker(players, table, deck, num_player, roundIdx, countActivePlayer);
        if (countActivePlayer == -1) {
            return -1;
        }
        countFold = 0;
        for (int i = 0; i < num_player; i++) {
            if (players[i].status == 0) {
                countFold++;
            }
        }
        // If only 1 player not fold, end game
        if (countFold == num_player - 1) {
            break;
        }
    }

    // Check winner
    if (countFold == num_player - 1 && roundIdx < 4 ) {
        for (int i = 0; i < num_player; i++) {
            if (players[i].status == 1) {
                players[i].isWinner = 1;
                break;
            }
        }
    } else {
        Hand *hands = createHands(players, table, num_player);
        testHand(hands, players, num_player);
        free(hands);
    }
    table->showCard = 1;
    drawWinner(players, num_player, *table, roundIdx);
    getch();
    award(players, table, num_player);
    return nextSB;
}

void setUpGame(int num_player, int isLoad) {
    int nextBlind = 0;
    Table *table = createTable();
    Deck *deck = newDeck();
    int size = 52;
    Player *players = createPlayers(num_player);
    int roundIdx = 0;
    if (isLoad == 1) {
        load(table, deck, players, &roundIdx);
        for (int i = 0; i < num_player; i++) {
            if (players[i].isSmallBlind == 1) {
                nextBlind = i;
            }
        }
        table->isLoad = 1;
        deck->card_index = num_player * 2 + table->card_idx;
    }
    for (int gameIdx = 0; ; gameIdx++) {
        if (isLoad == 0) {
            int remain = num_player;
            for (int m = 0; m < num_player; m++) {
                if (players[m].money <= 0) {
                    remain--;
                }
            }
            if (players[0].money == 0 || remain == 1) {
                clear();
                if (players[0].money == 0) {
                    center(0, "Game Over");
                } else {
                    center(0, "No player left");
                }
                mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
                refresh();
                getch();
                break;
            }
            shuffleDeck(deck, size);
        }
        nextBlind = game(players, table, deck, num_player, nextBlind, roundIdx);
        if (nextBlind == -1) {
            break;
        }
        isLoad = 0;
        roundIdx = 0;
    }
    // Free everything
    for (int i = 0; i < num_player; i++) {
        free(players[i].max_hand);
    }
    free(players);
    free(deck);
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
            if (num_player != 5) {
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
                if (item == 2 && num_player == 5) {
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
                    if (num_player != 5) {
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
    int choice = 1;

    while (!endMenu) {
        item = choice;
        choice = interactStartMenu(num_player, item);
        if (choice == 0) {
            if (num_player > 2) {
                num_player--;
            }
        } else if (choice == 1) {
            if (num_player < 5) {
                num_player++;
            }
        } else if (choice == 2) {
            setUpGame(num_player, 0);

        } else if (choice == 3) {
            endMenu = 1;
        }
    }
}

void drawMainMenu(int item) {
    int c;
    char menu[5][20] = {"Start", "Load", "Credit", "Highscore", "Exit"};

    clear();
    center(3, "POKER");
    center(7, "Main Menu");
    for (c = 0; c < 5; c++) {
        if (c == item) {
            attron(A_REVERSE);
        }
        mvaddstr(11 + (c * 2), COLS / 2 - 5, menu[c]);
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
            int num_player = 2;
            if (loadNumPlayer(&num_player) == 1){
                setUpGame(num_player, 1);
            } else {
                center(1, "LOADING ERROR");
                center(3, "Can not find save file");
                mvaddstr(LINES - 1, COLS - 20, "Back: Any key");
                refresh();
                getch();
            }
        } else if (choice == 2) {
            credit();
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
        init_pair(1, POKER_COLOR_YELLOW, POKER_COLOR_BACKGROUND);
        init_pair(2, POKER_COLOR_RED, POKER_COLOR_BACKGROUND);
        init_pair(3, POKER_COLOR_WHITE, POKER_COLOR_BACKGROUND);
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

