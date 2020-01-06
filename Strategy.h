#ifndef UNTITLED_STRATEGY_H
#define UNTITLED_STRATEGY_H
#include "map.h"
#include "card.h"
#include "deck.h"

class Player;

class Strategy {
public:
    /**
     * Method whose implementation varies depending on the strategy adopted.
     */
    virtual void rollDice(Player*) = 0;
    virtual void resolveDice(Player*, vector<Player>*, Map*) = 0;
    virtual void move(Player*, vector<Player>*, Map*) = 0;
    virtual void buyCards(Player*, Deck<Card>*) = 0;
};


#endif //UNTITLED_STRATEGY_H
