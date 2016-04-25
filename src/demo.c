//
// Created by bloo on 12/04/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum suit {HEARTS, DIAMONDS, CLUBS, SPADES};
typedef enum suit Suit;

enum rank {HighCard, OnePair, TwoPairs, Three, Four, Straight, Flush, FullHouse, StraightFlush, RoyalStraightFlush};
typedef enum rank Rank;

enum option {Call, Raise, Check, Bet, Allin, Fold};
typedef enum option Option;

struct card {
    Suit suit;
    int rank;
};
typedef struct card Card;

struct deck {
    Card cards[52];
    int card_index;
};
typedef struct deck Deck;

struct player {
    char name[20];
    int money;
    int bet;
    int status;
    Card hand[2];
    Card *max_hand;
    Rank rank;
    int isBigBlind;
    int isSmallBlind;
    Option option;
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

struct hand {
    Card card[7];
};
typedef struct hand Hand;

char* getSuit(Suit s) {
    switch(s){
        case HEARTS: return "\u2665";
        case DIAMONDS: return "\u2666";
        case CLUBS: return "\u2663";
        case SPADES: return "\u2660";
    }
    return NULL;
}

Deck* newDeck() {
    Deck* deck = malloc(sizeof(Deck));
    int k = 0;
    deck->card_index = 0;
    Card c;
    for (Suit i = HEARTS; i <= SPADES; i++) {
        for (int j = 1; j < 14; j++) {
            c.rank = j;
            c.suit = i;
            deck->cards[k] = c;
            k++;
            if (k == 52) {
                return deck;
            }
        }
    }
    return NULL;
}

void shuffleDeck(Deck* deck, int size) {
    srand((unsigned int) time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int randIdx = rand() % i;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[randIdx];
        deck->cards[randIdx] = temp;
    }
}

Player* createPlayers(int num_player) {
    if (num_player >= 2 && num_player <= 10) {
        Player *players = malloc(sizeof(Player) * num_player);
        for (int i = 0; i < num_player; i++) {
            strcpy(players[i].name, "Player 0");
            players[i].status = 1;
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

void displayPlayerInfo(Player player) {
    printf("%s -- ", player.name);
    printf("Money: %i -- ", player.money);
    if (player.status == 0) {
        printf("Status: Fold -- ");
    } else {printf("Status: Active -- ");}
    printf("Bet: %i -- ", player.bet);
    if (player.isBigBlind) {
        printf("Big Blind");
    } else if (player.isSmallBlind) {
        printf("Small Blind");
    }
}

Table * createTable() {
    Table * table = malloc(sizeof(Table));
    table->card_idx = 0;
    table->highest_bet = 0;
    return table;
}

void displayTableInfo(Table table) {
    printf("Pot: %i\n", table.pot_money);
    if (table.card[0].rank != 0) {
        printf("Shared cards: ");
    }
    for (int i = 0; i <= table.card_idx; i++) {
        if (table.card[i].rank == 0) {
            break;
        }
        printf("%s %i; ", getSuit(table.card[i].suit), table.card[i].rank);
        if (i == table.card_idx) {
            printf("\n");
        }
    }
    printf("\n");
}

void dealStartingHand(Player*players, Deck* deck, int num_player) {
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < num_player; i++) {
            players[i].hand[j] = deck->cards[deck->card_index];
            deck->card_index++;
        }
    }
}

void dealSharedCards(Table *table, Deck* deck, int time){
    //1st time: deal 3 cards: largest idx = 2
    //2nd time: deal 1 card:  largest idx = 3
    //3rd time: deal 1 card:  largest idx = 4 --> largest idx = time + 1
    for (; table->card_idx <= time + 1; table->card_idx++) {
        table->card[table->card_idx] = deck->cards[deck->card_index];
        deck->card_index++;
    }
};

Hand* createHand(Player *players, Table *table, int num_player) {
    Hand * hands = malloc(sizeof(Hand) * num_player);
    for (int i = 0; i < num_player; i++) {
        for (int j = 0; j < 2; j++) {
            hands[i].card[j] = players[i].hand[j];
        }
    }

    for (int i = 0; i < num_player; i++) {
        for (int j = 0; j < 5; j++) {
            hands[i].card[j + 2] = table[0].card[j];
        }
    }
    return hands;
}

void swapCards(Hand* hand, int player, int fstIdx, int secIdx) {
    Card temp = hand[player].card[fstIdx];
    hand[player].card[fstIdx] = hand[player].card[secIdx];
    hand[player].card[secIdx] = temp;
}

void sortHand(Hand* hand, int num_player) {
    for (int i = 0; i < num_player; i++) {
        int max_rank;
        for (int j = 0; j < 7; j++) {
            max_rank = hand[i].card[j].rank;
            for (int k = j+1; k < 7; k++) {
                if (hand[i].card[k].rank > max_rank) {
                    max_rank = hand[i].card[k].rank;
                    swapCards(hand, i, j, k);
                }
            }
        }
    }
}

int searchHandRank(Hand hand, int rank) {
    for (int i = 0; i < 7; i++) {
        if (hand.card[i].rank == rank) {
            return 1;
        }
    }
    return 0;
}

Card searchCard(Hand hand, int rank) {
    Card temp = {HEARTS};
    for (int i = 0; i < 7; i++) {
        if (hand.card[i].rank == rank) {
            return hand.card[i];
        }
    }
    return temp;
}

Card isHighestCard(Hand hand) {
    Card maxCard;
    maxCard.rank = 1;
    for (int i = 0; i < 7; i++){
        if (hand.card[i].rank == 1) {
            maxCard = hand.card[i];
            return maxCard;
        } else if (hand.card[i].rank > maxCard.rank) {
            maxCard = hand.card[i];
        }
    }
    return maxCard;
};

//the hand must be sorted before checking
void isHighCard(Hand hand, Player* player) {
    player->max_hand[0] = isHighestCard(hand);
    for (int i = 1; i < 5; i++) {
        player->max_hand[i] = hand.card[i];
    }
    player->rank = HighCard;
}

int isPair(Hand hand, Player* player){
    int exist= 0;
    int idx = 2;
    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 7; j++) {
            if(hand.card[i].rank == hand.card[j].rank) {
                exist = hand.card[i].rank;
                player->max_hand[0] = hand.card[i];
                player->max_hand[1] = hand.card[j];
                player->rank = OnePair;
                for (int l = 0; l < 7; l++) {
                    if(hand.card[l].rank != exist) {
                        player->max_hand[idx] = hand.card[l];
                        idx++;
                    }
                    if (idx == 5) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int is2Pair(Hand hand, Player*player) {
    int count = 0;
    int exist[2] = {0, 0};
    int idx = 0;
    int ext_idx = 0;

    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 7; j++) {
            if(hand.card[i].rank == hand.card[j].rank && hand.card[i].rank != exist[0]) {
                exist[ext_idx] = hand.card[i].rank;
                player->max_hand[idx] = hand.card[i];
                idx++;
                player->max_hand[idx] = hand.card[j];
                count++;
                ext_idx++;
                idx++;
            }
            if (count == 2) {
                for (int k = 0; k < 7; k++) {
                    if (hand.card[k].rank != exist[0] && hand.card[k].rank != exist[1]) {
                        player->max_hand[idx] = hand.card[k];
                        player->rank = TwoPairs;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int is3OfAKind(Hand hand, Player*player) {
    int exist = 0;
    int idx = 3;
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 6; j++) {
            for (int k = j + 1; k < 7; k++) {
                if (hand.card[i].rank == hand.card[j].rank &&
                    hand.card[i].rank == hand.card[k].rank &&
                    hand.card[i].rank != exist) {
                    player->max_hand[0] = hand.card[i];
                    player->max_hand[1] = hand.card[j];
                    player->max_hand[2] = hand.card[k];
                    exist = hand.card[i].rank;
                    for (int l = 0; l < 7; l++) {
                        if (hand.card[l].rank != exist) {
                            player->max_hand[idx] = hand.card[l];
                            idx++;
                        }
                        if (idx == 5) {
                            player->rank = Three;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int is4OfAKind(Hand hand, Player*player) {
    int exist = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            for (int k = j + 1; k < 6; k++) {
                for (int l = k + 1; l < 7; l++) {
                    if (hand.card[i].rank == hand.card[j].rank &&
                        hand.card[i].rank == hand.card[k].rank &&
                        hand.card[i].rank == hand.card[l].rank) {
                        exist = hand.card[i].rank;
                        player->max_hand[0] = hand.card[i];
                        player->max_hand[1] = hand.card[j];
                        player->max_hand[2] = hand.card[k];
                        player->max_hand[3] = hand.card[l];
                        for (int m = 0; m < 7; m++) {
                            if(hand.card[m].rank != exist) {
                                player->max_hand[4] = hand.card[m];
                                player->rank = Four;
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int isFullHouse(Hand hand, Player*player) {
    int exist = 0;
    int count = 0;
    if (is3OfAKind(hand, player)){
        for (int i = 0; i < 5; i++) {
            for (int j = i + 1; j < 6; j++) {
                for (int k = j + 1; k < 7; k++) {
                    if (hand.card[i].rank == hand.card[j].rank &&
                        hand.card[i].rank == hand.card[k].rank &&
                        hand.card[i].rank != exist) {
                        exist = hand.card[i].rank;
                    }
                }
            }
        }
        for (int i = 0; i < 6; i++) {
            for (int j = i + 1; j < 7; j++) {
                if(hand.card[i].rank == hand.card[j].rank && hand.card[i].rank != exist) {
                    player->max_hand[3] = hand.card[i];
                    player->max_hand[4] = hand.card[j];
                    count++;
                }
                if (count >= 1) {
                    player->rank = FullHouse;
                    return 1;
                }
            }
        }
    }
    return 0;
}

//the hand must be sorted before checking
int isStraight(Hand hand, Player*player) {
    for (int j = 0; j < 3; j++) {
        int check = 0;
        int temp = hand.card[j].rank;
        player->max_hand[check] = searchCard(hand, temp);
        check++;
        if (temp == 13) {
            for (int k = 12; k >= 10; k--) {
                if (searchHandRank(hand, k)) {
                    player->max_hand[check] = searchCard(hand, k);
                    check++;
                };
            }
            if (searchHandRank(hand, 1)) {
                player->max_hand[check] = searchCard(hand, 1);
                check++;
            };
            if (check == 5) {
                player->rank = Straight;
                return 1;
            }
        }

        if (check > 1) {
            check = 1;
        }
        for (int k = 1; k < 5; k++) {
            if (temp - k > 0 && searchHandRank(hand, temp - k)) {
                player->max_hand[check] = searchCard(hand, temp - k);
                check++;
            };
        }
        if (check == 5) {
            player->rank = Straight;
            return 1;
        }
    }
    return 0;
}

//the hand must be sorted before checking
int isFlush(Hand hand, Player* player) {
    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        int count = 0; //count number of cards that have the same suit
        for (int j = 0; j < 7; j++) {
            if (hand.card[j].suit == suit) {
                player->max_hand[count] = hand.card[j]; //add card to max_hand
                count++;
                if (count == 5) {
                    player->rank = Flush;
                    return 1;
                }
            }
        }
    }
    return 0;
}

//the hand must be sorted before checking
int isStraightFlush(Hand hand, Player* player) {
    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        Hand* temp = malloc(sizeof(Hand));
        int count = 0; //count number of cards that have the same suit
        for (int j = 0; j < 7; j++) {
            if (hand.card[j].suit == suit) {
                temp->card[count] = hand.card[j];
                count++;
            }
        }

        if (count >= 5 && isStraight(*temp, player)) {
            free(temp);
            player->rank = StraightFlush;
            return 1;
        }
        free(temp);
    }
    return 0;
}

int isRoyalStraightFlush(Hand hand, Player* player) {
    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        Hand* temp = malloc(sizeof(Hand));
        int count = 0; //count number of cards that have the same suit
        int check = 0;
        for (int j = 0; j < 7; j++) {
            if (hand.card[j].suit == suit) {
                temp->card[count] = hand.card[j];
                count++;
            }
        }
        if (count >= 5) {
            for (int i = 13; i >= 10; i--) {
                if (searchHandRank(*temp, i)) {
                    player->max_hand[check] = searchCard(*temp, i); //add card to max_hand
                    check++;
                };
            }
            if (searchHandRank(*temp, 1)) {
                player->max_hand[check] = searchCard(*temp, 1); //add card to max_hand
                check++;
            };
            if (check == 5) {
                free(temp);
                player->rank = RoyalStraightFlush;
                return 1;
            }
        }
        free(temp);
    }
    return 0;
}

int scanInput(int num_option) {
    int n;
    int input = 0;
    char c;
    int the_end = 0;

    while (!the_end) {
        printf("Enter input: ");
        n = scanf("%d", &input);
        if (n == 0) {
            c = getchar();
            while (c != '\n') {
                c = getchar();
            }
            printf("Invalid input\n");
        } else if (input >= 1 && input <= num_option){
            the_end = 1;

        } else {
            printf("Invalid input\n");
        }
    }
    printf("\n");
    return input;
}

void updateMoney(Player *player, Table * table, int money) {
    player->money = player->money - money;
    player->bet += money;
    table->pot_money += money;
}

void displayRangeMoney(int min, int max) {
    printf("The money should be between %i and %i\n", min, max);
}

int inputMoney(int min, int max) {
    int n;
    int input = 0;
    char c;
    int the_end = 0;

    while (!the_end) {
        printf("Enter money: ");
        n = scanf("%d", &input);
        if (n == 0) {
            c = getchar();
            while (c != '\n') {
                c = getchar();
            }
            printf("Invalid input\n");
        } else if (input >= min && input <= max){
            the_end = 1;
        } else {
            printf("Invalid input\n");
        }
    }
    printf("\n");
    return input;
}

int isAllin(Player player, Table table) {
    return player.money < table.highest_bet - player.bet;
}

int isCallRaise(Player player, Table table) {
    return player.bet < table.highest_bet;
}

int isCheckBet(Player player, Table table) {
    return player.bet == table.highest_bet;
}

void allin(Player *player, Table * table) {
    if (isAllin(*player, *table)) {
        player->option = Allin;
        updateMoney(player, table, player->money);
    }
}

void call(Player *player, Table * table) {
    if (player->money >= table->highest_bet - player->bet) {
        player->option = Call;
        updateMoney(player, table, table->highest_bet - player->bet);
    }
}

void raise (Player *player, Table * table, int money) {
    int call_money = table->highest_bet - player->bet;
    if (isAllin(*player, *table)) {
        allin(player, table);
    } else if (player->money >= money && money >= call_money + table->last_bet ) {
        player->option = Raise;
        updateMoney(player, table, money);
    }
}

void check(Player * player) {
    player->option = Check;
}

void bet(Player * player, Table * table, int money) {
    if (isAllin(*player, *table)) {
        allin(player, table);
    } else if (money >= table->ante * 2 && player->money >= money) {
        player->option = Bet;
        updateMoney(player, table, money);
    }
}

void fold(Player * player) {
    player->status = 0;
}

int minMoney(Player player, Table table) {
    if (isCallRaise(player, table)) {
        return table.highest_bet - player.bet + table.last_bet;
    } else if (isCheckBet(player, table)) {
        return table.ante * 2;
    }
    return 0;
}

void displayOption(Player player, Table table) {
    printf("Choose option:\n");
    if (isCallRaise(player, table)) {
        if (isAllin(player, table)) {
            printf("1. Allin");
        } else if(player.money < table.highest_bet - player.bet + table.last_bet){
            printf("1. Call\n");
            printf("2. Allin\n");
        } else {
            printf("1. Call\n");
            printf("2. Raise\n");
        }
    } else if (isCheckBet(player, table)) {
        printf("1. Check\n");
        if (player.money >= table.ante * 2) {
            printf("2. Bet\n");
        } else {
            printf("2. Allin\n");
        }
    }
    printf("3. Fold\n");
}

void runOption(Player * player, Table * table, int option, int money) {
    if (isCallRaise(*player, *table)) {
        if (isAllin(*player, *table)) {
            if (option == 1) {
                allin(player, table);
            }
        } else if (player->money < table->highest_bet - player->bet + table->last_bet) {
            if (option == 1) {
                call(player, table);
            } else if (option == 2) {
                allin(player, table);
            }
        } else {
            if (option == 1) {
                call(player, table);
            } else if (option == 2) {
                raise(player, table, money);
            }
        }
    } else if (isCheckBet(*player, *table)) {
        if (option == 1) {
            check(player);
        } else {
            if (player->money >= table->ante *2) {
                if (option == 2) {
                    bet(player, table, money);
                }
            } else {
                allin(player, table);
            }
        }
    }
    if (option == 3) {
        fold(player);
    }
}

int turn(Player *player, Table * table) {
    int input;
    printf("\n\n%s's Turn\n", player->name);

    //print cards in hand
    for (int j = 0; j < 2; j++) {
        printf("%s %i; ", getSuit(player->hand[j].suit), player->hand[j].rank);
    }
    printf("\n\n");

    //let user choose option
    displayOption(*player, * table);
    input = scanInput(3);
    //let user input money if they choose raise or bet
    int money = 0;
    int isRaiseOrBet = 0;
    if (isCallRaise(*player, *table)) {
        int call_money = table->highest_bet - player->bet;
        if (player->money >= call_money + table->last_bet){
            if (input == 2) {
                isRaiseOrBet = 1;
            }
        }
    } else if (isCheckBet(*player, *table)) {
        if (player->money >= table->ante * 2) {
            if (input == 2) {
                isRaiseOrBet = 1;
            }
        }
    }
    if (isRaiseOrBet == 1) {
        int min = minMoney(*player, *table);
        displayRangeMoney(min, player->money);
        money = inputMoney(min, player->money);
    }

    //execute the option
    runOption(player, table, input, money);

    //Update highest_bet
    if (player->bet > table->highest_bet) {
        table->highest_bet = player->bet;
    }
    printf("\nHigh bet: %i\n", table->highest_bet);
    printf("\n------End%sTurn-----\n", player->name);
    return input;
}

void roundPoker(Player *players, Table *table, Deck *deck, int num_player, int roundIdx, int bigBlind) {
    int prevPlayer = 0; //idx of prevPlayer
    int lastHighest = 0;
    int playerIdx = 0;
    int count_fold = 0;
    int end_game = 0;
    int is_1st_bet = 0;
//    int isFirstTurn = 1;
    if (roundIdx > 0) {
        dealSharedCards(table, deck, roundIdx);
    } else if(roundIdx == 0) {
        is_1st_bet = 1;
    }

    while (!end_game) {
        for (int i = 0; i < num_player; i++) {
            if (playerIdx >= num_player) {
                playerIdx = 0;
            }
            if (is_1st_bet) {
                table->highest_bet = table->ante * 2;
                if (players[i].isBigBlind == 1 || players[i].isSmallBlind == 1) {
                    if (players[i].isBigBlind == 1){
                        playerIdx = i + 1;
                        lastHighest = i;
                    } else if (players[i].isSmallBlind == 1){
                        playerIdx = i + 2;
                        lastHighest = i + 1;
                    }
                    continue;
                }
            }
            displayTableInfo(*table);
            if (turn(&players[playerIdx], table) == 3) {
                count_fold++;
            }
            if (count_fold == num_player - 1) {
                end_game = 1;
                break;
            }
            if(!is_1st_bet) {
                if (playerIdx == lastHighest) {
                    int check = 0;
                    for (int x = 0; x < num_player; x++) {
                        if ((players[x].status != 0 || !isAllin(players[x], *table)) &&
                            players[x].bet < table->highest_bet) {
                            check++;
                        }
                    }
                    printf("%i", check);
                    if (check == 0) {
                        end_game = 1;
                        break;
                    }
                }
            }
            //check if the user is able to choose option (get turn)
            if (players[playerIdx].option != Allin  && players[playerIdx].status == 1) {
                //print all player info
                for (int z = 0; z < num_player; z++) {
                    displayPlayerInfo(players[z]);
                    printf("\n");
                }
            }
            playerIdx++;
        }
        is_1st_bet = 0;
    }
}

/*void roundPoker(Player *players, Table *table, Deck *deck, int num_player, int round_idx) {
    int prevPlayer = 0; //idx of prevPlayer
    int count_fold = 0;
    int the_end = 0;
    int isFirstTurn = 1;

    if (round_idx > 0) {
        dealSharedCards(table, deck, round_idx);
    }

    while (!the_end) {
        //Except for first turn, if there is still any active or not allin player that bet < highest_bet , continue round
        int check = 0;
        if (!isFirstTurn) {
            for (int x = 0; x < num_player; x++) {
                if ((players[x].status != 0 || !isAllin(&players[x])) && players[x].bet < table->highest_bet) {
                    check++;
                }
            }
            if (check == 0) {
                break;
            }
        }
        for (int i = 0; i < num_player; i++) {
            //TODO check if there is only 1 active player (work), but game also need to check again to break game
            if (count_fold == num_player - 1) {
                the_end = 1;
                break;
            }
            printf("\nCountFold: %i\n", count_fold);
            //set prevPlayer
            if (i == 0) {
                prevPlayer = num_player - 1;
            } else {
                prevPlayer = i - 1;
            }
            while (players[prevPlayer].status == 0 && prevPlayer != i) {
                if (prevPlayer == 0) {
                    prevPlayer = num_player - 1;
                } else {
                    prevPlayer--;
                }
            }

            //check if the user is able to choose option (get turn)
            if (players[i].option != Allin  && players[i].status == 1) {
                displayTableInfo(*table);
                //print all player info
                for (int z = 0; z < num_player; z++) {
                    displayPlayerInfo(players[z]);
                    printf("\n");
                }
                if (turn(&players[i], table) == 3) {
                    count_fold++;
                }
            }
        }
        isFirstTurn = 0;
    }
    printf("-------------------EndRound------------------------\n\n");
}*/

void game (Player * players, Table * table, Deck * deck, int num_player, int gameIdx) {
    int nextPlayer = 0;
    int prevPlayer = 0;
    int bigBlind = 0;
    table->ante = 250;
    if (gameIdx == 0) {
        players[0].isSmallBlind = 1;
        players[0].bet = table->ante;
        players[1].isBigBlind = 1;
        players[1].bet = table->ante * 2;
        table->pot_money = players[0].bet + players[1].bet;
        nextPlayer++;
        prevPlayer = 0;
        bigBlind = 1;
    } else {
        players[nextPlayer].isSmallBlind = 1;
        players[nextPlayer].bet = table->ante;
        players[nextPlayer + 1].isBigBlind = 1;
        players[nextPlayer + 1].bet = table->ante * 2;
        players[prevPlayer].isSmallBlind = 0;
        players[nextPlayer].isBigBlind = 0;
        table->pot_money = players[nextPlayer].bet + players[nextPlayer + 1].bet;
        nextPlayer++;
        bigBlind = nextPlayer + 1;
        prevPlayer = nextPlayer - 1;
        if (nextPlayer == num_player) {
            nextPlayer = 0;
        }
        if (prevPlayer == 0) {
            prevPlayer = num_player - 1;
        }
    }
    dealStartingHand(players, deck, num_player);
    for (int roundIdx = 0; roundIdx < 4; roundIdx++) {
        roundPoker(players, table, deck, num_player, roundIdx, bigBlind);
        printf("-------End round--------");
        int count = 0;
        for (int i = 0; i < num_player; i++) {
            if (players[i].status == 0) {
                count++;
            }
        }
        if (count == num_player - 1) {
            printf("%i", roundIdx);
            break;
        }
    }
}

void testHand(Hand *hands, Player * players, int num_player) {
    sortHand(hands, num_player);
    for (int i = 0; i < num_player; i++) {
        printf("%s: ", players[i].name);
        for (int j = 0; j < 7; j++) {
            printf("%s %i; ", getSuit(hands[i].card[j].suit), hands[i].card[j].rank);
        }
        if (isRoyalStraightFlush(hands[i], &players[i])) {
            printf("Player %i has royal straight flush.\n", i + 1);
        } else if (isStraightFlush(hands[i], &players[i])) {
            printf("Player %i has straight flush.\n", i + 1);
        } else if (is4OfAKind(hands[i], &players[i])) {
            printf("Player %i has four of a kind.\n", i + 1);
        } else if (isFullHouse(hands[i], &players[i])) {
            printf("Player %i has a fullhouse.\n", i + 1);
        } else if (isFlush(hands[i], &players[i])) {
            printf("Player %i has a flush.\n", i + 1);
        } else if (isStraight(hands[i], &players[i])) {
            printf("Player %i has a straight.\n", i + 1);
        } else if (is3OfAKind(hands[i], &players[i])) {
            printf("Player %i has three of a kind.\n", i + 1);
        } else if (is2Pair(hands[i], &players[i])) {
            printf("Player %i has double pair.\n", i + 1);
        } else if (isPair(hands[i], &players[i])) {
            printf("Player %i has a pair.\n", i + 1);
        } else {
            isHighCard(hands[i], &players[i]);
            printf("Player %i has high card.\n", i + 1);
        }
        printf("Player %i max hand: ", i + 1);
        for (int j = 0; j < 5; j++) {
            printf("%s %i; ", getSuit(players[i].max_hand[j].suit), players[i].max_hand[j].rank);
        }
        printf("\n");
    }
}

void mainMenu(){
    printf("----------POKER----------\n1. Start game\n2. Options\n3. Exit\nYour option: ");
}

int main() {
    int opt = 0;
    int opt1 = 0;
    int opt2 = 0;
    for (int a = 0; ;a++) {
        mainMenu();
        scanf("%d", &opt);
        if (opt == 1) {
            // Create table
            Table *table = createTable();

            // Create deck
            Deck *deck;
            deck = newDeck();
            int size = 52;

            // Test new deck
            for (int m = 0; m < size; m++) {
                printf("%s %i; ", getSuit(deck->cards[m].suit), deck->cards[m].rank);
            }
            printf("\n");

            // Shuffle the deck
            shuffleDeck(deck, size);

            // Test shuffle
            for (int m = 0; m < size; m++) {
                printf("%s %i; ", getSuit(deck->cards[m].suit), deck->cards[m].rank);
            }
            printf("\n");

            // Create players
            int num_player = 3;
            Player *players = createPlayers(num_player);
            game(players, table, deck, num_player, a);

            // Free everything
            free(players);
            free(deck);
            free(table);
        } else if (opt == 2) {
            printf("Options\n");
        } else if (opt == 3) {
            break;
        }


/*---------------------------------------------------
    // Deal hole cards for players
    dealStartingHand(players, deck, num_player);

    // Deal shared cards
    dealSharedCards(table, deck, 1);
    dealSharedCards(table, deck, 2);
    dealSharedCards(table, deck, 3);

    // Test player starting hand
    for (int i = 0; i < num_player; i++) {
        printf("%s: ", players[i].name);
        for (int j = 0; j < 2; j++) {
            printf("%s %i; ", getSuit(players[i].hand[j].suit), players[i].hand[j].rank);
        }
        printf("\n");
    }

    // Test shared cards
    printf("Shared Cards: ");
    for (int i = 0; i < 5; i++) {
        printf("%s %i; ", getSuit(table->card[i].suit), table->card[i].rank);
    }
    printf("\n");

    // Test hands
    Hand *hands = createHand(players, table, num_player);
    testHand(hands, players, num_player)

//    Player *test_player = malloc(sizeof(Player));
//    test_player->max_hand = malloc(sizeof(Card) * 5);
//    Hand *test = malloc(sizeof(Hand));
//    test->card[0].rank = 13; test->card[0].suit = HEARTS;
//    test->card[1].rank = 13; test->card[1].suit = HEARTS;
//    test->card[2].rank = 13; test->card[2].suit = HEARTS;
//    test->card[3].rank = 12; test->card[3].suit = HEARTS;
//    test->card[4].rank = 10; test->card[4].suit = HEARTS;
//    test->card[5].rank = 10; test->card[5].suit = DIAMONDS;
//    test->card[6].rank = 2; test->card[6].suit = HEARTS;
//    for (int j = 0; j < 7; j++) {
//        printf("%s %i; ", getSuit(test->card[j].suit), test->card[j].rank);
//    }
//    if (isFullHouse(*test, test_player)) {
//        printf("True");
//    } else {printf("False");}
//    printf("\n%d\n", test_player->rank);
//    for (int j = 0; j < 5; j++) {
//        printf("%s %i; ", getSuit(test_player->max_hand[j].suit), test_player->max_hand[j].rank);
//    }
-------------------------------------------*/



//    free(hands);
    }
//    free(test);
//    free(test_player->max_hand);
//    free(test_player);

    return 0;
}

