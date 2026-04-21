#pragma once
#include <vector>
#include "Map.h"
#include <unordered_map>

//estructura para contener los resultados de la busqueda y sus estadisticas
struct SearchResult
{
    std::vector<std::pair<int,int>> path;
    int visitedCount;
    int openSize;
    double executionTime; //en milisegundos
    float totalCost;
    float weight;
    bool found;
};

class Search
{

public:
    static std::vector<std::pair<int,int>> reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start);
    static SearchResult BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal);
    static SearchResult Greedy(const Map& map, std::pair<int, int> start, std::pair<int, int> goal);
    static SearchResult AStar(const Map& map, std::pair<int, int> start, std::pair<int, int> goal);
    static SearchResult WeightedAStar(const Map& map, std::pair<int, int> start, std::pair<int, int> goal,float w);
    static float Heuristic(std::pair<int, int> start, std::pair<int, int> goal);
};