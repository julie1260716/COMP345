#ifndef UNTITLED_DECK_H
#define UNTITLED_DECK_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

template <class T>
class Deck {
    vector<T> deck;

public:
    Deck(vector<T> deck) : deck(deck) {}
    void shuffle();
    vector<T>* getDeck() { return &deck; }
    int getNumberOfCards() { return deck.size(); }
    /********************************Test****************************/
    void print();
};


#endif //UNTITLED_DECK_H
