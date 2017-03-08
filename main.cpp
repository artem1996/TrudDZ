#include <iostream>
#include "MathModel.h"

int VoltageSource::countVS = 0;

int main() {
    MathModel MM(5, 1, 1e-3, 5e-7);
    VoltageSource VS = VoltageSource::getVoltageSource(0, 1, 10, 1e-4);
    Capacitor C1(1, 2, 1e-6), CB(3, 4, 2e-12);
    Inductor L1(1, 2, 1e-3);
    Resistor RU(3, 4, 1e6), RB(4, 5, 20), R1(5, 0, 1e3), RD(2, 3, 100);
    ElectricitySource ES(4, 3);
    MM.addCapasitor(C1);
    MM.addCapasitor(CB);
    MM.addInductor(L1);
    MM.addResistor(RU);
    MM.addResistor(RB);
    MM.addResistor(R1);
    MM.addResistor(RD);
    MM.addElectricitySource(ES);
    MM.addVoltageSource(VS);
    MM.resolve();
    return 0;
}