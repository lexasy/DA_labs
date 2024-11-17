#include <bits/stdc++.h>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

using namespace std;

// Функция для вычисления площади треугольника по формуле Герона
double heron_area(double a, double b, double c) {
    double s = (a + b + c) / 2; // Полупериметр
    return sqrt(s * (s - a) * (s - b) * (s - c)); // Площадь
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    int N;
    cin >> N;

    vector<int> lengths(N);
    for (int i = 0; i < N; ++i) {
        cin >> lengths[i];
    }

    double max_area = 0;
    vector<int> best_sides;

    // Перебор всех комбинаций трех отрезков
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            for (int k = j + 1; k < N; ++k) {
                int a = lengths[i];
                int b = lengths[j];
                int c = lengths[k];

                // Проверка условия существования треугольника
                if (a + b > c && a + c > b && b + c > a) {
                    double area = heron_area(a, b, c);
                    if (area > max_area) {
                        max_area = area;
                        best_sides = {a, b, c};
                    }
                }
            }
        }
    }

    // Вывод результата
    if (best_sides.empty()) {
        cout << 0 << endl; // Если треугольник не может быть образован
    } else {
        sort(best_sides.begin(), best_sides.end()); // Сортировка сторон
        cout << max_area << endl;
        for (int side : best_sides) {
            cout << side << " "; // Длины сторон
        }
        std::cout << endl;
    }
    auto end_ts = std::chrono::system_clock::now();
    uint64_t sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "time: " << sort_ts << DURATION_PREFIX << "\n";
}
