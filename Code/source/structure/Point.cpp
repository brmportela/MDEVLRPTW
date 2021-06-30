#include "../../header/structure/Point.h"

Point::Point(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Point::getCoordX()
{
    return this->x;
}

float Point::getCoordY()
{
    return this->y;
}

float Point::getDistance(std::shared_ptr<Point> p2)
{
    if(p2 != NULL)
    {
        // Dist�ncia entre dois pontos
        return sqrt(pow((p2->getCoordX() - this->x), 2) + pow((p2->getCoordY() - this->y), 2));
    }

    cout << "[ Erro ]: N�o foi poss�vel encontrar a dist�ncia entre dois pontos" << endl;

    return INFINITE;
}

bool Point::equals(std::shared_ptr<Point> p2)
{
    if(p2 != NULL)
    {
        // Compara as coordenadas dos pontos
        return ((p2->getCoordX() == this->x) && (p2->getCoordY() == this->y));
    }

    cout << "[ Erro ]: N�o foi poss�vel verificar se dois pontos s�o iguais" << endl;

    return false;
}
