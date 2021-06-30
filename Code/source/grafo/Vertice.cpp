#include "../../header/grafo/Vertice.h"

Vertice::Vertice()
{
    this->id = 0;
    this->weight = 0;
    this->coord = NULL;
    this->angle = 0;
    this->readyTime = 0;
    this->dueTime = 0;
    this->serviceTime = 0;
    this->clearance = 0;
    this->wait = 0;
    this->in = 0;
    this->out = 0;
}

void Vertice::setId(int idVertice)
{
    this->id = idVertice;
}

int Vertice::getId()
{
    return this->id;
}

std::shared_ptr<Point> Vertice::getCoord()
{
    return this->coord;
}

void Vertice::setCoord(std::shared_ptr<Point> coord)
{
    this->coord = coord;
}

float Vertice::getWeight()
{
    return this->weight;
}

void Vertice::setWeight(float weight)
{
    this->weight = weight;
}

float Vertice::getAngle()
{
    return this->angle;
}

void Vertice::setAngle(float angle)
{
    this->angle = angle;
}


float Vertice::getReadyTime()
{
    return this->readyTime;
}

void Vertice::setReadyTime(float readyTime)
{
    this->readyTime = readyTime;
}

float Vertice::getDueTime()
{
    return this->dueTime;
}

void Vertice::setDueTime(float dueTime)
{
    this->dueTime = dueTime;
}

float Vertice::getServiceTime()
{
    return this->serviceTime;
}

void Vertice::setServiceTime(float serviceTime)
{
    this->serviceTime = serviceTime;
}

float Vertice::getClearance()
{
    return this->clearance;
}

void Vertice::setClearance(float clearance)
{
    this->clearance = clearance;
}

float Vertice::getWait()
{
    return this->wait;
}

void Vertice::setWait(float wait)
{
    this->wait = wait;
}

float Vertice::getIn()
{
    return this->in;
}

void Vertice::setIn(float in)
{
    this->in = in;
}

float Vertice::getOut()
{
    return this->out;
}

void Vertice::setOut(float out)
{
    this->out = out;
}
