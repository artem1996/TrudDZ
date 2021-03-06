#include "gauss.h"

Gauss::Gauss(int n) {
    capacity = n;
    matrix = new double*[capacity];
    for(int i = 0; i < capacity; i++) {
        matrix[i] = new double[capacity];
    }
    constants = new double[capacity];
    solution = new double[capacity];
}

Gauss::~Gauss() {
    for(int i = 0; i < capacity; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] constants;
    delete[] solution;
}

int Gauss::into_matrix(int str, int column, double value) {
    matrix[str][column] += value;
    return 0;
}

int Gauss::into_constants(int str, double value) {
    constants[str] += value;
    return 0;
}

double* Gauss::get_solution() {
    double* answer = new double[capacity];
    for(int i = 0; i < capacity; i++) {
        answer[i] = solution[i];
    }
    return answer;
}

int Gauss::triangle() {
    for(int i = 0; i < capacity; i++) {
        if(fabs(matrix[i][i]) < TROUBLE) {
            int j;
            for(j = i + 1; j < capacity; j++) {
                if(fabs(matrix[j][i]) >= TROUBLE) {
                    double* temp_adress = matrix[j];
                    matrix[j] = matrix[i];
                    matrix[i] = temp_adress;
                    double temp = constants[j];
                    constants[j] = constants[i];
                    constants[i] = temp;
                    break;
                }
            }
            if(j == capacity) {
                cout << "Матрица похожа на вырожденную в строке: " << i + 1 << endl;
                return -1;
            }
        }
        for(int j = i + 1; j < capacity; j++) {
            if (fabs(matrix[j][i]) < TROUBLE) continue;
            double mult = matrix[j][i] / matrix[i][i];
            constants[j] -= constants[i] * mult;
            matrix[j][i] = 0;
            for(int k = i + 1; k < capacity; k++) {
                matrix[j][k] -= matrix[i][k] * mult;
            }
        }
    }
    return 0;
}

int Gauss::do_solution() {
    for(int i = capacity - 1; i > -1; i--) {
        double only_con = constants[i];
        for(int j = capacity - 1; j > i; j--) {
            only_con -= matrix[i][j] * solution[j];
        }
        solution[i] = only_con / matrix[i][i];
    }
    return 0;
}

int Gauss::reset() {
    for(int i = 0; i < capacity; i++) {
        for(int j = 0; j < capacity; j++) {
            matrix[i][j] = 0;
        }
        constants[i] = 0;
        solution[i] = 0;
    }
    return 0;
}

int Gauss::print_system() {
    for(int i = 0; i < capacity; i++) {
        for(int j = 0; j < capacity; j++) {
            cout << '\t' << matrix[i][j] << '\t';
        }
        cout << "\t = " << constants[i] << endl;
    }
    cout << ".................." << endl;
    return 0;
}

int Gauss::clearStr(int str) {
    for(int i = 0; i < capacity; i++) {
        matrix[str][i] = 0;
    }
    constants[str] = 0;
    return 0;
}

double **Gauss::getMatrix() const {
    return matrix;
}

double *Gauss::getConstants() const {
    return constants;
}

void Gauss::setSolution(double *solution) {
    Gauss::solution = solution;
}

int Gauss::solution2() {
    double a;
    double b;
    double sum;
    for (int i = 0; i < capacity; i++) {
        if (matrix[i][i] == 0.0) {
            double maxD = 0.0;
            int maxI = i;
            for (int j = i + 1; j < capacity; j++) {
                if (abs(matrix[j][i]) > maxD) {
                    maxD = abs(matrix[j][i]);
                    maxI = j;
                }
            }
            double* tmpStrA = new double[capacity];
            double tmpStrB;
            for (int str = 0; str < capacity; str++) {
                tmpStrA[str] = matrix[maxI][str];
                matrix[maxI][str] = matrix[i][str];
                matrix[i][str] = tmpStrA[str];
            }
            delete[] tmpStrA;
            tmpStrB = constants[maxI];
            constants[maxI] = constants[i];
            constants[i] = tmpStrB;
        }

        a = matrix[i][i];
        for (int l = 0; l < capacity; l++) {
            matrix[i][l] = matrix[i][l] / a;
        }
        constants[i] = constants[i] / a;

        a = matrix[i][i];
        for (int j = i + 1; j < capacity; j++) {
            b = matrix[j][i];
            for (int k = i; k < capacity; k++)
                matrix[j][k] = matrix[i][k] * b - matrix[j][k] * a;
            constants[j] = constants[i] * b - constants[j] * a;
        }
    }
//    îáðàòíûé õîä
    for (int i = capacity - 1; i >= 0; i--) {
        sum = 0.0;
        for (int j = i + 1; j < capacity; j++)
            sum += matrix[i][j] * constants[j];
        sum = constants[i] - sum;
        constants[i] = sum / matrix[i][i];
    }
    swap(solution, constants);
    return 0;
}
