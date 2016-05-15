//
// Created by bloo on 18/04/2016.
//

#ifndef POKER
#define POKER

/**
 * These are 4 different suits in a deck
 */
enum suit {HEARTS, DIAMONDS, CLUBS, SPADES, NONE};
typedef enum suit Suit;

/**
 * These are 10 different ranks that players achieve if they play until the game ends
 */
enum rank {HighCard, OnePair, TwoPairs, Three, Four, Straight, Flush, FullHouse, StraightFlush, RoyalFlush};
typedef enum rank Rank;

/**
 * The first 7 states is the state of players after they chose options in their turn
 * SB and BB are SmallBlind and BigBlind respectively and are assigned to players at the beginning of each game.
 */
enum state {None, Called, Raised, Checked, Bets, Allins, Folded, SB, BB};
typedef enum state State;

/**
 * card struct.
 */
struct card {
    Suit suit;
    int rank;
};
typedef struct card Card;

/**
 * deck struct.
 */
struct deck {
    Card cards[52]; //a deck has 52 cards
    int card_index; //start with 0, the card_index increases by 1 each time one card was dealt
};
typedef struct deck Deck;

/**
 * player struct.
 */
struct player {
    char name[20];
    int money;
    int bet; //total bet of player in one game
    int status; //0 if the player chose to fold, and 1 otherwise (active player)
    Card hand[2]; //each player was dealt with 2 cards at the beginning of the game
    Card *max_hand; //among 7 cards (2 from player and 5 from the table), choose 5 cards that can create a highest player's rank
    Rank rank; //player's rank
    State state; //option that player chose in each turn
    int isBigBlind; //1 if the player is big blind, and 0 otherwise
    int isSmallBlind; //1 if the player is small blind, and 0 otherwise
    int isWinner; //1 if the player is a winner, and 0 otherwise
};
typedef struct player Player;

/**
 * table struct.
 */
struct table {
    int pot_money; //total of player's bet in a game
    int ante; //the first bet of small blind
    int highest_bet; //in one round, find the player that has the highest bet and store that number
    Card card[5]; //shared card
    int card_idx; //start from 0, increases by 1 each time one shared card was dealt
    int last_bet; //in one round, when the player chose to raise of bet, last_bet is the amount of money he spent for that choice
    int showCard; //if all players allin or at the end of the game, showCard is 1, and 2 cards of active players will turn face up
    int isLoad; //check if the player choose load
};
typedef struct table Table;

/**
 * hand struct.
 */
struct hand {
    Card card[7]; //hands contain 2 cards of players and 5 shared cards on table
};
typedef struct hand Hand;

/** instead of numbers, get a unicode string corresponding to Suit
  * @param s suit
  */
char* getSuit(Suit s);

/** instead of numbers, get a string corresponding to State
  * @param s state
  */
char* getState(State s);

/** instead of numbers, get a string corresponding to Player's rank
  * @param r player's rank
  */
char* getRank(Rank r);

/** instead of numbers, get a string corresponding to Card's rank
  * @param r card's rank
  */
char* getCardRank(int rank);

/** create a deck
  * The deck will be allocated on the heap.
  */
Deck* newDeck();

/** shuffle the deck
  * @param deck the deck
  * @param size number of cards in the deck
  * Algorithms URL: http://www.programming-algorithms.net/article/43676/Fisher-Yates-shuffle
  */
void shuffleDeck(Deck* deck, int size);

/** create players based on the given number of players
  * Players will be allocated on the heap.
  * @param num_player the number of players
  */
Player* createPlayers(int num_player);

/** create a table
  * table will be allocated on the heap.
  */
Table * createTable();

/** deal 2 cards to players
  * @param players all players
  * @param deck the deck
  * @param num_player the number of players
  */
void dealStartingHand(Player*players, Deck* deck, int num_player);

/** deal shared cards on the table
  * @param table the table
  * @param deck the deck
  * @param roundIdx start from round 0 and end with round 3
  */
void dealSharedCards(Table *table, Deck* deck, int roundIdx);

/** create hands which contains 2 cards of players and 5 cards on the table
  * the number of hands created is based on the number of players
  * Hands will be allocated on the heap.
  * @param players all players
  * @param table the table
  * @param num_player the number of players
  */
Hand * createHands(Player *players, Table *table, int num_player);

/** swap two cards of the hand which corresponds to the given player index
  * @param hand hand
  * @param playerIdx player index
  * @param fstIdx index of the first card
  * @param secIdx index of the second card
  */
void swapCards(Hand* hand, int playerIdx, int fstIdx, int secIdx);

/** sort cards of hands from the highest card's rank (A or 1) to smallest (2)
  * @param hands hands
  * @param num_player number of players
  */
void sortHands(Hand *hands, int num_player);

/** check in the hand if there is any card that has the given rank
  * @param hand hand
  * @param rank card's rank
  */
int searchHandRank(Hand hand, int rank);

/** find and return the card that has the given rank in the hand
  * @param hand hand
  * @param rank card's rank
  */
Card searchCard(Hand hand, int rank);

/** store 5 cards of the hand that have the highest rank to player's max_hand and change the rank of player to HighCard
  * assume that the hand has been sorted, therefore
  * @param hand hand
  * @param player player
  */
void isHighCard(Hand hand, Player* player);

/** if the hand has a pair, store that pair and 3 other cards that have highest rank to player's max_hand
  * and change the rank of player to OnePair
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int isPair(Hand hand, Player* player);

/** if the hand has 2 pairs, store those pairs and another card that has highest rank to player's max_hand
  * and change the rank of player to TwoPairs
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int is2Pair(Hand hand, Player*player);

/** if the hand has 3 cards with the same rank, store these cards and 2 other cards that has highest rank to player's max_hand
  * and change the rank of player to Three
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int is3OfAKind(Hand hand, Player*player);

/** if the hand has 4 cards with the same rank, store these cards and another card that has highest rank to player's max_hand
  * and change the rank of player to Four
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int is4OfAKind(Hand hand, Player*player);

/** if the hand has 3 cards with the same rank and a pair, store these cards to player's max_hand
  * and change the rank of player to FullHouse
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int isFullHouse(Hand hand, Player*player);

/** if the hand has 5 cards whose rank increase by 1, store these cards to player's max_hand
  * and change the rank of player to Straight
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int isStraight(Hand hand, Player*player);

/** if the hand has 5 cards with the same suit, store these cards to player's max_hand
  * and change the rank of player to Flush
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int isFlush(Hand hand, Player* player);

/** if the hand has 5 cards with the same suit and the rank of each increase by 1, store these cards to player's max_hand
  * and change the rank of player to StraightFlush
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int isStraightFlush(Hand hand, Player* player);

/** if the hand has 5 cards with the same suit and the rank of each are A K Q J 10, store these cards to player's max_hand
  * and change the rank of player to RoyalFlush
  * assume that the hand has been sorted
  * @param hand hand
  * @param player player
  */
int isRoyalFlush(Hand hand, Player *player);

/** after the player chose their option, this function will increase the bet of player and pot on table, decrease player's money
  * @param player player
  * @param table table
  * @param money the money spent for that option
  */
void updateMoney(Player *player, Table * table, int money);

/** check if the player's need to All in
  * @param player player
  * @param table table
  */
int isAllin(Player player, Table table);

/** check if the player's can call and raise
  * @param player player
  * @param table table
  */
int isCallRaise(Player player, Table table);

/** check if the player's can check and bet
  * @param player player
  * @param table table
  */
int isCheckBet(Player player, Table table);

/** change player's state to Allins and updateMoney
  * @param player player
  * @param table table
  */
void allin(Player *player, Table * table);

/** change player's state to Called and updateMoney
  * @param player player
  * @param table table
  */
void call(Player *player, Table * table);

/** change player's state to Raised and updateMoney
  * @param player player
  * @param table table
  * @param money money spent
  */
void raisePoker(Player *player, Table *table, int money);

/** change player's state to Checked
  * @param player player
  */
void check(Player * player);

/** change player's state to Bet and updateMoney
  * @param player player
  * @param table table
  * @param money money spent
  */
void bet(Player * player, Table * table, int money);

/** change player's state to Folded and status to 0
  * @param player player
  */
void fold(Player * player);

/** calculate the minimum money needed to spent if the player chose to raise or bet
  * @param player player
  * @param table table
  */
int minMoney(Player player, Table table);

/** check the combination of AI player's cards and shared cards is at which rank
  * @param player player
  * @param hand hand
  */
void checkHandRanking(Hand * hand, Player * player);

/** set how the aggressive AI plays in the first round (roundIdx 0)
  * @param player player
  * @param table table
  */
int aggrAIround0(Player *ai, Table *table);

/** set how the aggressive AI plays in other rounds (except for the first round - roundIdx 0)
  * @param player player
  * @param table table
  */
int aggrAIrounds(Player *ai, Table *table);

/** set how the aggressive AI plays
  * @param player player
  * @param table table
  * @param roundIdx round index
  */
int aggrAI(Player *ai, Table *table, int roundIdx);

/** set how the conservative AI plays in the first round (roundIdx 0)
  * @param player player
  * @param table table
  */
int consAIround0(Player *ai, Table * table);

/** set how the conservative AI plays in other rounds (except for the first round - roundIdx 0)
  * @param player player
  * @param table table
  */
int consAIrounds(Player *ai, Table * table);

/** set how the conservative AI plays
  * @param player player
  * @param table table
  * @param roundIdx round index
  */
int consAI (Player *ai, Table *table, int roundIdx);

/** save all information of the game into text file
  * @param players all players
  * @param table table
  * @param deck deck
  * @param num_player number of players
  * @param round_index round index
  * @param player_index player index
  * @param countCheck count the number of players check in a round
  * @param countAllin count the number of players allin in a round
  * @param countCall count the number of players call in a round
  * @param is_1st_bet check if this is the first turn of players in a round
  */
int save(Player *players, Table * table, Deck * deck, int num_player, int round_index, int player_index,
         int countCheck, int countAllin, int countCall, int is_1st_bet);

/** load number of players
  * @param num_player number of players
  */
int loadNumPlayer(int * num_player);

/** load the information of round - the number of players check / allin / call and if this is the first turn
  * @param countCheck count the number of players check in a round
  * @param countAllin count the number of players allin in a round
  * @param countCall count the number of players call in a round
  * @param is_1st_bet check if this is the first turn of players in a round
  */
void loadRoundInfo(int * countCheck, int * countAllin, int * countCall, int * is_1st_bet);

/** load information of table, deck, players and round index
  * @param players all players
  * @param table table
  * @param deck deck
  * @param round_index round index
  * @param player_index player index
  */
void load(Table * table, Deck * deck, Player * players, int * round_index);

/** find the winner
  * @param players all players
  * @param num_player number of players
  */
void checkWinner(Player * players, int num_player);

/** check the rank of each players and find the winner
  * at first, players are considered as winners and after checking, they might be changed to losers
  * after checking, players who still be considered as winners are actually become the winners of the game
  * @param players players that have the same highest rank compared to other players
  * @param num_player number of players that have the same highest rank compared to other players
  */
void testHand(Hand *hands, Player * players, int num_player);

/** split pot of table and award the money to the winners
  * @param players all players
  * @param table table
  * @param num_player number of players
  */
void award(Player * players, Table * table, int num_player);

/** reset all the info to start new game
  * @param players all players
  * @param table table
  * @param num_player number of players
  * @param deck deck
  */
void reset (Player * players, Table * table, int num_player, Deck * deck);

#endif
