//
// Created by note on 02.03.17.
//

#include "MathModel.h"

MathModel::MathModel(int pointCount, int idealSourceCount, double finishTime, double timeIncrement) :
        pointCount(pointCount),
        idealSourceCount(idealSourceCount),
        matrix(new Gauss(pointCount * 3 + idealSourceCount)),
        finishTime(finishTime),
        timeIncrement(timeIncrement),
        tempTime(0) {
}

double E(double t) {
    double A = 10.0;
    double T = 1e-4;
    double phase = 0.0;
    double f = 2 * M_PI / T;
    return A * sin(f * t + phase);
}

void gauss(double** A, double* B) {
    double a;
    double b;
    double sum;
    int dimension = 13;
//    ïðÿìîé õîä
    for (int i = 0; i < dimension; i++) {
        if (A[i][i] == 0.0) {
            double maxD = 0.0;
            int maxI = i;
            for (int j = i + 1; j < dimension; j++) {
                if (abs(A[j][i]) > maxD) {
                    maxD = abs(A[j][i]);
                    maxI = j;
                }
            }
            double* tmpStrA = new double[dimension];
            double tmpStrB;
            for (int str = 0; str < dimension; str++) {
                tmpStrA[str] = A[maxI][str];
                A[maxI][str] = A[i][str];
                A[i][str] = tmpStrA[str];
            }
            tmpStrB = B[maxI];
            B[maxI] = B[i];
            B[i] = tmpStrB;
        }

        a = A[i][i];
        for (int l = 0; l < dimension; l++) {
            A[i][l] = A[i][l] / a;
        }
        B[i] = B[i] / a;

        a = A[i][i];
        for (int j = i + 1; j < dimension; j++) {
            b = A[j][i];
            for (int k = i; k < dimension; k++)
                A[j][k] = A[i][k] * b - A[j][k] * a;
            B[j] = B[i] * b - B[j] * a;
        }
    }
//    îáðàòíûé õîä
    for (int i = dimension - 1; i >= 0; i--) {
        sum = 0.0;
        for (int j = i + 1; j < dimension; j++)
            sum += A[i][j] * B[j];
        sum = B[i] - sum;
        B[i] = sum / A[i][i];
    }
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
//    std::cout << column1 << ' ' << column2 << std::endl;
//    std::cout << row << std::endl;
//    mEq[8] += -(-solution[12]
//                + C1 * (solution[0] - solution[1])
//                + (1.0 / L1) * (solution[4] - solution[5]));
//    mEq[9] += -(-(C1 * (solution[0] - solution[1])
    if(firstPoint != 0) {
        matrix->into_matrix(row, column1, value);
        matrix->into_matrix(row, column2, secondPoint == 0 ? 0 : - value);
        //std::cout << row << ' ' << ( firstPoint == 0 ? 0 : solution[column1]) << ' ' << (secondPoint == 0 ? 0 : solution[column2]) << endl;
        matrix->into_constants(row, -value * variablesDiff);
    }
    row = pointCount * 2 + secondPoint - 1;
    //std::cout << row << std::endl;
    if(secondPoint != 0) {
        matrix->into_matrix(row, column1, firstPoint == 0 ? 0 : - value);
        matrix->into_matrix(row, column2, value);
        //std::cout << row << ' ' << (variablesDiff) << endl;
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
    int row = 12;
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

//    while(tempTime < finishTime) {
//
//        //matrix->print_system();
//
//        delete[] prevSolution;
//        prevSolution = solution;
//        solution = matrix->get_solution();
//
//        tempTime += timeIncrement;
//
//        for (int i = 0; i < pointCount * 3 + idealSourceCount; ++i) {
//            out << solution[i] << '\t';
//        }
//        out << '\n';
//    }

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


            
            //double C1 = capacitors[0].getValue();
            //double CB = capacitors[1].getValue();
            //double L1 = inductors[0].getValue();
            //double RU = resistors[0].getValue(), RB = resistors[1].getValue(), R1 = resistors[2].getValue();
            const double It = 1e-12;
            const double MFt = 0.026;
            int dimension = 13;
            double** mY = matrix->getMatrix();
            double* mEq = matrix->getConstants();

//            mY[0][0] = 1.0;
//            mY[0][8] = -1.0 / timeIncrement;
//
//            mY[1][1] = 1.0;
//            mY[1][9] = -1.0 / timeIncrement;
//
//            mY[2][2] = 1.0;
//            mY[2][10] = -1.0 / timeIncrement;
//
//            mY[3][3] = 1.0;
//            mY[3][11] = -1.0 / timeIncrement;
//            //
//            mY[4][4] = 1.0;
//            mY[4][8] = -timeIncrement;
//
//            mY[5][5] = 1.0;
//            mY[5][9] = -timeIncrement;
//
//            mY[6][6] = 1.0;
//            mY[6][10] = -timeIncrement;
//
//            mY[7][7] = 1.0;
//            mY[7][11] = -timeIncrement;
//            //
            //mY[8][0] += C1;
            //mY[8][1] += -C1;
            //mY[8][4] += 1.0 / L1;
            //mY[8][5] += -1.0 / L1;
            //mY[8][12] += -1.0;

            //mY[9][0] += -C1;
            //mY[9][1] += /*C1*/ + CB;
            //mY[9][2] += -CB;
            //mY[9][4] += -1.0 / L1;
            //mY[9][5] += 1.0 / L1;
            //mY[9][9] += /*1.0 / RU*/ + (It / MFt) * exp(-(solution[9] - solution[10]) / MFt);
            //mY[9][10] += /*-1.0 / RU*/ - (It / MFt) * exp(-(solution[9] - solution[10]) / MFt);

            //mY[10][1] += -CB;
            //mY[10][2] += CB;
            //mY[10][9] += /*-1.0 / RU*/ - (It / MFt) * exp(-(solution[9] - solution[10]) / MFt);
            //mY[10][10] += /*1.0 / RB + 1.0 / RU*/ + (It / MFt) * exp(-(solution[9] - solution[10]) / MFt);
            //mY[10][11] += -1.0 / RB;

            //mY[11][10] += -1.0 / RB;
            //mY[11][11] += 1.0 / R1 + 1.0 / RB;
            //
            //mY[12][8] += 1.0;
            //
//
//            //Çàïîëíåíèå âåêòîðà óðàâíåíèé
//            mEq[0] = -(solution[0] - (solution[8] - prevSolution[8]) / timeIncrement);
//            mEq[1] = -(solution[1] - (solution[9] - prevSolution[9]) / timeIncrement);
//            mEq[2] = -(solution[2] - (solution[10] - prevSolution[10]) / timeIncrement);
//            mEq[3] = -(solution[3] - (solution[11] - prevSolution[11]) / timeIncrement);
//
//            mEq[4] = -(solution[4] - (prevSolution[4] + solution[8] * timeIncrement));
//            mEq[5] = -(solution[5] - (prevSolution[5] + solution[9] * timeIncrement));
//            mEq[6] = -(solution[6] - (prevSolution[6] + solution[10] * timeIncrement));
//            mEq[7] = -(solution[7] - (prevSolution[7] + solution[11] * timeIncrement));
//
            //mEq[8] +=  -(-solution[12]);
                       //+ (1.0 / L1) * (solution[4] - solution[5]));
            //std::cout << 8 << ' ' << solution[0] << ' ' << solution[1] << endl;
            //mEq[8] += - C1 * (solution[0] - solution[1]);
            //mEq[9] +=  ///////////////////////////////////////////
            //          -(-(//(1.0 / L1) * (solution[4] - solution[5])
                         //+ CB * (solution[2] - solution[1])
                         //+ (1.0 / RU) * (solution[10] - solution[9])
            //             + It * (exp((solution[10] - solution[9]) / MFt) - 1.0)));
            //std::cout << 9 << ' ' << (solution[0] - solution[1]) << endl;
            //mEq[9] += C1 * (solution[0] - solution[1]);
            //mEq[10] += -(//CB * (solution[2] - solution[1])
                        //+ (1.0 / RU) * (solution[10] - solution[9])
            //            + It * (exp((solution[10] - solution[9]) / MFt) - 1.0));
                        //- (1.0 / RB) * (solution[11] - solution[10]));
            //mEq[11] += -((1.0 / RB) * (solution[11] - solution[10]) + (1.0 / R1) * (solution[11]));
            //mEq[12] += -(solution[8] - E(tempTime));








//            matrix->print_system();
            gauss(mY, mEq);
//            matrix->triangle();
//            matrix->do_solution();
            matrix->setSolution(mEq);

            double* tempSolution = matrix->get_solution();
//            std::cout << countAll << ": ";
//            for (int i = 0; i < pointCount * 3 + idealSourceCount; ++i) {
//                std::cout << tempSolution[i] << '\t';
//            }
//            std::cout << '\n';

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
        //std::cout << deviationLocal << " " << deviationE1E2[1] << '\n';
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
            for (int i = 8; i < pointCount * 3 + idealSourceCount; ++i) {
                out << solution[i] << '\t';
            }
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