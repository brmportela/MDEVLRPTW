#ifndef VEHICLE_H_INCLUDED
#define VEHICLE_H_INCLUDED

#include <iostream>

using namespace std;

/**
*   Classe que representa um ve�culo capacitado
*   para a resolu��o do problema de roteamento
*   de ve�culos capacitados
**/
class Vehicle
{
    private:
        int id; // Id do ve�culo
        float initialCapacity; // Capacidade m�xima do ve�culo
        float capacity; // Capacidade atual do ve�culo
        float inicialBatteryCap;
        float batteryCap;



    public:
        Vehicle(int id, float initialCapacity, float inicialBatteryCap);

        int getId();

        // Retorna a capacidade m�xima
        float getInitialCapacity();

        // Retorna a capacidade atual
        float getCapacity();

        // Aumenta a capacidade do ve�culo, caso o problema seja de coleta (Limitado � capacidade m�xima)
        void addCapacity(float increase);

        // Diminui a capacidade do ve�culo, caso o problema seja de entrega
        void removeCapacity(float decrease);

        // Restaura a capacidade do ve�culo para a capacidade m�xima
        void resetCapacity();

         // Retorna a capacidade de bateria m�xima
        float getInitialBatteryCap();

        // Retorna a capacidade de bateria atual
        float getBatteryCap();

        // Aumenta a capacidade de bateria do ve�culo.
        void addBatteryCap(float increase);

        // Diminui a capacidade de bateria do ve�culo.
        void removeBatteryCap(float decrease);

        // Restaura a capacidade de bateria do ve�culo para a capacidade m�xima
        void resetBatteryCap();
};

#endif // VEHICLE_H_INCLUDED
