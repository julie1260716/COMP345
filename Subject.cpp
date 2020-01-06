#include "Subject.h"
#include "Observer.h"
/*
 * This class keeps track of all the observers and provides the facility to add or remove the observers.
 * Also it is the class that is responsible for updating the observers when any change occurs
 */
Subject::Subject() {
    _observers = new list<Observer*>;
}

Subject::~Subject() {
    delete _observers;
}

void Subject::Attach(Observer *o) {
    _observers->push_back(o);
}

void Subject::Detach(Observer *o) {
    _observers->remove(o);
}

void Subject::Notify() {
    //this method will start at the beginning of the list of observers
    //and will notify all the Observers in the list, until it reaches the end.
    list<Observer*>::iterator i = _observers->begin();
    for(; i != _observers->end(); ++i)
        (*i)->Update();
}