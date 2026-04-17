#pragma once
#include "Map.h"
#include <array>




class ColorMap:public Map //clase que hereda de Map y reimplementa los metodos print para imprimir con colores
{
private:
    std::array<int,5> colors;
public:
    ColorMap(const Map& rhs);
    void print() const;                                //imprime el mapa en pantalla
    void print(std::vector<std::pair<int,int>>) const; //imprime el mapa y el camino encontrado


};