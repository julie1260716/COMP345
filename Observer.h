#ifndef COMP345_OBSERVER_H
#define COMP345_OBSERVER_H
#include "Subject.h"
class Observer {

public:
    ~Observer(); //destructor for Observer();
    virtual void Update(); //will be used at runtime for Polymorphism
protected:
    Observer(); //default constructor for Observer objects

};




#endif //COMP345_OBSERVER_H
