#ifndef Grafo_H_INCLUDED
#define Grafo_H_INCLUDED

#include "Vertice.h"
#include "../structure/Point.h"

#include <iostream>
#include <vector>
#include <math.h>

typedef shared_ptr<Vertice> vertice_ptr;

using namespace std;

/**
*   Classe que representa um grafo
**/
class Grafo
{
    private:
        int verticeCount; // Quantidade de v�rtices
        vector<vertice_ptr> verticeList; // Vetor de v�rtices do grafo
        int bssCount;
        bool isDirected; // Define se o grafo � direcionado ou n�o
        int vehicleCount;
        bool depth(bool *visited); // Busca em profundidade
        bool depth(int idVertice1, int idVertice2); // Busca em profundidade para verificar caminho entre dois n�s

    public:
        Grafo(int verticeCount, bool isDirected);

        int getVerticeId(int idVertice);
        void setVerticeId(int indice, int idVertice);
        int getVerticeCount(); // Retorna a quantidade de v�rtices
        int getVerticeGrau(int idVertice); // Retorna o grau de um v�rtice
        float getVerticeAngle(int idVertice); // Retorna o �ngulo de um v�rtice
        void setVerticeAngle(int idVertice, float theta); // Define o �ngulo do v�rtice
        float getVerticeWeight(int idVertice); // Retorna o peso de um v�rtice
        void setVerticeWeight(int idVertice, float weight); // Define o peso de um v�rtice
        shared_ptr<Point> getVerticePoint(int idVertice); // Retorna a coordenada do v�rtice
        void setVerticePoint(int idVertice, shared_ptr<Point> p); // Define a coordenada do v�rtice
        bool existsVertice(int idVertice); // Verifica se um v�rtice existe no grafo
        void setBss(int bssCount);
        int getBssCount();
        void orderAngle(int left, int right); // Quicksort no vetor de v�rtices com base nos �ngulos
        void orderWeight(int left, int right);
        shared_ptr<Vertice> getVertice(int idVertice);
        int getVehicleCount();
        void setVehicleCount(int vehicleCount);

        float getVerticeReadyTime(int idVertice);
        void setVerticeReadyTime(int idVertice, float readyTime);
        float getVerticeDueTime(int idVertice);
        void setVerticeDueTime(int idVertice, float dueTime);
        float getVerticeServiceTime(int idVertice);
        void setVerticeServiceTime(int idVertice, float serviceTime);
};

#endif // Grafo_H_INCLUDED
