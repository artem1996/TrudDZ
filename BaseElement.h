//
// Created by note on 02.03.17.
//

#ifndef ARINATRUDHW_BASEELEMENT_H
#define ARINATRUDHW_BASEELEMENT_H


class BaseElement {
    int firstPoint;
    int secondPoint;

public:
    BaseElement(int firstPoint, int secondPoint);
    int getFirstPoint() const;
    int getSecondPoint() const;
};


#endif //ARINATRUDHW_BASEELEMENT_H
