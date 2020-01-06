#ifndef COMP345_STATSUBJECT_H
#define COMP345_STATSUBJECT_H
#include <vector>
#include <string>
#include "Subject.h"
#include "player.h"


class StatSubject : public Subject {

public:
    StatSubject(); //constructor for Observer objects
    ~StatSubject(); //destructor for Observer();
    void NotifyAll();
    void setPlayers(vector<Player>* _players){players = _players;};
    vector<Player>* getPlayers(){return players;};
    StatSubject* getSubject(){return this;};


private:
    vector<Player>* players;


};


#endif //COMP345_STATSUBJECT_H
