//
// Created by bloo on 12/04/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    // Create players & places for cards
    int no_player = 2; //number of players
    int no_card = 2; //number of cards
    Card *player[no_player]; //an array of player (each player is a pointer)
    for (int i = 0; i < no_player; i++) { //set place for cards that each player holds (refers to)
        player[i] = malloc(no_card * sizeof(Card));
    }

    // Deal the deck
    int k = 0;
    for (int j = 0; j < no_player; j++) {
        for (int i = 0; i < no_card; i++) {
            player[i][j] = deck[k];
            k++;
        }
    }
    // Test deal
    for (int i = 0; i < no_player; i++) {
        for (int j = 0; j < no_card; j++) {
            printf("Player %d: %s %i \n", i + 1, getSuit(player[i][j].suit), player[i][j].rank);
        }
    }
    for (int i = 0; i < no_player; i++) {
        free(player[i]);
    }
    free(deck);
}

