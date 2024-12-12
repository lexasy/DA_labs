#include <bits/stdc++.h>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

class TGraph
{
public:
    int64_t vertices_quantity;
    // Weights matrix
    std::vector<std::vector<int64_t>> weights;
    // Vectors of linked vertices
    std::vector<std::vector<int64_t>> linked_vertices;
    // Constructor
    TGraph(int64_t vertices)
    {
        this->vertices_quantity = vertices;
        this->weights.resize(vertices, std::vector<int64_t>(vertices, 0)); // Initialize with 0
        this->linked_vertices.resize(vertices);
    }
    // Method for adding edges
    void AddEdge(int64_t vertice_a, int64_t vertice_b, int64_t weight)
    {
        this->weights[vertice_a - 1][vertice_b - 1] += weight;
        this->linked_vertices[vertice_a - 1].push_back(vertice_b - 1); // Adding link
        this->linked_vertices[vertice_b - 1].push_back(vertice_a - 1); // Adding reversed link for returning part of flow
    }
};

bool dfs(TGraph& graph, std::vector<int64_t>& parent, int64_t current_vertice)
{
    // Если достигли целевой вершины, возвращаем true
    if (current_vertice == graph.vertices_quantity - 1)
        return true;

    for (int64_t i = 0; i < graph.linked_vertices[current_vertice].size(); i++)
    {
        int64_t next_vertice = graph.linked_vertices[current_vertice][i];
        // Если не посещали и есть остаточная емкость
        if (parent[next_vertice] == -1 && graph.weights[current_vertice][next_vertice] > 0)
        {
            parent[next_vertice] = current_vertice; // Запоминаем путь
            // Рекурсивно вызываем dfs для следующей вершины
            if (dfs(graph, parent, next_vertice))
                return true;
        }
    }
    return false; // Если не нашли путь
}

int64_t EdmondsKarp(TGraph& graph)
{
    int64_t max_flow = 0;
    std::vector<int64_t> parent(graph.vertices_quantity, -1);
    while (true)
    {
        // Сбрасываем родительский массив перед каждой итерацией
        std::fill(parent.begin(), parent.end(), -1);
        parent[0] = -2; // Отметим источник как посещенный

        // Запускаем DFS с источника
        if (!dfs(graph, parent, 0))
            break; // Если не нашли путь, выходим из цикла

        int64_t flow_path = std::numeric_limits<int64_t>::max();
        // Вычисляем минимальную емкость в найденном пути
        for (int64_t current_vertice = graph.vertices_quantity - 1; current_vertice != 0; current_vertice = parent[current_vertice])
        {
            int64_t prev_vertice = parent[current_vertice];
            flow_path = std::min(flow_path, graph.weights[prev_vertice][current_vertice]);
        }
        // Обновляем емкости и обратные ребра
        for (int64_t current_vertice = graph.vertices_quantity - 1; current_vertice != 0; current_vertice = parent[current_vertice])
        {
            int64_t prev_vertice = parent[current_vertice];
            graph.weights[prev_vertice][current_vertice] -= flow_path;
            graph.weights[current_vertice][prev_vertice] += flow_path;
        }
        max_flow += flow_path;
    }
    return max_flow;
}


int main()
{
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    int64_t vertices_quantity, edges_quantity;
    std::cin >> vertices_quantity >> edges_quantity;
    TGraph graph(vertices_quantity);
    for (int64_t _ = 0; _ < edges_quantity; _++)
    {
        int64_t vertice_a, vertice_b, weight;
        std::cin >> vertice_a >> vertice_b >> weight;
        graph.AddEdge(vertice_a, vertice_b, weight);
    }
    int64_t max_flow = EdmondsKarp(graph);
    std::cout << max_flow << "\n";
    auto end_ts = std::chrono::system_clock::now();
    uint64_t sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "time: " << sort_ts << DURATION_PREFIX << "\n";
}