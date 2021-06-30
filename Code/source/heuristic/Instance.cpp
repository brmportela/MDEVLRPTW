#include "../../header/heuristic/Instance.h"

Instance::Instance()
{
    this->verticeCount = 0;
    this->depCount = 0;
    this->stationCount = 0;
    this->clientCount = 0;
    this->verticeBase = -1;
    this->vehicleCapacity = 0;
    this->vehicleConsum = 1;
    this->vehicleRefueling = 0;
    this->vehicleVelocity = 1;
    this->batteryCap = 0;
    this->costBss = 0;
    this->bssCount = 0;
    this->grafo = NULL;
    this->gResult = 0;
}

std::shared_ptr<Grafo> Instance::getInstance()
{
    return grafo;
}

int Instance::getVerticeCount()
{
    return this->verticeCount;
}

void Instance::setVehicleCount(int vehicleCount)
{
    if(grafo != NULL)
    {
        grafo->setVehicleCount(vehicleCount);
    }
    else
    {
        cout << "[ Erro ] : Instância não encontrada" << endl;
    }
}

float Instance::getGResult()
{
    return this->gResult;
}

void Instance::setGResult(float gResult)
{
    this->gResult = gResult;
}

void Instance::setVerticeCount(int verticeCount)
{
    if(verticeCount >= 2)
    {
        this->verticeCount = verticeCount;
        this->grafo = make_shared<Grafo>(verticeCount, true);
    }
    else
    {
        cout << "[ Erro ] : Quantidade de vértices inválida" << endl;
    }
}

int Instance::getDepCount()
{
    return this->depCount;
}

void Instance::setDepCount(int depCount)
{
    this->depCount = depCount;
}

int Instance::getStationCount()
{
    return this->stationCount;
}

void Instance::setStationCount(int stationCount)
{
    this->stationCount = stationCount;
}

int Instance::getClientCount()
{
    return this->clientCount;
}

void Instance::setClientCount(int clientCount)
{
    this->clientCount = clientCount;
}

float Instance::getVehicleConsum()
{
    return this->vehicleConsum;
}

void Instance::setVehicleConsum(float vehicleConsum)
{
    this->vehicleConsum = vehicleConsum;
}

float Instance::getVehicleRefueling()
{
    return this->vehicleRefueling;
}

void Instance::setVehicleRefueling(float vehicleRefueling)
{
    this->vehicleRefueling = vehicleRefueling;
}

float Instance::getVehicleVelocity()
{
    return this->vehicleVelocity;
}

void Instance::setVehicleVelocity(float vehicleVelocity)
{
    this->vehicleVelocity = vehicleVelocity;
}


int Instance::getVehicleCapacity()
{
    return this->vehicleCapacity;
}

void Instance::setVehicleCapacity(float vehicleCapacity)
{
    if(vehicleCapacity >= 0)
    {
        this->vehicleCapacity = vehicleCapacity;
    }
    else
    {
        cout << "[ Erro ] : Capacidade do veículo inválida" << endl;
    }
}

int Instance::getVerticeBase()
{
    return this->verticeBase;
}

void Instance::setVerticeBase(int nodeId)
{
    if(grafo != NULL)
    {
        if((nodeId >= 0) && (nodeId < verticeCount))
        {
            this->verticeBase = nodeId;
            grafo->setVerticeWeight(verticeBase, 0);
        }
        else
        {
            cout << "[ Erro ] : Vértice da base inválido para a instância" << endl;
        }
    }
    else
    {
        cout << "[ Erro - VBase ] : Nao foi definida a quantidade de vertices para a instancia" << endl;
    }
}

float Instance::getBatteryCap()
{
    return this->batteryCap;
}

void Instance::setBatteryCap(float batteryCap)
{
    if(batteryCap >= 0)
    {
        this->batteryCap = batteryCap;

    }
    else
    {
        cout << "[ Erro ] : Capacidade da bateria inválida" << endl;
    }
}

int Instance::getCostBss()
{
    return this->costBss;
}

void Instance::setCostBss(int costBss)
{
    if(costBss >= 0)
    {
        this->costBss = costBss;

    }
    else
    {
        cout << "[ Erro ] : Custo de construção de BSS inválida" << endl;
    }
}

int Instance::getBssCount()
{
    return this->bssCount;
}

void Instance::setBssCount(int bssCount)
{
    if(bssCount >= 0)
    {
        this->bssCount = bssCount;
        this->grafo->setBss(bssCount);
    }
    else
    {
        cout << "[ Erro ] : Quantidade de candidatos a BSS inválida" << endl;
    }
}

void Instance::addVerticePoint(int idVertice, std::shared_ptr<Point> verticePoint)
{
    if(grafo != NULL)
    {
        if(grafo->existsVertice(idVertice))
        {
            this->grafo->setVerticePoint(idVertice, verticePoint);
            this->grafo->setVerticeId(idVertice, idVertice);
        }
        else
        {
            cout << "[ Erro ] : Vértice não existe na instância" << endl;
        }
    }
    else
    {
        cout << "[ Erro - VPoint ] : Nao foi definida a quantidade de vertices para a instancia" << endl;
    }
}

void Instance::addVerticeInfos(int idVertice, float demand, float readyTime, float dueTime, float serviceTime)
{
    if(grafo != NULL)
    {
        if((idVertice != verticeBase) && grafo->existsVertice(idVertice))
        {
            this->grafo->setVerticeWeight(idVertice, demand);
            this->grafo->setVerticeReadyTime(idVertice, readyTime);
            this->grafo->setVerticeDueTime(idVertice, dueTime);
            this->grafo->setVerticeServiceTime(idVertice, serviceTime);
        }
        else
        {
            cout << "[ Erro ] : Vértice não existe na instância" << endl;
        }
    }
    else
    {
        cout << "[ Erro - VInfo ] : Nao foi definida a quantidade de vertices para a instancia" << endl;
    }
}

float Instance::getVerticeAngle(int idVertice)
{
    return this->grafo->getVerticeAngle(idVertice);
}

void Instance::addVerticeAngle(int idVertice, std::shared_ptr<Point> coord)
{
    if(grafo != NULL)
    {
        if(grafo->existsVertice(idVertice))
        {
            double theta = 0;
            if(idVertice > 0){
                double x = coord->getCoordX() - grafo->getVerticePoint(0)->getCoordX();
                double y = coord->getCoordY() - grafo->getVerticePoint(0)->getCoordY();
                theta = atan2(y, x) * 180 / 3.14159265;
                if(theta < 0){
                    theta = theta + 360;
                }
            }
            this->grafo->setVerticeAngle(idVertice, theta);
        }
        else
        {
            cout << "[ Erro ] : Vértice não existe na instância" << endl;
        }
    }
    else
    {
        cout << "[ Erro - VAngle ] : Nao foi definida a quantidade de vertices para a instancia" << endl;
    }
}

void Instance::orderAngle(int left, int right)
{
    grafo->orderAngle(left, right);
}
