//
// Created by note on 02.03.17.
//

#include "VoltageSource.h"

VoltageSource::VoltageSource(int firstPoint, int secondPoint, double potential, double period) :
        BaseElement(firstPoint, secondPoint),
        potential(potential) {
    frequency = 2 * M_PI / period;
}

double VoltageSource::getTempPotential(double time) {
    return potential * sin(time * frequency);
}

int VoltageSource::getNumberVS() const {
    return numberVS;
}

VoltageSource VoltageSource::getVoltageSource(int firstPoint, int secondPoint, double potential, double period) {
    VoltageSource VS(firstPoint, secondPoint, potential, period);
    VS.numberVS = VoltageSource::countVS;
    VoltageSource::countVS += 1;
    return VS;
}
