/*
 * CONCRETE SUBJECT: Game Statistic Subject
 * It maintains the state of the observed object
 * When a change in its state occurs it notifies the attached Observers
*/
#include "StatSubject.h"
#include "player.h"
#include <vector>
#include <string>

StatSubject::StatSubject() {

};

void StatSubject::NotifyAll() {
    Notify();
}