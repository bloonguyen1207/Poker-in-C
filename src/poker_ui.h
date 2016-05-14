#ifndef POKER_UI
#define POKER_UI

void drawCard(Card card, int y, int x);

void center(int row, char *title);

void drawPlayerCards(Player * players, int num_player, Table table);

void drawPlayerInfo(Player * players, int num_player, int playerIdx);

void drawPot(Table table);

void drawSharedCard(Table table, int roundIdx);

void drawRangeMoney(int min, int max, int item, int money);

int interactRangeMoney(int min, int max, int money, int item);

int roundPoker(Player *players, Table *table, Deck *deck, int num_player, int roundIdx, int countActivePlayer);

int rangeMoney(int min, int max);

void drawGame(int num_player, int roundIdx, Player * players, Table * table, int playerIdx, int item);

int interactGame(int num_player, int roundIdx, Player * players, Table * table, int playerIdx);

int turn(Player * players, Table * table, int roundIdx, int playerIdx, int num_player, Deck * deck);

int roundPoker(Player *players, Table *table, Deck *deck, int num_player, int roundIdx, int countActivePlayer);

int game (Player * players, Table * table, Deck * deck, int num_player, int gameIdx, int nextSB);

void setUpGame(int num_player);

void drawStartMenu(int item, int num_player);

int interactStartMenu(int num_player, int item);

void startMenu();

void drawMainMenu(int item);

int interactMainMenu();



#endif
