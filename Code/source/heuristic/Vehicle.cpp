#include "../../header/heuristic/Vehicle.h"

Vehicle::Vehicle(int id, float initialCapacity, float inicialBatteryCap)
{
    this->id = id;
    this->initialCapacity = initialCapacity;
    this->capacity = initialCapacity;
    this->inicialBatteryCap = inicialBatteryCap;
    this->batteryCap = inicialBatteryCap;
}

int Vehicle::getId()
{
    return this->id;
}

float Vehicle::getInitialCapacity()
{
    return this->initialCapacity;
}

float Vehicle::getCapacity()
{
    return this->capacity;
}

void Vehicle::addCapacity(float increase)
{
    float sum = this->capacity + increase;

    if(sum <= this->initialCapacity)
    {
        this->capacity = sum;
    }
}

void Vehicle::removeCapacity(float decrease)
{
    float sum = this->capacity - decrease;

    if(sum >= 0)
    {
        this->capacity = sum;
    }
    else
    {
        if(sum < 0)
        {
            cout << endl << "Passou Capacidade: " << sum << " - ";
        }
        this->capacity = 0;
    }
}

void Vehicle::resetCapacity()
{
    this->capacity = this->initialCapacity;
}

float Vehicle::getInitialBatteryCap()
{
    return this->inicialBatteryCap;
}

float Vehicle::getBatteryCap()
{
    return this->batteryCap;
}

void Vehicle::addBatteryCap(float increase)
{
    float sum = this->batteryCap + increase;

    if(sum < this->inicialBatteryCap)
    {
        this->batteryCap = sum;
    }
}

void Vehicle::removeBatteryCap(float decrease)
{
    float sum = this->batteryCap - decrease;

    //if(sum > 0)
    //{
        this->batteryCap = sum;
    //}
    //else
    //{
    //    this->batteryCap = 0;
    //}
}

void Vehicle::resetBatteryCap()
{
    this->batteryCap = this->inicialBatteryCap;
}
