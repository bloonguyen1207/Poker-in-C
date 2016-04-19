//
// Created by bloo on 12/04/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum suit {HEARTS, DIAMONDS, CLUBS, SPADES};
typedef enum suit Suit;

struct card {
    Suit suit;
    int rank;
};
typedef struct card Card;

char* getSuit(Suit s) {
    switch(s){
        case HEARTS: return "\u2665";
        case DIAMONDS: return "\u2666";
        case CLUBS: return "\u2663";
        case SPADES: return "\u2660";
    }
}

struct deck {
    Card* cards;
    int card_index;
};
typedef struct deck Deck;

Deck* newDeck() {
    Deck* deck = malloc(sizeof(Deck));
    int k = 0;
    deck->card_index = 0;
    Card c;
    deck->cards = malloc(sizeof(Card) * 52);
    for (Suit i = HEARTS; i <= SPADES; i++) {
        for (int j = 0; j < 13; j++) {
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

struct player {
    char name[20];
    Card* hand;
    int money;
};
typedef struct player Player;

Player* createPlayers(int no_player) {
    if (no_player >= 2) {
        Player* player = malloc(sizeof(Player) * no_player);
        for (int i = 0; i < no_player; i++) {
            strcpy(player[i].name, "Player0");
            player[i].name[6] += i + 1;
            player[i].hand = malloc(sizeof(Card) * 2);
            player[i].money = 5000;
        }
        return player;
    }
    return NULL;
}

struct pot {
    int money;
    Card* card;
    int card_idx;
};
typedef struct pot Pot;

Pot* createPot() {
    Pot* pot = malloc(sizeof(Pot));
    pot->card = malloc(sizeof(Card) * 5);
    pot->card_idx = 0;
    return pot;
}

void dealPlayer(int no_player, Player* player, Deck* deck) {
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < no_player; i++) {
            player[i].hand[j] = deck->cards[deck->card_index];
            deck->card_index++;
        }
    }
}

void dealPot(Pot* pot, Deck* deck, int time){
    //1st time: deal 3 cards: largest idx = 2
    //2nd time: deal 1 card:  largest idx = 3
    //3rd time: deal 1 card:  largest idx = 4 --> largest idx = time + 1
    for (; pot->card_idx <= time + 1; pot->card_idx++) {
        pot->card[pot->card_idx] = deck->cards[deck->card_index];
        deck->card_index++;
    }
};

int main() {
    //create pot
    Pot* pot = createPot();

    //create deck
    Deck * deck;
    deck = newDeck();
    int size = 52;

    // Test new deck
    for (int m = 0; m < size; m++) {
        printf("%s %i; ", getSuit(deck->cards[m].suit), deck->cards[m].rank);
    }
    printf("\n");

    // Shuffle the deck
    shuffleDeck(deck, size);

    //test shuffle
    for (int m = 0; m < size; m++) {
        printf("%s %i; ", getSuit(deck->cards[m].suit), deck->cards[m].rank);
    }
    printf("\n");

    //create players
    int no_player = 2;
    Player* player = createPlayers(no_player);

    // Deal deck to players
    dealPlayer(no_player, player, deck);

    //deal on the pot
    dealPot(pot, deck, 1);
    dealPot(pot, deck, 2);
    dealPot(pot, deck, 3);

    // Test player hand
    for (int i = 0; i < no_player; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%s: %s %i \n", player[i].name,  getSuit(player[i].hand[j].suit), player[i].hand[j].rank);
        }
    }

    //test cards on pot
    printf("On pot: \n");
    for (int i = 0; i < 5; i++) {
        printf("%s %i \n", getSuit(pot->card[i].suit),pot->card[i].rank);
    }

    //free player & deck
    free(player);
    free(deck);

    return 0;
}

