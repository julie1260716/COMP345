#ifndef UNTITLED_MODERATECOMPUTER_H
#define UNTITLED_MODERATECOMPUTER_H

#include "Strategy.h"

class ModerateComputer : public Strategy{
public:
    void rollDice(Player*);
    void resolveDice(Player*, vector<Player>*, Map*);
    void move(Player*, vector<Player>*, Map*);
    void buyCards(Player*, Deck<Card>*);
};


#endif //UNTITLED_MODERATECOMPUTER_H
