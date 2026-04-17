#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
    //Verificacion basica de argumentos (archivo y coordenadas)
    if (argc != 6) 
    {
        std::cerr << "Error: Argumentos insuficientes." << std::endl;
        std::cerr << "Uso: " << argv[0] << " <map_file> <x1> <y1> <x2> <y2>" << std::endl;
        return 1;
    }

    //Convierte argumentos de coordenadas a enteros
    int start_x = std::stoi(argv[2]);
    int start_y = std::stoi(argv[3]);
    int goal_x = std::stoi(argv[4]);
    int goal_y = std::stoi(argv[5]);

    Map map(argv[1]);
    ColorMap colorMap(map);

    if (!map.isValidCordinates(start_x, start_y) || !map.isValidCordinates(goal_x, goal_y)) 
    {
        std::cerr << "Error: Coordenadas fuera de los límites del mapa." << std::endl;
        return 1;
    }

    int opcion = 0;
    do 
    {
        std::cout << "\n--- MENU DE BUSQUEDA DE RUTAS ---" << std::endl;
        std::cout << "1. Busqueda BFS (Exploracion total)" << std::endl;
        std::cout << "2. Busqueda Greedy (Heuristica Manhattan)" << std::endl;
        std::cout << "3. Salir" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        std::vector<std::pair<int, int>> path;

        switch (opcion) //Cada opcion ejecuta un algoritmo distinto
        {
            case 1:
                path = Search::BFS(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 2:
                path = Search::Greedy(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 3:
                std::cout << "Saliendo del programa..." << std::endl;
                continue; 
            default:
                std::cout << "Opcion no valida." << std::endl;
                continue;
        }

        if (!path.empty()) //Si se encontro un camino, se imprime en consola
        {
            std::cout << "\nMapa con la ruta encontrada:" << std::endl;
            colorMap.print(path);
            std::cout << "Distancia: " << (path.size() - 1) << " pasos." << std::endl;
        } else //Si no se encontro un camino, se da este mensaje
        {
            std::cout << "\nNo se encontro un camino posible." << std::endl;
        }

    } while (opcion != 3);

    return 0;
}