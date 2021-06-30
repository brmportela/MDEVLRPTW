#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <locale.h>
#include "header/FileHelper.h"
#include "header/heuristic/Heuristic.h"

using namespace std;
using namespace std::chrono;
typedef std::shared_ptr<Heuristic> heuristic_s;
typedef std::shared_ptr<Vertice> vertice_s;
//int qt = 1;
//heuristic_s *resultados = new heuristic_s[qt];
/*
void orderResultados(int left, int right)
{
    int i = left;
    int j = right;
    heuristic_s tmp;
    float pivot = resultados[(left + right)/2]->getMinCost();

    while (i <= j)
    {
        while (resultados[i]->getMinCost() < pivot && i < right)
        {
            i++;
        }
        while (resultados[j]->getMinCost() > pivot && j > left)
        {
            j--;
        }
        if (i <= j)
        {
            tmp = resultados[i];

            resultados[i] = resultados[j];

            resultados[j] = tmp;

            i++;
            j--;
        }
    }
    if (left < j)
    {
        orderResultados(left, j);
    }
    if (i < right)
    {
        orderResultados(i, right);
    }

}
*/
int main(int argc, char *argv[])
{
    //cout << "argc: " << argc << endl;
    // Verifica os parâmetros de entrada do console
    switch(argc)
    {
        // Gerar solução
        case 6:
        {
            if((argv[1] != NULL) && (argv[1] == string("run")))
            {
                string ab = string(argv[2]);
                string rWFile = "results/R_" + ab.substr(9);
                fstream rW;
                rW.open(rWFile.c_str());
                rW.seekp(0, ios_base::end);

                string nFile = "results/N_" + ab.substr(9);
                fstream n;
                n.open(nFile.c_str());
                n.seekp(0, ios_base::end);

                if((argv[2] != NULL) && (argv[3] != NULL) && (argv[4] != NULL) && (argv[5] != NULL) )
                {
                    //std::shared_ptr<Heuristic> h = make_shared<Heuristic>(string(argv[2]));
                    std::shared_ptr<Heuristic> h;
                    float alfa = atof(argv[3]);
                    int seed;
                    int maxIteration = atoi(argv[4]);
                    //int maxIteration = 4;
                    int heuristica = atoi(argv[5]);
                    //int heuristica = 1;
                    seed = time(NULL);
                    seed = seed % 10000;
                    //seed = 0;
                    seed = seed + alfa;
                    //int bseed[30] = {3152, 9908, 8501, 7434, 9150, 6690, 2345, 1007, 6318, 4420, 6824, 1038, 2286, 400, 5195, 1627, 494, 7363, 9177, 6568, 9079, 461, 9160, 108, 2280, 5889, 6597, 4709, 4343, 334};
                    //seed = bseed[seed];
                    //seed = 3632;
                    srand(seed);

                    if(alfa == 0) switch(heuristica)
                    {
                        case 1:
                        {
                            cout << "Heuristica 1 - Insercao mais barata" << endl;
                            break;
                        }
                        case 2:
                        {
                            cout << "Heuristica 2 - AG com chave randomica" << endl;
                            break;
                        }
                        case 3:
                        {
                            cout << "Heuristica 3 - AG2 com chave randomica" << endl;
                            break;
                        }
                        case 4:
                        {
                            cout << "Heuristica 4 - Destruicao de rotas" << endl;
                            break;
                        }
                        case 5:
                        {
                            cout << "Heuristica 5 - Intercalacao entre Destruicao de rotas e Insercao mais barata" << endl;
                            break;
                        }
                        default:
                        {
                            cout << "Heuristica Incorreta!";
                            break;
                        }
                    }
                    cout << "Gerando solucao [ Seed = " << seed << " | maxIteration = " << maxIteration << "]"<< endl;
                    float minCost = INFINITE/100; // Custo mínimo
                    float cost = 0; // Custo atual
                    int j = 0;
                    int m = maxIteration;
                    cout << "Iteracao: " << endl;
                    clock_t t;
                    t = clock();
                    int rE = 0;
                    float delta;
                    int iter = -1;

                    bool melhora = false;
                    // Gera até maxIteration de soluções
                    for(int i = 0; i < m; i++)
                    {
                        //cout << "A1" << endl;
                        std::shared_ptr<Heuristic> g = make_shared<Heuristic>(string(argv[2]));
                        //cout << "A2" << endl;
                        if(heuristica == 1 && (i == 400 || i == 600 || i == 800))
                        {
                            rE++;
                            g->incRota(rE);
                        }
                        //cout << "A3" << endl;
                        switch(heuristica)
                        {
                            case 1:
                            {
                                cost = g->run();
                                break;
                            }
                            case 2:
                            {
                                cost = g->runB();
                                break;
                            }
                            case 3:
                            {
                                //cost = g->runC();
                                break;
                            }
                            case 4:
                            {
                                cost = g->runD();
                                break;
                            }
                            case 5:
                            {
                                if (i % 2 == 0)
                                    cost = g->runD();
                                else
                                    cost = g->run();
                                break;
                            }
                            default:
                            {
                                cout << "heuristica incorreta!";
                                break;
                            }
                        }
                        //cout << "A4" << endl;


                        /*bool a = true;
                        // Monta tabela de melhores soluções
                        if(j < qt)
                        {
                            resultados[j] = g;
                        }
                        else
                        {
                            orderResultados(0, qt-1);
                            for(int w = 0; w < qt; w++)
                            {
                                if(g->getMinCost() == resultados[w]->getMinCost())
                                {
                                    a = false;
                                }
                            }
                            if(resultados[qt-1]->getMinCost() > g->getMinCost() && a)
                            {
                                resultados[qt-1] = g;
                            }
                        }*/
                        melhora = false;
                        //cout << "A5" << endl;
                        // Atualiza custo mínimo
                        if(roundf(cost*100)/100 < roundf(minCost*100)/100)
                        {
                            delta = minCost - cost;
                            iter = j;
                            minCost = cost;
                            h = g;
                            /*if(m != 1) //Zerar contador com melhora
                            {
                                i = 0;
                            }*/
                            melhora = true;
                        }
                        //cout << "A6" << endl;
                        j++;
                        if(melhora)
                        {
                            cout << "*" << j  << " " << minCost << endl;
                            n << j << " " << minCost << endl;
                        }
                        else if(j % 50 == 0)
                        {
                            //if(minCost < 99999) n << j << " " << minCost << endl;
                            cout << j << endl;
                        }
                        //cout << "A7" << endl;
                        //Parar no ótimo
                        if(g->getGResult() != 0 && g->getGResult() == roundf(minCost*100)/100)
                        {
                            i = m;
                        }
                        //cout << "A8" << endl;
                        //g = NULL;
                        //out << "A9" << endl;
                    }
                    cout << "F " << j << " " << minCost << endl;
                    n << j << " " << minCost << endl;
                    cout << endl;
                    t = clock() - t;
                    rW << h->getCDep() << " ";
                    rW << h->getCRotas() << " ";
                    rW << h->getCBss() << " ";
                    rW << minCost << " ";
                    rW << ((float)t)/CLOCKS_PER_SEC << " ";
                    rW << seed << " ";
                    rW << delta << " ";
                    rW << iter << " ";
                    rW << j;
                    rW << endl;
                    cout << endl << "Tempo clock: " << ((float)t)/CLOCKS_PER_SEC << endl;
                    cout << "Resultado: " << minCost << endl;
                    cout << "dp. " << h->getCDep() << "/ vh. " <<  h->getCRotas() << "/ st. " << h->getCBss() << endl;
                    string resFile = "results/" + ab.substr(9);
                    ifstream res;
                    res.open(resFile.c_str());
                    float numero;
                    if(!res.is_open())
                    {
                        numero = INFINITE;
                    }
                    else
                    {
                        string txtLine;
                        getline(res, txtLine);
                        istringstream streamLine1(txtLine);
                        streamLine1 >> numero;
                        res.close();
                    }
                    if(numero > minCost)
                    {
                        numero = minCost;
                        h->plot();
                        h->printSol(((float)t)/CLOCKS_PER_SEC);
                    }
                    cout << endl << "Melhor resultado global: " << numero << endl << endl;
                }
                else
                {
                    cout << "[ Erro ] Parâmetros inválidos!" << endl;
                }
                rW.flush();
                rW.close();

                n.flush();
                n.close();
            }
            break;
        }
        default:
        {
            cout << "[ Erro ]: Nenhum comando válido!" << endl;
        }
    }
    return 0;
}
