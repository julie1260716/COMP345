/*
 * CONCRETE OBSERVER : Statistic
 * This is the class which needs to keep itself updated with the change.
 * This class just needs to implement the Observer and register itself with
 * the ConcreteSubject and it is all set to receive the updates
 */
#ifndef COMP345_STATOBSERVER_H
#define COMP345_STATOBSERVER_H
#include "StatSubject.h"
#include "Player.h"
#include "Observer.h"
class StatObserver : public Observer{

public:
    StatObserver(); //default constructor for Observer objects
    StatObserver(StatSubject * subject); //will hold the subject, to access its values
    ~StatObserver(); //destructor for Observer();
    void Update(); //will be used at runtime for Polymorphism
    void display();

private:
    //holds the subject(pointer) so that we can access its values
    StatSubject * _subject;




};
#endif //COMP345_STATOBSERVER_H
