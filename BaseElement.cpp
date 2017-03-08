//
// Created by note on 02.03.17.
//

#include "BaseElement.h"

BaseElement::BaseElement(int firstPoint, int secondPoint) : firstPoint(firstPoint), secondPoint(secondPoint) {}

int BaseElement::getFirstPoint() const {
    return firstPoint;
}

int BaseElement::getSecondPoint() const {
    return secondPoint;
}
