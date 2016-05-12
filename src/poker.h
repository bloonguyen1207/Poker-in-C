//
// Created by bloo on 18/04/2016.
//

#ifndef POKER
#define POKER
enum suit {HEARTS, DIAMONDS, CLUBS, SPADES, NONE};
typedef enum suit Suit;

enum rank {HighCard, OnePair, TwoPairs, Three, Four, Straight, Flush, FullHouse, StraightFlush, RoyalFlush};
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
    State state;
    int isBigBlind;
    int isSmallBlind;
    Option option;
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
    int showCard;
};
typedef struct table Table;

struct hand {
    Card card[7];
};
typedef struct hand Hand;

char* getSuit(Suit s);

char* getState(State s);

char* getRank(Rank r);

Deck* newDeck();

void shuffleDeck(Deck* deck, int size);

Player* createPlayers(int num_player);

void displayPlayerInfo(Player player);

Table * createTable();

void displayTableInfo(Table table);

void dealStartingHand(Player*players, Deck* deck, int num_player);

void dealSharedCards(Table *table, Deck* deck, int time);

Hand* createHand(Player *players, Table *table, int num_player);

void swapCards(Hand* hand, int player, int fstIdx, int secIdx);

void sortHand(Hand * hands, int num_player);

int searchHandRank(Hand hand, int rank);

Card searchCard(Hand hand, int rank);

Card isHighestCard(Hand hand);

void isHighCard(Hand hand, Player* player);

int isPair(Hand hand, Player* player);

int is2Pair(Hand hand, Player*player);

int is3OfAKind(Hand hand, Player*player);

int is4OfAKind(Hand hand, Player*player);

int isFullHouse(Hand hand, Player*player);

int isStraight(Hand hand, Player*player);

int isFlush(Hand hand, Player* player);

int isStraightFlush(Hand hand, Player* player);

int isRoyalStraightFlush(Hand hand, Player* player);

int scanInput(int num_option);

void updateMoney(Player *player, Table * table, int money);

void displayRangeMoney(int min, int max);

int inputMoney(int min, int max);

int isAllin(Player player, Table table);

int isCallRaise(Player player, Table table);

int isCheckBet(Player player, Table table);

void allin(Player *player, Table * table);

void call(Player *player, Table * table);

void raisePoker(Player *player, Table *table, int money);

void check(Player * player);

void bet(Player * player, Table * table, int money);

void fold(Player * player);

int minMoney(Player player, Table table);

void checkHandRanking(Hand * hand, Player * player);

int aggrAIround0(Player *ai, Table *table);

int aggrAIrounds(Player *ai, Table *table);

int aggrAI(Player *ai, Table *table, int roundIdx);

int consAIround0(Player *ai, Table * table);

int consAIrounds(Player *ai, Table * table);

int consAI (Player *ai, Table *table, int roundIdx);

void checkWinner(Player * players, int num_player);

void testHand(Hand *hands, Player * players, int num_player);

void award(Player * players, Table * table, int num_player);

void reset (Player * players, Table * table, int num_player, Deck * deck);

void mainMenu();

#endif
