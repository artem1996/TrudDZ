//
// Created by note on 02.03.17.
//

#ifndef ARINATRUDHW_REZISTOR_H
#define ARINATRUDHW_REZISTOR_H


#include "BaseElement.h"

class Resistor: public BaseElement {
    double resistance;
public:
    double getValue() const;
    int getPosition() const;

public:
    Resistor(int firstPoint, int secondPoint, double resistance);

};


#endif //ARINATRUDHW_REZISTOR_H
