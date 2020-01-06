#ifndef UNTITLED_DICE_H
#define UNTITLED_DICE_H

#include <map>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Dice {
public:
    Dice(){
        dice = {{1, "Energy"}, {2, "Attack"}, {3, "Destruction"},
                {4, "Heal"}, {5, "Star"}, {6, "Ouch"}};
    }

    void roll(){
        state = dice[rand()%6+1];
    };

    const string getState() const {return state;}

private:
    map<int, string> dice;
    string state;
};


#endif //UNTITLED_DICE_H
