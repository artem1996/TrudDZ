#ifndef ARINATRUDHW_CAPACITOR_H
#define ARINATRUDHW_CAPACITOR_H


#include "BaseElement.h"

class Capacitor: public BaseElement {
    double capacity;
public:
    double getValue() const;
    int getPosition() const;

public:
    Capacitor(int firstPoint, int secondPoint, double capacity);
};


#endif //ARINATRUDHW_CAPACITOR_H
