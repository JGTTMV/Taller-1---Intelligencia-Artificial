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

//reconstruct
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

//1. Implementacion de BFS (Breadth-First Search)
std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal)
{
    std::cout<<"===========================\nRunning BFS...\n";
	auto startTime = std::chrono::high_resolution_clock::now();

    //guarda direcciones de movimiento: arriba, derecha, abajo, izquierda
    std::vector<std::pair<int,int>> dirs = 
    {
    {0,1}, {0,-1}, {1,0}, {-1,0},
    {1,1}, {1,-1}, {-1,1}, {-1,-1}
    };

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;    //guarda de donde viene cada nodo para poder reconstruir el camino al final

    int visitedCount = 0;

    //inserta el nodo inicial en la cola y marcalo como visitado
    OPEN.push(start);
    visited[start.first][start.second]=true;

    while(!OPEN.empty())
    {
        //saca el nodo al frente de la cola
        auto pos=OPEN.front();
        OPEN.pop();
        visitedCount++;

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

//Implementacion de la Heuristica 
float Search::Heuristic(std::pair<int, int> start, std::pair<int, int> goal) 
{
    float dx = std::abs(start.first - goal.first);
    float dy = std::abs(start.second - goal.second);
    return (dx + dy) + (1.4142f - 2) * std::min(dx, dy);
}

//estructura auxiliar para la Priority Queue (Cola de prioridad)
//esto nos permite guardar la posicion y su valor heuristico juntos.
struct NodeRecord 
{
    std::pair<int, int> pos;
    float h;
    
    //se sobrecarga el operador '>' para que la Priority Queue actue como un Min-Heap
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

    std::vector<std::pair<int,int>> dirs = 
    {
    {0,1}, {0,-1}, {1,0}, {-1,0},
    {1,1}, {1,-1}, {-1,1}, {-1,-1}
    };

    int visitedCount = 0;
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

        visitedCount++;

        //si se llega a la meta
        if(pos == goal)
        {
            auto endTime = std::chrono::high_resolution_clock::now();

            std::cout << "VISITED: " << visitedCount << std::endl;
            std::cout << "OPEN: " << OPEN.size() << std::endl;
            std::cout << "FOUND in " << std::chrono::duration<double, std::milli>(endTime-startTime).count() << "ms\n";
            
            return reconstruct(pathCache, pos);
        }

        //expande a los vecinos
        for(auto dir : dirs){
            std::pair<int,int> child = pos;
            child.first += dir.first;
            child.second += dir.second;
            
            //si las coordenadas son invalidas, si es un muro (1) o si ya fue visitado, se ignora
            if(!map.isValidCordinates(child.first, child.second) || 
               map._map[child.first][child.second] == 1 || 
               visited[child.first][child.second]){
                continue;
            }
            
            //se marca como visitado, guardamos de donde viene y lo metemos a la cola
            visited[child.first][child.second] = true;
            pathCache[child] = pos;
            OPEN.push({child, Heuristic(child, goal)});
        }
    }
    std::cout << "NOT FOUND!!!!\n";
    return {};
}

// 4. Implementación del Algoritmo A*
std::vector<std::pair<int, int>> Search::AStar(const Map& map, std::pair<int, int> start, std::pair<int, int> goal) {
    
    //inicia medicion de tiempo
    auto startTime = std::chrono::high_resolution_clock::now();

    struct AStarNode //estructura auxiliar para la Priority Queue
    {
        std::pair<int, int> pos;
        float g, f;
        bool operator>(const AStarNode& other) const { return f > other.f; }
    };

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> OPEN;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>> pathCache;
    std::unordered_map<std::pair<int, int>, float> g_costs;
    std::unordered_map<std::pair<int, int>, bool> closed; 

    OPEN.push({start, 0.0f, Heuristic(start, goal)});
    g_costs[start] = 0.0f;

    std::vector<std::pair<int,int>> dirs = {
        {0,1},{0,-1},{1,0},{-1,0},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };
    
    int visitedCount = 0;

    OPEN.push({start, 0.0f, Heuristic(start, goal)});
    g_costs[start] = 0.0f;

    while (!OPEN.empty()) 
    {
        AStarNode current = OPEN.top();
        OPEN.pop();

        if(closed[current.pos]) continue;   
        closed[current.pos] = true;

        visitedCount++;

        if (current.pos == goal) 
        {
            auto endTime = std::chrono::high_resolution_clock::now();

            std::cout << "Costo total: " << g_costs[goal] << std::endl;
            std::cout << "VISITED: " << visitedCount << std::endl;
            std::cout << "FOUND in: " << std::chrono::duration<double, std::milli>(endTime-startTime).count() << " ms\n";

            return reconstruct(pathCache, goal);
        }

        //expande a los vecinos
        for (auto dir : dirs) 
        {
            std::pair<int, int> neighbor = {current.pos.first + dir.first, current.pos.second + dir.second};

            if (!map.isValidCordinates(neighbor.first, neighbor.second) || map._map[neighbor.first][neighbor.second] == 1)
                continue;

            //costo diferenciado: 1.0 recto, 1.41 diagonal
            float moveCost = (dir.first != 0 && dir.second != 0) ? 1.4142f : 1.0f;
            float tentative_g = g_costs[current.pos] + moveCost;

            //si el vecino no ha sido visitado o se encuentra un camino mas barato hacia el vecino
            //se actualiza su costo y se agrega a la cola
            if (g_costs.find(neighbor) == g_costs.end() || tentative_g < g_costs[neighbor]) 
            {
                g_costs[neighbor] = tentative_g;
                float f = tentative_g + Heuristic(neighbor, goal);
                pathCache[neighbor] = current.pos;
                OPEN.push({neighbor, tentative_g, f});
            }
        }
    }
    
    std::cout << "Ruta no encontrada." << std::endl;
    return {};
}

// 5. Implementación del Algoritmo Weighted A*
std::vector<std::pair<int, int>> Search::WeightedAStar(const Map& map, std::pair<int, int> start, std::pair<int, int> goal,float w)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    struct Node
    {
        std::pair<int, int> pos;
        float g, f;
        bool operator>(const Node& other) const { return f > other.f; }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> OPEN;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>> pathCache;
    std::unordered_map<std::pair<int, int>, float> g_costs;
    std::unordered_map<std::pair<int, int>, bool> closed;

    std::vector<std::pair<int,int>> dirs = {
        {0,1},{0,-1},{1,0},{-1,0},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    int visitedCount = 0;

    //inserta el nodo inicial en la cola con su valor f = g + w*h, donde g=0 para el nodo inicial
    OPEN.push({start, 0.0f, w * Heuristic(start, goal)});
    g_costs[start] = 0.0f;

    while (!OPEN.empty()) 
    {
        Node current = OPEN.top();
        OPEN.pop();

        if(closed[current.pos]) continue;
        closed[current.pos] = true;

        visitedCount++;

        if (current.pos == goal)
        {
            auto endTime = std::chrono::high_resolution_clock::now();

            //A diferencia del A*, aqui imprimimos el peso usado para la heuristica
            std::cout << "[WA*] Peso: " << w << std::endl;
            std::cout << "Costo total: " << g_costs[goal] << std::endl;
            std::cout << "VISITED: " << visitedCount << std::endl;
            std::cout << "FOUND in: " << std::chrono::duration<double, std::milli>(endTime-startTime).count() << " ms\n";

            return reconstruct(pathCache, goal);
        }

        for (auto dir : dirs) 
        {
            std::pair<int, int> neighbor = {
                current.pos.first + dir.first, 
                current.pos.second + dir.second
            };

            if (!map.isValidCordinates(neighbor.first, neighbor.second) || 
                map._map[neighbor.first][neighbor.second] == 1)
                continue;

            float moveCost = (dir.first != 0 && dir.second != 0) ? 1.4142f : 1.0f;
            float tentative_g = g_costs[current.pos] + moveCost;

            if (g_costs.find(neighbor) == g_costs.end() || tentative_g < g_costs[neighbor]) 
            {
                g_costs[neighbor] = tentative_g;

                float f = tentative_g + w * Heuristic(neighbor, goal);

                pathCache[neighbor] = current.pos;
                OPEN.push({neighbor, tentative_g, f});
            }
        }
    }

    std::cout << "Ruta no encontrada." << std::endl;
    return {};
}