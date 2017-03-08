#include "MathModel.h"

MathModel::MathModel(int pointCount, int idealSourceCount, double finishTime, double timeIncrement) :
        pointCount(pointCount),
        idealSourceCount(idealSourceCount),
        matrix(new Gauss(pointCount * 3 + idealSourceCount)),
        finishTime(finishTime),
        timeIncrement(timeIncrement),
        tempTime(0) {
}

template <typename ELEMENT>
void MathModel::addElement(ELEMENT &element) {
    int firstPoint = element.getFirstPoint();
    int secondPoint = element.getSecondPoint();
    int column1 = pointCount * element.getPosition() + firstPoint - 1;
    int column2 = pointCount * element.getPosition() + secondPoint - 1;
    int row = pointCount * 2 + firstPoint - 1;
    double value = element.getValue();
    double variablesDiff = (firstPoint == 0 ? 0 : solution[column1]) - (secondPoint == 0 ? 0 : solution[column2]);
    if(firstPoint != 0) {
        matrix->into_matrix(row, column1, value);
        matrix->into_matrix(row, column2, secondPoint == 0 ? 0 : - value);
        matrix->into_constants(row, -value * variablesDiff);
    }
    row = pointCount * 2 + secondPoint - 1;
    if(secondPoint != 0) {
        matrix->into_matrix(row, column1, firstPoint == 0 ? 0 : - value);
        matrix->into_matrix(row, column2, value);
        matrix->into_constants(row, value * variablesDiff);
    }
}

template <typename ELEMENT>
void MathModel::addDifficultElement(ELEMENT &element) {
    int firstPoint = element.getFirstPoint();
    int secondPoint = element.getSecondPoint();
    int column1 = pointCount * element.getPosition() + firstPoint - 1;
    int column2 = pointCount * element.getPosition() + secondPoint - 1;
    int row = pointCount * 2 + firstPoint - 1;
    double matrixValue = element.getMatrixValue();
    double constantValue = element.getConstantValue();
    if(firstPoint != 0) {
        matrix->into_matrix(row, column1, matrixValue);
        matrix->into_matrix(row, secondPoint == 0 ? 0 : column2, - matrixValue);
        matrix->into_constants(row, -constantValue);
    }
    row = pointCount * 2 + secondPoint - 1;
    if(secondPoint != 0) {
        matrix->into_matrix(row, firstPoint == 0 ? 0 : column1, - matrixValue);
        matrix->into_matrix(row, column2, matrixValue);
        matrix->into_constants(row, constantValue);
    }
}

void MathModel::firstSet() {
    for(int i = 0; i < pointCount; i++) {
        matrix->into_matrix(i, i, 1);
        matrix->into_matrix(i, pointCount * 2 + i, -1 / timeIncrement);
        matrix->into_constants(i, - (solution[i] - (solution[pointCount * 2 + i] - prevSolution[pointCount * 2 + i]) / timeIncrement));
    }
}

void MathModel::secondSet() {
    int row = pointCount;
    for(int i = 0; i < pointCount; i++) {
        matrix->into_matrix(row + i, row + i, 1);
        matrix->into_matrix(row + i, pointCount * 2 + i, - timeIncrement);
        matrix->into_constants(row + i, - (solution[pointCount + i] - (prevSolution[pointCount + i] + solution[pointCount * 2 + i] * timeIncrement)));
    }
}

void MathModel::addVS(VoltageSource &element) {
    int firstPoint = element.getFirstPoint();
    int secondPoint = element.getSecondPoint();
    int number = pointCount * 3 + element.getNumberVS();
    int column1 = pointCount * 2 + firstPoint - 1;
    int column2 = pointCount * 2 + secondPoint - 1;
    int row = number;
    double potentialDiff = 0;
    if(firstPoint != 0) {
        matrix->into_matrix(row, column1, 1);
        potentialDiff -= solution[column1];
        matrix->into_matrix(column1, number, 1);
        matrix->into_constants(column1, -solution[number]);
    }
    if(secondPoint != 0) {
        matrix->into_matrix(row, column2, -1);
        potentialDiff += solution[column2];
        matrix->into_matrix(column2, number, -1);
        matrix->into_constants(column2, solution[number]);
    }
    matrix->into_constants(row, potentialDiff - element.getTempPotential(tempTime));
}

void MathModel::addCapasitor(Capacitor &capacitor) {
    capacitors.push_back(capacitor);
}

void MathModel::addElectricitySource(ElectricitySource &ES) {
    this->ES.push_back(ES);
}

void MathModel::addInductor(Inductor &inductor) {
    inductors.push_back(inductor);
}

void MathModel::addResistor(Resistor &resistor) {
    resistors.push_back(resistor);
}

void MathModel::addVoltageSource(VoltageSource &VS) {
    this->VS.push_back(VS);
}

static double maxElement(int dimension, double mDelta[]) {
    double max = 0.0;
    for (int i = 0; i < dimension; i++)
        if (abs(mDelta[i]) > max)
            max = abs(mDelta[i]);
    return max;
}

void MathModel::resolve() {

    matrix->reset();
    prevSolution = matrix->get_solution();
    solution = matrix->get_solution();

    ofstream out("fout.txt");

    double prevStep[] = {timeIncrement, timeIncrement};
    double controlValue = 0;
    int flagContinue = 1;
    double deviationE = 0.01;
    double deviationLocal;
    double deviationE1E2[] = {0.01, 0.05};
    int count7;
    int countAll = 0;

    while (tempTime < finishTime) {
        timeIncrement = prevStep[1];
        count7 = 0;
        flagContinue = 1;

        while (flagContinue == 1) {
            matrix->reset();
            firstSet();
            secondSet();
            for(int i = 0; i < capacitors.size(); ++i) {
                addElement(capacitors[i]);
            }
            for(int i = 0; i < ES.size(); i++) {
                double potential1 = solution[pointCount * 2 + ES[i].getFirstPoint() - 1];
                double potential2 = solution[pointCount * 2 + ES[i].getSecondPoint() - 1];
                ES[i].setPotentialDifference(potential1 - potential2);
                addDifficultElement(ES[i]);
            }
            for (int i = 0; i < inductors.size(); ++i) {
                addElement(inductors[i]);
            }
            for (int i = 0; i < resistors.size(); ++i) {
                addElement(resistors[i]);
            }
            for (int i = 0; i < VS.size(); ++i) {
                addVS(VS[i]);
            }
            matrix->solution2();

            double* tempSolution = matrix->get_solution();

            for(int i = 0; i < pointCount * 3 + idealSourceCount; i++) {
                solution[i] += tempSolution[i];
            }

            count7++;
            countAll++;

            if (deviationE < maxElement(pointCount * 3 + idealSourceCount, tempSolution))
                flagContinue = 1;
            else
                flagContinue = 0;

            delete tempSolution;

            if (count7 > 7 && flagContinue == 1) {
                count7 = 0;
                timeIncrement /= 2;

                for(int i = 0; i < pointCount * 3 + idealSourceCount; i++) {
                    solution[i] = prevSolution[i];
                }
            }
        }
        deviationLocal = abs(prevStep[1] / (prevStep[1] + prevStep[0]) * ((solution[pointCount * 2] - prevSolution[pointCount * 2]) - prevStep[1] / prevStep[0] * (prevSolution[pointCount * 2] - controlValue)));
        if (deviationLocal > deviationE1E2[1]) {
            prevStep[1] /= 2;

            for(int i = 0; i < pointCount * 3 + idealSourceCount; i++) {
                solution[i] = prevSolution[i];
            }

            std::cout << "Step-" << std::endl;
        } else {
            controlValue = prevSolution[pointCount * 2];
            for(int i = 0; i < pointCount * 3 + idealSourceCount; i++) {
                prevSolution[i] = solution[i];
            }
            out << tempTime << '\t';
            for (int i = pointCount * 2; i < pointCount * 3; ++i) {
                out << solution[i] << '\t';
            }
            out << (inductors[0].getValue() * (solution[pointCount] - solution[pointCount + 1]));
            out << '\n';

            prevStep[0] = timeIncrement;
            tempTime += timeIncrement;
            if (deviationLocal < deviationE1E2[0] && prevStep[1] < 1e-6) {
                prevStep[1] *= 2;
                std::cout << "Step+" << std::endl;
            }
        }
    }
    out.close();
    std::cout << countAll;
}