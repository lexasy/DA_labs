#include <bits/stdc++.h>

size_t find_min_idx(std::vector<int64_t> vec)
{
    int64_t min = 0;
    for (size_t i = 1; i < vec.size(); ++i)
    {
        if (vec[i] < vec[min])
        {
            min = i;
        }
    }
    return min;
}

int main()
{
    int64_t rows, columns;
    std::cin >> rows >> columns;
    std::vector<std::vector<int64_t>> matrix(rows, std::vector<int64_t>(columns));
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            std::cin >> matrix[i][j];
        }
    }

    // min penalty calculating
    std::vector<std::vector<int64_t>> matrix_penalties(rows, std::vector<int64_t>(columns, std::numeric_limits<int64_t>::max()));
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            if (i == 0)
            {
                // initial penalties
                matrix_penalties[i][j] = matrix[i][j];
            }
            else
            {
                if (j != 0)
                {
                    // up and left
                    matrix_penalties[i][j] = std::min(matrix_penalties[i][j], matrix[i][j] + matrix_penalties[i - 1][j - 1]);
                }
                // up
                matrix_penalties[i][j] = std::min(matrix_penalties[i][j], matrix[i][j] + matrix_penalties[i - 1][j]);
                if (j != columns - 1)
                {
                    // up and right
                    matrix_penalties[i][j] = std::min(matrix_penalties[i][j], matrix[i][j] + matrix_penalties[i - 1][j + 1]);
                }
            }
        }
    }
    size_t min_idx = find_min_idx(matrix_penalties[rows - 1]);
    std::cout << matrix_penalties[rows - 1][min_idx] << "\n";

    // path calculating
    std::vector<int64_t> path(rows);
    path[rows - 1] = min_idx;
    for (int i = rows - 1; i > 0; --i)
    {
        int64_t min_penalty_sum = std::numeric_limits<int64_t>::max();
        // up and left
        if (min_idx != 0)
        {
            if (min_penalty_sum > matrix_penalties[i - 1][min_idx - 1])
            {
                min_penalty_sum = matrix_penalties[i - 1][min_idx - 1];
                path[i - 1] = min_idx - 1;
            }
        }
        // up
        if (min_penalty_sum > matrix_penalties[i - 1][min_idx])
        {
            min_penalty_sum = matrix_penalties[i - 1][min_idx];
            path[i - 1] = min_idx;
        }
        // up and right   
        if (min_idx != columns - 1)
        {
            if (min_penalty_sum > matrix_penalties[i - 1][min_idx + 1])
            {
                min_penalty_sum = matrix_penalties[i - 1][min_idx + 1];
                path[i - 1] = min_idx + 1;
            }
        }
        min_idx = path[i - 1];             
    }

    for (size_t i = 0; i < path.size(); ++i)
    {
        std::cout << "(" << i + 1 << "," << path[i] + 1 << ") ";
    }
    std::cout << "\n";
}