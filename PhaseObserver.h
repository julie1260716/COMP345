/*
 * CONCRETE OBSERVER : Phase
 * This is the class which needs to keep itself updated with the change.
 * This class just needs to implement the Observer and register itself with
 * the ConcreteSubject and it is all set to receive the updates
 */
#ifndef COMP345_PHASEOBSERVER_H
#define COMP345_PHASEOBSERVER_H
#include "PhaseSubject.h"
#include "Observer.h"
class PhaseObserver : public Observer{

public:
    PhaseObserver(); //default constructor for Observer objects
    PhaseObserver(PhaseSubject * subject); //will hold the subject, to access its values
    ~PhaseObserver(); //destructor for Observer();
    void Update(); //will be used at runtime for Polymorphism
    void display();

private:
   //holds the subject(pointer) so that we can access its values
   PhaseSubject * _subject;

};


#endif //COMP345_PHASEOBSERVER_H
