#include <iostream>
#include "MathModel.h"

int VoltageSource::countVS = 0;

int main() {
    MathModel MM(4, 1, 1e-3, 5e-7);
    VoltageSource VS = VoltageSource::getVoltageSource(0, 1, 10, 1e-4);
    Capacitor C1(1, 2, 1e-6), CB(2, 3, 2e-12);
    Inductor L1(1, 2, 1e-3);
    Resistor RU(2, 3, 1e6), RB(3, 4, 20), R1(4, 0, 1e3);
    ElectricitySource ES(3, 2);
    MM.addCapasitor(C1);
    MM.addCapasitor(CB);
    MM.addInductor(L1);
    MM.addResistor(RU);
    MM.addResistor(RB);
    MM.addResistor(R1);
    MM.addElectricitySource(ES);
    MM.addVoltageSource(VS);
    MM.resolve();
    return 0;
}