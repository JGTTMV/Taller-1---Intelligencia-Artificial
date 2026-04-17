#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
    //verificacion basica de argumentos (archivo y coordenadas)
    if (argc != 6) 
    {
        std::cerr << "Error: Argumentos insuficientes." << std::endl;
        std::cerr << "Uso: " << argv[0] << " <map_file> <x1> <y1> <x2> <y2>" << std::endl;
        return 1;
    }

    //convierte argumentos de coordenadas a enteros
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
        std::cout << "\n--- MENU PRINCIPAL ---\n";
        std::cout << "Coordenadas actuales: Inicio(" << start_x << "," << start_y << ") Destino(" << goal_x << "," << goal_y << ")\n";
        std::cout << "1. Cambiar Coordenadas\n";
        std::cout << "2. BFS (8-conectado)\n";
        std::cout << "3. Greedy BFS\n";
        std::cout << "4. A* (A-Star)\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        std::vector<std::pair<int, int>> path;

        switch (opcion) //cada opcion ejecuta un algoritmo distinto
        {
            case 1:
                std::cout << "Nuevo Inicio (x y): "; std::cin >> start_x >> start_y;
                std::cout << "Nuevo Destino (x y): "; std::cin >> goal_x >> goal_y;
                continue;
            case 2:
                path = Search::BFS(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 3:
                path = Search::Greedy(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 4:
                path = Search::AStar(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 5:
                std::cout << "Saliendo del programa..." << std::endl;
                continue; 
            default:
                std::cout << "Opcion no valida." << std::endl;
                continue;
        }

        if (!path.empty()) 
        {
        colorMap.print(path);
        std::cout << "Ruta encontrada. Nodos en el camino: " << path.size() << std::endl;
    } else if (opcion != 5) 
    {
        std::cout << "No se encontro ruta.\n";
    }

    } while (opcion != 5);

    return 0;
}