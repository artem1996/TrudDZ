//
// Created by note on 02.03.17.
//

#ifndef ARINATRUDHW_DIFFICULTELEMENT_H
#define ARINATRUDHW_DIFFICULTELEMENT_H

#include "cmath"
#include "BaseElement.h"

class ElectricitySource: public BaseElement {
    const double It = 1e-12;
    const double MFt = 0.026;
    double potentialDifference;
public:
    ElectricitySource(int firstPoint, int secondPoint);
    int getPosition();
    double getMatrixValue();
    double getConstantValue();
    void setPotentialDifference(double potentialDifference);
};


#endif //ARINATRUDHW_DIFFICULTELEMENT_H
