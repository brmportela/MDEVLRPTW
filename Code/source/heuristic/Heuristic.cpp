#include "../../header/heuristic/Heuristic.h"

Heuristic::Heuristic(string filename)
{
    std::shared_ptr<Instance> instance = FileHelper::readInstance(filename);
    this->filename = filename;

    if(instance != NULL)
    {
        this->tRand = false;
        this->numK = 0;
        this->testPrint = 0;
        this->vehicleCapacity = instance->getVehicleCapacity();
        this->vehicleRefueling = instance->getVehicleRefueling();
        this->batteryCap = instance->getBatteryCap();
        this->costBss = instance->getCostBss();
        this->grafo = instance->getInstance();
        this->depCount = instance->getDepCount();
        this->stationCount = instance->getStationCount();
        this->clientCount = instance->getClientCount();
        this->noClientCount = depCount + stationCount;
        this->minCost = INFINITE;
        this->cBss = 0;
        this->cRotas = 0;
        this->pRotas = 0;
        this->pointCount = grafo->getVerticeCount();
        this->qtRotas = 0;
        this->erro = true;
        this->rotaExtra = 0;
        this->gera = 0;
        this->ready = true;
        this->gResult = instance->getGResult();
        preparation();
    }
    else
    {
        ready = false;
        cout << "[ Erro ]: A inst�ncia n�o foi carregada corretamente! " << endl;
    }
}

Heuristic::~Heuristic()
{
    for(int i=0; i < pointCount; i++)
    {
        delete[] dij[i];
    }
    delete[] dij;
    //delete[] pij;
    //delete[] vi;
    //delete[] ti;
    //cout << "\n Destrutor";
}

void Heuristic::incRota(int rE)
{
    this->rotaExtra = this->rotaExtra + rE;
}

int Heuristic::getPRotas()
{
    return pRotas;
}

int Heuristic::getCBss()
{
    return cBss;
}

int Heuristic::getCDep()
{
    return dep.size();
}

int Heuristic::getCRotas()
{
    return cRotas;
}

float Heuristic::getMinCost()
{
    return minCost;
}

bool Heuristic::isReady()
{
    return this->ready;
}

float Heuristic::getGResult()
{
    return this->gResult;
}

float Heuristic::run()
{
    // Se a inst�ncia n�o foi carregada corretamente, retorna
    if(!ready)
    {
        return -1;
    }
    return this->heuristic(0); // Gera uma solu��o
}

float Heuristic::runB()
{
    // Se a inst�ncia n�o foi carregada corretamente, retorna
    if(!ready)
    {
        return -1;
    }
    return this->heuristicB(0); // Gera uma solu��o
}

float Heuristic::runD()
{
    // Se a inst�ncia n�o foi carregada corretamente, retorna
    if(!ready)
    {
        return -1;
    }
    return this->heuristicD(0); // Gera uma solu��o
}

void Heuristic::preparation()
{
    float maiorDist = 0;
    float menor;
    float maior = 0;

    // Matriz de dist�ncias
    dij = Helper::initializeFloatMatriz(pointCount);
    float tSwap;
    for(int id1 = 0; id1 < pointCount; id1++)
    {
        std::shared_ptr<Point> p1 = this->grafo->getVerticePoint(id1);
        for(int id2 = 0; id2 < pointCount; id2++)
        {
            std::shared_ptr<Point> p2 = this->grafo->getVerticePoint(id2);
            dij[id1][id2] = p1->getDistance(p2);
            if(dij[id1][id2] > maiorDist)
            {
                maiorDist = dij[id1][id2];
            }
        }
    }

    // Verificar distancia de deposito mais pr�ximo de cliente mais distante
    for(int i = noClientCount; i < pointCount; i++)
    {
        menor = 9999999;
        for(int j = 0; j < depCount; j++)
        for(int j = 0; j < depCount; j++)
        {
            if(dij[i][j] < menor)
            {
                menor = dij[i][j];
                if(dij[i][j] > maior)
                {
                    maior = dij[i][j];
                }
            }
        }
    }

    // Capacidade da bateria
    if (this->batteryCap == 0)
    {
        batteryCap = ceil(maior);
    }

    // Tempo para a troca da bateria
    tSwap = (this->vehicleRefueling * this->batteryCap)/10;
    for(int i = this->depCount; i < this->stationCount + this->depCount; i++)
    {
        grafo->getVertice(i)->setServiceTime(tSwap);
    }
}

float Heuristic::heuristic(float alfa)
{
    float dPath = 0; //Custo da solu��o atual
    int k = 0;
    vector<vector<int>> aux;
    this->qtRotas = 0;
    while(qtRotas != -1)
    {
        this->erro = false;
        imb(); // Inser��o mais barata para as rotas
        if(qtRotas == -1) addBss();
        if(qtRotas != -1)
        {
            solution.clear();
            truck.clear();
            clearFull();
        }
        k++;
        if(k >= clientCount) return 9999999;
    }
    vnd();
    dPath = calcCusto();
    updateBss();
    this->minCost = dPath;
    this->cRotas = solution.size();
    this->cBss = bss.size();
    if(!viability())
    {
        dPath = 9999999;
    }
    return dPath;
}

float Heuristic::heuristicB(float alfa)
{
    // Heuristica com AG
    float dPath = 0; //Custo da solu��o atual
    int geracoes = 10;
    ag(geracoes);

    solution.clear();
    truck.clear();
    clearFull();
    this->qtRotas = 0;
    while(qtRotas != -1)
    {
        this->erro = false;
        imbB(population[0]); // Inser��o mais barata para as rotas
        if(qtRotas == -1) addBss();
        if(qtRotas != -1)
        {
            solution.clear();
            truck.clear();
            clearFull();
        }
    }
    dPath = calcCusto();
    updateBss();
    this->minCost = dPath;
    this->cRotas = solution.size();
    this->cBss = bss.size();
    if(!viability())
    {
        dPath = 9999999;
    }
    return dPath;
}

float Heuristic::heuristicC(float alfa)
{
    // Heuristica AG2
    float dPath = 0; //Custo da solu��o atual
    float custo = INFINITE;
    int geracoes = 10;
    int k = 0;
    ag(geracoes);
    for(int i = 0; i < (int)population.size(); i++)
    {
        solution.clear();
        truck.clear();
        clearFull();
        this->qtRotas = 0;
        while(qtRotas != -1)
        {
            this->erro = false;
            imbB(population[0]); // Inser��o mais barata para as rotas
            if(qtRotas == -1) addBss();
            if(qtRotas != -1)
            {
                solution.clear();
                truck.clear();
                clearFull();
            }
            else
            {
                vnd();
            }
        }
        dPath = calcCusto();
        if(dPath < custo)
        {
            custo = dPath;
            k = i;
        }
    }

    solution.clear();
    truck.clear();
    clearFull();
    this->qtRotas = 0;
    while(qtRotas != -1)
    {
        this->erro = false;
        imbB(population[k]); // Inser��o mais barata para as rotas
        if(qtRotas == -1) addBss();
        if(qtRotas != -1)
        {
            solution.clear();
            truck.clear();
            clearFull();
        }
        else
        {
            vnd();
        }
    }
    dPath = calcCusto();
    updateBss();
    this->minCost = dPath;
    this->cRotas = solution.size();
    this->cBss = bss.size();
    if(!viability())
    {
        dPath = 9999999;
    }
    return dPath;
}

float Heuristic::heuristicD(float alfa)
{
    //Construtivo de rotas individuais
    float dPath = INFINITE; //Custo da solu��o atual
    float custo;
    bool blast;
    int k = 0;
    dRotas(); // disttribuicao das rotas
    blast = true;
    this->erro = true;
    exRota.clear();
    int num;
    float num2;
    int demand = 0;

    // Calcula n�mero m�nimo de rotas (ve�culos) para o problema
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        demand += grafo->getVerticeWeight(i);
    }
    num = demand/vehicleCapacity;
    num2 = demand/vehicleCapacity;
    if(num < num2)
    {
        num++;
    }
    while(blast && (int)solution.size() > num)
    {
        blast = routeBlast();
        if(!blast)
        {
            solution = solutions[solutions.size()-1].solution;
            sTime = solutions[solutions.size()-1].sTime;
            truck = solutions[solutions.size()-1].truck;
        }
        if((int)solution.size() <= (int)exRota.size() || (int)solution.size() == num)
            blast = false;
        else
            blast = true;
    }
    int qq = 0;
    int iK = clientCount;
    int i = solutions.size();
    while(i > 0 && qq - iK <= this->numK)
    {
        i--;
        this->erro = false;
        solution = solutions[i].solution;
        sTime = solutions[i].sTime;
        truck = solutions[i].truck;
        if(testPrint)
        {
            cout << endl << "---------------" << endl;
            cout << "Solucao " << i << ": " << endl;
            telaSol();
        }
        addBss();
        if(testPrint && !erro)
        {
            cout << endl << "Solucao BSS: ";
            telaSol();
        }
        if(!erro)
        {
            vnd();
            if(testPrint)
            {
                cout << endl << "Solucao VND: ";
                telaSol();
            }
            custo = calcCusto();
            if(custo < dPath)
            {
                if(dPath == INFINITE) iK = qq;
                dPath = custo;
                k = i;
            }
            solutions[i].solution = solution;
            solutions[i].sTime = sTime;
            solutions[i].truck = truck;
        }
        else
        {
            if(testPrint)
            {
                cout << endl << "Inviavel!" << endl;
            }
        }
        if(dPath == INFINITE)
        {
            this->erro = true;
        }
        if(qq == 0)
        {
            this->erro = true;
        }
        qq++;

    }
    solution = solutions[k].solution;
    sTime = solutions[k].sTime;
    truck = solutions[k].truck;
    custo = dPath;
    dPath = calcCusto();
    updateBss();
    this->minCost = dPath;
    this->cRotas = solution.size();
    this->cBss = bss.size();
    if(!viability())
    {
        dPath = 9999999;
    }
    return dPath;
}

void Heuristic::ag(int geracoes)
{
    float elite;
    int x;
    vector<Individuo> populationB;
    int populacao = this->clientCount * 5;
    if(populacao > 100) populacao = 100;
    while((int)population.size() < populacao)
    {
        population.push_back(geraIndividuo());
    }
    ordena(0, population.size()-1);
    elite = population[0].custo;
    populationB = population;
    int k = 1;
    for(int i = 1; i < geracoes; i++)
    {
        for(int j = 0; j < populacao; j++)
        {
            populationB[j] = crossover();
            if(j+1 < populacao)
            {
                populationB[j+1] = individuo2;
                j++;
            }
        }
        for(int j = 0; j < populacao; j++)
        {
            x = rand() % 100;
            if(x < 95)
            {
                populationB[j] = mutacao(populationB[j]);
            }
        }
        ordena(0, populationB.size()-1);
        int l = 0;
        for(int j = populacao * 0.90; j < populacao; j++)
        {
            populationB[j] = population[l];
            l++;
        }
        population = populationB;
        ordena(0, population.size()-1);
        if(population[0].custo < elite)
        {
            elite = population[0].custo;
            i = -1;
        }
        k++;
    }
    gera = k;
}

Individuo Heuristic::crossover()
{
    Individuo individuo;
    vector<float> crossKey, crossKey2;
    float key;
    solution.clear();
    truck.clear();
    clearFull();
    int corte, x1, x, y1, y, k;

    corte = (rand() % (this->clientCount - 2))+1;

    x = rand() % population.size();
    x1 = rand() % population.size();
    if(x1 < x)
    {
        x = x1;
    }
    y = rand() % population.size();
    y1 = rand() % population.size();
    if(y1 < y)
    {
        y = y1;
    }

    crossKey.clear();
    crossKey2.clear();
    individuo2.keys.clear();
    individuo2.custo = INFINITE;
    k = rand() % 100;
    if(k < 80)
    {
        for(int i = 0; i < this->clientCount; i++)
        {
            if(i < corte)
            {
                key = population[x].keys[i];
                crossKey.push_back(key);
            }
            else
            {
                key = population[x].keys[i];
                crossKey2.push_back(key);
            }

        }
        for(int i = 0; i < this->clientCount; i++)
        {
            if(i < corte)
            {
                key = population[y].keys[i];
                crossKey2.insert(crossKey2.begin()+i, key);
            }
            else
            {
                key = population[y].keys[i];
                crossKey.push_back(key);
            }
        }
        individuo.keys = crossKey;
        individuo2.keys = crossKey2;

        solution.clear();
        truck.clear();
        clearFull();
        this->qtRotas = 0;
        while(qtRotas != -1)
        {
            this->erro = false;
            imbB(individuo); // Inser��o mais barata para as rotas
            if(qtRotas == -1) addBss();
            if(qtRotas != -1)
            {
                solution.clear();
                truck.clear();
                clearFull();
            }
        }
        individuo.custo = calcCusto();

        solution.clear();
        truck.clear();
        clearFull();
        this->qtRotas = 0;
        while(qtRotas != -1)
        {
            this->erro = false;
            imbB(individuo2); // Inser��o mais barata para as rotas
            if(qtRotas == -1) addBss();
            if(qtRotas != -1)
            {
                solution.clear();
                truck.clear();
                clearFull();
            }
        }
        individuo2.custo = calcCusto();
    }
    else
    {
        individuo = population[x];
        individuo2 = population[y];
    }

    return individuo;
}

Individuo Heuristic::crossover2()
{
    Individuo individuo;
    vector<float> crossKey, crossKey2;
    float key1, key2, a;
    solution.clear();
    truck.clear();
    clearFull();
    int alfa, x1, x, y1, y, k;

    x = rand() % population.size();
    x1 = rand() % population.size();
    if(x1 < x)
    {
        x = x1;
    }
    y = rand() % population.size();
    y1 = rand() % population.size();
    if(y1 < y)
    {
        y = y1;
    }
    crossKey.clear();
    crossKey2.clear();
    individuo2.keys.clear();
    individuo2.custo = INFINITE;
    k = rand() % 100;
    if(k < 80)
    {
        for(int i = 0; i < this->clientCount; i++)
        {
            alfa = rand() % 100;
            a = (float)alfa / 100;

            key1 = (population[x].keys[i] * a) + (population[y].keys[i] * (1 - a));
            key2 = (population[y].keys[i] * a) + (population[x].keys[i] * (1 - a));
            crossKey.push_back(key1);
            crossKey2.push_back(key2);
        }
        individuo.keys = crossKey;
        individuo2.keys = crossKey2;

        crossKey.clear();
        crossKey2.clear();

        solution.clear();
        truck.clear();
        clearFull();
        this->qtRotas = 0;
        while(qtRotas != -1)
        {
            this->erro = false;
            imbB(individuo); // Inser��o mais barata para as rotas
            if(qtRotas == -1) addBss();
            if(qtRotas != -1)
            {
                solution.clear();
                truck.clear();
                clearFull();
            }
        }
        individuo.custo = calcCusto();

        solution.clear();
        truck.clear();
        clearFull();
        this->qtRotas = 0;
        while(qtRotas != -1)
        {
            this->erro = false;
            imbB(individuo2); // Inser��o mais barata para as rotas
            if(qtRotas == -1) addBss();
            if(qtRotas != -1)
            {
                solution.clear();
                truck.clear();
                clearFull();
            }
        }
        individuo2.custo = calcCusto();
    }
    else
    {
        individuo = population[x];
        individuo2 = population[y];
    }

    return individuo;
}

Individuo Heuristic::mutacao(Individuo individuo)
{
    solution.clear();
    truck.clear();
    clearFull();
    int position, position2;
    float key;
    position = rand() % this->clientCount;
    position2 = rand() % this->clientCount;
    key = individuo.keys[position];
    individuo.keys[position] = individuo.keys[position2];
    individuo.keys[position2] = key;
    this->qtRotas = 0;
    while(qtRotas != -1)
    {
        this->erro = false;
        imbB(individuo); // Inser��o mais barata para as rotas
        if(qtRotas == -1) addBss();
        if(qtRotas != -1)
        {
            solution.clear();
            truck.clear();
            clearFull();
        }
    }
    individuo.custo = calcCusto();
    return individuo;
}

Individuo Heuristic::geraIndividuo()
{
    Individuo individuo;
    float key;
    solution.clear();
    truck.clear();
    clearFull();
    for(int i = 0; i < this->clientCount; i++)
    {
        key = (float)(rand() % 1000)/1000;
        individuo.keys.push_back(key);
    }
    this->qtRotas = 0;
    while(qtRotas != -1)
    {
        this->erro = false;
        imbB(individuo); // Inser��o mais barata para as rotas
        if(qtRotas == -1) addBss();
        if(qtRotas != -1)
        {
            solution.clear();
            truck.clear();
            clearFull();
        }
    }
    individuo.custo = calcCusto();
    return individuo;
}

void Heuristic::ordena(int left, int right)
{
    int i = left;
    int j = right;
    Individuo tmp;
    float pivot = population[(left + right)/2].custo;

    while (i <= j)
    {
        while (population[i].custo < pivot && i < right)
        {
            i++;
        }
        while (population[j].custo > pivot && j > left)
        {
            j--;
        }
        if (i <= j)
        {
            tmp = population[i];

            population[i] = population[j];

            population[j] = tmp;

            i++;
            j--;
        }
    }
    if (left < j)
    {
        ordena(left, j);
    }
    if (i < right)
    {
        ordena(i, right);
    }
}

void Heuristic::imbB(Individuo individuo)
{
    // Metodo da adi��o mais barata com torneio para AG
    int demand = 0;
    int num, p, s, r, x, y;
    float num2, adj, minAdj, minAdjT, wait;
    BestInsert aux;
    vector<int> bestRoute;
    vector<vector<BestInsert>> bestInsert;
    vector<BestInsert> vAux;
    vector<Time> vtAux;
    Time tAux;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    vector<float> keys;
    keys = individuo.keys;

    // Calcula demanda total
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        demand += grafo->getVerticeWeight(i);
    }

    // Calcula n�mero m�nimo de rotas (ve�culos) para o problema
    num = demand/vehicleCapacity;
    num2 = demand/vehicleCapacity;
    if(num < num2)
    {
        num++;
    }
    if(qtRotas > 0)
    {
        num = qtRotas;
    }
    else
    {
        num = num + rotaExtra;
    }
    if(num > clientCount)
        num = clientCount;

    // Limpa vetores
    points.clear();
    solution.clear();
    routeAux.clear();

    // Cria cada uma das rotas iniciais
    for(int i = 0; i < num; i++)
    {
        solution.push_back(routeAux);
        this->truck.push_back(make_shared<Vehicle>(i, vehicleCapacity, batteryCap));
        sTime.push_back(vtAux);
    }

    // Monta um vetor com todos os pontos a serem inseridos
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        points.push_back(grafo->getVertice(i));
    }

    // Adiciona o primeiro ponto em cada uma das rotas
    for(int i = 0; i < (int)solution.size(); i++)
    {
        x = 0;
        y = 0;
        for(int j = 0; j < (int)keys.size(); j++)
        {
            if(keys[j] > x)
            {
                x = keys[j];
                y = j;
            }
        }
        solution[i].insert(solution[i].begin(), points[y]);
        sTime[i].insert(sTime[i].begin(), tAux);
        this->truck[i]->removeCapacity(points[y]->getWeight());
        points.erase(points.begin()+y);
        keys.erase(keys.begin()+y);
    }

    // Insere dep�sito mais pr�ximo
    for(int i = 0; i < (int)solution.size(); i++)
    {
        int k = -1;
        float dist = INFINITE;
        int client = solution[i][0]->getId();
        for(int j = 0; j < depCount; j++)
        {
            if(dij[j][client] < dist)
            {
                k = j;
                dist = dij[j][client];
            }
        }
        if(k != -1)
        {
            solution[i].insert(solution[i].begin(),grafo->getVertice(k));
            sTime[i].insert(sTime[i].begin(), tAux);
            solution[i].push_back(grafo->getVertice(k));
            sTime[i].push_back(tAux);

            updateTime(i);
            updateClearance(i);

            if(erro)
            {
                this->qtRotas = solution.size()+1;
                return;
            }
        }
        else
        {
            cout << "Erro de deposito na rota " << i << endl;
        }
    }

    // Monta vetor de melhores inser��es
    for(int k = 0; k < (int)points.size(); k++)
    {
        minAdjT = INFINITE;
        r = -1;
        for(int i = 0; i < (int)solution.size(); i++)
        {
            minAdj = INFINITE;
            s = -1;
            if(points[k]->getWeight() <= truck[i]->getCapacity())
            {
                for(int j = 1; j < (int)solution[i].size(); j++)
                {
                    adj = dij[solution[i][j-1]->getId()][points[k]->getId()] + dij[points[k]->getId()][solution[i][j]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
                    wait = points[k]->getReadyTime() - (sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[k]->getId()]);
                    if(wait < 0) wait = 0;
                    if(adj < minAdj &&
                       adj + points[k]->getServiceTime() + wait <= sTime[i][j].clearance &&
                       sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[k]->getId()] <= points[k]->getDueTime())
                    {
                        minAdj = adj; // Menor custo de inserss�o na rota atual
                        s = j; // Posi��o a ser inserido
                        if(minAdj < minAdjT)
                        {
                            minAdjT = minAdj; // Menor custo de inserss�o entre todas as rotas
                            r = i; // Melhor rota
                        }
                    }
                }
            }
            aux.cost = minAdj;
            aux.successor = s;
            vAux.push_back(aux);
        }
        if(r == -1)
        {
            solution.push_back(routeAux);
            sTime.push_back(vtAux);

            int h = solution.size()-1;

            sTime[h].push_back(tAux);
            truck.push_back(make_shared<Vehicle>(h, vehicleCapacity, batteryCap));
            solution[h].push_back(points[k]);
            truck[h]->removeCapacity(points[k]->getWeight());
            points.erase(points.begin()+k);
            keys.erase(keys.begin()+k);

            //insere dep�sito mais pr�ximo
            int kk = -1;
            float dist = INFINITE;
            int client = solution[h][0]->getId();
            for(int jj = 0; jj < depCount; jj++)
            {
                if(dij[jj][client] < dist)
                {
                    kk = jj;
                    dist = dij[jj][client];
                }
            }
            if(kk != -1)
            {
                solution[h].insert(solution[h].begin(),grafo->getVertice(kk));
                sTime[h].insert(sTime[h].begin(), tAux);
                solution[h].push_back(grafo->getVertice(kk));
                sTime[h].push_back(tAux);

                updateTime(h);
                updateClearance(h);

                if(erro)
                {
                    this->qtRotas = solution.size()+1;
                    return;
                }
            }
            else
            {
                cout << "Erro de deposito na rota NOVA " << solution.size()-1 << endl;
            }
            //fim - insere dep�sito mais pr�ximo

            //adicionando nova rota a matriz de melhor inser��o
            for(int l = 0; l < k; l++)
            {
                int i = solution.size() - 1;
                minAdj = INFINITE;
                s = -1;
                if(points[l]->getWeight() <= truck[i]->getCapacity())
                {
                    for(int j = 1; j < (int)solution[i].size(); j++)
                    {
                        wait = points[l]->getReadyTime() - (sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[l]->getId()]);
                        if(wait < 0) wait = 0;
                        adj = dij[solution[i][j-1]->getId()][points[l]->getId()] + dij[points[l]->getId()][solution[i][j]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
                        if(adj < minAdj &&
                           adj + points[l]->getServiceTime() + wait <= sTime[i][j].clearance &&
                           sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[l]->getId()] <= points[l]->getDueTime())
                        {
                            minAdj = adj; // Menor custo de inserss�o na rota atual
                            s = j; // Posi��o a ser inserido
                        }
                    }
                }
                aux.cost = minAdj;
                aux.successor = s;
                bestInsert[l].push_back(aux);
                if(bestInsert[l][i].cost < bestInsert[l][bestRoute[l]].cost)
                {
                    bestRoute[l] = i;
                }
            }
            k = k-1;
        }
        else
        {
            bestInsert.push_back(vAux);
            bestRoute.push_back(r);
        }
        vAux.clear();
    }

    while(points.size() > 0)
    {
        int x = 0;
        for(int j = 0; j < (int)keys.size(); j++)
        {
            if(keys[j] > x)
            {
                x = keys[j];
            }
        }
        p = x;
        minAdj = bestInsert[p][bestRoute[p]].cost;

        if(minAdj < INFINITE)
        {
            r = bestRoute[p];
            s = bestInsert[p][r].successor;
            solution[r].insert(solution[r].begin()+s, points[p]);
            sTime[r].insert(sTime[r].begin()+s, tAux);
            truck[r]->removeCapacity(points[p]->getWeight());
            points.erase(points.begin()+p);
            keys.erase(keys.begin()+p);
            bestRoute.erase(bestRoute.begin()+p);
            bestInsert.erase(bestInsert.begin()+p);
            updateTime(r);
            updateClearance(r);
            if(erro)
            {
                this->qtRotas = solution.size()+1;
                return;
            }

            bool change = false;
            for(int j = 0; j < (int)bestInsert.size(); j++)
            {
                if(points[j]->getWeight() > truck[r]->getCapacity() || bestInsert[j][r].successor == -1)
                {
                    bestInsert[j][r].successor = -1;
                    bestInsert[j][r].cost = INFINITE;
                    if(bestRoute[j] == r)
                    {
                        cout << "AQUI" << endl;
                        float custoB = bestInsert[j][r].cost;
                        int rotaB = r;
                        for(int t = 0; t < (int)bestInsert[j].size(); t++)
                        {
                            if(bestInsert[j][t].cost < custoB)
                            {
                                custoB = bestInsert[j][t].cost;
                                rotaB = t;
                            }
                        }
                        bestRoute[j] = rotaB;
                    }
                    change = true;
                }
                change = true;
                for(int i = 1; i < (int)solution[r].size(); i++)
                {
                    wait = points[j]->getReadyTime() - (sTime[r][i-1].out + dij[solution[r][i-1]->getId()][points[j]->getId()]);
                    if(wait < 0) wait = 0;
                    adj = dij[solution[r][i-1]->getId()][points[j]->getId()] + dij[points[j]->getId()][solution[r][i]->getId()] - dij[solution[r][i-1]->getId()][solution[r][i]->getId()];
                    if(adj <  bestInsert[j][r].cost &&
                       adj + points[j]->getServiceTime() + wait <= sTime[r][i].clearance &&
                       sTime[r][i-1].out + dij[solution[r][i-1]->getId()][points[j]->getId()] <= points[j]->getDueTime())
                    {
                        bestInsert[j][r].cost = adj; // Menor custo de inserss�o na rota atual
                        bestInsert[j][r].successor = i; // Posi��o a ser inserido
                    }
                }
                if(change)
                {
                    for(int k = 0; k < (int)bestInsert[j].size(); k++)
                    {
                        if(bestInsert[j][k].cost < bestInsert[j][bestRoute[j]].cost)
                        {
                            bestRoute[j] = k;
                        }
                    }
                    if(bestInsert[j][bestRoute[j]].cost == INFINITE)
                    {
                        sTime.push_back(vtAux);
                        solution.push_back(routeAux);
                        truck.push_back(make_shared<Vehicle>(num, vehicleCapacity, batteryCap));
                        num++;
                        x = solution.size()-1;
                        solution[x].push_back(points[j]);
                        truck[x]->removeCapacity(points[j]->getWeight());
                        points.erase(points.begin()+j);
                        keys.erase(keys.begin()+j);
                        bestRoute.erase(bestRoute.begin()+j);
                        bestInsert.erase(bestInsert.begin()+j);
                        sTime[x].push_back(tAux);

                        //insere dep�sito mais pr�ximo
                        int kk = -1;
                        float dist = INFINITE;
                        int client = solution[x][0]->getId();
                        for(int jj = 0; jj < depCount; jj++)
                        {
                            if(dij[jj][client] < dist)
                            {
                                kk = jj;
                                dist = dij[jj][client];
                            }
                        }
                        if(kk != -1)
                        {
                            solution[x].insert(solution[x].begin(),grafo->getVertice(kk));
                            solution[x].push_back(grafo->getVertice(kk));
                            sTime[x].insert(sTime[x].begin(), tAux);
                            sTime[x].push_back(tAux);
                            updateTime(x);
                            updateClearance(x);
                            if(erro)
                            {
                                this->qtRotas = solution.size()+1;
                                return;
                            }
                        }
                        else
                        {
                            cout << "Erro de deposito na rota NOVA " << solution.size()-1 << endl;
                        }
                        //fim - insere dep�sito mais pr�ximo

                        j = j-1;
                        for(int l = 0; l < (int)bestInsert.size(); l++)
                        {
                            aux.successor = -1;
                            aux.cost = INFINITE;
                            if(points[l]->getWeight() <= truck[x]->getCapacity())
                            {
                                adj = dij[solution[x][0]->getId()][points[l]->getId()] + dij[points[l]->getId()][solution[x][1]->getId()] - dij[solution[x][0]->getId()][solution[x][1]->getId()];
                                wait = points[l]->getReadyTime() - (sTime[x][0].out + dij[solution[x][0]->getId()][points[l]->getId()]);
                                if(wait < 0) wait = 0;
                                if(adj < aux.cost &&
                                   adj + points[l]->getServiceTime() + wait <= sTime[x][1].clearance &&
                                   sTime[x][0].out + dij[solution[x][0]->getId()][points[l]->getId()] <= points[l]->getDueTime())
                                {
                                    aux.cost = adj;
                                    aux.successor = 1;
                                }
                                adj = dij[solution[x][1]->getId()][points[l]->getId()] + dij[points[l]->getId()][solution[x][2]->getId()] - dij[solution[x][1]->getId()][solution[x][2]->getId()];
                                wait = points[l]->getReadyTime() - (sTime[x][1].out + dij[solution[x][1]->getId()][points[l]->getId()]);
                                if(wait < 0) wait = 0;
                                if(adj < aux.cost &&
                                   adj + points[l]->getServiceTime() + wait <= sTime[x][2].clearance &&
                                   sTime[x][1].out + dij[solution[x][1]->getId()][points[l]->getId()] <= points[l]->getDueTime())
                                {
                                    aux.cost = adj;
                                    aux.successor = 2;
                                }
                            }
                            bestInsert[l].push_back(aux);
                            if(bestInsert[l][x].cost < bestInsert[l][bestRoute[l]].cost)
                            {
                                bestRoute[l] = x;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            cout << "ERRO_IMB" << endl;
        }
    }
    bestInsert.clear();
    bestRoute.clear();
    routeAux.clear();
    points.clear();
    this->qtRotas = -1;
}

void Heuristic::dRotas()
{
    // Metodo de distribui��o de rotas individuais

    int num;
    vector<Time> vtAux;
    Time tAux;
    Solucao aSolution;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    num = clientCount;

    // Limpa vetores
    points.clear();
    solution.clear();
    routeAux.clear();

    // Cria cada uma das rotas iniciais
    for(int i = 0; i < num; i++)
    {
        solution.push_back(routeAux);
        this->truck.push_back(make_shared<Vehicle>(i, vehicleCapacity, batteryCap));
        sTime.push_back(vtAux);
    }

    // Monta um vetor com todos os pontos a serem inseridos
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        points.push_back(grafo->getVertice(i));
    }

    // Adiciona o primeiro ponto em cada uma das rotas aleatoriamente
    for(int i = 0; i < (int)solution.size(); i++)
    {
        //Guloso
        solution[i].push_back(points[0]);
        sTime[i].insert(sTime[i].begin(), tAux);
        this->truck[i]->removeCapacity(points[0]->getWeight());
        points.erase(points.begin());
    }

    // Insere dep�sito mais pr�ximo
    for(int i = 0; i < (int)solution.size(); i++)
    {
        int k = -1;
        float dist = INFINITE;
        int client = solution[i][0]->getId();
        for(int j = 0; j < this->depCount; j++)
        {
            if(dij[j][client] < dist)
            {
                k = j;
                dist = dij[j][client];
            }
        }
        if(k != -1)
        {
            solution[i].insert(solution[i].begin(),grafo->getVertice(k));
            sTime[i].insert(sTime[i].begin(), tAux);
            solution[i].push_back(grafo->getVertice(k));
            sTime[i].push_back(tAux);
            updateTime(i);
            updateClearance(i);
        }
        else
        {
            cout << "Erro de deposito na rota " << i << endl;
        }
    }
    routeAux.clear();
    points.clear();
    this->qtRotas = -1;
    aSolution.solution = solution;
    aSolution.sTime = sTime;
    aSolution.truck = truck;
    solutions.push_back(aSolution);
}

bool Heuristic::shiftRotas()
{
    bool better = false;
    float auxA, auxB, auxC, adj, wait;
    int p, q, r, s;
    r = 0;
    p = 0;
    auxC = 0;
    vector<int> rota;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size() -1; j++)
        {
            auxA = round((dij[solution[i][j-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[i][j+1]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j+1]->getId()])*1000);
            for(int k = 0; k < (int)solution.size(); k++)
            {
                if(k != i && solution[i][j]->getWeight() <= truck[k]->getCapacity() &&
                   solution[i][j]->getId() >= this->noClientCount)
                {
                    for(int l = 1; l < (int)solution[k].size(); l++)
                    {
                        auxB = round((dij[solution[k][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[k][l]->getId()] - dij[solution[k][l-1]->getId()][solution[k][l]->getId()])*1000);


                        adj = dij[solution[k][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[k][l]->getId()] - dij[solution[k][l-1]->getId()][solution[k][l]->getId()];
                        wait = solution[i][j]->getReadyTime() - (sTime[k][l-1].out + dij[solution[k][l-1]->getId()][solution[i][j]->getId()]);
                        if(wait < 0) wait = 0;
                        if(auxA - auxB > auxC &&
                           adj + solution[i][j]->getServiceTime() + wait <= sTime[k][l].clearance &&
                           sTime[k][l-1].out + dij[solution[k][l-1]->getId()][solution[i][j]->getId()] <= solution[i][j]->getDueTime())
                        {
                            rota.clear();
                            for(int w = 0; w < (int)solution[k].size(); w++)
                            {
                                if(w == l)
                                {
                                    rota.push_back(solution[i][j]->getId());
                                }
                                rota.push_back(solution[k][w]->getId());
                            }
                            if(valido(rota) && valido2(rota) && valido3(rota))
                            {
                                better = true;
                                p = i;
                                q = j;
                                r = k;
                                s = l;
                                auxC = auxA - auxB;
                            }
                        }
                    }
                }
            }
        }
    }
    if(better)
    {
        solution[r].insert(solution[r].begin()+s, solution[p][q]);
        truck[r]->removeCapacity(solution[p][q]->getWeight());
        truck[p]->addCapacity(solution[p][q]->getWeight());
        solution[p].erase(solution[p].begin()+q);
        sTime[r].insert(sTime[r].begin()+s, sTime[p][q]);
        sTime[p].erase(sTime[p].begin()+q);
        updateTime(r);
        updateClearance(r);
        updateTime(p);
        updateClearance(p);
    }
    return better;
}

bool Heuristic::routeBlast()
{
    Solucao aSolution;
    vector<vertice_s> client;
    float cost = 0;
    float costTotal = 0;
    Rrota r[solution.size()];
    Rrota k;
    int l, x, a, b, p, q;
    float y, adj, wait, adjM;
    bool insere = true;
    bool better, ok;
    Time tAux;
    vector<int> rota;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    p = 0;

    //dados das rotas
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size(); j++)
        {
            cost = cost + dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
        }
        cost = cost / (solution[i].size() - 2);
        r[i].key = i;
        r[i].custo = cost;
        costTotal = costTotal + cost;
        cost = 0;
    }

    //Ordena vetor de rotas
    for(int i = solution.size()-1; i > 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(r[j].custo < r[j+1].custo)
            {
                k = r[j];
                r[j] = r[j+1];
                r[j+1] = k;
            }
        }
    }

    //Seleciona rota por roleta
    ok = false;
    while(!ok)
    {
        if(!tRand)
        {
            x = (rand() % (int)costTotal) + 1;
            y = 0;
            for(int i = 0; i < (int)solution.size(); i++)
            {
                y += r[i].custo;
                if (y >= (float)x)
                {
                    l = r[i].key;
                    i = solution.size();
                    ok = true;
                }
            }
        }
        else
        {
            x = rand() % (int)solution.size();
            l = r[x].key;
            ok = true;
        }

        for(int i = 0; i < (int)exRota.size(); i++)
        {
            if(exRota[i] == l)
            {
                ok = false;
            }
        }
    }

    //coleta clientes
    client.clear();
    for(int i = 1; i < (int)solution[l].size()-1; i++)
    {
        client.push_back(solution[l][i]);
    }

    //destroi rota
    solution.erase(solution.begin()+l);
    sTime.erase(sTime.begin()+l);
    truck.erase(truck.begin()+l);

    //distribui clientes
    while(client.size() > 0)
    {
        // Torneio por menor janela de tempo
        a = (rand() % client.size());
        b = (rand() % client.size());
        if(!tRand) if(client[a]->getDueTime() - client[a]->getReadyTime() > client[b]->getDueTime() - client[b]->getReadyTime()) a = b;

        //insere cliente a
        adjM = INFINITE;
        better = false;
        for(int i = 0; i < (int)solution.size(); i++)
        {
            if(client[a]->getWeight() <= truck[i]->getCapacity())
            {
                for(int j = 1; j < (int)solution[i].size(); j++)
                {
                    adj = dij[solution[i][j-1]->getId()][client[a]->getId()] + dij[client[a]->getId()][solution[i][j]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
                    wait = client[a]->getReadyTime() - (sTime[i][j-1].out + dij[solution[i][j-1]->getId()][client[a]->getId()]);
                    if(wait < 0) wait = 0;

                    if(adj < adjM)
                    if(adj + client[a]->getServiceTime() + wait <= sTime[i][j].clearance)
                    if(sTime[i][j-1].out + dij[solution[i][j-1]->getId()][client[a]->getId()] <= client[a]->getDueTime())
                    {
                        rota.clear();
                        for(int w = 0; w < (int)solution[i].size(); w++)
                        {
                            if(w == j)
                            {
                                rota.push_back(client[a]->getId());
                            }
                            rota.push_back(solution[i][j]->getId());
                        }
                        if(valido2(rota) && valido3(rota))
                        {
                            better = true;
                            p = i;
                            q = j;
                            adjM = adj;
                        }
                    }
                }
            }
        }
        if(better)
        {
            solution[p].insert(solution[p].begin()+q, client[a]);
            truck[p]->removeCapacity(client[a]->getWeight());
            sTime[p].insert(sTime[p].begin()+q, tAux);
            updateTime(p);
            updateClearance(p);
            client.erase(client.begin()+a);
        }
        else
        {
            insere = false;
            exRota.push_back(l);
            break;
        }
    }
    if(insere)
    {
        aSolution.solution = solution;
        aSolution.sTime = sTime;
        aSolution.truck = truck;
        solutions.push_back(aSolution);
        for(int i = 0; i < (int)exRota.size(); i++)
        {
            if(exRota[i] > l) exRota[i] = exRota[i] - 1;
        }
    }
    return insere;
}

void Heuristic::eraseBss()
{
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 0; j < (int)solution[i].size(); j++)
        {
            if(solution[i][j]->getId() > clients)
            {
                solution[i].erase(solution[i].begin()+j);
            }
        }
    }
    bss.clear();
}

void Heuristic::recontTruck(int i)
{
    truck[i]->resetCapacity();
    for(int j = 0; j < (int)solution[i].size(); j++)
    {
        truck[i]->removeCapacity(solution[i][j]->getWeight());
    }
}

void Heuristic::imb()
{
    //Metodo da adi��o mais barata com torneio

    int demand = 0;
    int num, p, s, r, x, y;
    float num2, adj, minAdj, minAdjT, wait;
    BestInsert aux;
    vector<int> bestRoute;
    vector<vector<BestInsert>> bestInsert;
    vector<BestInsert> vAux;
    vector<Time> vtAux;
    Time tAux;

    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;

    // Calcula demanda total
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        demand += grafo->getVerticeWeight(i);
    }

    // Calcula n�mero m�nimo de rotas (ve�culos) para o problema
    num = demand/vehicleCapacity;
    num2 = demand/vehicleCapacity;
    if(num < num2)
    {
        num++;
    }
    if(qtRotas > 0)
    {
        num = qtRotas;
    }
    else
    {
        num = num + rotaExtra;
    }
    if(num > clientCount)
        num = clientCount;
    // Limpa vetores
    points.clear();
    solution.clear();
    routeAux.clear();

    // Cria cada uma das rotas iniciais
    for(int i = 0; i < num; i++)
    {
        solution.push_back(routeAux);
        this->truck.push_back(make_shared<Vehicle>(i, vehicleCapacity, batteryCap));
        sTime.push_back(vtAux);
    }

    // Monta um vetor com todos os pontos a serem inseridos
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        points.push_back(grafo->getVertice(i));
    }

    // Adiciona o primeiro ponto em cada uma das rotas aleatoriamente
    for(int i = 0; i < (int)solution.size(); i++)
    {
        if(tRand)
        {
            //Randomico puro
            x = rand() % (points.size());
            solution[i].insert(solution[i].begin(), points[x]);
            sTime[i].insert(sTime[i].begin(), tAux);
            this->truck[i]->removeCapacity(points[x]->getWeight());
            points.erase(points.begin() + x);
        }
        else
        {
            //Roleta
            x = (rand() % demand) + 1;
            y = 0;
            for(int j = 0; j < (int)points.size(); j++)
            {
                y += points[j]->getWeight();
                if (y >= x)
                {
                    demand -= points[j]->getWeight();
                    solution[i].insert(solution[i].begin(), points[j]);
                    sTime[i].insert(sTime[i].begin(), tAux);
                    this->truck[i]->removeCapacity(points[j]->getWeight());
                    points.erase(points.begin() + j);
                    j = points.size();
                }
            }
        }

        /*
        //Guloso
        solution[i].insert(solution[i].begin()+1, points[0]);
        this->truck[i]->removeCapacity(points[0]->getWeight());
        points.erase(points.begin());
        */
    }

    // Insere dep�sito mais pr�ximo
    for(int i = 0; i < (int)solution.size(); i++)
    {
        int k = -1;
        float dist = INFINITE;
        int client = solution[i][0]->getId();
        for(int j = 0; j < depCount; j++)
        {
            if(dij[j][client] < dist)
            {
                k = j;
                dist = dij[j][client];
            }
        }
        if(k != -1)
        {
            solution[i].insert(solution[i].begin(),grafo->getVertice(k));
            sTime[i].insert(sTime[i].begin(), tAux);
            solution[i].push_back(grafo->getVertice(k));
            sTime[i].push_back(tAux);

            updateTime(i);
            updateClearance(i);

            if(erro)
            {
                this->qtRotas = solution.size()+1;
                return;
            }
        }
        else
        {
            cout << "Erro de deposito na rota " << i << endl;
        }
    }

    // Monta vetor de melhores inser��es
    for(int k = 0; k < (int)points.size(); k++)
    {
        minAdjT = INFINITE;
        r = -1;
        for(int i = 0; i < (int)solution.size(); i++)
        {
            minAdj = INFINITE;
            s = -1;
            if(points[k]->getWeight() <= truck[i]->getCapacity())
            {
                for(int j = 1; j < (int)solution[i].size(); j++)
                {
                    adj = dij[solution[i][j-1]->getId()][points[k]->getId()] + dij[points[k]->getId()][solution[i][j]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
                    wait = points[k]->getReadyTime() - (sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[k]->getId()]);
                    if(wait < 0) wait = 0;
                    if(adj < minAdj &&
                       adj + points[k]->getServiceTime() + wait <= sTime[i][j].clearance &&
                       sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[k]->getId()] <= points[k]->getDueTime())
                    {
                        minAdj = adj; // Menor custo de inserss�o na rota atual
                        s = j; // Posi��o a ser inserido
                        if(minAdj < minAdjT)
                        {
                            minAdjT = minAdj; // Menor custo de inserss�o entre todas as rotas
                            r = i; // Melhor rota
                        }
                    }
                }
            }
            aux.cost = minAdj;
            aux.successor = s;
            vAux.push_back(aux);
        }
        if(r == -1)
        {
            solution.push_back(routeAux);
            sTime.push_back(vtAux);

            int h = solution.size()-1;

            sTime[h].push_back(tAux);
            truck.push_back(make_shared<Vehicle>(h, vehicleCapacity, batteryCap));
            solution[h].push_back(points[k]);
            truck[h]->removeCapacity(points[k]->getWeight());
            points.erase(points.begin()+k);

            //insere dep�sito mais pr�ximo
            int kk = -1;
            float dist = INFINITE;
            int client = solution[h][0]->getId();
            for(int jj = 0; jj < depCount; jj++)
            {
                if(dij[jj][client] < dist)
                {
                    kk = jj;
                    dist = dij[jj][client];
                }
            }
            if(kk != -1)
            {
                solution[h].insert(solution[h].begin(),grafo->getVertice(kk));
                sTime[h].insert(sTime[h].begin(), tAux);
                solution[h].push_back(grafo->getVertice(kk));
                sTime[h].push_back(tAux);
                updateTime(h);
                updateClearance(h);
                if(erro)
                {
                    this->qtRotas = solution.size()+1;
                    return;
                }
            }
            else
            {
                cout << "Erro de deposito na rota NOVA " << solution.size()-1 << endl;
            }
            //fim - insere dep�sito mais pr�ximo

            //adicionando nova rota a matriz de melhor inser��o
            for(int l = 0; l < k; l++)
            {
                int i = solution.size() - 1;
                minAdj = INFINITE;
                s = -1;
                if(points[l]->getWeight() <= truck[i]->getCapacity())
                {
                    for(int j = 1; j < (int)solution[i].size(); j++)
                    {
                        wait = points[l]->getReadyTime() - (sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[l]->getId()]);
                        if(wait < 0) wait = 0;
                        adj = dij[solution[i][j-1]->getId()][points[l]->getId()] + dij[points[l]->getId()][solution[i][j]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
                        if(adj < minAdj &&
                           adj + points[l]->getServiceTime() + wait <= sTime[i][j].clearance &&
                           sTime[i][j-1].out + dij[solution[i][j-1]->getId()][points[l]->getId()] <= points[l]->getDueTime())
                        {
                            minAdj = adj; // Menor custo de inserss�o na rota atual
                            s = j; // Posi��o a ser inserido
                        }
                    }
                }
                aux.cost = minAdj;
                aux.successor = s;
                bestInsert[l].push_back(aux);
                if(bestInsert[l][i].cost < bestInsert[l][bestRoute[l]].cost)
                {
                    bestRoute[l] = i;
                }
            }
            k = k-1;
        }
        else
        {
            bestInsert.push_back(vAux);
            bestRoute.push_back(r);
        }
        vAux.clear();
    }
    while(points.size() > 0)
    {
        //Randomico entre melhores (torneio tern�rio)
        int a, b, c;
        a = rand() % bestRoute.size();
        b = rand() % bestRoute.size();
        c = rand() % bestRoute.size();
        minAdj = bestInsert[a][bestRoute[a]].cost;
        p = a;
        if(!tRand)
        {
            if(minAdj > bestInsert[b][bestRoute[b]].cost)
            {
                p = b;
                minAdj = bestInsert[b][bestRoute[b]].cost;
            }

            if(minAdj > bestInsert[c][bestRoute[c]].cost)
            {
                p = c;
                minAdj = bestInsert[c][bestRoute[c]].cost;
            }
        }
        if(minAdj < INFINITE)
        {
            r = bestRoute[p];
            s = bestInsert[p][r].successor;
            solution[r].insert(solution[r].begin()+s, points[p]);
            sTime[r].insert(sTime[r].begin()+s, tAux);
            //cout << "C12";
            truck[r]->removeCapacity(points[p]->getWeight());
            points.erase(points.begin()+p);
            bestRoute.erase(bestRoute.begin()+p);
            bestInsert.erase(bestInsert.begin()+p);
            updateTime(r);
            updateClearance(r);

            if(erro)
            {
                this->qtRotas = solution.size()+1;
                return;
            }
            bool change = false;
            for(int j = 0; j < (int)bestInsert.size(); j++)
            {
                if(points[j]->getWeight() > truck[r]->getCapacity() || bestInsert[j][r].successor == -1)
                {
                    bestInsert[j][r].successor = -1;
                    bestInsert[j][r].cost = INFINITE;
                    if(bestRoute[j] == r)
                    {
                        float custoB = bestInsert[j][r].cost;
                        int rotaB = r;
                        for(int t = 0; t < (int)bestInsert[j].size(); t++)
                        {
                            if(bestInsert[j][t].cost < custoB)
                            {
                                custoB = bestInsert[j][t].cost;
                                rotaB = t;
                            }
                        }
                        bestRoute[j] = rotaB;
                    }
                    change = true;
                }
                change = true;
                for(int i = 1; i < (int)solution[r].size(); i++)
                {
                    wait = points[j]->getReadyTime() - (sTime[r][i-1].out + dij[solution[r][i-1]->getId()][points[j]->getId()]);
                    if(wait < 0) wait = 0;
                    adj = dij[solution[r][i-1]->getId()][points[j]->getId()] + dij[points[j]->getId()][solution[r][i]->getId()] - dij[solution[r][i-1]->getId()][solution[r][i]->getId()];
                    if(adj <  bestInsert[j][r].cost &&
                       adj + points[j]->getServiceTime() + wait <= sTime[r][i].clearance &&
                       sTime[r][i-1].out + dij[solution[r][i-1]->getId()][points[j]->getId()] <= points[j]->getDueTime())
                    {
                        bestInsert[j][r].cost = adj; // Menor custo de inserss�o na rota atual
                        bestInsert[j][r].successor = i; // Posi��o a ser inserido
                    }
                }
                if(change)
                {
                    for(int k = 0; k < (int)bestInsert[j].size(); k++)
                    {
                        if(bestInsert[j][k].cost < bestInsert[j][bestRoute[j]].cost)
                        {
                            bestRoute[j] = k;
                        }
                    }
                    if(bestInsert[j][bestRoute[j]].cost == INFINITE)
                    {
                        sTime.push_back(vtAux);
                        solution.push_back(routeAux);
                        truck.push_back(make_shared<Vehicle>(num, vehicleCapacity, batteryCap));
                        num++;
                        x = solution.size()-1;
                        solution[x].push_back(points[j]);
                        truck[x]->removeCapacity(points[j]->getWeight());
                        points.erase(points.begin()+j);
                        bestRoute.erase(bestRoute.begin()+j);
                        bestInsert.erase(bestInsert.begin()+j);
                        sTime[x].push_back(tAux);

                        //insere dep�sito mais pr�ximo
                        int kk = -1;
                        float dist = INFINITE;
                        int client = solution[x][0]->getId();
                        for(int jj = 0; jj < depCount; jj++)
                        {
                            if(dij[jj][client] < dist)
                            {
                                kk = jj;
                                dist = dij[jj][client];
                            }
                        }
                        if(kk != -1)
                        {
                            solution[x].insert(solution[x].begin(),grafo->getVertice(kk));
                            solution[x].push_back(grafo->getVertice(kk));

                            sTime[x].insert(sTime[x].begin(), tAux);
                            sTime[x].push_back(tAux);

                            updateTime(x);
                            updateClearance(x);
                            if(erro)
                            {
                                this->qtRotas = solution.size()+1;
                                return;
                            }
                        }
                        else
                        {
                            cout << "Erro de deposito na rota NOVA " << solution.size()-1 << endl;
                        }
                        //fim - insere dep�sito mais pr�ximo

                        j = j-1;
                        for(int l = 0; l < (int)bestInsert.size(); l++)
                        {
                            aux.successor = -1;
                            aux.cost = INFINITE;
                            if(points[l]->getWeight() <= truck[x]->getCapacity())
                            {
                                adj = dij[solution[x][0]->getId()][points[l]->getId()] + dij[points[l]->getId()][solution[x][1]->getId()] - dij[solution[x][0]->getId()][solution[x][1]->getId()];
                                wait = points[l]->getReadyTime() - (sTime[x][0].out + dij[solution[x][0]->getId()][points[l]->getId()]);
                                if(wait < 0) wait = 0;
                                if(adj < aux.cost &&
                                   adj + points[l]->getServiceTime() + wait <= sTime[x][1].clearance &&
                                   sTime[x][0].out + dij[solution[x][0]->getId()][points[l]->getId()] <= points[l]->getDueTime())
                                {
                                    aux.cost = adj;
                                    aux.successor = 1;
                                }
                                adj = dij[solution[x][1]->getId()][points[l]->getId()] + dij[points[l]->getId()][solution[x][2]->getId()] - dij[solution[x][1]->getId()][solution[x][2]->getId()];
                                wait = points[l]->getReadyTime() - (sTime[x][1].out + dij[solution[x][1]->getId()][points[l]->getId()]);
                                if(wait < 0) wait = 0;
                                if(adj < aux.cost &&
                                   adj + points[l]->getServiceTime() + wait <= sTime[x][2].clearance &&
                                   sTime[x][1].out + dij[solution[x][1]->getId()][points[l]->getId()] <= points[l]->getDueTime())
                                {
                                    aux.cost = adj;
                                    aux.successor = 2;
                                }
                            }
                            bestInsert[l].push_back(aux);
                            if(bestInsert[l][x].cost < bestInsert[l][bestRoute[l]].cost)
                            {
                                bestRoute[l] = x;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            cout << "eita" << endl;
        }
    }
    bestInsert.clear();
    bestRoute.clear();
    routeAux.clear();
    points.clear();
    this->qtRotas = -1;
}

void Heuristic::rbss()
{
    bss.clear();
    float cost; //Custo da solu��o atual
    float costBssT;
    int percent = 40;
    bool a;
    int b;
    vector<vector<vertice_s>> ss = solution;
    vector<vector<vertice_s>> best;
    vector<vertice_s> bssB;
    vector<int> aux;
    float bestcost = INFINITE;
    while(percent > 0)
    {
        a = true;
        cost = 0;
        costBssT = 0;
        bss.clear();
        abss.clear();
        solution = ss;
        sdm(percent);
        addBss(); // Adiciona BSSs na solu��o
        b = bss.size();
        aux.clear();
        for(int i = 0; i < (int)solution.size(); i++)
        {
            for(int j = 0; j < (int)solution[i].size(); j++)
            {
                aux.push_back(solution[i][j]->getId());
            }
            if(!valido(aux))
            {
                a = false;
            }
            aux.clear();
        }
        if(a)
        {
            cost = calcCusto();
            for(int i = 0; i < (int)bss.size(); i++)
            {
                costBssT += bss[i]->getWeight();
            }
            cost += costBssT;
            if(cost < bestcost)
            {
                best = solution;
                bestcost = cost;
                bssB = bss;
            }
        }
        if(a && b == 0)
        {
            percent = 0;
        }
        else
        {
            percent -= 5;
        }
    }
    solution = best;
    bss = bssB;
}

void Heuristic::sdm(int percent)
{
    //Subconjunto dominante m�nimo
    pij = Helper::initializeIntegerMatriz(grafo->getVerticeCount(), grafo->getBssCount()); // Matriz de subconjunto dominante m�nimo (BSSsxClientes)
    vi = new int[grafo->getBssCount()]; // Vetor de n�mero de adjacentes BSSs
    ti = new float[grafo->getBssCount()]; // Vetor de valor das adjacencias (numero de adj / quadrado do custo da bss)

    vector<int> ni;
    vector<int> sdmV;
    for(int d2 = 0; d2 < grafo->getBssCount(); d2++)
    {
        vi[d2] = 0;
        ti[d2] = 0;
        ni.push_back(d2);
    }
    ni.erase(ni.begin());
    for(int d1 = 0; d1 < grafo->getVerticeCount(); d1++)
    {
        for(int d2 = 0; d2 < grafo->getBssCount(); d2++)
        {
            if(dij[d1][d2 + grafo->getVerticeCount()] <= (batteryCap*percent)/100)
            {
                pij[d1][d2] = 1;
                vi[d2] += 1;
            }
            else
            {
                pij[d1][d2] = 0;
            }
        }
    }
    float maior;
    float m;
    int i = 1;
    int a;
    while(ni.size() != 0)
    {
        i = 0;
        maior = 0;
        for(int d2 = 1; d2 < grafo->getBssCount(); d2++)
        {
            ti[d2] = vi[d2]/(pow(grafo->getVertice(d2 + grafo->getVerticeCount())->getWeight(), 3));
            m = ti[d2];
            if(m > maior)
            {
                maior = m;
                i = d2;
            }
            else
            {
                if(m == maior && m != 0)
                {
                    a = (rand() % 2);
                    if( a == 1)
                    {
                        maior = m;
                        i = d2;
                    }
                }
            }
        }
        if(i != 0 && maior != 0)
        {
            sdmV.push_back(i);
            vi[i] = 0;
            for(int d1 = 1; d1 < grafo->getVerticeCount(); d1++)
            {
                if(pij[d1][i] == 1)
                {
                    pij[d1][i] = 0;
                    for(int d2 = 1; d2 < grafo->getBssCount(); d2++)
                    {
                        if(pij[d1][d2] == 1)
                        {
                            vi[d2] -= 1;
                            pij[d1][d2] = 0;
                        }
                    }
                }
            }
            for(int d1 = 1; d1 < grafo->getVerticeCount(); d1++)
            {
                if(vi[d1] == 0){
                    for(int k = 0; k < (int)ni.size(); k++)
                    {
                        if(d1 == ni[k])
                        {
                            ni.erase(ni.begin() + k);
                            k = ni.size();
                        }
                    }
                }
            }
        }
    }
    for(int k = 0; k < (int)sdmV.size(); k++)
    {
        bss.push_back(grafo->getVertice(sdmV[k] + grafo->getVerticeCount()));
        abss.push_back(0);
    }
    delete [] pij;
    delete [] vi;
    delete [] ti;
}

void Heuristic::addBss()
{
    //Inser��o gulosa de BSSs
    int menor = -1;
    bool add;
    vector<int> tempRote;
    int noBss, noBss2;
    float cost, nDis;
    Time tAux;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    int kco = 0;
    NBSS nBSS;
    vector<NBSS> candBSS;

    for(int i = 0; i < (int)solution.size(); i++)
    {
        this->truck[i]->resetBatteryCap();
        kco = 0;
        // Caminha pela rota at� achar ruptura
        for(int j = 1; j < (int)solution[i].size(); j++)
        {
            if(kco > 300)
            {
                this->qtRotas = solution.size()+1;
                this->erro = true;
                return;
            }
            kco++;
            if (this->truck[i]->getBatteryCap() >= dij[solution[i][j-1]->getId()][solution[i][j]->getId()])
            {
                this->truck[i]->removeBatteryCap(dij[solution[i][j-1]->getId()][solution[i][j]->getId()]);
                if(solution[i][j]->getId() <  this->noClientCount)
                {
                    this->truck[i]->resetBatteryCap();
                }
            }

            // Caso haja ruptura tenta adicionar BSS
            else
            {
                cost = INFINITE;
                menor = -1;
                add = false;
                candBSS.clear();
                // Busca candidata a BSS mais pr�ximo do ponto anterior a ruptura
                for (int l = this->depCount; l < this->noClientCount; l++)
                {
                    nDis = dij[solution[i][j-1]->getId()][l] + dij[l][solution[i][j]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
                    if(solution[i][j-1]->getId() == l || solution[i][j]->getId() == l){}
                    else if(dij[solution[i][j-1]->getId()][l] <= truck[i]->getBatteryCap() &&
                       nDis + grafo->getVertice(l)->getServiceTime() <= sTime[i][j].clearance && nDis < cost)
                    {
                        nBSS.cost = nDis;
                        nBSS.id = l;
                        candBSS.push_back(nBSS);
                    }
                }

                // Se candidata a BSS � alcan��vel sem ultrapassar a folga
                if ((int)candBSS.size() > 0)
                {
                    int a = (rand() % (int)candBSS.size());
                    cost = candBSS[a].cost;
                    menor = candBSS[a].id;
                    if(dij[solution[i][j-1]->getId()][menor] <= this->truck[i]->getBatteryCap())
                    {
                        if(solution[i][j-1]->getId() == menor || solution[i][j]->getId() == menor)
                        {
                            add = false;
                        }
                        else
                        {
                            this->truck[i]->removeBatteryCap(dij[solution[i][j-1]->getId()][menor]);
                            this->bss.push_back(grafo->getVertice(menor));
                            solution[i].insert(solution[i].begin()+(j), grafo->getVertice(menor));
                            sTime[i].insert(sTime[i].begin()+j, tAux);
                            updateTime(i);
                            updateClearance(i);
                            if(erro)
                            {
                                this->qtRotas = solution.size()+1;
                                return;
                            }
                            j = 0;
                            add = true;
                        }
                    }
                    else
                    {
                        add = false;
                    }
                }
                if(!add)
                {
                    menor = -1;
                    noBss2 = -1;

                    // Busca candidata a BSS mais pr�ximo de cada ponto anterior a ruptura
                    for(int m = j-1; m >= 0; m--)
                    {
                        if(m == 0)
                        {
                            this->qtRotas = solution.size()+1;
                            this->erro = true;
                            return;
                        }
                        noBss = -1;
                        truck[i]->addBatteryCap(dij[solution[i][m-1]->getId()][solution[i][m]->getId()]);
                        cost = INFINITE;
                        candBSS.clear();
                        for (int l = this->depCount; l < this->noClientCount; l++)
                        {
                            if(solution[i][m-1]->getId() == l)
                            {
                                noBss = l;
                                if(noBss2 == -1) noBss2 = l;
                                truck[i]->addBatteryCap(dij[solution[i][m-2]->getId()][solution[i][m-1]->getId()]);
                                solution[i].erase(solution[i].begin()+(m-1));
                                sTime[i].erase(sTime[i].begin()+(m-1));
                                m--;
                                l = this->depCount;
                                updateTime(i);
                                updateClearance(i);
                            }
                            nDis = dij[solution[i][m-1]->getId()][l] + dij[l][solution[i][m]->getId()] - dij[solution[i][m-1]->getId()][solution[i][m]->getId()];
                            if(solution[i][m-1]->getId() == l || solution[i][m]->getId() == l){}
                            else if(dij[solution[i][m-1]->getId()][l] <= truck[i]->getBatteryCap() &&
                                nDis + grafo->getVertice(l)->getServiceTime() <= sTime[i][m].clearance && nDis < cost && l != noBss && l != noBss2)
                            {
                                nBSS.cost = nDis;
                                nBSS.id = l;
                                candBSS.push_back(nBSS);
                            }
                        }
                        if((int)candBSS.size() > 0)
                        {
                            int a = (rand() % (int)candBSS.size());
                            cost = candBSS[a].cost;
                            menor = candBSS[a].id;
                            if (dij[solution[i][m-1]->getId()][menor] <= this->truck[i]->getBatteryCap())
                            {
                                this->bss.push_back(grafo->getVertice(menor));
                                solution[i].insert(solution[i].begin()+(m), grafo->getVertice(menor));
                                sTime[i].insert(sTime[i].begin()+m, tAux);
                                updateTime(i);
                                updateClearance(i);
                                if(erro)
                                {
                                    this->qtRotas = solution.size()+1;
                                    return;
                                }
                                j = 0;
                                m = 0;
                                add = true;
                                this->truck[i]->resetBatteryCap();
                            }
                            else
                            {
                                add = false;
                            }
                        }
                    }
                    if(!add)
                    {
                        cout << endl << endl << "BSS - Solu��o Invi�vel!!" << endl;
                        this->qtRotas = solution.size()+1;
                        return;
                    }
                }
                this->truck[i]->resetBatteryCap();
            }
        }
    }
    for(int i = 0; i < (int)solution.size(); i++)
    {
        this->truck[i]->resetBatteryCap();
    }

    this->qtRotas = -1;
}

void Heuristic::printSol(float tempT)
{
    //Gera arquivo de solu��o
    string outputFile = "results/" + filename.substr(9);
    solucao.open(outputFile.c_str());
    solucao << minCost << endl;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        solucao << solution[i][0]->getId();
        for(int j = 1; j < (int)solution[i].size(); j++)
        {
            if (solution[i][j]->getId() < grafo->getBssCount())
            {
                solucao << " " << solution[i][j]->getId();
            }
            else
            {
                solucao << " " << solution[i][j]->getId();
            }
        }
        solucao << endl;
    }
    solucao << "Capacidade bateria: " << batteryCap << endl;
    solucao << "Tempo total: " <<  tempT << endl;
    solucao << "Custo m�nimo: " << minCost << endl;
    solucao << "Total de dep usados: " << this->dep.size() << endl;
    solucao << "Total de rotas: " << solution.size() << endl;
    solucao << "Total de BSSs usadas: " << this->bss.size() << endl;
    solucao.flush();
    solucao.close();
}

void Heuristic::plot()
{
    //Gera arquivos para plotar o gr�fico
    for(int i = 0; i < (int)solution.size(); i++)
    {
        stringstream ss;
        ss << i;
        string str;
        str = ss.str();
        string outputFile = "results/" + filename.substr(9, filename.size() -13)  + "/" + str + ".txt";
        solucao.open(outputFile.c_str());
        for(int j = 0; j < (int)solution[i].size(); j++)
        {
            if(solution[i][j]->getId() < this->depCount)
                solucao << "d" << solution[i][j]->getId() << " " << solution[i][j]->getCoord()->getCoordX() << " " << solution[i][j]->getCoord()->getCoordY() << endl;
            else if(solution[i][j]->getId() < this->noClientCount)
                    solucao << "s" << solution[i][j]->getId() - this->depCount << " " << solution[i][j]->getCoord()->getCoordX() << " " << solution[i][j]->getCoord()->getCoordY() << endl;
                else solucao << "c" << solution[i][j]->getId() - this->noClientCount << " " << solution[i][j]->getCoord()->getCoordX() << " " << solution[i][j]->getCoord()->getCoordY() << endl;
        }
        solucao << "EOF";
        solucao.flush();
        solucao.close();
    }
    string outputFile = "results/" + filename.substr(9, filename.size() -13)  + "/point.txt";
    solucao.open(outputFile.c_str());
    for(int i = this->noClientCount; i < this->pointCount; i++)
    {
        solucao << "c" << grafo->getVertice(i)->getId() - this->noClientCount << " " << grafo->getVertice(i)->getCoord()->getCoordX() << " " << grafo->getVertice(i)->getCoord()->getCoordY() << endl;
    }
    solucao << "EOF";
    solucao.flush();
    solucao.close();

    outputFile = "results/" + filename.substr(9, filename.size() -13)  + "/dep.txt";
    solucao.open(outputFile.c_str());
    for(int i = 0; i < this->depCount; i++)
    {
        solucao << "d" << grafo->getVertice(i)->getId() << " " << grafo->getVertice(i)->getCoord()->getCoordX() << " " << grafo->getVertice(i)->getCoord()->getCoordY() << endl;
    }

    solucao << "EOF";
    solucao.flush();
    solucao.close();

    outputFile = "results/" + filename.substr(9, filename.size() -13)  + "/bss.txt";
    solucao.open(outputFile.c_str());
    for(int i = this->depCount; i < this->noClientCount; i++)
    {
        solucao << "s" << grafo->getVertice(i)->getId() - this->depCount << " " << grafo->getVertice(i)->getCoord()->getCoordX() << " " << grafo->getVertice(i)->getCoord()->getCoordY() << endl;
    }

    solucao << "EOF";
    solucao.flush();
    solucao.close();

    outputFile = "results/" + filename.substr(9, filename.size() -13)  + "/bssU.txt";
    solucao.open(outputFile.c_str());
    for(int j = 0; j < (int)bss.size(); j++)
    {
        solucao << "s" << this->bss[j]->getId()- this->depCount << " " << this->bss[j]->getCoord()->getCoordX() << " " << this->bss[j]->getCoord()->getCoordY() << endl;
    }

    solucao << "EOF";
    solucao.flush();
    solucao.close();

    outputFile = "results/" + filename.substr(9, filename.size() -13)  + "/plot.plt";
    solucao.open(outputFile.c_str());
    //solucao << "cd 'C:/Users/Br�ulio/Dropbox/Projeto_TCC/1.9/results'" << endl;
    //solucao << "cd 'C:/Users/Braulio/Dropbox/Projeto_TCC/1.9 (CEAD)/results'" << endl;
    //solucao << "cd 'D:/Compartilhados/Braulio - Roteamento de ve�culos el�tricos/Novo problema/Projeto/results'" << endl;
    //solucao << "cd '/home/brm/Dropbox/Projeto_TCC/1.9 (Linux)/results'" << endl;
    solucao << "cd '/home/brm/Dropbox/Projeto/results'" << endl;
    solucao << "set terminal svg size 1920,1080;  set output '" << filename.substr(9, filename.size() -13) << "/" << filename.substr(9, filename.size() -13) << ".svg'; plot ";
    stringstream n;
    string st;
    for(int j = 0; j < (int)solution.size(); j++)
    {
        stringstream n;
        n << j;
        st = n.str();
        solucao << "'" << filename.substr(9, filename.size() -13) << "/" << st << ".txt' using 2:3 title '" << st << "' with lines, ";
    }
    solucao << "'" << filename.substr(9, filename.size() -13) << "/point.txt' using 2:3 title 'Pontos' with points linestyle 4 linecolor 'black' linewidth 2, '" /*<< filename.substr(9, filename.size() -13) << "/point.txt' using 2:3:($1) with labels offset 2 notitle, '"*/ << filename.substr(9, filename.size() -13) << "/bss.txt' using 2:3 title 'BSSs' with points linestyle 2 linecolor 'green' linewidth 2, '" << filename.substr(9, filename.size() -13) << "/bssU.txt' using 2:3 title 'BSSs Usadas' with points linestyle 2 linecolor 'red' linewidth 2, '" << filename.substr(9, filename.size() -13) << "/dep.txt' using 2:3 title 'Dep�sitos' with points linestyle 6 linecolor 'blue' linewidth 2;";
    solucao.flush();
    solucao.close();
}

float Heuristic::calcCusto()
{
    //Calcula custo das rotas
    float custo = 0;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size(); j++)
        {
            custo += dij[solution[i][j-1]->getId()][solution[i][j]->getId()];
        }
    }
    return custo;
}

float Heuristic::calcCusto(vector<int> rota)
{
    //Calcula custo de uma rota
    float custo = 0;
    for(int i = 1; i < (int)rota.size(); i++)
    {
        custo += dij[rota[i-1]][rota[i]];
    }
    return custo;
}

float Heuristic::calcCustoT(vector<int> rota)
{
    //Calcula custo de uma rota
    float custo = 0;
    int k = 0;
    for(int i = 1; i < (int)rota.size(); i++)
    {
        custo += dij[rota[i-1]][rota[i]];
        if(rota[i] < noClientCount &&  k != rota[i])
        {
            k = rota[i];
            custo += grafo->getVertice(rota[i])->getWeight();
        }
    }
    return custo;
}

void Heuristic::vnd()
{
    int a, b, c, d, e, f;
    int x = 0;
    bool better = true;
    while(x <= 6)
    {
        if(better)
        {
            a = (rand() % (6)) + 1;
            b = a;
            c = a;
            d = a;
            e = a;
            f = a;
            while(b == a)
            {
                b = (rand() % (6)) + 1;
            }
            while(c == a || c == b)
            {
                c = (rand() % (6)) + 1;
            }
            while(d == a || d == b || d == c)
            {
                d = (rand() % (6)) + 1;
            }
            while(e == a || e == b || e == c || e == d)
            {
                e = (rand() % (6)) + 1;
            }
            while(f == a || f == b || f == c || f == d || f == e)
            {
                f = (rand() % (6)) + 1;
            }
            x = a;
        }
        switch(x)
        {
            //2-opt
            case 1:
            {
                better = opt();
                if(better){ x = a;
                    if(testPrint) cout << endl << "2-opt: ";
                    if(testPrint) telaSol();
                }
                else
                {
                    if(x == a) x = b;
                    else
                    {
                        if(x == b) x = c;
                        else
                        {
                            if(x == c) x = d;
                            else
                            {
                                if(x == d) x = e;
                                else
                                {
                                    if(x == e) x = f;
                                    else
                                    {
                                        if(x == f) x = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }

            //Troca Inter(0,1)
            case 2:
            {
                better = shift();
                if(better){ x = a;
                    if(testPrint) cout << endl << "Shift: ";
                    if(testPrint) telaSol();
                    }
                else
                {
                    if(x == a) x = b;
                    else
                    {
                        if(x == b) x = c;
                        else
                        {
                            if(x == c) x = d;
                            else
                            {
                                if(x == d) x = e;
                                else
                                {
                                    if(x == e) x = f;
                                    else
                                    {
                                        if(x == f) x = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }

            //Troca Inter(1,1)
            case 3:
            {
                better = swap1();
                if(better){ x = a;
                    if(testPrint) cout << endl << "Swap: ";
                    if(testPrint) telaSol();
                    }
                else
                {
                    if(x == a) x = b;
                    else
                    {
                        if(x == b) x = c;
                        else
                        {
                            if(x == c) x = d;
                            else
                            {
                                if(x == d) x = e;
                                else
                                {
                                    if(x == e) x = f;
                                    else
                                    {
                                        if(x == f) x = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }

            //Troca Intra(0,1)
            case 4:
            {
                better = reinsertion();
                if(better){ x = a;
                    if(testPrint) cout << endl << "Reinsertion: ";
                    if(testPrint) telaSol();
                    }
                else
                {
                    if(x == a) x = b;
                    else
                    {
                        if(x == b) x = c;
                        else
                        {
                            if(x == c) x = d;
                            else
                            {
                                if(x == d) x = e;
                                else
                                {
                                    if(x == e) x = f;
                                    else
                                    {
                                        if(x == f) x = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }

            //Cross entre duas rotas
            case 5:
            {
                better = changeBss();
                if(better){ x = a;
                    if(testPrint) cout << endl << "ChangeBss: ";
                    if(testPrint) telaSol();
                    }
                else
                {
                    if(x == a) x = b;
                    else
                    {
                        if(x == b) x = c;
                        else
                        {
                            if(x == c) x = d;
                            else
                            {
                                if(x == d) x = e;
                                else
                                {
                                    if(x == e) x = f;
                                    else
                                    {
                                        if(x == f) x = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }
            case 6:
            {
                better = isolate();
                if(better){ x = a;
                    if(testPrint) cout << endl << "Isolate: ";
                    if(testPrint) telaSol();
                    }
                else
                {
                    if(x == a) x = b;
                    else
                    {
                        if(x == b) x = c;
                        else
                        {
                            if(x == c) x = d;
                            else
                            {
                                if(x == d) x = e;
                                else
                                {
                                    if(x == e) x = f;
                                    else
                                    {
                                        if(x == f) x = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }
        }
    }

    // Apaga rotas vazias
    for(int i = 0; i < (int)solution.size(); i++)
    {
        if(solution[i].size() <= 2)
        {
            solution.erase(solution.begin()+i);
            sTime.erase(sTime.begin()+i);
            truck.erase(truck.begin()+i);
            i--;
        }
    }
}

bool Heuristic::reinsertion2()
{
    bool better = false;
    float auxA, auxB, auxC, auxD;
    int p, q, s, k;
    s = 0;
    p = 0;
    q = 0;
    auxC = 0;
    k = 0;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size() -2; j++)
        {
            auxA = round((dij[solution[i][j-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j+1]->getId()][solution[i][j+2]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j+2]->getId()])*1000);
            for(int l = 1; l < (int)solution[i].size(); l++)
            {
                if(l != j && l != j+1 && l != j+2)
                {
                    auxB = round((dij[solution[i][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j+1]->getId()][solution[i][l]->getId()] - dij[solution[i][l-1]->getId()][solution[i][l]->getId()])*1000);
                    auxD = round((dij[solution[i][l-1]->getId()][solution[i][j+1]->getId()] + dij[solution[i][j]->getId()][solution[i][l]->getId()] - dij[solution[i][l-1]->getId()][solution[i][l]->getId()])*1000);
                    if(auxB < auxD)
                    {
                        if(auxA - auxB > auxC)
                        {
                            p = i;
                            q = j;
                            s = l;
                            auxC = auxA - auxB;
                            better = true;
                            k = 1;
                        }
                    }
                    else
                    {
                        if(auxA - auxD > auxC)
                        {
                            p = i;
                            q = j;
                            s = l;
                            auxC = auxA - auxD;
                            better = true;
                            k = 2;
                        }
                    }
                }
            }
        }
    }
    if(better && k != 0)
    {
        if(k == 1)
        {
            solution[p].insert(solution[p].begin()+s, solution[p][q]);
            if(s <= q)
            {
                q++;
                s++;
            }
            solution[p].insert(solution[p].begin()+(s+1), solution[p][q+1]);
            if(s <= q)
            {
                q++;
            }
            solution[p].erase(solution[p].begin()+q);
            solution[p].erase(solution[p].begin()+q);
        }
        if(k == 2)
        {
            solution[p].insert(solution[p].begin()+s, solution[p][q+1]);
            if(s <= q)
            {
                q++;
                s++;
            }
            solution[p].insert(solution[p].begin()+(s+1), solution[p][q]);
            if(s <= q)
            {
                q++;
            }
            solution[p].erase(solution[p].begin()+q);
            solution[p].erase(solution[p].begin()+q);
        }
    }
    return better;
}

bool Heuristic::opt()
{
    float a, b;
    bool better = false;
    vector<int> aux;
    vector<int> aux2, aux3;
    vector<vertice_s> aux4;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        aux2.clear();
        for(int j = 0; j < (int)solution[i].size(); j++)
        {
            aux2.push_back(solution[i][j]->getId());
        }
        aux3 = aux2;
        for(int j = 0; j < (int)solution[i].size() -2; j++)
        {
            for(int k = j+2; k < (int)solution[i].size() -1; k++)
            {
                aux2 = aux3;
                a = round((dij[solution[i][j]->getId()][solution[i][j+1]->getId()] + dij[solution[i][k]->getId()][solution[i][k+1]->getId()])*1000);
                b = round((dij[solution[i][j]->getId()][solution[i][k]->getId()] + dij[solution[i][j+1]->getId()][solution[i][k+1]->getId()])*1000);
                if(a > b)
                {
                    for(int l = k; l > j; l--)
                    {
                        aux.insert(aux.begin(), solution[i][l]->getId());
                        aux2.erase(aux2.begin()+l);
                    }
                    while(aux.size() > 0)
                    {
                        aux2.insert(aux2.begin()+(j+1), aux[0]);
                        aux.erase(aux.begin());
                    }
                    aux.clear();
                    if(valido(aux2) && valido2(aux2) && valido3(aux2))
                    {
                        for(int w = 0; w < (int)aux2.size(); w++)
                        {
                            aux4.push_back(grafo->getVertice(aux2[w]));
                        }
                        solution[i] = aux4;
                        return true;
                    }
                }
            }
        }
    }
    return better;
}

bool Heuristic::shift()
{
    bool better = false;
    float auxA, auxB, auxC, adj, wait;
    int p, q, r, s;
    r = 0;
    p = 0;
    auxC = 0;
    bool doShift;
    vector<int> rota;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size() -1; j++)
        {
            auxA = round((dij[solution[i][j-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[i][j+1]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j+1]->getId()])*1000);
            for(int k = 0; k < (int)solution.size(); k++)
            {
                if(k != i && solution[i][j]->getWeight() <= truck[k]->getCapacity() &&
                   solution[i][j]->getId() >= this->noClientCount)
                {
                    for(int l = 1; l < (int)solution[k].size(); l++)
                    {
                        auxB = round((dij[solution[k][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[k][l]->getId()] - dij[solution[k][l-1]->getId()][solution[k][l]->getId()])*1000);

                        doShift = true;
                        this->truck[i]->resetBatteryCap();
                        for(int u = 1; u < (int)solution[i].size(); u++)
                        {
                            if(u != j)
                            {
                                if(u !=  j+1)
                                {
                                    if (this->truck[i]->getBatteryCap() >= dij[solution[i][u-1]->getId()][solution[i][u]->getId()])
                                    {
                                        this->truck[i]->removeBatteryCap(dij[solution[i][u-1]->getId()][solution[i][u]->getId()]);
                                        if(solution[i][u]->getId() < this->noClientCount)
                                        {
                                            this->truck[i]->resetBatteryCap();
                                        }
                                    }
                                    else
                                    {
                                        doShift = false;
                                    }
                                }
                                else
                                {
                                    if (this->truck[i]->getBatteryCap() >= dij[solution[i][u-2]->getId()][solution[i][u]->getId()])
                                    {
                                        this->truck[i]->removeBatteryCap(dij[solution[i][u-2]->getId()][solution[i][u]->getId()]);
                                        if(solution[i][u]->getId() < this->noClientCount)
                                        {
                                            this->truck[i]->resetBatteryCap();
                                        }
                                    }
                                    else
                                    {
                                        doShift = false;
                                    }
                                }
                            }
                        }
                        this->truck[i]->resetBatteryCap();
                        this->truck[k]->resetBatteryCap();
                        for(int u = 1; u < (int)solution[k].size(); u++)
                        {
                            if(solution[k][u]->getId() !=  solution[k][l]->getId())
                            {
                                if (this->truck[k]->getBatteryCap() >= dij[solution[k][u-1]->getId()][solution[k][u]->getId()])
                                {
                                    this->truck[k]->removeBatteryCap(dij[solution[k][u-1]->getId()][solution[k][u]->getId()]);
                                    if(solution[k][u]->getId() < this->noClientCount)
                                    {
                                        this->truck[k]->resetBatteryCap();
                                    }
                                }
                                else
                                {
                                    doShift = false;
                                }
                            }
                            else
                            {
                                if(this->truck[k]->getBatteryCap() >= dij[solution[k][u-1]->getId()][solution[i][j]->getId()])
                                {
                                    this->truck[k]->removeBatteryCap(dij[solution[k][u-1]->getId()][solution[i][j]->getId()]);
                                    if(solution[i][j]->getId() < this->noClientCount)
                                    {
                                        this->truck[k]->resetBatteryCap();
                                    }
                                }
                                else
                                {
                                    doShift = false;
                                }
                                if(this->truck[k]->getBatteryCap() >= dij[solution[i][j]->getId()][solution[k][u]->getId()])
                                {
                                    this->truck[k]->removeBatteryCap(dij[solution[i][j]->getId()][solution[k][u]->getId()]);
                                    if(solution[k][u]->getId() < this->noClientCount)
                                    {
                                        this->truck[k]->resetBatteryCap();
                                    }
                                }
                                else
                                {
                                    doShift = false;
                                }
                            }
                        }
                        this->truck[i]->resetBatteryCap();
                        this->truck[k]->resetBatteryCap();

                        adj = dij[solution[k][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[k][l]->getId()] - dij[solution[k][l-1]->getId()][solution[k][l]->getId()];
                        wait = solution[i][j]->getReadyTime() - (sTime[k][l-1].out + dij[solution[k][l-1]->getId()][solution[i][j]->getId()]);
                        if(wait < 0) wait = 0;
                        if(auxA - auxB > auxC && doShift &&
                           adj + solution[i][j]->getServiceTime() + wait <= sTime[k][l].clearance &&
                           sTime[k][l-1].out + dij[solution[k][l-1]->getId()][solution[i][j]->getId()] <= solution[i][j]->getDueTime())
                        {
                            rota.clear();
                            for(int w = 0; w < (int)solution[k].size(); w++)
                            {
                                if(w == l)
                                {
                                    rota.push_back(solution[i][j]->getId());
                                }
                                rota.push_back(solution[k][w]->getId());
                            }
                            if(valido(rota) && valido2(rota) && valido3(rota))
                            {
                                better = true;
                                p = i;
                                q = j;
                                r = k;
                                s = l;
                                auxC = auxA - auxB;
                            }
                        }
                    }
                }
            }
        }
    }
    if(better)
    {
        solution[r].insert(solution[r].begin()+s, solution[p][q]);
        truck[r]->removeCapacity(solution[p][q]->getWeight());
        truck[p]->addCapacity(solution[p][q]->getWeight());
        solution[p].erase(solution[p].begin()+q);
        sTime[r].insert(sTime[r].begin()+s, sTime[p][q]);
        sTime[p].erase(sTime[p].begin()+q);
        updateTime(r);
        updateClearance(r);
        updateTime(p);
        updateClearance(p);
    }
    return better;
}

bool Heuristic::swap1()
{
    vertice_s aux;
    Time aux2;
    bool better = false;
    float auxA, auxB, auxC, adj1, adj2, wait1, wait2;
    int p, q, r, s;
    r = 0;
    p = 0;
    auxC = 0;
    bool doSwap;
    vector<int> rota;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size() -1; j++)
        {
            for(int k = 0; k < (int)solution.size(); k++)
            {
                if(k != i)
                {
                    for(int l = 1; l < (int)solution[k].size()-1; l++)
                    {
                        if(solution[i][j]->getWeight() <= truck[k]->getCapacity() + solution[k][l]->getWeight() &&
                           solution[k][l]->getWeight() <= truck[i]->getCapacity() + solution[i][j]->getWeight() &&
                           solution[i][j]->getId() >= this->noClientCount && solution[k][l]->getId() >= this->noClientCount)
                        {
                            auxA = round(((dij[solution[i][j-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[i][j+1]->getId()]) - (dij[solution[i][j-1]->getId()][solution[k][l]->getId()] + dij[solution[k][l]->getId()][solution[i][j+1]->getId()]))*1000);
                            auxB = round(((dij[solution[k][l-1]->getId()][solution[k][l]->getId()] + dij[solution[k][l]->getId()][solution[k][l+1]->getId()]) - (dij[solution[k][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[k][l+1]->getId()]))*1000);

                            doSwap = true;
                            for(int u = 1; u < (int)solution[i].size() -1; u++)
                            {
                                if(u !=  j)
                                {
                                    if(u !=  j+1)
                                    {
                                        if (this->truck[i]->getBatteryCap() >= dij[solution[i][u-1]->getId()][solution[i][u]->getId()])
                                        {
                                            this->truck[i]->removeBatteryCap(dij[solution[i][u-1]->getId()][solution[i][u]->getId()]);
                                            if(solution[i][u]->getId() < this->noClientCount)
                                            {
                                                this->truck[i]->resetBatteryCap();
                                            }
                                        }
                                        else
                                        {
                                            doSwap = false;
                                        }
                                    }
                                    else
                                    {
                                        if (this->truck[i]->getBatteryCap() >= dij[solution[k][l]->getId()][solution[i][u]->getId()])
                                        {
                                            this->truck[i]->removeBatteryCap(dij[solution[k][l]->getId()][solution[i][u]->getId()]);
                                            if(solution[i][u]->getId() < this->noClientCount)
                                            {
                                                this->truck[i]->resetBatteryCap();
                                            }
                                        }
                                        else
                                        {
                                            doSwap = false;
                                        }
                                    }
                                }
                                else
                                {
                                    if(this->truck[i]->getBatteryCap() >= dij[solution[i][u-1]->getId()][solution[k][l]->getId()])
                                    {
                                        this->truck[i]->removeBatteryCap(dij[solution[i][u-1]->getId()][solution[k][l]->getId()]);
                                    }
                                    else
                                    {
                                        doSwap = false;
                                    }
                                }
                            }
                            this->truck[i]->resetBatteryCap();
                            for(int u = 1; u < (int)solution[k].size() -1; u++)
                            {
                                if(u !=  l)
                                {
                                    if(u !=  l+1)
                                    {
                                        if (this->truck[k]->getBatteryCap() >= dij[solution[k][u-1]->getId()][solution[k][u]->getId()])
                                        {
                                            this->truck[k]->removeBatteryCap(dij[solution[k][u-1]->getId()][solution[k][u]->getId()]);
                                            if(solution[k][u]->getId() < this->noClientCount)
                                            {
                                                this->truck[k]->resetBatteryCap();
                                            }
                                        }
                                        else
                                        {
                                            doSwap = false;
                                        }
                                    }
                                    else
                                    {
                                        if (this->truck[k]->getBatteryCap() >= dij[solution[i][j]->getId()][solution[k][u]->getId()])
                                        {
                                            this->truck[k]->removeBatteryCap(dij[solution[i][j]->getId()][solution[k][u]->getId()]);
                                            if(solution[k][u]->getId() < this->noClientCount)
                                            {
                                                this->truck[k]->resetBatteryCap();
                                            }
                                        }
                                        else
                                        {
                                            doSwap = false;
                                        }
                                    }
                                }
                                else
                                {
                                    if(this->truck[k]->getBatteryCap() >= dij[solution[k][u-1]->getId()][solution[i][j]->getId()])
                                    {
                                        this->truck[k]->removeBatteryCap(dij[solution[k][u-1]->getId()][solution[i][j]->getId()]);
                                    }
                                    else
                                    {
                                        doSwap = false;
                                    }
                                }
                            }
                            this->truck[k]->resetBatteryCap();

                            adj2 = dij[solution[i][j-1]->getId()][solution[k][l]->getId()] + dij[solution[k][l]->getId()][solution[i][j+1]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j]->getId()] - dij[solution[i][j]->getId()][solution[i][j+1]->getId()];
                            adj1 = dij[solution[k][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[k][l+1]->getId()] - dij[solution[k][l-1]->getId()][solution[k][l]->getId()] - dij[solution[k][l]->getId()][solution[k][l+1]->getId()];
                            wait1 = solution[i][j]->getReadyTime() - (sTime[k][l-1].out + dij[solution[k][l-1]->getId()][solution[i][j]->getId()]);
                            wait2 = solution[k][l]->getReadyTime() - (sTime[i][j-1].out + dij[solution[i][j-1]->getId()][solution[k][l]->getId()]);

                            if(auxA + auxB > auxC && doSwap &&
                               adj1 + solution[i][j]->getServiceTime() + wait1 <= sTime[k][l+1].clearance &&
                               adj2 + solution[k][l]->getServiceTime() + wait2 <= sTime[i][j+1].clearance &&
                               sTime[i][j-1].out + dij[solution[i][j-1]->getId()][solution[k][l]->getId()] <= solution[k][l]->getDueTime() &&
                               sTime[k][l-1].out + dij[solution[k][l-1]->getId()][solution[i][j]->getId()] <= solution[i][j]->getDueTime())
                            {
                                rota.clear();
                                for(int w = 0; w < (int)solution[k].size(); w++)
                                {
                                    if(w == l)
                                    {
                                        rota.push_back(solution[i][j]->getId());
                                    }
                                    else
                                    {
                                        rota.push_back(solution[k][w]->getId());
                                    }
                                }
                                if(valido(rota) && valido2(rota) && valido3(rota))
                                {
                                    rota.clear();
                                    for(int z = 0; z < (int)solution[i].size(); z++)
                                    {
                                        if(z == j)
                                        {
                                            rota.push_back(solution[k][l]->getId());
                                        }
                                        else
                                        {
                                            rota.push_back(solution[i][z]->getId());
                                        }
                                    }
                                    if(valido(rota) && valido2(rota) && valido3(rota))
                                    {
                                        better = true;
                                        p = i;
                                        q = j;
                                        r = k;
                                        s = l;
                                        auxC = auxA + auxB;
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(better)
    {
        truck[r]->addCapacity(solution[r][s]->getWeight());
        truck[r]->removeCapacity(solution[p][q]->getWeight());
        truck[p]->addCapacity(solution[p][q]->getWeight());
        truck[p]->removeCapacity(solution[r][s]->getWeight());
        aux = solution[r][s];
        solution[r][s] = solution[p][q];
        solution[p][q] = aux;
        aux = NULL;
        aux2 = sTime[r][s];
        sTime[r][s] = sTime[p][q];
        sTime[p][q] = aux2;
        updateTime(r);
        updateClearance(r);
        updateTime(p);
        updateClearance(p);
    }
    return better;
}

bool Heuristic::reinsertion()
{
    bool better = false;
    float auxA, auxB, auxC;
    int p, q, s;
    auxC = 0;
    bool doReins;
    solution.push_back(solution[0]);
    sTime.push_back(sTime[0]);
    vector<int> rota;
    for(int i = 0; i < (int)solution.size()-1; i++)
    {
        for(int j = 1; j < (int)solution[i].size() -1; j++)
        {
            auxA = round((dij[solution[i][j-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[i][j+1]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j+1]->getId()])*1000);
            for(int l = 1; l < (int)solution[i].size(); l++)
            {
                if(l != j && l != j + 1)
                {
                    auxB = round((dij[solution[i][l-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[i][l]->getId()] - dij[solution[i][l-1]->getId()][solution[i][l]->getId()])*1000);


                    doReins = true;
                    for(int u = 1; u < (int)solution[i].size() -1; u++)
                    {
                        if(u !=  j)
                        {
                            if(u != j+1)
                            {
                                if(u != l)
                                {
                                    if (this->truck[i]->getBatteryCap() >= dij[solution[i][u-1]->getId()][solution[i][u]->getId()])
                                    {
                                        this->truck[i]->removeBatteryCap(dij[solution[i][u-1]->getId()][solution[i][u]->getId()]);
                                        if(solution[i][u]->getId() < this->noClientCount)
                                        {
                                            this->truck[i]->resetBatteryCap();
                                        }
                                    }
                                    else
                                    {
                                        doReins = false;
                                    }
                                }
                                else
                                {
                                    if (this->truck[i]->getBatteryCap() >= dij[solution[i][l-1]->getId()][solution[i][j]->getId()])
                                    {
                                        this->truck[i]->removeBatteryCap(dij[solution[i][l-1]->getId()][solution[i][j]->getId()]);
                                        if(solution[i][u]->getId() < this->noClientCount)
                                        {
                                            this->truck[i]->resetBatteryCap();
                                        }
                                    }
                                    else
                                    {
                                        doReins = false;
                                    }
                                    if (this->truck[i]->getBatteryCap() >= dij[solution[i][j]->getId()][solution[i][u]->getId()])
                                    {
                                        this->truck[i]->removeBatteryCap(dij[solution[i][j]->getId()][solution[i][u]->getId()]);
                                        if(solution[i][u]->getId() < this->noClientCount)
                                        {
                                            this->truck[i]->resetBatteryCap();
                                        }
                                    }
                                    else
                                    {
                                        doReins = false;
                                    }
                                }
                            }
                            else
                            {
                                if(this->truck[i]->getBatteryCap() >= dij[solution[i][u-2]->getId()][solution[i][u]->getId()])
                                {
                                    this->truck[i]->removeBatteryCap(dij[solution[i][u-2]->getId()][solution[i][u]->getId()]);
                                }
                                else
                                {
                                    doReins = false;
                                }
                            }
                        }
                    }
                    this->truck[i]->resetBatteryCap();



                    if(auxA - auxB > auxC && doReins)
                    {
                        int z = j;
                        solution[solution.size()-1] = solution[i];
                        sTime[sTime.size()-1] = sTime[i];
                        solution[solution.size()-1].insert(solution[solution.size()-1].begin()+l, solution[i][j]);
                        sTime[sTime.size()-1].insert(sTime[sTime.size()-1].begin()+l, sTime[i][j]);
                        if(l <= j)
                        {
                            z = j+1;
                        }
                        solution[solution.size()-1].erase(solution[solution.size()-1].begin()+z);
                        sTime[sTime.size()-1].erase(sTime[sTime.size()-1].begin()+z);
                        updateTime(solution.size()-1);
                        if(updateClearance2(solution.size()-1) && sTime[i][l-1].out + dij[solution[i][l-1]->getId()][solution[i][j]->getId()] <= solution[i][j]->getDueTime())
                        {
                            //better = false;
                            rota.clear();
                            for(int w = 0; w < (int)solution[i].size(); w++)
                            {
                                if(w != j)
                                {
                                    if(w == l)
                                    {
                                        rota.push_back(solution[i][j]->getId());
                                    }
                                    rota.push_back(solution[i][w]->getId());
                                }
                            }
                            if(valido(rota) && valido2(rota) && valido3(rota))
                            {
                                better = true;
                                p = i;
                                q = j;
                                s = l;
                                auxC = auxA - auxB;
                            }
                        }
                    }
                }
            }
        }
    }
    if(better)
    {
        solution[p].insert(solution[p].begin()+s, solution[p][q]);
        if(s <= q)
        {
            q++;
        }
        solution[p].erase(solution[p].begin()+q);
        updateTime(p);
        updateClearance(p);
    }
    solution.erase(solution.begin()+(solution.size()-1));
    sTime.erase(sTime.begin()+(sTime.size()-1));
    return better;
}

bool Heuristic::cross()
{
    vector<vertice_s> v1, v2;
    bool better = false;
    float auxA, auxB, auxC, auxD;
    int p, q, r, s, t;
    float capA, capB, cA, cB;
    cA = 0;
    cB = 0;
    s = 0;
    q = 0;
    auxD = 0;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int k = 0; k < (int)solution.size(); k++)
        {
            capA = 0;
            if(i != k)
            {
                for(int j = 0; j < (int)solution[i].size() -1; j++)
                {
                    capA += solution[i][j]->getWeight();
                    capB = 0;
                    for(int l = solution[k].size() -1; l > 0; l--)
                    {
                        recontTruck(i);
                        recontTruck(k);
                        capB += solution[k][l]->getWeight();
                        auxA = round((dij[solution[i][j]->getId()][solution[i][j+1]->getId()] + dij[solution[k][l]->getId()][solution[k][l-1]->getId()])*1000);
                        auxB = round((dij[solution[i][j]->getId()][solution[k][l-1]->getId()] + dij[solution[k][l]->getId()][solution[i][j+1]->getId()])*1000);
                        auxC = round((dij[solution[i][j]->getId()][solution[k][l]->getId()] + dij[solution[k][l-1]->getId()][solution[i][j+1]->getId()])*1000);
                        if(auxA - auxB > auxD && capA <= truck[k]->getCapacity() + capB && capB <= truck[i]->getCapacity() + capA)
                        {
                            p = i;
                            q = j;
                            r = k;
                            s = l;
                            t = 0;
                            cA = capA;
                            cB = capB;
                            auxD = auxA - auxB;
                            better = true;
                        }
                        if(auxA - auxC > auxD && capA + capB <= truck[i]->getInitialCapacity() && (truck[i]->getInitialCapacity() - truck[i]->getCapacity() - capA) + (truck[k]->getInitialCapacity() - truck[k]->getCapacity() - capB) <= truck[k]->getInitialCapacity())
                        {
                            p = i;
                            q = j;
                            r = k;
                            s = l;
                            t = 1;
                            cA = capA;
                            cB = capB;
                            auxD = auxA - auxC;
                            better = true;
                        }
                    }
                }
            }
        }
    }
    if(better)
    {
        if(t == 0)
        {
            for(int i = 0; i <= q; i++)
            {
                v1.push_back(solution[p][i]);
            }
            for(int l = s-1; l >= 0; l--)
            {
                v1.push_back(solution[r][l]);
            }
            for(int j = solution[r].size()-1; j >= s; j--)
            {
                v2.push_back(solution[r][j]);
            }
            for(int k = q+1; k < (int)solution[p].size(); k++)
            {
                v2.push_back(solution[p][k]);
            }
            solution[p] = v1;
            solution[r] = v2;
            capA = cA + (truck[r]->getInitialCapacity() - (cB + truck[r]->getCapacity()));
            capB = cB + (truck[p]->getInitialCapacity() - (cA + truck[p]->getCapacity()));
            truck[p]->resetCapacity();
            truck[p]->removeCapacity(capA);
            truck[r]->resetCapacity();
            truck[r]->removeCapacity(capB);
            v1.clear();
            v2.clear();
        }
        else
        {
            if(t == 1)
            {
                for(int i = 0; i <= q; i++)
                {
                    v1.push_back(solution[p][i]);
                }
                for(int j = s; j < (int)solution[r].size(); j++)
                {
                    v1.push_back(solution[r][j]);
                }
                for(int l = 0; l < s; l++)
                {
                    v2.push_back(solution[r][l]);
                }
                for(int k = q+1; k < (int)solution[p].size(); k++)
                {
                    v2.push_back(solution[p][k]);
                }
                solution[p] = v1;
                solution[r] = v2;
                capA = cA + cB;
                capB = (truck[p]->getInitialCapacity() - (truck[p]->getCapacity() + cA)) + (truck[r]->getInitialCapacity() - (truck[r]->getCapacity() + cB));
                truck[p]->resetCapacity();
                truck[p]->removeCapacity(capA);
                truck[r]->resetCapacity();
                truck[r]->removeCapacity(capB);
                v1.clear();
                v2.clear();
            }
            else
            {
                cout << endl << "ERRO NO CROSS" << endl;
            }
        }
    }
    return better;
}

bool Heuristic::changeBss()
{
    Time tAux;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    float custo, mCusto, custo2;
    int o, p, q, r, b;
    vector<int> aux;
    vector<int> aux2;
    vector<vector<int>> rotas;
    bool doChange = false;
    int tentativa = 0;
    aux.clear();
    bool problema = true;

    while(problema)
    {
    // Rotas da solu��o
        for(int i = 0; i < (int)solution.size(); i++)
        {
            problema = false;
            for(int j = 0; j < (int)solution[i].size(); j++)
            {
                aux.push_back(solution[i][j]->getId());
            }


            if(!valido(aux) || !valido2(aux))
            {
                if(testPrint) telaSol();
                addBss();
                if(testPrint) telaSol();
                problema = true;
                break;
            }
            else
            {
                rotas.push_back(aux);
            }
            aux.clear();
        }
        tentativa++;
    }
    mCusto = 0;

    // Rotas variantes
    for(int i = 0; i < (int)solution.size(); i++)
    {
        custo = calcCusto(rotas[i]);
        for(int j = 1; j < (int)rotas[i].size(); j++)
        {
            aux = rotas[i];
            if(aux[j] < this->noClientCount && aux[j] >= this->depCount)
            {
                b = aux[j];
                aux.erase(aux.begin() + j);
                aux2 = aux;
                if(valido(aux) && valido2(aux) && valido3(aux))
                {
                    solution[i].erase(solution[i].begin() + j);
                    sTime[i].erase(sTime[i].begin()+j);
                    updateTime(i);
                    updateClearance(i);
                    return true;
                }
                else
                {
                    for(int k = this->depCount; k < this->noClientCount; k++)
                    {
                        if(k != b)
                        {
                            for(int l = 0; l < (int)aux.size(); l++)
                            {
                                aux.insert(aux.begin()+l, k);
                                if(valido(aux) && valido2(aux) && valido3(aux))
                                {
                                    custo2 = calcCusto(aux);
                                    if(custo - custo2 > mCusto)
                                    {
                                        mCusto = custo - custo2;
                                        o = i;
                                        p = j;
                                        q = k;
                                        r = l;
                                        doChange = true;
                                    }
                                }
                                aux = aux2;
                            }
                        }
                    }
                }
            }
        }
    }
    if(doChange)
    {
        solution[o].erase(solution[o].begin() + p);
        sTime[o].erase(sTime[o].begin() + p);
        solution[o].insert(solution[o].begin()+r, grafo->getVertice(q));
        sTime[o].insert(sTime[o].begin()+r, tAux);
        updateTime(o);
        updateClearance(o);
        return true;
    }
    return false;
}

bool Heuristic::isolate()
{
    bool better = false;
    float auxA, auxB, auxC;
    int p, q, r, s;
    auxC = 0;
    bool doShift;
    vector<Time> vtAux;
    Time tAux;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    r = 0;
    p = 0;
    q = 0;

    vector<int> rota;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 1; j < (int)solution[i].size() -1; j++)
        {
            if(solution[i][j]->getId() >= this->noClientCount)
            {
                auxA = round((dij[solution[i][j-1]->getId()][solution[i][j]->getId()] + dij[solution[i][j]->getId()][solution[i][j+1]->getId()] - dij[solution[i][j-1]->getId()][solution[i][j+1]->getId()])*1000);
                for(int k = 0; k < depCount; k++)
                {
                    auxB = round((dij[k][solution[i][j]->getId()] + dij[solution[i][j]->getId()][k])*1000);
                    doShift = true;
                    if(auxA - auxB > auxC && doShift)
                    {
                        rota.clear();
                        for(int w = 0; w < 2; w++)
                        {
                            if(w == 1)
                            {
                                rota.push_back(solution[i][j]->getId());
                            }
                            rota.push_back(k);
                        }
                        if(valido(rota) && valido2(rota) && valido3(rota))
                        {
                            better = true;
                            p = i;
                            q = j;
                            r = k;
                            auxC = auxA - auxB;
                        }
                    }
                }
            }
        }
    }
    if(better)
    {
        s = solution.size();
        truck.push_back(make_shared<Vehicle>(s, vehicleCapacity, batteryCap));
        sTime.push_back(vtAux);
        routeAux.clear();
        routeAux.push_back(grafo->getVertice(r));
        sTime[s].insert(sTime[s].begin(), tAux);
        routeAux.push_back(solution[p][q]);
        truck[s]->removeCapacity(solution[p][q]->getWeight());
        sTime[s].insert(sTime[s].begin(), tAux);
        routeAux.push_back(grafo->getVertice(r));
        sTime[s].insert(sTime[s].begin(), tAux);
        solution.push_back(routeAux);
        routeAux.clear();

        updateTime(s);
        updateClearance(s);

        truck[p]->addCapacity(solution[p][q]->getWeight());
        solution[p].erase(solution[p].begin()+q);
        sTime[p].erase(sTime[p].begin()+q);

        updateTime(p);
        updateClearance(p);
    }
    return better;
}

bool Heuristic::viability()
{
    bool a, b, c;
    bool val = true;
    vector<int> rota;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        rota.clear();
        for(int j = 0; j < (int)solution[i].size(); j++)
        {
           rota.push_back(solution[i][j]->getId());
        }
        a = valido(rota);
        b = valido2(rota);
        c = valido3(rota);
        if(!a || !b || !c) val = false;
    }
    return val;
}

bool Heuristic::valido(vector<int> rota)
{
    float bCap = this->batteryCap;
    bool a = true;
    for(int i = 1; i < (int)rota.size(); i++)
    {
        bCap = bCap - dij[rota[i-1]][rota[i]];
        if(bCap < 0)
        {
            a = false;
            i = rota.size();
        }
        else
        {
            if (rota[i] < this->noClientCount)
            {
                bCap = this->batteryCap;
            }
        }
    }
    return a;
}

bool Heuristic::valido2(vector<int> rota)
{
    float vCap = this->vehicleCapacity;
    bool a = true;
    for(int i = 1; i < (int)rota.size(); i++)
    {
        if (rota[i] < this->clients)
        {
            vCap = vCap - grafo->getVertice(rota[i])->getWeight();
            if(vCap < 0)
            {
                a = false;
                i = rota.size();
            }
        }
    }
    return a;
}

bool Heuristic::valido3(vector<int> rota)
{
    bool a = true;
    float wait;
    float clearance = INFINITE;
    float aux;
    vector<Time> rTime;
    Time tAux;
    tAux.clearance = 0;
    tAux.wait = 0;
    tAux.in = 0;
    tAux.out = 0;
    rTime.push_back(tAux);
    for(int k = 1; k < (int)rota.size(); k++)
    {
        rTime.push_back(tAux);
        rTime[k].in = dij[rota[k-1]][rota[k]] + rTime[k-1].out;
        wait = grafo->getVertice(rota[k])->getReadyTime() - rTime[k].in;
        if(wait < 0) wait = 0;
        rTime[k].wait = wait;
        rTime[k].out = rTime[k].in + rTime[k].wait + grafo->getVertice(rota[k])->getServiceTime();
    }
    for(int k = rota.size()-1; k >= 1; k--)
    {
        aux = grafo->getVertice(rota[k])->getDueTime() - rTime[k].in;
        if(aux <= clearance)
        {
            clearance = aux;
            if(clearance < 0)
            {
                a = false;
                clearance = 0;
            }
        }
        else
        {
            clearance = clearance + rTime[k].wait;
            if(clearance > aux)
            {
                clearance = aux;
            }
        }
        rTime[k].clearance = clearance;
    }
    return a;
}

void Heuristic::updateClearance(int i)
{
    float clearance = INFINITE;
    float aux;
    for(int k = solution[i].size()-1; k >= 1; k--)
    {
        aux = solution[i][k]->getDueTime() - sTime[i][k].in;
        if(aux <= clearance)
        {
            clearance = aux;
            if(clearance < 0)
            {
                clearance = 0;
                this->erro = true;
            }
        }
        else
        {
            clearance = clearance + sTime[i][k].wait;
            if(clearance > aux)
            {
                clearance = aux;
            }
        }
        sTime[i][k].clearance = clearance;
    }
}

void Heuristic::updateTime(int i)
{
    float wait;
    for(int k = 1; k < (int)solution[i].size(); k++)
    {
        sTime[i][k].in = dij[solution[i][k-1]->getId()][solution[i][k]->getId()] + sTime[i][k-1].out;
        wait = solution[i][k]->getReadyTime() - sTime[i][k].in;
        if(wait < 0) wait = 0;
        sTime[i][k].wait = wait;
        sTime[i][k].out = sTime[i][k].in + sTime[i][k].wait + solution[i][k]->getServiceTime();
    }
}

bool Heuristic::updateClearance2(int i)
{
    float clearance = INFINITE;
    float aux;
    bool ok;
    ok = true;
    for(int k = solution[i].size()-1; k >= 1; k--)
    {
        aux = solution[i][k]->getDueTime() - sTime[i][k].in;
        if(aux <= clearance)
        {
            clearance = aux;
            if(clearance < 0)
            {
                ok = false;
                clearance = 0;
            }
        }
        else
        {
            clearance = clearance + sTime[i][k].wait;
            if(clearance > aux)
            {
                clearance = aux;
            }
        }
        sTime[i][k].clearance = clearance;
    }
    return ok;
}

void Heuristic::clearFull()
{
    route.clear();
    points.clear();
    bss.clear();
    routeAux.clear();
    abss.clear();
    sTime.clear();
}

void Heuristic::telaSol()
{
    cout << endl << "Solucao:";
    for(int i = 0; i < (int)solution.size(); i++)
    {
        cout << endl << "[ ";
        for(int j = 0; j < (int)solution[i].size(); j++)
        {
            if(solution[i][j]->getId() < this->depCount)
                cout << "d" << solution[i][j]->getId() << " ";
            else if(solution[i][j]->getId() < this->noClientCount)
                    cout << "s" << solution[i][j]->getId() - this->depCount << " ";
                else cout << "c" << solution[i][j]->getId() - this->noClientCount << " ";
        }
        cout << "]" << endl;
    }
    cout << "Custo: " << calcCusto() << endl;
}

void Heuristic::updateBss()
{
    bss.clear();
    dep.clear();
    bool encontrado;
    for(int i = 0; i < (int)solution.size(); i++)
    {
        for(int j = 0; j < (int)solution[i].size(); j ++)
        {
            if(solution[i][j]->getId() >= this->depCount && solution[i][j]->getId() < this->noClientCount)
            {
                encontrado = false;
                for(int k = 0; k < (int)bss.size(); k++)
                {
                    if(solution[i][j]->getId() == bss[k]->getId())
                    {
                        encontrado = true;
                        k = bss.size();
                    }
                }
                if (!encontrado)
                {
                    bss.push_back(solution[i][j]);
                }
            }
            if(solution[i][j]->getId() < this->depCount)
            {
                encontrado = false;
                for(int k = 0; k < (int)dep.size(); k++)
                {
                    if(solution[i][j]->getId() == dep[k]->getId())
                    {
                        encontrado = true;
                        k = dep.size();
                    }
                }
                if (!encontrado)
                {
                    dep.push_back(solution[i][j]);
                }
            }
        }
    }
}
