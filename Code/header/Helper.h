#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <iostream>
#include <climits>

#define INFINITE            9999999
#define SCALERATIO          1000000

using namespace std;

// Estrutura que guarda o custo entre dois nós e a quantidade de nós intermediários
struct Floyd
{
    int** pij;
    float** dij;
};

//  Estrutura que guarda o cálculo de uma inserção na solução
struct InsertCalculation
{
    int nodeId;
    int difPath;
    int idVertice1;
    int idVertice2;
};

// Tipo de heurística usada
enum HeuristicType
{
    Gulosa, Randomizada, Reativa
};

/**
*   Classe que contêm métodos auxiliares diversos
**/
class Helper
{
    public:
        static int** initializeIntegerMatriz(int length);
        static int** initializeIntegerMatriz(int length, int width);
        static float** initializeFloatMatriz(int length);
};

#endif // HELPER_H_INCLUDED
