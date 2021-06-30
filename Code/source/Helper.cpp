#include "../header/Helper.h"

int** Helper::initializeIntegerMatriz(int length)
{
    int **pij = new int *[length];

    for(int i = 0; i < length; i++)
    {
        pij[i] = new int[length];
    }
    return pij;
}

int** Helper::initializeIntegerMatriz(int length, int width)
{
    int **pij = new int *[length];

    for(int i = 0; i < length; i++)
    {
        pij[i] = new int[width];
    }
    return pij;
}

float** Helper::initializeFloatMatriz(int length)
{
    float **dij = new float *[length];

    for(int i = 0; i < length; i++)
    {
        dij[i] = new float[length];
    }
    return dij;
}

