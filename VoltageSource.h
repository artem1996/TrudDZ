//
// Created by note on 02.03.17.
//

#ifndef ARINATRUDHW_VOLTAGESOURCE_H
#define ARINATRUDHW_VOLTAGESOURCE_H

#include "cmath"
#include "BaseElement.h"

class VoltageSource: public BaseElement {
    int numberVS;
    VoltageSource(int firstPoint, int secondPoint, double potential, double period);

private:
    double potential;
    double frequency;

public:
    static int countVS;
    static VoltageSource getVoltageSource(int firstPoint, int secondPoint, double potential, double period);
    double getTempPotential(double timeIncrement);
    int getNumberVS() const;

};


#endif //ARINATRUDHW_VOLTAGESOURCE_H
