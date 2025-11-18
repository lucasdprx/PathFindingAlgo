#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

struct Node
{
    int x;
    int y;
    int gWeight;
    bool isWall;
};

static bool operator==(const Node& a, const Node& b)
{
    return a.x == b.x && a.y == b.y;
}

static std::vector<Node> AStarPathfinding(const std::vector<Node>& graph, const int sizeX, const int sizeY, const Node start, const Node end)
{
    std::vector<Node> openList;
    std::vector<Node> closedList;
    bool targetIsFound = false;
    openList.push_back(start);
    
    while (!openList.empty() && !targetIsFound)
    {
        Node lessCostNode = openList[0];
        int lessCostNodeWeight = 9999999;
        for (const Node& node : openList)
        {
            const int coordWeight = node.gWeight + abs(node.x - end.x) + abs(node.y - end.y);
            if (coordWeight < lessCostNodeWeight)
            {
                lessCostNodeWeight = coordWeight;
                lessCostNode = node;
            }
        }
        openList.erase(std::remove(openList.begin(), openList.end(), lessCostNode), openList.end());
        
        std::vector<Node> neighbors;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                if (x == 0 && y == 0) 
                    continue;

                const int newX = lessCostNode.x + x;
                const int newY = lessCostNode.y + y;
                
                if (newX >= 0 && newX < sizeX && newY >= 0 && newY < sizeY) {
                    const int neighborIndex = newY * sizeX + newX;
                    if (!graph[neighborIndex].isWall && std::find(closedList.begin(), closedList.end(), graph[neighborIndex]) == closedList.end()) {
                        neighbors.push_back(graph[neighborIndex]);
                    }
                }
            }
        }
        
        for (Node& neighbor : neighbors)
        {
            if (neighbor.x == end.x && neighbor.y == end.y)
            {
                targetIsFound = true;
                break;
            }

            neighbor.gWeight = lessCostNode.gWeight + abs(lessCostNode.x - neighbor.x) + abs(lessCostNode.y - neighbor.y);
            if (std::find(openList.begin(), openList.end(), neighbor) == openList.end())
            {
                openList.push_back(neighbor);
            }
        }
        closedList.push_back(lessCostNode);
    }
    
    std::vector<Node> path;
    path.push_back(end);
    Node currentNode = end;
    
    while (currentNode.x != start.x || currentNode.y != start.y)
    {
        std::vector<Node> neighbors;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                if (x == 0 && y == 0) 
                    continue;

                const int newX = currentNode.x + x;
                const int newY = currentNode.y + y;
                
                if (newX >= 0 && newX < sizeX && newY >= 0 && newY < sizeY) {
                    const int neighborIndex = newY * sizeX + newX;
                    if (!graph[neighborIndex].isWall && std::find(closedList.begin(), closedList.end(), graph[neighborIndex]) != closedList.end()) 
                    {
                        neighbors.push_back(graph[neighborIndex]);
                    }
                }
            }
        }
        
        if (neighbors.empty())
            break;
            
        Node bestNeighbor = neighbors[0];
        int bestNeighborWeight = 9999999;
        for (const Node& neighbor : neighbors)
        {
            if (neighbor.gWeight < bestNeighborWeight)
            {
                bestNeighborWeight = neighbor.gWeight;
                bestNeighbor = neighbor;
            }
        }
        path.push_back(bestNeighbor);
        currentNode = bestNeighbor;
    }
    
    
    return path;
}

static void DrawGraph(const std::vector<Node>& graph, const std::vector<Node>& path, const int sizeX, const int sizeY, const Node start, const Node end)
{
    std::cout << "Grille (" << sizeX << " x " << sizeY << ") :\n";
    for (int x = 0; x < sizeX; ++x)
    {
        for (int y = 0; y < sizeY; ++y)
        {
            if (graph[y * sizeX + x].x == start.x && graph[y * sizeX + x].y == start.y)
            {
                std::cout << "\033[32m" << "S" << "\033[0m";
            }
            else if (graph[y * sizeX + x].x == end.x && graph[y * sizeX + x].y == end.y)
            {
                std::cout << "\033[34m" << "E" << "\033[0m";
            }
            else if (graph[y * sizeX + x].isWall)
            {
                std::cout << "\033[31m" << "X" << "\033[0m";
            }
            else if (std::find(path.begin(), path.end(), graph[y * sizeX + x]) != path.end())
            {
                std::cout << "\033[33m" << "1" << "\033[0m";
            }
            else
            {
                std::cout << "0";
            }
        }
        std::cout << '\n';
    }
}


int main(int argc, char* argv[])
{
    constexpr int sizeX = 100;
    constexpr int sizeY = 100;
    constexpr Node start{0, 0, 0, false};
    constexpr Node end{sizeX - 1, sizeY / 2 - 1, 0 , false};
    std::vector<Node> graph;
    graph.reserve(sizeX * sizeY);
    for (int y = 0; y < sizeY; ++y)
    {
        for (int x = 0; x < sizeX; ++x)
        {
            graph.push_back(Node{ x, y, 0 });
        }
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, graph.size() - 1);
    for (int i = 0; i < 1000; ++i)
    {
        const int rand = dis(gen);
        if (graph[rand].isWall)
        {
            i--;
            continue;
        }
        graph[rand].isWall = true;
    }
    
    const std::vector<Node> path = AStarPathfinding(graph, sizeX, sizeY, start, end);

    DrawGraph(graph, path, sizeX, sizeY, start, end);
    return 0;
}
