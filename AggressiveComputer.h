#ifndef UNTITLED_AGGRESSIVECOMPUTER_H
#define UNTITLED_AGGRESSIVECOMPUTER_H


#include "Strategy.h"

class AggressiveComputer : public Strategy{
public:
    void rollDice(Player*);
    void resolveDice(Player*, vector<Player>*, Map*);
    void move(Player*, vector<Player>*, Map*);
    void buyCards(Player*, Deck<Card>*);
};


#endif //UNTITLED_AGGRESSIVECOMPUTER_H
