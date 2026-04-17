#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cmath>


namespace std
{
    //esto se necesita para almacenar un pair<int,int> en un contenedor asociativo
    //como unordered_map, que se usa para reconstruir el camino encontrado
	template<> struct hash<std::pair<int,int>>
	{
		std::size_t operator()( const std::pair<int,int> & p) const noexcept
		{
            hash<int> hasher;
			return hasher(p.first) ^ (hasher(p.second)<<1);

		}
	};
}

std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start){
	std::deque<std::pair<int,int>> nodes;
	auto node = start;  //crea una copia

    //recorre el pathCache desde el nodo final hasta el nodo inicial, agregando cada nodo al frente de la lista
    while(pathCache.find(node)!=pathCache.end())
    {
        nodes.push_front(node);
        node=pathCache.at(node);
    }   
    
    nodes.push_front(node); //agrega el nodo inicial al frente de la lista

    //convierte la deque a vector para regresar el resultado
    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes)
    {
        vec.push_back(p);
    }
    return vec;
}

std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal)
{
    std::cout<<"===========================\nRunning BFS...\n";
	auto startTime = std::chrono::high_resolution_clock::now();

    //guarda direcciones de movimiento: arriba, derecha, abajo, izquierda
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;    //guarda de donde viene cada nodo para poder reconstruir el camino al final

    //inserta el nodo inicial en la cola y márcalo como visitado
    OPEN.push(start);
    visited[start.first][start.second]=true;

    while(!OPEN.empty())
    {
        //saca el nodo al frente de la cola
        auto pos=OPEN.front();
        OPEN.pop();

        //si es el nodo objetivo, reconstruye el camino y regresa el resultado
		if(pos==goal)
        {
			auto endTime = std::chrono::high_resolution_clock::now();
			int count=0;
            for(int i=0;i<map.h;i++)
            {
                for(int j=0;j<map.w;j++)
                {
                    if(visited[i][j])count++;
                }
            }
            std::cout<<"VISITED: "<<count<<std::endl;
			std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
			
            //se arregla el calculo de milisegundos
            std::cout<<"FOUND in "<<std::chrono::duration<double, std::milli>(endTime-startTime).count()<<"ms\n";
			
            return reconstruct(pathCache,pos);
		}

		for(auto dir:dirs){
			//copia la posicion
            std::pair<int,int> child=pos;

            //mueve la copia a la nueva posicion
            child.first+=dir.first;
            child.second+=dir.second;
            
            //si las coordenadas son invalidas, si es un muro (1), o si ya fue visitado, lo ignoramos
            if(!map.isValidCordinates(child.first, child.second) || visited[child.first][child.second])
            {
                continue;
            }
            
            if(map._map[child.first][child.second] == 1)
            {
                continue;
            }
            
            //añade el nodo a la cola y lo marca como visitado
            OPEN.push(child);
            visited[child.first][child.second]=true;

            //se registra el camino
            pathCache[child]=pos;
		}
	}
	std::cout<<"NOT FOUND!!!!\n";
    
    //se regresa un vector con el nodo inicial y el nodo objetivo para que el programa no falle al intentar imprimir el camino encontrado
    //aunque no sea un camino válido
    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}

// 1. Implementacion de la Heuristica (Distancia Manhattan)
float Search::Heuristic(std::pair<int, int> start, std::pair<int, int> goal) 
{
    //Distancia Manhattan = |x1 - x2| + |y1 - y2|
    return std::abs(start.first - goal.first) + std::abs(start.second - goal.second);
}

// 2. Estructura auxiliar para la Priority Queue (Cola de prioridad)
//esto nos permite guardar la posicion y su valor heuristico juntos.
struct NodeRecord 
{
    std::pair<int, int> pos;
    float h;
    
    //sobrecargamos el operador '>' para que la Priority Queue actue como un Min-Heap
    //(es decir, que el nodo con la MENOR heuristica este siempre en la cima)
    bool operator>(const NodeRecord& other) const 
    {
        return h > other.h;
    }
};

// 3. Implementación del Algoritmo Greedy
std::vector<std::pair<int, int>> Search::Greedy(const Map& map, std::pair<int, int> start, std::pair<int, int> goal) 
{
    //declaramos nuestra Priority Queue usando la estructura que creamos
    std::priority_queue<NodeRecord, std::vector<NodeRecord>, std::greater<NodeRecord>> OPEN;
    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;
    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));

    std::vector<std::pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    auto startTime = std::chrono::high_resolution_clock::now();

    //inserta el nodo inicial en la cola y lo marca como visitado
    OPEN.push({start, Heuristic(start, goal)});
    visited[start.first][start.second] = true;

    while(!OPEN.empty())
    {
        //extrae el nodo con menor heurística
        auto currentRecord = OPEN.top();
        OPEN.pop();
        std::pair<int,int> pos = currentRecord.pos;

        //si llegamos a la meta
        if(pos == goal)
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            int count = 0;
            for(int i = 0; i < map.h; i++)
            {
                for(int j = 0; j < map.w; j++)
                {
                    if(visited[i][j]) count++;
                }
            }
            std::cout << "VISITED: " << count << std::endl;
            std::cout << "OPEN: " << OPEN.size() << std::endl;
            std::cout << "FOUND in " << std::chrono::duration<double, std::milli>(endTime-startTime).count() << "ms\n";
            std::cout << "Dist: " << reconstruct(pathCache, pos).size() - 1 << std::endl; //imprime la distancia
            
            return reconstruct(pathCache, pos);
        }

        //expande a los vecinos
        for(auto dir : dirs){
            std::pair<int,int> child = pos;
            child.first += dir.first;
            child.second += dir.second;
            
            //si las coordenadas son invalidas, si es un muro (1), o si ya fue visitado, lo ignoramos
            if(!map.isValidCordinates(child.first, child.second) || 
               map._map[child.first][child.second] == 1 || 
               visited[child.first][child.second]){
                continue;
            }
            
            //lo marcamos como visitado, guardamos de donde viene y lo metemos a la cola
            visited[child.first][child.second] = true;
            pathCache[child] = pos;
            OPEN.push({child, Heuristic(child, goal)});
        }
    }
    std::cout << "NOT FOUND!!!!\n";

    // Dentro del bucle while(!OPEN.empty())
    std::cout << "Visitando: " << pos.first << ", " << pos.second << " | Heurística: " << currentRecord.h << std::endl;

    return {};
}