//
// Created by note on 02.03.17.
//

#include "Resistor.h"

Resistor::Resistor(int firstPoint, int secondPoint, double resistance) : BaseElement(firstPoint, secondPoint), resistance(resistance) {}

double Resistor::getValue() const {
//    return 1 / resistance;
    return resistance;
}

int Resistor::getPosition() const {
    return 2;
}
