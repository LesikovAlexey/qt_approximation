#include <stdio.h>
#include <stdlib.h>
#include "method2_init.h"
int method2_init(int n, double a, double b, double *values, double *derivatives, double *state)
{
    double h;
    double *g = NULL;
    double *d = NULL;
    d = (double *)malloc(n * n * sizeof(double));
    g = (double *)malloc(n * sizeof(double));
    h = (b - a) / (double)(n - 1);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            d[i * n + j] = 0;
        }
        g[i] = 0;
    }
    for (int i = 1; i < n - 1; i++)
    {
        d[i + (i - 1) * n] = h;
        d[i + i * n] = 4 * h;
        d[i + (1 + i) * n] = h;
        g[i] = 3 * h * ((values[i] - values[i - 1]) / h + (values[i + 1] - values[i]) / h);
    }
    d[0] = 2;
    d[1] = 1;
    d[n - 1 + (n - 1) * (n - 3)] = 1;
    d[n - 1 + (n - 1) * (n - 2)] = 2;
    g[0] = 3 * (values[1] - values[0]) / h;
    g[n - 1] = 3 * (values[n - 1] - values[n - 2]) / h;
    for (int i = 0; i < n; i++)
    {
        for (int k = i + 1; k < n; k++)
        {
            d[i * n + k] = d[i * n + k] / d[i * n + i];
        }
        d[i * n + i] = 1;
        for (int j = i + 1; j < n; j++)
        {
            for (int k = i; k < n; k++)
            {
                d[j * n + k] = d[j * n + k] - d[i * n + k] * d[j * n + i];
            }
            g[j] = g[j] - g[i] * d[j * n + i];
        }
    }
    for (int i = n - 2; i > -1; i -= 1)
    {
        for (int j = i + 1; j < n; j++)
        {
            g[i] = g[i] - g[j] * d[i * n + j];
        }
        g[i] = g[i] / d[i * n + i];
    }
    for (int i = 0; i < 4 * (n - 1); i += 4)
    {
        state[i] = values[i / 4];
        state[i + 1] = g[i / 4];
        state[i + 2] = (((values[i / 4 + 1] - values[i / 4]) / h) - g[i / 4]) / h;
        state[i + 3] = (g[i / 4] + g[i / 4 + 1] - 2 * ((values[i / 4 + 1] - values[i / 4]) / h)) / (h * h);
    }
    state[4 * (n - 1)] = values[(n - 1)];
    state[4 * (n - 1) + 1] = 0;
    state[4 * (n - 1) + 2] = 0;
    state[4 * (n - 1) + 3] = 0;
    free(g);
    free(d);
    return 0;
}
