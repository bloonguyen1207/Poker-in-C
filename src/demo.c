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
        case 0: return "\u2665";
        case 1: return "\u2666";
        case 2: return "\u2663";
        case 3: return "\u2660";
    }
};

void shuffleDeck(Card deck[], int size) {

    srand(time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int randIdx = rand() % i;
        Card temp = deck[i];
        deck[i] = deck[randIdx];
        deck[randIdx] = temp;
    }
}

int main() {
    //create a deck
    int size = 52;
    int k = 0;
    Card c;
    Card deck[size];
    for (int i = HEARTS; i <= SPADES; i++) {
        for (int j = 0; j < 13; j++) {
            c.suit = i;
            c.rank = j;
            deck[k] = c;
            k++;
        }
    }
    //test
    for (int m = 0; m < size; m++) {
        printf("%s %i \n", getSuit(deck[m].suit), deck[m].rank);
    }

    //shuffle the deck
    shuffleDeck(deck, size);
    //test
    for (int m = 0; m < size; m++) {
        printf("%s %i \n", getSuit(deck[m].suit), deck[m].rank);
    }

    //create players & places for cards
    int no_player = 2; //number of players
    int no_card = 2; //number of cards
    Card *player[no_player]; //an array of player (each player is a pointer)
    for (int i = 0; i < no_player; i++) { //set place for cards that each player holds (refers to)
        player[i] = malloc(no_card * sizeof(Card));
    }

    //deal the deck
    k = 0;
    for (int i = 0; i < no_card; i++) {
        for (int j = 0; j < no_player; j++) {
            player[i][j] = deck[k];
            k++;
        }
    }
    //test
    for (int i = 0; i < no_player; i++) {
        for (int j = 0; j < no_card; j++) {
            printf("Player %d: %s %i \n", i + 1, getSuit(player[i][j].suit), player[i][j].rank);
        }
    }
}

