//
// Created by note on 02.03.17.
//

#include "ElectricitySource.h"

ElectricitySource::ElectricitySource(int firstPoint, int secondPoint) : BaseElement(firstPoint, secondPoint) {}

double ElectricitySource::getMatrixValue() {
    return (It / MFt) * exp(potentialDifference / MFt);
}

double ElectricitySource::getConstantValue() {
    return It * (exp(potentialDifference / MFt) - 1);
}

void ElectricitySource::setPotentialDifference(double potentialDifference) {
    this->potentialDifference = potentialDifference;
}

int ElectricitySource::getPosition() {
    return 2;
}