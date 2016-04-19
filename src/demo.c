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

Card* newDeck() {
    int k = 0;
    Card c;
    Card * deck = malloc(sizeof(c) * 52);
    for (Suit i = HEARTS; i <= SPADES; i++) {
        for (int j = 0; j < 13; j++) {
            c.rank = j;
            c.suit = i;
            deck[k] = c;
            k++;
            if (k == 52) {
                return deck;
            }
        }
    }
    return NULL;
}

void shuffleDeck(Card deck[], int size) {
    srand((unsigned int) time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int randIdx = rand() % i;
        Card temp = deck[i];
        deck[i] = deck[randIdx];
        deck[randIdx] = temp;
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
            player[i].money = 0;
        }
        return player;
    }
    return NULL;
}

void dealDeck(int no_player, Player* player, Card* deck) {
    int k = 0;
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < no_player; i++) {
            player[i].hand[j] = deck[k];
            k++;
        }
    }
}

int main() {
    Card * deck;
    deck = newDeck();
    int size = 52;

    // Test new deck
    for (int m = 0; m < size; m++) {
        printf("%s %i; ", getSuit(deck[m].suit), deck[m].rank);
    }
    printf("\n");

    // Shuffle the deck
    shuffleDeck(deck, size);

    //test shuffle
    for (int m = 0; m < size; m++) {
        printf("%s %i; ", getSuit(deck[m].suit), deck[m].rank);
    }
    printf("\n");

    //create players
    int no_player = 2;
    Player* player = createPlayers(no_player);

    // Deal the deck
    dealDeck(no_player, player, deck);

    // Test deal
    for (int i = 0; i < no_player; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%s: %s %i \n", player[i].name,  getSuit(player[i].hand[j].suit), player[i].hand[j].rank);
        }
    }

    //free player & deck
    free(player);
    free(deck);
}

