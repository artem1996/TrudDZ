//
// Created by note on 02.03.17.
//

#include "Capacitor.h"

Capacitor::Capacitor(int firstPoint, int secondPoint, double capacity) : BaseElement(firstPoint, secondPoint), capacity(capacity) {}

double Capacitor::getValue() const {
    return capacity;
}

int Capacitor::getPosition() const {
    return 0;
}