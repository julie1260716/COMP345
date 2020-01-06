#ifndef COMP345_SUBJECT_H
#define COMP345_SUBJECT_H
#include <list>
using namespace std;

class Observer;

class Subject {
public:
    virtual void Attach(Observer* o);   //Will be used to 'Subscribe' Observers to the Observable Subject
    virtual void Detach(Observer* o);   //Will be used to 'Unsubscribe' the Observer from the Subject
    virtual void Notify();              //Will be used to notify ALL Observers (subscribers) that change has occurred
    Subject();                          //Default constructor
    ~Subject();                         //Desctructor
private:
    //holds a list pointing to all observers of the subject
    list<Observer*> *_observers;
};


#endif //COMP345_SUBJECT_H
