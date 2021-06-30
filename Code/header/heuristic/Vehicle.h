#ifndef VEHICLE_H_INCLUDED
#define VEHICLE_H_INCLUDED

#include <iostream>

using namespace std;

/**
*   Classe que representa um veículo capacitado
*   para a resolução do problema de roteamento
*   de veículos capacitados
**/
class Vehicle
{
    private:
        int id; // Id do veículo
        float initialCapacity; // Capacidade máxima do veículo
        float capacity; // Capacidade atual do veículo
        float inicialBatteryCap;
        float batteryCap;



    public:
        Vehicle(int id, float initialCapacity, float inicialBatteryCap);

        int getId();

        // Retorna a capacidade máxima
        float getInitialCapacity();

        // Retorna a capacidade atual
        float getCapacity();

        // Aumenta a capacidade do veículo, caso o problema seja de coleta (Limitado à capacidade máxima)
        void addCapacity(float increase);

        // Diminui a capacidade do veículo, caso o problema seja de entrega
        void removeCapacity(float decrease);

        // Restaura a capacidade do veículo para a capacidade máxima
        void resetCapacity();

         // Retorna a capacidade de bateria máxima
        float getInitialBatteryCap();

        // Retorna a capacidade de bateria atual
        float getBatteryCap();

        // Aumenta a capacidade de bateria do veículo.
        void addBatteryCap(float increase);

        // Diminui a capacidade de bateria do veículo.
        void removeBatteryCap(float decrease);

        // Restaura a capacidade de bateria do veículo para a capacidade máxima
        void resetBatteryCap();
};

#endif // VEHICLE_H_INCLUDED
