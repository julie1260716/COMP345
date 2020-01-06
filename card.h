#ifndef UNTITLED_CARD_H
#define UNTITLED_CARD_H

#include <string>
#include <iostream>
using namespace std;

class Card {
    int cost;
    string name;
    string type; // "Discard" or "Keep"
    string effect;

public:
    Card(int cost, string name, string type, string effect) :
        cost(cost), name(name), type(type), effect(effect) {}

    int getCost() { return cost; }

    friend ostream& operator<<(ostream& os, const Card& card);

    /********************************Test****************************/
    void print(){
        cout << "Name:" << name << " ";
        cout << "Type:" << type << " ";
        cout << "Effect:" << effect << endl;
    }
};


#endif //UNTITLED_CARD_H
