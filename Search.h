#pragma once
#include <vector>
#include "Map.h"
#include <unordered_map>



class Search
{

public:
    static std::vector<std::pair<int,int>> reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start);
    static std::vector<std::pair<int,int>> BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal);
    static std::vector<std::pair<int, int>> Greedy(const Map& map, std::pair<int, int> start, std::pair<int, int> goal);
    static std::vector<std::pair<int, int>> AStar(const Map& map, std::pair<int, int> start, std::pair<int, int> goal);
    static std::vector<std::pair<int, int>> WeightedAStar(const Map& map, std::pair<int, int> start, std::pair<int, int> goal,float w);
    static float Heuristic(std::pair<int, int> start, std::pair<int, int> goal);
};