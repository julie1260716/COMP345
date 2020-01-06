#include "deck.h"
#include "card.h"
#include "tile.h"
#include <iostream>
#include <algorithm>

template <class T>
void Deck<T>::print(){
    for (T& i : deck){
        i.print();
    }
}

template <class T>
void Deck<T>::shuffle(){
    random_shuffle(deck.begin(), deck.end());
}

template class Deck<Card>;
template class Deck<Tile>;