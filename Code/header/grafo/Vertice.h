#ifndef VERTICE_H_INCLUDED
#define VERTICE_H_INCLUDED

//#include "Adjacencia.h"

#include <iostream>
#include <memory>
#include "../structure/Point.h"

using namespace std;

/**
*   Classe que representa um vértice do grafo
**/
class Vertice
{
    private:
        int id;
        float weight; // Peso do vértice
        float angle; // Ângulo formado entre este vértice e o vértice inicial
        std::shared_ptr<Point> coord; // Coordenada do vértice

        float readyTime;
        float dueTime;
        float serviceTime;

        float clearance;
        float wait;

        float in;
        float out;

    public:
        Vertice();
//        ~Vertice();

        void setId(int idVertice);
        int getId();
        std::shared_ptr<Point> getCoord();
        void setCoord(std::shared_ptr<Point> coord);


        int getGrau();
        float getWeight();
        void setWeight(float weight);
        float getAngle();
        void setAngle(float angle);

        float getReadyTime();
        void setReadyTime(float readyTime);
        float getDueTime();
        void setDueTime(float dueTime);
        float getServiceTime();
        void setServiceTime(float serviceTime);

        float getClearance();
        void setClearance(float clearance);
        float getWait();
        void setWait(float wait);

        float getIn();
        void setIn(float in);
        float getOut();
        void setOut(float out);

        void addAdjacencia(int idVertice1, int idVertice2, float weight);
        void removeAdjacencia(int idVertice2);
        bool existsAdjacencia(int idVertice2);
};

#endif // VERTICE_H_INCLUDED
