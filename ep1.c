//
// Created by beloin on 10/24/22.
//

#include "stdio.h"
#include <malloc.h>
#include "math.h"

// Needed Structs
typedef struct RetData_ {
    double **matrix;
    int n;
} RetData_;


// Utils
double **CreatePrimitiveMatrix(int i, int j);

void ShowPrimitiveMatrix(double **m, int i, int j);

RetData_ *ReadFileMatrix(char file_name[]);

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

RetData_ *ReadFileMatrix(char fileName[]) {
    int n, i, j;
    double **matrix;
    RetData_ *data = malloc(sizeof(RetData_));
    FILE *fp;

    fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("O arquivo não existe\n");
        perror("fopen");
    }
    fscanf(fp, " %d", &n);

    matrix = CreatePrimitiveMatrix(n, n + 1);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n + 1; j++) {
            fscanf(fp, " %lf", &matrix[i][j]);
        }
    }
//    fclose(fp);

    data->matrix = matrix;
    data->n = n;

    return data;
}
// END: UTILS


// START: LinearSystemMenu
void PrintResults(double **mx, int n);
int GetResultsAndReturnType(double **mx, double *results, int n);
void JordanMethod(double **m, int n);

void LinearSystemMenu() {
    char fileName[200] = "/home/beloin/Documents/aulas/8_sem/CN/Exercícios/project-01-cn-ep1/examples/matrix2.txt";
    printf("Por favor, informe o caminho absoluto do arquivo: ");
    scanf("%s", fileName);
    RetData_ *data = ReadFileMatrix(fileName);
    double **mx = data->matrix;
    int n = data->n;

    JordanMethod(mx, n);
    ShowPrimitiveMatrix(mx, n, n + 1);

    PrintResults(mx, n);
}

void PrintResults(double **mx, int n) {
    int type, i;
    double *results = malloc(sizeof(double) * n);

    type = GetResultsAndReturnType(mx, results, n);
    if (type == 2) {
        printf("O Sistema é incompatível.\n");
    } else {
        if (type == 1) {
            printf("O Sistema é indeterminado.\n");
        }
        printf("{ ");
        for (i = 0; i < n; ++i) {
            printf("X%d = %.2f ", i+1, results[i]);
        }
        printf("}");
    }
}

int GetResultsAndReturnType(double **mx, double *results, int n) {
    int type = 0;
    for (int i = 0; i < n; ++i) {
        if (mx[i][i] == 0 && mx[i][n] == 0) {
            results[i] = 0;
            type = 1;
            continue;
        }

        if (mx[i][i] == 0 && mx[i][n] != 0) {
            return 2;
        }

        results[i] = mx[i][n] / mx[i][i];
    }

    return type;
}

void JordanMethod(double **m, int n) {
    int i, j, k, l, i1, aux2, *varsTrack;
    double multiplier, aux, *aux3;

    // Keep track of the vars
    varsTrack = malloc(sizeof(int) * n);
    for (i1 = 0; i1 < n; ++i1) {
        varsTrack[i1] = i1;
    }

    for (i = 0; i < n; ++i) {
        if (m[i][i] == 0) { // Null pivot
            // Check for non-null column
            j = i + 1;
            while (j < n && m[i][j] == 0) {
                j++;
            }

            // Change null pivots to another, not null, column
            if (j < n) {
                // Keep track of changes
                // Column i, changed to j
                aux2 = varsTrack[i];
                varsTrack[i] = varsTrack[j];
                varsTrack[j] = aux2;

                // Column change
                for (l = 0; l < n; l++) {
                    aux = m[l][i];
                    m[l][i] = m[l][j];
                    m[l][j] = aux;
                }
            } else {
                for (l = 0; l < n; l++) {
                    m[l][i] = 0;
                }
            }
        }

        if (m[i][i] != 0) {
            for (j = 0; j < n; ++j) {
                if (j != i) {
                    multiplier = -(m[j][i] / m[i][i]);
                    m[j][i] = 0;
                    for (k = i + 1; k <= n; k++) {
                        m[j][k] = m[j][k] + (multiplier * m[i][k]);
                    }
                }
            }
        }
    }

    // RE-INVERT ROW ORDER
    for (int j1 = 0; j1 < n; ++j1) {
        if (varsTrack[j1] == j1) continue;
        int change = varsTrack[j1];
        aux3 = m[j1];
        m[j1] = m[change];
        m[change] = aux3;

        // Column change
        for (l = 0; l < n; l++) {
            aux = m[l][j1];
            m[l][j1] = m[l][change];
            m[l][change] = aux;
        }

        varsTrack[j1] = j1;
        varsTrack[change] = change;
    }
}


// END: LinearSystemMenu

// START: Menu
char PrintMainMenuAndReturnOption() {
    return 'S';
}
// END: Menu

// START: NumericalConversionMenu
void NumericalConversionMenu() {}
// END: NumericalConversionMenu

// START: AlgebraicEquationMenu
void AlgebraicEquationMenu() {}
// END: AlgebraicEquationMenu
