#ifndef Heuristic_H
#define Heuristic_H

#include "../Helper.h"
#include "../FileHelper.h"

#include "Vehicle.h"
#include "Instance.h"
#include "../grafo/Grafo.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <memory>

#include <stdio.h>
#include <stdlib.h>



typedef std::shared_ptr<Vertice> vertice_s;
typedef std::shared_ptr<Vehicle> vehicle_s;

using namespace std;
using namespace std::chrono;

struct BestInsert
{
    float cost;
    int successor;
};

struct NBSS
{
    float cost;
    int id;
};

struct BestRoute
{
    float cost;
    vector<int> aux;
};

struct NoRepetido
{
    int noX;
    int noY;
    float noValor;
};

struct Time
{
    float clearance;
    float wait;
    float in;
    float out;
};

struct Individuo
{
    vector<float> keys;
    float custo;
};

struct Rrota
{
    int key;
    float custo;
};

struct Solucao
{
    vector<vector<vertice_s>> solution;
    vector<vector<Time>> sTime;
    vector<vehicle_s> truck;
};

/**
*   Heurística para o problema de roteamento de veículos capacitados
**/
class Heuristic
{
    private:
        float temp;
        int nodeBase; // Vértice base do problema
        float vehicleCapacity; // Capacidade máxima do veículo utilizado no problema
        float batteryCap;
        int costBss;
        float minCost;
        int rotaCount;
        string filename;
        std::shared_ptr<Grafo> grafo; // Grafo do problema
        vector<vertice_s> route; // Subconjuntos de vértices da solução que representa uma rota
        vector<vertice_s> points;
        vector<vector<vertice_s>> solution; // Vetor de rotas da solução
        vector<Solucao> solutions;
        vector<vertice_s> bss;
        vector<vertice_s> dep;
        vector<vertice_s> routeAux;
        vector<int> abss;
        vector<vehicle_s> truck;
        vector<vector<Time>> sTime;
        ofstream solucao;
        fstream nK;
        int clients;
        float **dij;
        int **pij;
        int *vi;
        float *ti;
        bool ready; // Indica se a instância foi construída corretamente
        void order(); // Ordena vértices pelo ângulo
        int sweep(); // Heurística de varredura
        void tsp();
        void addBss();
        bool opt();
        void vnd();
        bool shift();
        bool swap1();
        bool reinsertion();
        bool reinsertion2();
        bool changeBss();
        bool cross();
		bool isolate();
        void imb();
        bool viability();
        ofstream in;
        int bestIn;
        void rbss();
        float heuristic(float alfa); // Heurística da inserção mais próxima
        bool valido(vector<int> rota);
        bool valido2(vector<int> rota);
        bool valido3(vector<int> rota);
        int cBss;
        int cRotas;
        int pRotas;
        int pointCount;
        void preparation();
        void perturbation(int g);
        void eraseBss();
        void recontTruck(int i);
        int depCount;
        int stationCount;
        int clientCount;
        int noClientCount;
        float vehicleRefueling;
        int qtRotas;
        bool erro;
        int rotaExtra;

        float heuristicB(float alfa);
        float heuristicC(float alfa);
        void ag(int geracoes);
        Individuo crossover();
        Individuo crossover2();
        Individuo individuo2;
        Individuo mutacao(Individuo individuo);
        Individuo geraIndividuo();
        void ordena(int left, int right);
        void imbB(Individuo individuo);
        vector<Individuo> population;
        int gera;

        void dRotas();
        bool shiftRotas();
        bool routeBlast();
        float heuristicD(float alfa);
        bool testPrint;
        vector<int> exRota;
        bool tRand;
        float gResult;

    public:
        Heuristic(string filename);
        ~Heuristic();
        void apaga();
        bool isReady();
        void plot();
        void printSol(float tempT);
        float run();
        float getMinCost();
        int getCBss();
        int getCRotas();
        int getPRotas();
        int getCDep();
        float calcCusto();
        float calcCusto(vector<int> rota);
        float calcCustoT(vector<int> rota);
        void sdm(int percent);
        void updateClearance(int i);
        bool updateClearance2(int i);
        void updateTime(int i);
        void updateBss();
        void clearFull();
        void telaSol();
        void incRota(int rE);
        int numK;
        float runB();
        float runD();
        float getGResult();
};

#endif // Heuristic_H
