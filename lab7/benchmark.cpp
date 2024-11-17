#include <iostream>
#include <vector>
#include <climits>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

using namespace std;

void dfs(int i, int j, int current_penalty, vector<vector<int>>& A, int& min_penalty, vector<pair<int, int>>& path, vector<pair<int, int>>& best_path) {
    int n = A.size();
    int m = A[0].size();

    // Если достигли последней строки
    if (i == n - 1) {
        if (current_penalty < min_penalty) {
            min_penalty = current_penalty;
            best_path = path; // Сохраняем текущий путь
        }
        return;
    }

    // Переход в соседние клетки следующей строки
    for (int dj = -1; dj <= 1; ++dj) { // Соседние клетки в следующей строке
        int nj = j + dj;
        if (nj >= 0 && nj < m) { // Проверка границ
            path.push_back({i + 1, nj}); // Добавляем координаты в путь
            dfs(i + 1, nj, current_penalty + A[i + 1][nj], A, min_penalty, path, best_path);
            path.pop_back(); // Убираем последнюю координату после возврата
        }
    }
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> A(n, std::vector<int>(m));
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            std::cin >> A[i][j];
        }
    }
    int min_penalty = INT_MAX;
    vector<pair<int, int>> best_path;

    // Запускаем DFS для каждой клетки первой строки
    for (int j = 0; j < m; ++j) {
        vector<pair<int, int>> path = {{0, j}}; // Начинаем с текущей клетки
        dfs(0, j, A[0][j], A, min_penalty, path, best_path);
    }

    // Вывод результата
    cout << min_penalty << endl;
    for (const auto& p : best_path) {
        cout << "(" << p.first + 1 << "," << p.second + 1 << ") ";
    }

    std::cout << "\n";
    auto end_ts = std::chrono::system_clock::now();
    uint64_t sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "time: " << sort_ts << DURATION_PREFIX << "\n";
}
