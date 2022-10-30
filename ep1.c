//
// Created by beloin on 10/24/22.
//

#include "stdio.h"
#include <malloc.h>

// Utils
double **CreatePrimitiveMatrix(int i, int j);
void ShowPrimitiveMatrix(double **m, int i, int j);

// Force the return to be one of those: C, S, E or F
char PrintMainMenuAndReturnOption();

void NumericalConversionMenu();

void LinearSystemMenu();

void AlgebraicEquationMenu();


int main() {
    char option; // C, S, E or F

    while (1) {
        option = PrintMainMenuAndReturnOption();

        if (option == 'C') {
            NumericalConversionMenu();
        }

        if (option == 'S') {
            LinearSystemMenu();
        }

        if (option == 'E') {
            AlgebraicEquationMenu();
        }

        if (option == 'F') {
            break;
        }
    }

    return 0;
};

// START: UTILS
double **CreatePrimitiveMatrix(int i, int j) {
    int k, l;
    double **matrix;
    matrix = malloc(sizeof(double) * i);
    if (matrix == NULL) {
        return NULL;
    }

    for (k = 0; k <= i; ++k) {
        matrix[k] = malloc(sizeof(double) * j);
        if (matrix[k] == NULL) {
            for (l = 0; l < k; ++k) {
                free(matrix[l]);
            }
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}
void ShowPrimitiveMatrix(double **m, int i, int j) {
    printf("{\n");
    for (int k = 0; k < i; ++k) {
        printf("    ");
        for (int l = 0; l < j; ++l) {
            printf("%10.3f", m[k][l]);
            if (l != i - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    printf("}\n");
}
// END: UTILS


// START: LinearSystemMenu
void getResults(double **mx, double **output);

void LinearSystemMenu() {

}

void getResults(double **mx, double **output){

}

// END: LinearSystemMenu
