#ifndef UNTITLED_HUMANPLAYER_H
#define UNTITLED_HUMANPLAYER_H


#include "Strategy.h"

class HumanPlayer : public Strategy{
public:
    void rollDice(Player*);
    void resolveDice(Player*, vector<Player>*, Map*);
    void move(Player*, vector<Player>*, Map*);
    void buyCards(Player*, Deck<Card>*);
};


#endif //UNTITLED_HUMANPLAYER_H
