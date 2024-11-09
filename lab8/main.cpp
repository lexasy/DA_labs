#include <bits/stdc++.h>

// triangle class
class TTriangle
{
public:
    int a, b, c;
    // sides constructor
    TTriangle(int a, int b, int c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    // triangle are with heron formula
    double HeronArea()
    {
        double perimetr = (1.0 * (this->a + this->b + this->c)) / 2;
        return sqrt(perimetr * (perimetr - this->a) * (perimetr - this->b) * (perimetr - this->c));
    }
};

int main()
{
    int n;
    std::cin >> n;
    std::vector<int> side_sizes;
    for (int _ = 0; _ < n; ++_)
    {
        int input;
        std::cin >> input;
        side_sizes.push_back(input);
    }
    // reverse sorting
    std::sort(side_sizes.begin(), side_sizes.end(), std::greater<>());
    double max_area = 0;
    int max_a = 0, max_b = 0, max_c = 0;
    for (size_t i = 0; i < n - 2; ++i)
    {
        // rule of triangle existance
        if (side_sizes[i] <  side_sizes[i + 1] + side_sizes[i + 2])
        {
            TTriangle triangle(side_sizes[i], side_sizes[i + 1], side_sizes[i + 2]);
            if (triangle.HeronArea() > max_area)
            {
                max_area = triangle.HeronArea();
                max_a = triangle.a;
                max_b = triangle.b;
                max_c = triangle.c;
            }
        }
    }
    if (max_area > 0)
    {
        std::cout << std::fixed << std::setprecision(3) << max_area << "\n"; 
        std::cout << std::defaultfloat << max_c << " " << max_b << " " << max_a << "\n";
    }
    else
    {
        std::cout << 0 << "\n";
    }
} 