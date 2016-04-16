//
// Created by bloo on 12/04/2016.
//

#include <stdio.h>
#include <locale.h>

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

int main() {
    setlocale(LC_ALL, "");
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
    for (int m = 0; m < size; m++) {
        printf("%s %i \n", getSuit(deck[m].suit), deck[m].rank);
    }
}

