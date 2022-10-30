//
// Created by beloin on 10/29/22.
//

#include <stdio.h>
#include <malloc.h>

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

        varsTrack[j1] = j1;
        varsTrack[change] = change;
    }
}
