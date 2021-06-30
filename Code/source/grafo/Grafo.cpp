#include "../../header/grafo/Grafo.h"
#include "../../header/Helper.h"


Grafo::Grafo(int verticeCount, bool isDirected)
{
    this->verticeCount = verticeCount;
    this->isDirected = isDirected;
    this->bssCount = 0;
    this->vehicleCount = 0;

    // Inicializa o vetor de vértices
    for(int i = 0; i < verticeCount; i++)
    {
        this->verticeList.push_back(make_shared<Vertice>());
    }
}

void Grafo::setBss(int bssCount)
{
    this->bssCount = bssCount;
    for(int j = 0; j < bssCount; j++)
    {
        this->verticeList.push_back(make_shared<Vertice>());
    }
}

int Grafo::getVehicleCount()
{
    return this->vehicleCount;
}

void Grafo::setVehicleCount(int vehicleCount)
{
    this->vehicleCount = vehicleCount;
}

int Grafo::getBssCount()
{
    return this->bssCount;
}

bool Grafo::existsVertice(int idVertice)
{
    return ((idVertice >= 0) && (idVertice < this->verticeCount + this->bssCount));
}

int Grafo::getVerticeId(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getId();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }

    return -1;
}

void Grafo::setVerticeId(int indice, int idVertice)
{
    if(existsVertice(indice))
    {
        (*this->verticeList[indice]).setId(idVertice);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

int Grafo::getVerticeCount()
{
    return this->verticeCount;
}

float Grafo::getVerticeWeight(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getWeight();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }

    return 0;
}

float Grafo::getVerticeAngle(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getAngle();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }

    return 0;
}

void Grafo::setVerticeAngle(int idVertice, float theta)
{
    if(existsVertice(idVertice))
    {
        (*this->verticeList[idVertice]).setAngle(theta);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

std::shared_ptr<Point> Grafo::getVerticePoint(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getCoord();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }

    return NULL;
}

void Grafo::setVerticePoint(int idVertice, std::shared_ptr<Point> coord)
{
    if(existsVertice(idVertice))
    {
        (*this->verticeList[idVertice]).setCoord(coord);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

void Grafo::setVerticeWeight(int idVertice, float weight)
{
    if(existsVertice(idVertice))
    {
        (*this->verticeList[idVertice]).setWeight(weight);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

void Grafo::setVerticeReadyTime(int idVertice, float readyTime)
{
    if(existsVertice(idVertice))
    {
        (*this->verticeList[idVertice]).setReadyTime(readyTime);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

float Grafo::getVerticeReadyTime(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getReadyTime();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
    return 0;
}

void Grafo::setVerticeDueTime(int idVertice, float dueTime)
{
    if(existsVertice(idVertice))
    {
        (*this->verticeList[idVertice]).setDueTime(dueTime);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

float Grafo::getVerticeDueTime(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getDueTime();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
    return 0;
}

void Grafo::setVerticeServiceTime(int idVertice, float serviceTime)
{
    if(existsVertice(idVertice))
    {
        (*this->verticeList[idVertice]).setServiceTime(serviceTime);
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
}

float Grafo::getVerticeServiceTime(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return (*this->verticeList[idVertice]).getServiceTime();
    }
    else
    {
        cout << "[ Erro ]: Nó não existente no grafo!" << endl;
    }
    return 0;
}

void Grafo::orderWeight(int left, int right)
{
    int i = left;
    int j = right;
    std::shared_ptr<Vertice> tmp = make_shared<Vertice>();
    float pivot = getVerticeWeight((left + right)/2);

    while (i <= j)
    {
        while (getVerticeWeight(i) < pivot && i < getVerticeCount())
        {
            i++;
        }
        while (getVerticeWeight(j) > pivot && j > 1)
        {
            j--;
        }
        if (i <= j)
        {
            tmp->setCoord(getVerticePoint(i));
            tmp->setWeight(getVerticeWeight(i));
            tmp->setAngle(getVerticeAngle(i));
            tmp->setId(getVerticeId(i));

            setVerticePoint(i, getVerticePoint(j));
            setVerticeWeight(i, getVerticeWeight(j));
            setVerticeAngle(i, getVerticeAngle(j));
            setVerticeId(i, getVerticeId(j));

            setVerticePoint(j, tmp->getCoord());
            setVerticeWeight(j, tmp->getWeight());
            setVerticeAngle(j, tmp->getAngle());
            setVerticeId(j, tmp->getId());

            i++;
            j--;
        }
    }
    if (left < j)
    {
        orderWeight(left, j);
    }
    if (i < right)
    {
        orderWeight(i, right);
    }
}

void Grafo::orderAngle(int left, int right)
{
    int i = left;
    int j = right;
    std::shared_ptr<Vertice> tmp = make_shared<Vertice>();
    float pivot = getVerticeAngle((left + right)/2);

    while (i <= j)
    {
        while (getVerticeAngle(i) < pivot && i < getVerticeCount())
        {
            i++;
        }
        while (getVerticeAngle(j) > pivot && j > 1)
        {
            j--;
        }
        if (i <= j)
        {
            tmp->setCoord(getVerticePoint(i));
            tmp->setWeight(getVerticeWeight(i));
            tmp->setAngle(getVerticeAngle(i));
            tmp->setId(getVerticeId(i));

            setVerticePoint(i, getVerticePoint(j));
            setVerticeWeight(i, getVerticeWeight(j));
            setVerticeAngle(i, getVerticeAngle(j));
            setVerticeId(i, getVerticeId(j));

            setVerticePoint(j, tmp->getCoord());
            setVerticeWeight(j, tmp->getWeight());
            setVerticeAngle(j, tmp->getAngle());
            setVerticeId(j, tmp->getId());

            i++;
            j--;
        }
    }
    if (left < j)
    {
        orderAngle(left, j);
    }
    if (i < right)
    {
        orderAngle(i, right);
    }
}


std::shared_ptr<Vertice> Grafo::getVertice(int idVertice)
{
    if(existsVertice(idVertice))
    {
        return verticeList[idVertice];
    }

    return NULL;
}


