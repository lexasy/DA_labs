#include <bits/stdc++.h>

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

bool bfs(TGraph& graph, std::vector<int64_t>& parent)
{
    std::vector<bool> visited(graph.vertices_quantity, false);
    std::queue<int64_t> current_queue;
    current_queue.push(0); // Starting always from 0-vertice (source)
    visited[0] = true;

    while (!current_queue.empty())
    {
        int64_t current_vertice = current_queue.front();
        current_queue.pop();

        for (int64_t i = 0; i < graph.linked_vertices[current_vertice].size(); i++)
        {
            // If did not visit and not 0 capacity
            if (!visited[graph.linked_vertices[current_vertice][i]] && graph.weights[current_vertice][graph.linked_vertices[current_vertice][i]] > 0)
            {
                current_queue.push(graph.linked_vertices[current_vertice][i]);
                parent[graph.linked_vertices[current_vertice][i]] = current_vertice;
                visited[graph.linked_vertices[current_vertice][i]] = true;
            }
        }
    }
    // Did we visit target vertice
    return visited[graph.vertices_quantity - 1];
}

int64_t EdmondsKarp(TGraph& graph)
{
    int64_t max_flow = 0;
    // Parent vertices vector, for restoring path
    std::vector<int64_t> parent(graph.vertices_quantity, -1);
    while (bfs(graph, parent))
    {
        int64_t flow_path = std::numeric_limits<int64_t>::max();
        // Calculating minimum capacity in found path
        for (int64_t current_vertice = graph.vertices_quantity - 1; current_vertice != 0; current_vertice = parent[current_vertice])
        {
            int64_t prev_vertice = parent[current_vertice];
            flow_path = std::min(flow_path, graph.weights[prev_vertice][current_vertice]);
        }
        // Refresh capacities and reversed edges
        for (int64_t current_vertice = graph.vertices_quantity - 1; current_vertice != 0; current_vertice = parent[current_vertice])
        {
            int64_t prev_vertice = parent[current_vertice];
            graph.weights[prev_vertice][current_vertice] -= flow_path;
            graph.weights[current_vertice][prev_vertice] += flow_path;
        }
        max_flow += flow_path;
        // Clearing vector of parents for next iteration of bfs
        parent = std::vector<int64_t>(graph.vertices_quantity, -1);
    }
    return max_flow;
}

int main()
{
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
}