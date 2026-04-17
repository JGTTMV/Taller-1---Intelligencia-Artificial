#pragma once
#include <string>
#include <vector>

class Map
{

protected: //se fine como protected para que la clase ColorMap pueda acceder a los miembros sin necesidad de getters
    int h;
    int w;
    std::vector<std::vector<int>> _map;

public:
    Map();                              //constructor por defecto con mapa vacio
    Map(std::string filename);          //constructor que carga el mapa desde un archivo
    Map(const Map& rhs);                //constructor copia
    ~Map();                             //destructor
    Map& operator=(const Map& rhs);     //asigna operador
   
    bool isValidCordinates(int x, int y) const;    //regresa true si las coordenadas (x,y) son validas dentro del mapa
    

    void print() const;                 //imprime mapa en pantalla
    void print(std::vector<std::pair<int,int>>) const; //imprime el mapa y el camino encontrado 

    friend bool operator==(const Map& lhs, const Map& rhs); //se declara un operador global de igualdad para comparar dos mapas
    friend class Search;                //para acceder a los miembros de Map desde Search sin necesidad de getters
};

bool operator==(const Map& lhs, const Map& rhs);    //regresa true si ambos mapas son equivalentes