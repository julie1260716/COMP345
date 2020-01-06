#ifndef COMP345_PHASESUBJECT_H
#define COMP345_PHASESUBJECT_H
#include <vector>
#include <string>
#include "Subject.h"

class PhaseSubject : public Subject {

public:
    PhaseSubject(); //default constructor for Observer objects
    PhaseSubject(int, int); //Parameterized constructor for Observer objects
    ~PhaseSubject(); //destructor for Observer();
    void NotifyAll();
    string getCurrentPlayer(){return currentPlayer;};
    int getCurrentStep(){return currentStep;};
    string setCurrentPlayer(string _player){currentPlayer = _player;};
    int setCurrentStep(int _step){currentStep = _step;};
    vector<string>* getLog(){return &log;};

private:
    string currentPlayer;
    int currentStep;
    vector<string> log;


};




#endif //COMP345_PHASESUBJECT_H
