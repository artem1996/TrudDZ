//
// Created by note on 02.03.17.
//

#ifndef ARINATRUDHW_MATHMODEL_H
#define ARINATRUDHW_MATHMODEL_H

#include <vector>
#include "fstream"
#include "gauss.h"
#include "Capacitor.h"
#include "Resistor.h"
#include "Inductor.h"
#include "VoltageSource.h"
#include "ElectricitySource.h"

class MathModel {

    int pointCount;
    int idealSourceCount;
    Gauss* matrix;
    double timeIncrement;
    double tempTime;
    double* prevSolution;
    double* solution;
    double finishTime;
    vector<Capacitor> capacitors;
    vector<ElectricitySource> ES;
    vector<Inductor> inductors;
    vector<Resistor> resistors;
    vector<VoltageSource> VS;

    template <typename ELEMENT> void addElement(ELEMENT &element);
    template <typename ELEMENT> void addDifficultElement(ELEMENT &element);
    void addVS(VoltageSource &element);
    void firstSet();
    void secondSet();

public:
    MathModel(int pointCount, int idealSourceCount, double finishTime, double timeIncrement);
    void addCapasitor(Capacitor &capacitor);
    void addElectricitySource(ElectricitySource &ES);
    void addInductor(Inductor &inductor);
    void addResistor(Resistor &resistor);
    void addVoltageSource(VoltageSource &VS);
    void resolve();

};


#endif //ARINATRUDHW_MATHMODEL_H
