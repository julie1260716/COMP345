/*
 * CONCRETE OBSERVER: Game Phase Observer
 * This is the class which needs to keep itself updated with the change.
 * This class just needs to implement the Observer and register itself with
 * the ConcreteSubject and it is all set to receive the updates
 */
#include "Subject.h"
#include "Observer.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "PhaseObserver.h"
using namespace std;

map<int, string> steps =  {{1, "Resolve Energy"},
                          {2, "Resolve Attack"},
                          {3, "Roll Die"},
                          {4, "Resolve Die"},
                          {5, "Move"},
                          {6, "Buy Cards"},
                          {7, "End You Turn"},
                          {9, "Resolve Heal"},
                          {10, "Resolve Destruction"},
                          {8, "Configuring Player"}};

    PhaseObserver::PhaseObserver() {};

    PhaseObserver::PhaseObserver(PhaseSubject* s) {
        //attach the Observer to the Subject
        _subject = s;
        _subject -> Attach(this);
    };

    PhaseObserver::~PhaseObserver() {
        //when we want to destroy it, we will detach it
        //from the subject
        _subject -> Detach(this);
    }

    void PhaseObserver::Update() {
        display();
    };

    void PhaseObserver::display() {
        string currentPlayer = _subject -> getCurrentPlayer();
        int currentStep = _subject -> getCurrentStep();
        vector<string>* log = _subject -> getLog();
        int size = log->size();
        cout<<"\n----------------------------------------------------------------------------------------------";
        cout<<"\n   Player "<< currentPlayer  <<": " << steps[currentStep] << " Step\n";
        cout<<"----------------------------------------------------------------------------------------------\n";
        for(int i = 0; i < size; i++)
            cout<< "     " << log->at(i) << endl;

        cout<<"----------------------------------------------------------------------------------------------\n";

    };

