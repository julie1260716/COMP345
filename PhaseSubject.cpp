/*
 * CONCRETE SUBJECT: Game Phase Subject
 * It maintains the state of the observed object
 * When a change in its state occurs it notifies the attached Observers
*/
#include "PhaseSubject.h"
#include <vector>
#include <string>

PhaseSubject::PhaseSubject() {
    currentPlayer = "";
    currentStep = 0;

};

void PhaseSubject::NotifyAll() {
    Notify();
}

