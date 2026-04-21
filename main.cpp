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
        std::cout << "5. Weighted A*\n";
        std::cout << "6. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        SearchResult result;

        switch (opcion) //cada opcion ejecuta un algoritmo distinto
        {
            case 1:
                std::cout << "Nuevo Inicio (x y): "; std::cin >> start_x >> start_y;
                std::cout << "Nuevo Destino (x y): "; std::cin >> goal_x >> goal_y;
                continue;
            case 2:
                result = Search::BFS(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 3:
                result = Search::Greedy(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 4:
                result = Search::AStar(map, {start_x, start_y}, {goal_x, goal_y});
                break;
            case 5:
            {
                //toma el peso de Weighted A*
                float w;
                std::cout << "Ingrese peso (ej: 1.5): ";
                std::cin >> w;
                result = Search::WeightedAStar(map, {start_x, start_y}, {goal_x, goal_y}, w);
                break;
            }
            case 6:
                std::cout << "Saliendo...\n";
                continue;
            default:
                std::cout << "Opcion no valida." << std::endl;
                continue;
        }

        if (result.found) 
        {
            //imprime el mapa primero
            colorMap.print(result.path);
            
            //luego imprime las estadisticas de ejecución
            std::cout << "\n=====================================\n";
            std::cout << "Nodos en el camino: " << result.path.size() << std::endl;
            std::cout << "Nodos visitados: " << result.visitedCount << std::endl;
            std::cout << "Nodos OPEN: " << result.openSize << std::endl;
            std::cout << "Tiempo de ejecucion: " << result.executionTime << " ms" << std::endl;
            
            //imprime costo total si aplica (para A* y Weighted A*)
            if (result.totalCost > 0.0f)
            {
                std::cout << "Costo total del camino: " << result.totalCost << std::endl;
            }
            
            //imprime el peso si es Weighted A*
            if (result.weight > 0.0f)
            {
                std::cout << "Peso heurístico (w): " << result.weight << std::endl;
            }
            std::cout << "=====================================\n";
        } 
        else if (opcion != 6) 
        {
            std::cout << "No se encontro ruta.\n";
        }

    } while (opcion != 6);

    return 0;
}