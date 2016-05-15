#ifndef POKER_UI
#define POKER_UI

/** draw a card
  * @param card card
  * @param y position based on y axis
  * @param x position based on x axis
  */
void drawCard(Card card, int y, int x);

/** display a string in the center of the line
  * @param row position based on y axis
  * @param title a string
  */
void center(int row, char *title);

/** display credit
  */
void credit();

/** draw players' name, if they are Big Blind or Small Blind and 2 cards of active players
  * @param players all active players
  * @param table table
  * @param num_player number of players (include folded player)
  */
void drawPlayerCards(Player * players, int num_player, Table table);

/** draw players' name, their money, bet and state
  * @param players all players
  * @param num_player number of players (include folded player)
  * @param playerIdx index of player who currently takes a turn
  */
void drawPlayerInfo(Player * players, int num_player, int playerIdx);

/** draw rank of active players and the announcement about the winner
  * @param players all players
  * @param num_player number of players (include folded player)
  * @param table table
  * @param roundIdx round index
  */
void drawWinner(Player * players, int num_player, Table table, int roundIdx);

/** draw pot
  * @param table table
  */
void drawPot(Table table);

/** draw shared card
  * @param table table
  * @param roundIdx round index
  */
void drawSharedCard(Table table, int roundIdx);

/** draw the money that player wants to raise/bet
  * @param min minimum money that player needs to raise/bet
  * @param max maximum money that player can raise/bet
  * @param item where the player currently chooses
  * @param money money that player wants to raise/bet
  */
void drawRangeMoney(int min, int max, int item, int money);

/** let the player choose item by using arrow keys
  * @param min minimum money that player needs to raise/bet
  * @param max maximum money that player can raise/bet
  * @param item where the player currently chooses
  * @param money money that player wants to raise/bet
  */
int interactRangeMoney(int min, int max, int money, int item);

/** increase/decrease the money based on the player's choice
  * @param min minimum money that player needs to raise/bet
  * @param max maximum money that player can raise/bet
  */
int rangeMoney(int min, int max);

/** draw players' cards and info, shared card, pot
  * @param players all players
  * @param num_player number of players (include folded player)
  * @param table table
  * @param roundIdx round index
  * @param playerIdx player index
  */
void updateInfo(Player * players, int num_player, Table table, int roundIdx, int playerIdx);

/** draw players' cards and info, table info, shared card, and option menu which displays options that players can choose
  * @param num_player number of players
  * @param roundIdx round index
  * @param players all players
  * @param table table
  * @param playerIdx player index
  * @param item where the player currently chooses
  */
void drawGame(int num_player, int roundIdx, Player * players, Table * table, int playerIdx, int item);

/** let the player choose options by using arrow keys
  * @param num_player number of players
  * @param roundIdx round index
  * @param players all players
  * @param table table
  * @param playerIdx player index
  */
int interactGame(int num_player, int roundIdx, Player * players, Table * table, int playerIdx);

/** where the player choose options
  * @param players all players
  * @param table table
  * @param roundIdx round index
  * @param playerIdx player index
  * @param num_player number of players
  * @param deck deck
  * @param countCheck count the number of players check in a round
  * @param countAllin count the number of players allin in a round
  * @param countCall count the number of players call in a round
  * @param is_1st_bet check if this is the first turn of players in a round
  */
int turn(Player * players, Table * table, int roundIdx, int playerIdx, int num_player, Deck * deck,
         int countCheck, int countAllin, int countCall, int is_1st_bet);

/** deal shared cards, set some condition to let player choose options and to end round
  * @param players all players
  * @param table table
  * @param deck deck
  * @param num_player number of players
  * @param roundIdx round index
  * @param countActivePlayer number of active players (who did not fold)
  */
int roundPoker(Player *players, Table *table, Deck *deck, int num_player, int roundIdx, int countActivePlayer);

/** set Big Blind and Small Blind, deal players' cards, set some condition to start game and end game
  * @param players all players
  * @param table table
  * @param deck deck
  * @param num_player number of players
  * @param nextSB next Small Blind
  * @param roundIdx round index
  */
int game (Player * players, Table * table, Deck * deck, int num_player, int nextSB, int roundIdx);

/** create all the object needed to start a game, check if it is able to restart a game or the player needs to start a new game
  * @param num_player number of players
  * @param isLoad check if the player choose load
  */
void setUpGame(int num_player, int isLoad);

/** draw start menu
  * @param item where the player currently chooses
  * @param num_player number of players
  */
void drawStartMenu(int item, int num_player);

/** let the player choose the number of players, start game or back to main menu by using arrow keys
  * @param item where the player currently chooses
  * @param num_player number of players
  */
int interactStartMenu(int num_player, int item);

/** execute player's choice when he interacts with start menu
  */
void startMenu();

/** draw main menu
  * @param item where the player currently chooses
  */
void drawMainMenu(int item);

int interactMainMenu();



#endif
