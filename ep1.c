//
// Created by beloin on 10/24/22.
//

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