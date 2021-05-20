#include <stdio.h>
#include "method1_init.h"
int method1_init(int n, double a, double b, double *values, double *derivatives, double *state)
{
    double h;
    double *d;
    d = (double *)malloc(n * sizeof(double));
    h = (b - a) / (double)(n - 1);
    for (int i = 0; i < n; i++)
    {
        d[i] = 0;
    }
    for (int i = 1; i < n - 1; i++)
    {
        d[i] = (values[i + 1] - values[i - 1]) / h;
    }
    d[0] = derivatives[0];
    d[n - 1] = derivatives[1];
    for (int i = 0; i < 4 * (n - 1); i += 4)
    {
        state[i] = values[i / 4];
        state[i + 1] = d[i / 4];
        state[i + 2] = (((values[i / 4 + 1] - values[i / 4]) / h) - d[i / 4]) / h;
        state[i + 3] = (d[i / 4] + d[i / 4 + 1] - 2 * ((values[i / 4 + 1] - values[i / 4]) / h)) / (h * h);
    }
    state[4 * (n - 1)] = values[(n - 1)];
    state[4 * (n - 1) + 1] = 0;
    state[4 * (n - 1) + 2] = 0;
    state[4 * (n - 1) + 3] = 0;
    free(d);
    return 0;
}