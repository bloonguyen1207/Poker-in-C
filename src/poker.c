//CREDITS
//COSC2174 - Programming Techniques
//Semester A - 2016
//Lecturer's Name: Denis Rinfret
//Student's ID - name:
//s3558475 - Nguyen Ha Phuong Mai
//s3515305 - Nguyen Thi Mai Thao

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "poker.h"

char* getSuit(Suit s) {
    switch(s){
        case HEARTS: return "\u2665";
        case DIAMONDS: return "\u2666";
        case CLUBS: return "\u2663";
        case SPADES: return "\u2660";
        default: return NULL;
    }
}

char* getState(State s) {
    switch(s){
        case Called: return "Call";
        case Raised: return "Raise";
        case Checked: return "Check";
        case Allins: return "Allin";
        case Folded: return "Fold";
        case Bets: return "Bets";
        case BB: return "BigBlind";
        case SB: return "SmallBlind";
        default: return "None";
    }
}

char* getRank(Rank r) {
    switch(r){
        case OnePair: return "OnePair";
        case TwoPairs: return "TwoPairs";
        case Three: return "3OfAKind";
        case Four: return "4OfAKind";
        case Straight: return "Straight";
        case Flush: return "Flush";
        case FullHouse: return "FullHouse";
        case StraightFlush: return "StraightFlush";
        case RoyalFlush: return "RoyalFlush";
        default: return "HighCard";
    }
}

char* getCardRank(int rank) {
    switch(rank){
        case 13: return "K";
        case 12: return "Q";
        case 11: return "J";
        case 10: return "10";
        case 9: return "9";
        case 8: return "8";
        case 7: return "7";
        case 6: return "6";
        case 5: return "5";
        case 4: return "4";
        case 3: return "3";
        case 2: return "2";
        case 1: return "A";
        default: return NULL;
    }
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
            players[i].state = None;
            players[i].name[7] += i + 1;
            players[i].money = 300;
            players[i].max_hand = malloc(sizeof(Card) * 5);
            players[i].bet = 0;
            players[i].isBigBlind = 0;
            players[i].isSmallBlind = 0;
            players[i].isWinner = 0;
        }
        return players;
    }
    return NULL;
}

Table * createTable() {
    Table * table = malloc(sizeof(Table));
    table->card_idx = 0;
    table->pot_money = 0;
    table->last_bet = 0;
    table->showCard = 0;
    table->ante = 2;
    table->highest_bet = table->ante * 2;
    table->isLoad = 0;
    return table;
}

void dealStartingHand(Player*players, Deck* deck, int num_player) {
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < num_player; i++) {
            if (players[i].money <= 0) {
                continue;
            }
            players[i].hand[j] = deck->cards[deck->card_index];
            deck->card_index++;
        }
    }
}

void dealSharedCards(Table *table, Deck* deck, int roundIdx){
    //1st round: deal 3 cards: table_last_card_idx = 2
    //2nd round: deal 1 card:  table_last_card_idx = 3
    //3rd round: deal 1 card:  table_last_card_idx = 4 --> table_last_card_idx = roundIdx + 1
    for (; table->card_idx <= roundIdx + 1; table->card_idx++) {
        table->card[table->card_idx] = deck->cards[deck->card_index];
        deck->card_index++;
    }
};

Hand * createHands(Player *players, Table *table, int num_player) {
    Hand * hands = malloc(sizeof(Hand) * num_player);
    //add player's cards to hand[0], [1]
    for (int i = 0; i < num_player; i++) {
        for (int j = 0; j < 2; j++) {
            hands[i].card[j] = players[i].hand[j];
        }
    }

    //add shared cards to hand[2] -> hand[6]
    for (int i = 0; i < num_player; i++) {
        for (int j = 0; j < 5; j++) {
            hands[i].card[j + 2] = table[0].card[j];
        }
    }
    return hands;
}

void swapCards(Hand* hand, int playerIdx, int fstIdx, int secIdx) {
    Card temp = hand[playerIdx].card[fstIdx];
    hand[playerIdx].card[fstIdx] = hand[playerIdx].card[secIdx];
    hand[playerIdx].card[secIdx] = temp;
}

void sortHands(Hand *hands, int num_player) {
    //if there are Ace cards, the ace cards will swap with other cards to be at the beginning of the hand
    for (int i = 0; i < num_player; i++) {
        int idx = 0;
        for (int j = 0; j < 7; j++) {
            if (hands[i].card[j].rank == 1) {
                swapCards(hands, i, idx, j);
                idx++;
            }
        }
    }

    //except for Ace cards (rank == 1), sort the hand from highest card's rank to lowest
    for (int i = 0; i < num_player; i++) {
        Card max_rank_card;
        for (int j = 0; j < 7; j++) {
            max_rank_card.rank = hands[i].card[j].rank;
            if (max_rank_card.rank == 1) { //have checked Ace already, do not touch Ace again
                continue;
            };
            for (int k = j+1; k < 7; k++) {
                if (hands[i].card[k].rank > max_rank_card.rank) {
                    max_rank_card.rank = hands[i].card[k].rank;
                    swapCards(hands, i, j, k);
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

//the hand must be sorted before checking
void isHighCard(Hand hand, Player* player) {
    for (int i = 0; i < 5; i++) {
        player->max_hand[i] = hand.card[i];
    }
    player->rank = HighCard;
}

//the hand must be sorted before checking
int isPair(Hand hand, Player* player){
    int exist= 0;
    int idx = 2;
    // Find 2 cards that has same rank
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

//the hand must be sorted before checking
int is2Pair(Hand hand, Player*player) {
    int count = 0;
    int exist[2] = {0, 0};
    int idx = 0;
    int ext_idx = 0;

    // Find 2 card with same rank
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
            // Check 2 different pairs
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

//the hand must be sorted before checking
int is3OfAKind(Hand hand, Player*player) {
    int exist = 0;
    int idx = 3;
    // Find 3 cards with same rank
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

//the hand must be sorted before checking
int is4OfAKind(Hand hand, Player*player) {
    int exist = 0;
    // Find 4 cards with same rank
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

//the hand must be sorted before checking
int isFullHouse(Hand hand, Player*player) {
    int exist = 0;
    int count = 0;
    // Check 3 of a kind
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
        // Find pair
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

// Card hand must be sorted before checking
int isStraight(Hand hand, Player*player) {
    //the algorithms here is starting from the first card, search if there are enough 5 cards in the hand that can create a straight
    //therefore, only need to check until the third card because the straight requires 5 cards and the hand has 7 cards
    for (int j = 0; j < 3; j++) {
        int check = 0;
        Card temp = hand.card[j];
        player->max_hand[check] = searchCard(hand, temp.rank);
        check++;
        if (temp.rank == 1) {
            //check if there is a A K Q J 10 straight
            for (int k = 13; k >= 10; k--) {
                if (searchHandRank(hand, k)) {
                    player->max_hand[check] = searchCard(hand, k);
                    check++;
                } else {
                    break;
                }
            }
            if (check == 5) {
                player->rank = Straight;
                return 1;
            }
            check = 0;
            //check if there is 5 4 3 2 1 straight
            //if the straight is 5 4 3 2 1, Ace will be placed at the end of max_hand
            for (int k = 5; k >= 2; k--) {
                if (searchHandRank(hand, k)) {
                    player->max_hand[check] = searchCard(hand, k);
                    check++;
                } else {
                    break;
                }
            }
            if (check == 4) {
                player->max_hand[check] = searchCard(hand, 1);
                player->rank = Straight;
                return 1;
            }
        }

        check = 1;
        //find a straight and add to max_hand
        for (int k = 1; k < 5; k++) {
            if (temp.rank - k > 0 && searchHandRank(hand, temp.rank - k)) {
                player->max_hand[check] = searchCard(hand, temp.rank - k);
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

// Card hand must be sorted before checking
int isFlush(Hand hand, Player* player) {
    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        int count = 0; //count number of cards that have the same suit
        for (int j = 0; j < 7; j++) {
            if (hand.card[j].suit == suit && hand.card[j].rank > 0) {
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
            if (hand.card[j].suit == suit && hand.card[j].rank > 0) {
                temp->card[count] = hand.card[j];
                count++;
            }
        }

        //check if these same suit cards are a straight
        if (count >= 5 && isStraight(*temp, player)) {
            free(temp);
            player->rank = StraightFlush;
            return 1;
        }
        free(temp);
    }
    return 0;
}

//the hand must be sorted before checking
int isRoyalFlush(Hand hand, Player *player) {
    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        Hand* temp = malloc(sizeof(Hand));
        int count = 0; //count number of cards that have the same suit
        int check = 0;
        for (int j = 0; j < 7; j++) {
            if (hand.card[j].suit == suit && hand.card[j].rank > 0) {
                temp->card[count] = hand.card[j];
                count++;
            }
        }
        if (count >= 5) {
            if (searchHandRank(*temp, 1)) {
                player->max_hand[check] = searchCard(*temp, 1); //add card to max_hand
                check++;
            }
            for (int i = 13; i >= 10; i--) {
                if (searchHandRank(*temp, i)) {
                    player->max_hand[check] = searchCard(*temp, i); //add card to max_hand
                    check++;
                }
            }
            if (check == 5) {
                free(temp);
                player->rank = RoyalFlush;
                return 1;
            }
        }
        free(temp);
    }
    return 0;
}

void updateMoney(Player *player, Table * table, int money) {
    player->money = player->money - money;
    player->bet += money;
    table->pot_money += money;
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
        updateMoney(player, table, player->money);
        player->state = Allins;
    }
}

void call(Player *player, Table * table) {
    if (isAllin(*player, *table)) {
        allin(player, table);
    } else if (player->money >= table->highest_bet - player->bet) {
        updateMoney(player, table, table->highest_bet - player->bet);
        player->state = Called;
    }
}

void raisePoker(Player *player, Table *table, int money) {
    int call_money = table->highest_bet - player->bet;
    if (isAllin(*player, *table)) {
        allin(player, table);
    } else if (player->money >= money && money >= call_money + table->last_bet ) {
        updateMoney(player, table, money);
        player->state = Raised;
    }
}

void check(Player * player) {
    player->state = Checked;
}

void bet(Player * player, Table * table, int money) {
    if (isAllin(*player, *table)) {
        allin(player, table);
    } else if (money >= table->ante * 2 && player->money >= money) {
        updateMoney(player, table, money);
        player->state = Bets;
    }
}

void fold(Player * player) {
    player->status = 0;
    player->state = Folded;
}

int minMoney(Player player, Table table) {
    if (isCallRaise(player, table)) {
        return table.highest_bet - player.bet + table.last_bet;
    } else if (isCheckBet(player, table)) {
        return table.ante * 2;
    }
    return 0;
}

void checkHandRanking(Hand * hand, Player * player) {
    sortHands(hand, 1);
    if (isRoyalFlush(*hand, player)) {
    } else if (isStraightFlush(*hand, player)) {
    } else if (is4OfAKind(*hand, player)) {
    } else if (isFullHouse(*hand, player)) {
    } else if (isFlush(*hand, player)) {
    } else if (isStraight(*hand, player)) {
    } else if (is3OfAKind(*hand, player)) {
    } else if (is2Pair(*hand, player)) {
    } else if (isPair(*hand, player)) {
    } else {
        isHighCard(*hand, player);
    }
}

int aggrAIround0(Player *ai, Table *table) {
    //Aggressive AI will raise/bet if their 2 cards are > 10 / a pair / has Ace
    if (ai->hand[0].rank == ai->hand[1].rank ||
        (ai->hand[0].rank >= 10 && ai->hand[1].rank >= 10) ||
        ai->hand[0].rank == 1 ||
        ai->hand[1].rank == 1) {
        if (isCallRaise(*ai, *table)) {
            int money = minMoney(*ai, *table);
            if (ai->money >= money * 25) {
                raisePoker(ai, table, money * 5);
                table->last_bet = money;
                return 1;
            }
        } else if (isCheckBet(*ai, *table)) {
            int money = minMoney(*ai, *table);
            if (ai->money >= money * 25) {
                bet(ai, table, money * 5);
                table->last_bet = money;
                return 3;
            }
        }
    }

    //Aggressive AI will fold if their 2 cards are not the same suit and clearly can not combine to create a straight
    if (abs(ai->hand[0].rank - ai->hand[1].rank) > 5 && ai->hand[0].suit != ai->hand[1].suit) {
        if (ai->hand[0].rank < 11 && ai->hand[1].rank < 11) {
            fold(ai);
            return 5;
        }
    }

    //Otherwise, Aggressive AI will call/check
    if (isCallRaise(*ai, *table)) {
        if (ai->money >= minMoney(*ai, *table)) {
            call(ai, table);
            return 0;
        }
    } else if (isCheckBet(*ai, *table)) {
        check(ai);
        return 2;
    }
    fold(ai);
    return 5;

}

int aggrAIrounds(Player *ai, Table *table) {
    Hand * temp = malloc(sizeof(Hand));
    temp->card[0] = ai->hand[0];
    temp->card[1] = ai->hand[1];
    for (int i = 0; i < table->card_idx; i++) {
        temp->card[i+2] = table->card[i];
    }
    //if there are not enough 5 shared cards,
    //set the suit of empty cards to NONE and the rank to -1, -2 to avoid pair/flush/straight in checking rank
    if (table->card_idx >= 3) {
        if (table->card_idx == 3) {
            temp->card[5].rank = -1;
            temp->card[5].suit = NONE;
        }
        temp->card[6].rank = -2;
        temp->card[6].suit = NONE;
    }
    checkHandRanking(temp, ai);

    int money = minMoney(*ai, *table);
    if (ai->rank >= OnePair) {
        if (ai->rank >= Three) {
            if (isCallRaise(*ai, *table)) {
                raisePoker(ai, table, ai->money);
                table->last_bet = money;
                return 1;
            } else if (isCheckBet(*ai, *table)) {
                bet(ai, table, ai->money);
                table->last_bet = money;
                return 3;
            }
        }
        if (ai->money >= money * 25) {
            if (isCallRaise(*ai, *table)) {
                raisePoker(ai, table, money * 5);
                table->last_bet = money;
                return 1;
            } else if (isCheckBet(*ai, *table)) {
                bet(ai, table, money * 5);
                table->last_bet = money;
                return 3;
            }
        }
        if (isCallRaise(*ai, *table)) {
            call(ai, table);
            return 0;
        } else if (isCheckBet(*ai, *table)) {
            check(ai);
            return 2;
        }
    }

    if (ai->money >= money * 5) {
        if (isCallRaise(*ai, *table)) {
            call(ai, table);
            return 0;
        } else if (isCheckBet(*ai, *table)) {
            check(ai);
            return 2;
        }

    }
    fold(ai);

    free(temp);
    return 5;
}

int aggrAI(Player *ai, Table *table, int roundIdx) {
    int input;
    if (roundIdx == 0) {
        input = aggrAIround0(ai, table);
    } else {
        input = aggrAIrounds(ai, table);
    }
    return input;
}

int consAIround0(Player *ai, Table * table) {
    // If hand has ace or pair of ace or pair > 5 or cards rank > 6 then check or call
    if (ai->hand[0].rank == 1 || ai->hand[1].rank == 1 ||
        (ai->hand[0].rank == ai->hand[1].rank && (ai->hand[0].rank == 1 || ai->hand[0].rank > 5)) ||
        (ai->hand[0].rank > 6 && ai->hand[1].rank > 6)) {
        if (isCallRaise(*ai, *table)) {
            call(ai, table);
            return 0;
        } else if (isCheckBet(*ai, *table)) {
            check(ai);
            return 2;
        }
    }
    // If not fold
    fold(ai);
    return 5;
}

int consAIrounds(Player *ai, Table * table) {
    Hand * temp = malloc(sizeof(Hand));
    temp->card[0] = ai->hand[0];
    temp->card[1] = ai->hand[1];
    for (int i = 0; i < table->card_idx; i++) {
        temp->card[i+2] = table->card[i];
    }
    if (table->card_idx >= 3) {
        if (table->card_idx == 3) {
            temp->card[5].rank = -1;
            temp->card[5].suit = NONE;
        }
        temp->card[6].rank = -2;
        temp->card[6].suit = NONE;
    }
    checkHandRanking(temp, ai);
    // If after 2nd round hand does not have pair or more then fold
    if (ai->rank > 0) {
        if (isCallRaise(*ai, *table)) {
            call(ai, table);
            return 0;
        } else if (isCheckBet(*ai, *table)) {
            check(ai);
            return 2;
        }
    }
    fold(ai);
    return 5;
}

int consAI (Player *ai, Table *table, int roundIdx) {
    int input;
    if (roundIdx == 0) {
        input = consAIround0(ai, table);
    } else {
        input = consAIrounds(ai, table);
    }
    return input;
}

int save(Player *players, Table * table, Deck * deck, int num_player, int round_index, int player_index,
         int countCheck, int countAllin, int countCall, int is_1st_bet) {
    FILE *save_point;
    save_point = fopen("src/game.txt", "w+");
    if (save_point != NULL) {
        fprintf(save_point, "Round index\n%i\n----------\n", round_index);
        fprintf(save_point, "Player index\n%i\n----------\n", player_index);
        fprintf(save_point, "Number of player\n%i\n----------\n", num_player);
        fprintf(save_point, "Deck\n");
        for (int i = 0; i < 52; i++) {
            fprintf(save_point, "%i %i\n", deck->cards[i].suit, deck->cards[i].rank);
        }
        fprintf(save_point, "----------\nCount Check\n%d\n", countCheck);
        fprintf(save_point, "----------\nCount Allin\n%d\n", countAllin);
        fprintf(save_point, "----------\nCount Call\n%d\n", countCall);
        fprintf(save_point, "----------\nIs First Bet\n%d\n", is_1st_bet);
        fprintf(save_point, "----------\nTable\n");
        fprintf(save_point, "%i\n%i\n%i\n%i\n", table->pot_money, table->ante, table->highest_bet, table->last_bet);

        fprintf(save_point, "Shared cards\n");
        if (round_index == 0) {
            fprintf(save_point, "\n\n\n\n\n");
        } else if (round_index == 1) {
            for (int j = 0; j < 3; j++) {
                fprintf(save_point, "%i %i\n", table->card[j].suit, table->card[j].rank);
            }
            fprintf(save_point, "\n\n");
        } else if (round_index == 2) {
            for (int j = 0; j < 4; j++) {
                fprintf(save_point, "%i %i\n", table->card[j].suit, table->card[j].rank);
            }
            fprintf(save_point, "\n");
        } else if (round_index == 3) {
            for (int j = 0; j < 5; j++) {
                fprintf(save_point, "%i %i\n", table->card[j].suit, table->card[j].rank);
            }
        } else {
            fprintf(save_point, "Error\n");
        }
        fprintf(save_point, "----------\nPlayers\n");
        for (int i = 0; i < num_player; i++) {
            fprintf(save_point, "Player %i:\n%i\n%i\n%i\n%i %i\n%i %i\n%i\n%i\n", i + 1,
                    players[i].money, players[i].bet, players[i].state,
                    players[i].hand[0].suit, players[i].hand[0].rank,
                    players[i].hand[1].suit, players[i].hand[1].rank,
                    players[i].isSmallBlind, players[i].isBigBlind);
        }
        fclose(save_point);
        return 1;
    } else {
        return 0;
    }
}

int loadNumPlayer(int * num_player) {
    FILE * save;
    char line [20];
    int i = 0; // i: count line
    save = fopen("src/game.txt", "r");
    if (save != NULL) {
        while (fgets(line, 20, save) != NULL) {
            i++;
            // Load number of players
            if (i == 8) {
                *num_player = atoi(line);
                break;
            }
        }
        fclose(save);
        return 1;
    } else {
        return 0;
    }
}

void loadRoundInfo(int * countCheck, int * countAllin, int * countCall, int * is_1st_bet) {
    FILE * save;
    char line [20];
    int i = 0; // i: count line
    save = fopen("src/game.txt", "r");
    if (save != NULL) {
        while (fgets(line, 20, save) != NULL) {
            i++;
            // Load count check
            if (i == 65) {
                *countCheck = atoi(line);
            } else if (i == 68) {
                *countAllin = atoi(line);
            } else if (i == 71) {
                *countCall = atoi(line);
            } else if (i == 74) {
                *is_1st_bet = atoi(line);
                break;
            }
        }
        fclose(save);
    } else {
    }
}

void load(Table * table, Deck * deck, Player * players, int * round_index) {
    FILE * save;
    char line [20];
    char *s;
    int i = 0, j = 0, l = 0, m = 0, num_player = 0; // i: count line, j: deck size, l: share card size
    save = fopen("src/game.txt", "r");
    if (save != NULL) {
        while (fgets(line, 20, save) != NULL) {
            i++;

            // Load round index
            if (i == 2) {
                *round_index = atoi(line);
            }

            // Load number of Player
            if (i == 8) {
                num_player = atoi(line);
            }

            // Load deck
            if (i >= 11 && i <= 62) {
                s = strtok(line, " ");
                int k = 0;
                while (s != NULL) {
                    k++;
                    if (j < 52) {
                        if (k == 1) {
                            if (atoi(s) == 0) {
                                deck->cards[j].suit = HEARTS;
                            } if (atoi(s) == 1) {
                                deck->cards[j].suit = DIAMONDS;
                            } if (atoi(s) == 2) {
                                deck->cards[j].suit = CLUBS;
                            } else if (atoi(s) == 3) {
                                deck->cards[j].suit = SPADES;
                            }
                        } else if (k == 2) {
                            deck->cards[j].rank = atoi(s);
                        }
                    }
                    s = strtok(NULL, " ");
                }
                j++;
            }

            // Load table
            if (i >= 77 && i <= 80) {
                if (i == 77) {
                    table->pot_money = atoi (line);
                }
                if (i == 78) {
                    table->ante = atoi (line);
                }
                if (i == 79) {
                    table->highest_bet = atoi (line);
                }
                if (i == 80) {
                    table->last_bet = atoi (line);
                }
            }

            // Load shared cards
            if (*round_index != 0) {
                if (*round_index == 1) {
                    table->card_idx = 3;

                    if (i >= 82 && i <= 84){
                        s = strtok(line, " ");
                        int k = 0;
                        while (s != NULL) {
                            k++;
                            if (l < 3) {
                                if (k == 1) {
                                    if (atoi(s) == 0) {
                                        table->card[l].suit = HEARTS;
                                    } if (atoi(s) == 1) {
                                        table->card[l].suit = DIAMONDS;
                                    } if (atoi(s) == 2) {
                                        table->card[l].suit = CLUBS;
                                    } else if (atoi(s) == 3) {
                                        table->card[l].suit = SPADES;
                                    }
                                } else if (k == 2) {
                                    table->card[l].rank = atoi(s);
                                }
                            }
                            s = strtok(NULL, " ");
                        }
                        l++;
                    }
                } else if (*round_index == 2) {
                    table->card_idx = 4;

                    if (i >= 82 && i <= 85){
                        s = strtok(line, " ");
                        int k = 0;
                        while (s != NULL) {
                            k++;
                            if (l < 4) {
                                if (k == 1) {
                                    if (atoi(s) == 0) {
                                        table->card[l].suit = HEARTS;
                                    } if (atoi(s) == 1) {
                                        table->card[l].suit = DIAMONDS;
                                    } if (atoi(s) == 2) {
                                        table->card[l].suit = CLUBS;
                                    } else if (atoi(s) == 3) {
                                        table->card[l].suit = SPADES;
                                    }
                                } else if (k == 2) {
                                    table->card[l].rank = atoi(s);
                                }
                            }
                            s = strtok(NULL, " ");
                        }
                        l++;
                    }
                } else if (*round_index == 3) {
                    table->card_idx = 5;

                    if (i >= 82 && i <= 86){
                        s = strtok(line, " ");
                        int k = 0;
                        while (s != NULL) {
                            k++;
                            if (l < 5) {
                                if (k == 1) {
                                    if (atoi(s) == 0) {
                                        table->card[l].suit = HEARTS;
                                    } if (atoi(s) == 1) {
                                        table->card[l].suit = DIAMONDS;
                                    } if (atoi(s) == 2) {
                                        table->card[l].suit = CLUBS;
                                    } else if (atoi(s) == 3) {
                                        table->card[l].suit = SPADES;
                                    }
                                } else if (k == 2) {
                                    table->card[l].rank = atoi(s);
                                }
                            }
                            s = strtok(NULL, " ");
                        }
                        l++;
                    }
                }
            }

            // Load player 1
            if (i >= 90 && i <= 96) {
                if (i == 90) {
                    players[0].money = atoi(line);
                }
                if (i == 91) {
                    players[0].bet = atoi(line);
                }
                if (i == 92) {
                    switch (atoi(line)) {
                        case 1: players[0].state = Called;
                            break;
                        case 2: players[0].state = Raised;
                            break;
                        case 3: players[0].state = Checked;
                            break;
                        case 4: players[0].state = Bets;
                            break;
                        case 5: players[0].state = Allins;
                            break;
                        case 6: players[0].state = Folded;
                            players[1].status = 0;
                            break;
                        case 7: players[0].state = SB;
                            break;
                        case 8: players[0].state = BB;
                            break;
                        default: players[0].state = None;
                    }
                }
                if (i >= 93 && i <= 94) {
                    s = strtok(line, " ");
                    int k = 0;
                    while (s != NULL) {
                        k++;
                        if (m < 2) {
                            if (k == 1) {
                                if (atoi(s) == 0) {
                                    players[0].hand[m].suit = HEARTS;
                                } if (atoi(s) == 1) {
                                    players[0].hand[m].suit = DIAMONDS;
                                } if (atoi(s) == 2) {
                                    players[0].hand[m].suit = CLUBS;
                                } else if (atoi(s) == 3) {
                                    players[0].hand[m].suit = SPADES;
                                }
                            } else if (k == 2) {
                                players[0].hand[m].rank = atoi(s);
                            }
                        }
                        s = strtok(NULL, " ");
                    }
                    m++;
                }
                if (i == 95) {
                    players[0].isSmallBlind = atoi(line);
                }
                if (i == 96) {
                    players[0].isBigBlind = atoi(line);
                }
            }

            // Load player 2
            if (i >= 98 && i <= 104) {
                m = 0;
                if (i == 98) {
                    players[1].money = atoi(line);
                }
                if (i == 99) {
                    players[1].bet = atoi(line);
                }
                if (i == 100) {
                    switch (atoi(line)) {
                        case 1: players[1].state = Called;
                            break;
                        case 2: players[1].state = Raised;
                            break;
                        case 3: players[1].state = Checked;
                            break;
                        case 4: players[1].state = Bets;
                            break;
                        case 5: players[1].state = Allins;
                            break;
                        case 6: players[1].state = Folded;
                            players[1].status = 0;
                            break;
                        case 7: players[1].state = SB;
                            break;
                        case 8: players[1].state = BB;
                            break;
                        default: players[1].state = None;
                    }
                }
                if (i >= 101 && i <= 102) {
                    if (i == 102) {
                        m = 1;
                    }
                    s = strtok(line, " ");
                    int k = 0;
                    while (s != NULL) {
                        k++;
                        if (m < 2) {
                            if (k == 1) {
                                if (atoi(s) == 0) {
                                    players[1].hand[m].suit = HEARTS;
                                } if (atoi(s) == 1) {
                                    players[1].hand[m].suit = DIAMONDS;
                                } if (atoi(s) == 2) {
                                    players[1].hand[m].suit = CLUBS;
                                } else if (atoi(s) == 3) {
                                    players[1].hand[m].suit = SPADES;
                                }
                            } else if (k == 2) {
                                players[1].hand[m].rank = atoi(s);
                            }
                        }
                        s = strtok(NULL, " ");
                    }
                    m++;
                }
                if (i == 103) {
                    players[1].isSmallBlind = atoi(line);
                }
                if (i == 104) {
                    players[1].isBigBlind = atoi(line);
                }
            }

            // Load player 3
            if (num_player >= 3) {
                if (i >= 106 && i <= 112) {
                    m = 0;
                    if (i == 106) {
                        players[2].money = atoi(line);
                    }
                    if (i == 107) {
                        players[2].bet = atoi(line);
                    }
                    if (i == 108) {
                        switch (atoi(line)) {
                            case 1: players[2].state = Called;
                                break;
                            case 2: players[2].state = Raised;
                                break;
                            case 3: players[2].state = Checked;
                                break;
                            case 4: players[2].state = Bets;
                                break;
                            case 5: players[2].state = Allins;
                                break;
                            case 6: players[2].state = Folded;
                                players[2].status = 0;
                                break;
                            case 7: players[2].state = SB;
                                break;
                            case 8: players[2].state = BB;
                                break;
                            default: players[2].state = None;
                        }
                    }
                    if (i >= 109 && i <= 110) {
                        if (i == 110) {
                            m = 1;
                        }
                        s = strtok(line, " ");
                        int k = 0;
                        while (s != NULL) {
                            k++;
                            if (m < 2) {
                                if (k == 1) {
                                    if (atoi(s) == 0) {
                                        players[2].hand[m].suit = HEARTS;
                                    } if (atoi(s) == 1) {
                                        players[2].hand[m].suit = DIAMONDS;
                                    } if (atoi(s) == 2) {
                                        players[2].hand[m].suit = CLUBS;
                                    } else if (atoi(s) == 3) {
                                        players[2].hand[m].suit = SPADES;
                                    }
                                } else if (k == 2) {
                                    players[2].hand[m].rank = atoi(s);
                                }
                            }
                            s = strtok(NULL, " ");
                        }
                        m++;
                    }
                    if (i == 111) {
                        players[2].isSmallBlind = atoi(line);
                    }
                    if (i == 112) {
                        players[2].isBigBlind = atoi(line);
                    }
                }
            }

            // Load player 4
            if (num_player >= 4) {
                if (i >= 114 && i <= 120) {
                    m = 0;
                    if (i == 114) {
                        players[3].money = atoi(line);
                    }
                    if (i == 115) {
                        players[3].bet = atoi(line);
                    }
                    if (i == 116) {
                        switch (atoi(line)) {
                            case 1: players[3].state = Called;
                                break;
                            case 2: players[3].state = Raised;
                                break;
                            case 3: players[3].state = Checked;
                                break;
                            case 4: players[3].state = Bets;
                                break;
                            case 5: players[3].state = Allins;
                                break;
                            case 6: players[3].state = Folded;
                                players[3].status = 0;
                                break;
                            case 7: players[3].state = SB;
                                break;
                            case 8: players[3].state = BB;
                                break;
                            default: players[3].state = None;
                        }
                    }
                    if (i >= 117 && i <= 118) {
                        if (i == 118) {
                            m = 1;
                        }
                        s = strtok(line, " ");
                        int k = 0;
                        while (s != NULL) {
                            k++;
                            if (m < 2) {
                                if (k == 1) {
                                    if (atoi(s) == 0) {
                                        players[3].hand[m].suit = HEARTS;
                                    } if (atoi(s) == 1) {
                                        players[3].hand[m].suit = DIAMONDS;
                                    } if (atoi(s) == 2) {
                                        players[3].hand[m].suit = CLUBS;
                                    } else if (atoi(s) == 3) {
                                        players[3].hand[m].suit = SPADES;
                                    }
                                } else if (k == 2) {
                                    players[3].hand[m].rank = atoi(s);
                                }
                            }
                            s = strtok(NULL, " ");
                        }
                        m++;
                    }
                    if (i == 119) {
                        players[3].isSmallBlind = atoi(line);
                    }
                    if (i == 120) {
                        players[3].isBigBlind = atoi(line);
                    }
                }
            }

            // Load player 5
            if (num_player == 5) {
                if (i >= 122 && i <= 128) {
                    m = 0;
                    if (i == 122) {
                        players[4].money = atoi(line);
                    }
                    if (i == 123) {
                        players[4].bet = atoi(line);
                    }
                    if (i == 124) {
                        switch (atoi(line)) {
                            case 1: players[4].state = Called;
                                break;
                            case 2: players[4].state = Raised;
                                break;
                            case 3: players[4].state = Checked;
                                break;
                            case 4: players[4].state = Bets;
                                break;
                            case 5: players[4].state = Allins;
                                break;
                            case 6: players[4].state = Folded;
                                players[4].status = 0;
                                break;
                            case 7: players[4].state = SB;
                                break;
                            case 8: players[4].state = BB;
                                break;
                            default: players[4].state = None;
                        }
                    }
                    if (i >= 125 && i <= 126) {
                        if (i == 126) {
                            m = 1;
                        }
                        s = strtok(line, " ");
                        int k = 0;
                        while (s != NULL) {
                            k++;
                            if (m < 2) {
                                if (k == 1) {
                                    if (atoi(s) == 0) {
                                        players[4].hand[m].suit = HEARTS;
                                    } if (atoi(s) == 1) {
                                        players[4].hand[m].suit = DIAMONDS;
                                    } if (atoi(s) == 2) {
                                        players[4].hand[m].suit = CLUBS;
                                    } else if (atoi(s) == 3) {
                                        players[4].hand[m].suit = SPADES;
                                    }
                                } else if (k == 2) {
                                    players[4].hand[m].rank = atoi(s);
                                }
                            }
                            s = strtok(NULL, " ");
                        }
                        m++;
                    }
                    if (i == 127) {
                        players[4].isSmallBlind = atoi(line);
                    }
                    if (i == 128) {
                        players[4].isBigBlind = atoi(line);
                    }
                }
            }
        }
    } else {
    }
    fclose(save);
}

void checkWinner(Player * players, int num_player) {
    int winner_idx[num_player]; //create an int array which stores idx of winner
    int idx; //idx of winner_idx array

    for (int j = 0; j < 5; j++) {// j is index of player's max_hand
        //initialize value of winner_idx array
        for (int i = 0; i < num_player; i++) {
            winner_idx[i] = -1;
        }
        idx = 0;

        //create an array to store the j card of each players
        int temp_array[num_player];
        //add rank of j card to the array, if the player is not a winner, leave 0
        for (int z = 0; z < num_player; z++) {
            if (players[z].isWinner != 1) {
                temp_array[z] = 0;
            }
            temp_array[z] = players[z].max_hand[j].rank;
        }

        //check if j card is Ace
        int countAce = 0;
        for (int z = 0; z < num_player; z++) {
            if (temp_array[z] == 1) {
                countAce++;
                winner_idx[idx] = z; //add the player_idx to winner_idx array
                idx++;
            }
        }

        //if only 1 player has Ace, the rest will be losers
        if (countAce == 1) {
            for (int z = 0; z < num_player; z++) {
                if (z != winner_idx[0]) {
                    players[z].isWinner = 0;
                }
            }
            return; //stop the checking
        }

            //if more than 2 players have Ace, the one who does not have Ace will be a loser
        else if (countAce > 1) {
            for (int z = 0; z < num_player; z++) {
                for (int k = 0; k < idx; k++) {
                    if (z == k) {
                        players[z].isWinner = 1;
                        break; //move to another player
                    } else {
                        players[z].isWinner = 0;
                    }
                }
            }
            continue; //move to another j card
        }

        //if countAce = 0, check the high
        int max_rank = temp_array[0];
        int countMax = 0;
        //find the highest j card's rank of players and count how many players have the j cards with that rank
        for (int i = 0; i < num_player; i++) {
            if (temp_array[i] > max_rank) {
                idx = 0;
                max_rank = temp_array[i];
                winner_idx[idx] = i;
                idx++;
                countMax = 1;
            } else if (temp_array[i] == max_rank) {
                winner_idx[idx] = i;
                idx++;
                countMax++;
            }
        }

        //if only 1 player has the highest j card, the rest will be losers
        if (countMax == 1) {
            for (int z = 0; z < num_player; z++) {
                if (z != winner_idx[0]) {
                    players[z].isWinner = 0;
                }
            }
            return; //stop the checking
        }

            //if more than 2 players have the highest j card rank, the one who does not have Ace will be a loser
        else if (countMax > 1) {
            for (int z = 0; z < num_player; z++) {
                for (int k = 0; k < idx; k++) {
                    if (z == k) {
                        players[z].isWinner = 1;
                        break; //move to another player
                    } else {
                        players[z].isWinner = 0;
                    }
                }
            }
            continue; //move to another j card
        }
        //countMax should never be 0 at the end
        //because before max_rank changes, the statement: temp_array[0] == max_rank is always true

        //at the end, there can be more than 1 players are winners
    }
}

void testHand(Hand *hands, Player * players, int num_player) {
    sortHands(hands, num_player);
    for (int i = 0; i < num_player; i++) {
        if (players[i].status == 0) {
            continue;
        }
        if (isRoyalFlush(hands[i], &players[i])) {
        } else if (isStraightFlush(hands[i], &players[i])) {
        } else if (is4OfAKind(hands[i], &players[i])) {
        } else if (isFullHouse(hands[i], &players[i])) {
        } else if (isFlush(hands[i], &players[i])) {
        } else if (isStraight(hands[i], &players[i])) {
        } else if (is3OfAKind(hands[i], &players[i])) {
        } else if (is2Pair(hands[i], &players[i])) {
        } else if (isPair(hands[i], &players[i])) {
        } else {
            isHighCard(hands[i], &players[i]);
        }
    }
    int countWinners;
    //start checking from the highest rank
    for (Rank rank = RoyalFlush; rank >= HighCard; rank--) {
        countWinners = 0;

        //If the player's rank == the checking rank, add his idx to winner_idx array, considered him as a winner
        for (int i = 0; i < num_player; i++) {
            if (players[i].rank == rank && players[i].status == 1) {
                players[i].isWinner = 1;
                countWinners++;
            }
        }

        // If there is only 1 winner, stop the checking
        if (countWinners == 1) {
            break; //stop the checking
        } else if (countWinners >= 2) {     // If there is more than 2 winners, check them
            // Create a temporary pointer
            Player * temp = malloc(sizeof(Player) * countWinners);

            // Copy the value of temporary winners to the addresses where temp pointer points to
            int idx = 0;
            for (int i = 0; i < num_player; i++) {
                if (players[i].isWinner) {
                    temp[idx] = players[i];
                    idx++;
                }
            }

            // Check who is the winner
            checkWinner(temp, countWinners);
            // Copy back the value of the pointer to the appropriate player
            for (int i = 0; i < num_player; i++) {
                for (int x = 0; x < idx; x++) {
                    if (strcmp(players[i].name, temp[x].name) == 0) {
                        players[i] = temp[x];
                    }
                }
            }

            free(temp);
            break; //stop the checking
        }
    }
}

void award(Player * players, Table * table, int num_player) {
    int winnersMoney = 0;
    int count_fold = 0;

    for (int i = 0; i < num_player; i++) {
        if (players[i].isWinner) {
            winnersMoney += players[i].bet;
        }
        if (players[i].status == 0) {
            count_fold++;
        }
    }
    //split pot by ratio; if there is only one active player, he takes all the chips.
    for (int i = 0; i < num_player; i++) {
        if (players[i].isWinner) {
            if (count_fold == num_player - 1) {
                players[i].money += table->pot_money;
            } else {
                players[i].money += players[i].bet / winnersMoney * table->pot_money;
            }
        }
    }
    table->pot_money = 0;
}

void reset (Player * players, Table * table, int num_player, Deck * deck) {
    for (int i = 0; i < num_player; i++) {
        players[i].bet = 0;
        players[i].isWinner = 0;
        players[i].state = None;
        players[i].status = 1;
        for (int j = 0; j < 5; j++) {
            if (j < 2) {
                players->hand[j].rank = 0;
            }
            players[i].max_hand[j].rank = 0;
        }
    }
    deck->card_index = 0;
    table->pot_money = 0;
    table->card_idx = 0;
    table->showCard = 0;
    for (int i = 0; i < 5; i++) {
        table->card[i].rank = 0;
    }
    table->highest_bet = 0;
    table->last_bet = 0;
}