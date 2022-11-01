//
// Created by beloin on 10/24/22.
//

#include "stdio.h"
#include <malloc.h>
#include "math.h"
#include <locale.h>
#include <stdlib.h>

// Estrutura para retorno da leitura do arquivo
typedef struct RetData_ {
    double **matrix;
    int n;
} RetData_;


// Utils - Funções que podem ser reutilizadas
double **CreatePrimitiveMatrix(int i, int j);

void ShowPrimitiveMatrix(double **m, int i, int j);

void FreePrimitiveMatrix(double **mx, int i);

RetData_ *ReadFileMatrix(char fileName[]);

// Force the return to be one of those: C, S, E or F
char PrintMainMenuAndReturnOption();

void NumericalConversionMenu();

void LinearSystemMenu();

void AlgebraicEquationMenu();

// ------NumericalConversionMenu------

void DecimalToBinary(int decimal);

void FractionToBinary(double fraction);

void DecimalWithFractionToBinary(double number);

void FractionToOctal(double fraction);

void DecimalToOctal(int number);

void DecimalWithFractionToOctal(double number);

void DecimalWithFractionToHexadecimal(int i, double f);

// ------NumericalConversionMenu------

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

/**
 * Cria uma matrix primitiva de double
 * @param i
 * @param j
 * @return
 */
double **CreatePrimitiveMatrix(int i, int j) {
    int k, l;
    double **matrix;
    matrix = malloc(sizeof(double) * i);
    if (matrix == NULL) {
        exit(1);
    }

    for (k = 0; k <= i; ++k) {
        matrix[k] = malloc(sizeof(double) * j);
        if (matrix[k] == NULL) {
            for (l = 0; l < k; ++k) {
                free(matrix[l]);
            }
            free(matrix);
            exit(1);
        }
    }

    return matrix;
}

/**
 * Limpa a matrix da memória
 * @param mx
 * @param i
 * @param j
 */
void FreePrimitiveMatrix(double **mx, int i) {
    int k;
    for (k = 0; k < i; ++k) {
//        free(mx[k]);
    }

    free(mx);
}

/**
 * Imprime a matrix primitiva de Double;
 * @param m
 * @param i
 * @param j
 */
void ShowPrimitiveMatrix(double **m, int i, int j) {
    int k, l;
    printf("{\n");
    for (k = 0; k < i; ++k) {
        printf("    ");
        for (l = 0; l < j; ++l) {
            printf("%10.3f", m[k][l]);
            if (l != i - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    printf("}\n");
}

/**
 * Lê o arquivo e retorna uma estrutura contendo os dados da matrix do sistema linear.
 * @param fileName
 * @return
 */
RetData_ *ReadFileMatrix(char fileName[]) {
    int n, i, j;
    double **matrix;
    RetData_ *data = malloc(sizeof(RetData_));
    if (data == NULL) {
        exit(1);
    }
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
    scanf(" %s", fileName);
    RetData_ *data = ReadFileMatrix(fileName);
    double **mx = data->matrix;
    int n = data->n;

    JordanMethod(mx, n);
    ShowPrimitiveMatrix(mx, n, n + 1);

    PrintResults(mx, n);

    free(data);
    FreePrimitiveMatrix(mx, n);
}

/**
 * Printa os resultados no terminal
 * @param mx
 * @param n
 */
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
            printf("X%d = %.2f ", i + 1, results[i]);
        }
        printf("}\n");
    }

    free(results);
}

/**
 * Adiciona os resultados ao array de resultados e retorna o tipo do sistema linear
 * @param mx
 * @param results
 * @param n
 * @return
 */
int GetResultsAndReturnType(double **mx, double *results, int n) {
    int i;
    int type = 0;
    for (i = 0; i < n; ++i) {
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

/**
 * Método de JOrndan
 * @param m
 * @param n
 */
void JordanMethod(double **m, int n) {
    int i, j, k, l, i1, aux2, *varsTrack, j1, change;
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
    for (j1 = 0; j1 < n; ++j1) {
        if (varsTrack[j1] == j1) continue;
        change = varsTrack[j1];
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
    char choice;

    printf("Escolha a funcionalidade:\n");
    printf("C - Conversão\n");
    printf("S - Sistema Linear\n");
    printf("E - Equação Algébrica\n");
    printf("F - Finalizar\n");
    do {
        printf("Opção: ");
        scanf(" %c", &choice);
    } while (choice != 'C' && choice != 'S' && choice != 'E' && choice != 'F');

    return choice;
}
// END: Menu

// START: NumericalConversionMenu

void conferirLetrasHexa(int value) {
    switch (value) {
        case 10:
            printf("A");
            break;
        case 11:
            printf("B");
            break;
        case 12:
            printf("C");
            break;
        case 13:
            printf("D");
            break;
        case 14:
            printf("E");
            break;
        case 15:
            printf("F");
            break;
        default:
            printf("%d", value);
            break;
    }
}

void DecimalWithFractionToHexadecimal(int i, double f) {
    int res_int[10], index_int = 0, index_frac = 0;
    double f_aux = f, res_fra[20];

    while (i >= 16) {
        res_int[index_int] = i % 16;
        i = (i - i % 16) / 16;
        index_int++;
    }
    res_int[index_int] = i;

    while (f_aux > 0 && index_frac < 20) {
        double k;
        f_aux = modf(f_aux * 16, &k);
        res_fra[index_frac] = k;
        index_frac++;
    }

    for (i = index_int; i >= 0; i--) {
        conferirLetrasHexa((int) res_int[i]);
    }
    if (f > 0) {
        printf(".");
        for (i = 0; i < index_frac; i++) {
            conferirLetrasHexa((int) res_fra[i]);
        }
    }
    printf("\n");
}

// Funcao para converter Decimal para Bin?rio
void DecimalToBinary(int decimal) {
    char aux[1000000];
    int i, index = 0, rest;

    // Divide o decimal por 2 enquanto ele for maior que 0
    while (decimal > 0) {
        // Guarda o rest das divisões
        rest = decimal % 2;
        aux[index++] = rest + '0';

        decimal /= 2;
    }

    // O numero binario em aux[] esta invertido: 12d --> 0011b.
    // abaixo ele sera consertado: 12d --> 0011b --> 1100b.
    for (i = 0; index > 0; index--, i++) {
        printf("%c", aux[index - 1]);
    }
}

void FractionToBinary(double fraction) {
    int rest, flag;

    // Remove a parte inteira
    fraction = fraction - (int) fraction;

    // A flag limita a 20 casas decimais
    flag = 0;
    while (fraction != 0.0 && flag < 20) {
        flag++;
        fraction *= 2;
        rest = (int) fraction;
        fraction -= rest;

        printf("%d", rest);
    }
}

void DecimalWithFractionToBinary(double number) {
    DecimalToBinary((int) number);
    printf(".");
    FractionToBinary(number);
    printf("\n");
}

void FractionToOctal(double fraction) {
    int flag, rest;
    // Remove a parte inteira
    fraction = fraction - (int) fraction;

    // A flag limita a 20 casas decimais
    flag = 0;
    while (fraction != 0.0 && flag < 20) {
        flag++;
        fraction *= 8;
        rest = (int) fraction;
        fraction -= rest;

        printf("%d", rest);
    }
}

// Funcao para converter Decimal para Octal
void DecimalToOctal(int number) {
    int valor_octal = 0, sequence = 1;

    // Enquanto o valor decimal for diferente de zero
    while (number != 0) {
        // Incrementa o valor octal com o resto da divisao do decimal por 8 multiplicado pelo sequencial
        valor_octal += (number % 8) * sequence;

        // O valor decimal sera dividido por 8
        number /= 8;

        // O sequencial sera multiplicado por 10
        sequence *= 10;
    }

    printf("%d", valor_octal);
}

void DecimalWithFractionToOctal(double number) {
    DecimalToOctal((int) number);
    printf(".");
    FractionToOctal(number);
    printf("\n");
}


void NumericalConversionMenu() {
    // Variaveis usadas
    double decimalValue, intPart, fracPart;

    // Inicio
    printf("Digite o numero decimal a ser convertido em Binario, Octal e Hexadecimal:\n");
    printf("Decimal: ");
    scanf(" %lf", &decimalValue);

    printf("---\n");
    printf("Binario: ");
    DecimalWithFractionToBinary(decimalValue);

    printf("Octal: ");
    DecimalWithFractionToOctal(decimalValue);

    printf("Hexadecimal: ");
    fracPart = modf(decimalValue, &intPart);
    DecimalWithFractionToHexadecimal(intPart, fracPart);
}
// END: NumericalConversionMenu

// START: AlgebraicEquationMenu
void AlgebraicEquationMenu() {}
// END: AlgebraicEquationMenu
