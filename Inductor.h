//
// Created by note on 02.03.17.
//

#ifndef ARINATRUDHW_INDUCTOR_H
#define ARINATRUDHW_INDUCTOR_H


#include "BaseElement.h"

class Inductor: public BaseElement {
    double inductance;
public:
    Inductor(int firstPoint, int secondPoint, double inductance);
    double getValue();
    int getPosition();
};


#endif //ARINATRUDHW_INDUCTOR_H
