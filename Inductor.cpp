//
// Created by note on 02.03.17.
//

#include "Inductor.h"

Inductor::Inductor(int firstPoint, int secondPoint, double inductance) : BaseElement(firstPoint, secondPoint), inductance(inductance){}

double Inductor::getValue() {
    return 1 / inductance;
//    return inductance;
}

int Inductor::getPosition() {
    return 1;
}