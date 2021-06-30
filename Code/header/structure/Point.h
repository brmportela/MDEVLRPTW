#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "../Helper.h"

#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

/**
*   Define um ponto 2D utilizado para localizar um vértice do grafo
**/
class Point
{
    private:
        float x;
        float y;
    public:
        Point(float x, float y);

        float getCoordX();
        float getCoordY();

        // Retorna a distância entre dois pontos
        float getDistance(std::shared_ptr<Point> p2);

        // Verifica se os pontos são iguais
        bool equals(std::shared_ptr<Point> p2);
};

#endif // POINT_H_INCLUDED
