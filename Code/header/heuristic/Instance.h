#ifndef Instance_H_INCLUDED
#define Instance_H_INCLUDED

#include "../grafo/Grafo.h"

#include <iostream>
#include <memory>

using namespace std;

/**
*   Classe que representa uma inst�ncia do problema CVRP
**/
class Instance
{
    private:
        int verticeCount; // Quantidade de v�rtices da inst�ncia
        int vehicleCapacity; // Capacidade do ve�culo utilizado

        int verticeBase; // Id do v�rtice da base
        float batteryCap; // Capacidade da bateria
        int costBss; // Custo para se construir uma BSS
        int bssCount; // Quantidade de candidatos a BSS

        int stationCount;
        int depCount; // Quantidade de dep�sitos da inst�ncia
        int clientCount;
        float vehicleConsum;
        float vehicleRefueling;
        float vehicleVelocity;
        float gResult;

        std::shared_ptr<Grafo> grafo; // Grafo do problema
    public:
        Instance();

        int getVerticeCount();
        void setVerticeCount(int verticeCount);

        int getDepCount();
        void setDepCount(int depCount);
        int getStationCount();
        void setStationCount(int stationCount);
        int getClientCount();
        void setClientCount(int clientCount);
        float getVehicleConsum();
        void setVehicleConsum(float vehicleConsum);
        float getVehicleRefueling();
        void setVehicleRefueling(float vehicleRefueling);
        float getVehicleVelocity();
        void setVehicleVelocity(float vehicleVelocity);

        int getVehicleCapacity();
        void setVehicleCapacity(float vehicleCapacity);

        int getVerticeBase();
        void setVerticeBase(int nodeId);

        float getBatteryCap();
        void setBatteryCap(float batteryCap);

        int getCostBss();
        void setCostBss(int costBss);

        int getBssCount();
        void setBssCount(int bssCount);

        void setVehicleCount(int vehicleCount);

        float getGResult();
        void setGResult(float gResult);

        void addVerticePoint(int idVertice, std::shared_ptr<Point> verticePoint); // Define as coordenadas do v�rtice
        void addVerticeInfos(int idVertice, float demand, float readyTime, float dueTime, float serviceTime); // Define a demanda do v�rtice
        void addVerticeAngle(int ideVertice, std::shared_ptr<Point> coord); // Define o �ngulo do v�rtice
        float getVerticeAngle(int idVertice);
        void orderAngle(int left, int right); // Quicksort no vetor de v�rtices com base nos �ngulos
        std::shared_ptr<Grafo> getInstance(); // Cria um grafo completo para o problema
};

#endif // Instance_H_INCLUDED
