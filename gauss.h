//
// Created by note on 02.02.17.
//

#ifndef ARINATRUD2_GAUSS_H
#define ARINATRUD2_GAUSS_H


#include <iostream>
#include <cmath>
//#define capacity 3
#define TROUBLE 0.000001
using namespace std;

class Gauss {
    double** matrix;
    double* constants;
public:
    double **getMatrix() const;

    double *getConstants() const;

private:
public:
    void setSolution(double *solution);

private:
    double* solution;
    //double TROUBLE;
    int capacity;
public:
    Gauss(int n);
    ~Gauss();
    int into_matrix(int, int, double);
    int into_constants(int, double);
    int clearStr(int);
    int triangle();
    int do_solution();
    double* get_solution();
    int reset();
    int print_system();
};


#endif //ARINATRUD2_GAUSS_H
