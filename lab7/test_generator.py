#! /usr/bin/env python3

import sys
import random

# Tests file counter
TESTS_CNT = 10
# Tests in file counter
CNT_TESTS_PER_FILE = 1

def generate_matrix():
    n = random.randint(2, 1000)
    m = random.randint(2, 1000)

    if n == 0 or m == 0:
        return [], n, m

    matrix = [[random.randint(-1000, 1000) for _ in range(m)] for _ in range(n)]
    
    return matrix, n, m


def min_penalty_path_with_trace(A):
    n = len(A)      
    m = len(A[0])   

    dp = [[float('inf')] * m for _ in range(n)]
    prev = [[-1] * m for _ in range(n)]
    
    for j in range(m):
        dp[0][j] = A[0][j]

    for i in range(1, n):
        for j in range(m):
            for k in range(max(0, j-1), min(m, j+2)):
                if dp[i-1][k] + A[i][j] < dp[i][j]:
                    dp[i][j] = dp[i-1][k] + A[i][j]
                    prev[i][j] = k

    min_penalty = min(dp[n-1])
    min_index = dp[n-1].index(min_penalty)

    path = []
    current_row = n - 1
    current_col = min_index

    while current_row >= 0:
        path.append((current_row + 1, current_col + 1))
        current_col = prev[current_row][current_col]
        current_row -= 1

    path.reverse()

    return min_penalty, path

def main() -> None:
    # Checking of arguments quantity
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <test path>")
        sys.exit(1)
    # Taking name of tests directory from argument
    tests_dir = sys.argv[1]
    for num in range(1, TESTS_CNT + 1):
        # Generating filename for test file
        filename = f"{tests_dir}/{num:02}"
        # Opening test and answer files
        with open(f"{filename}.t", 'w', encoding="utf-8") as tfd,\
             open(f"{filename}.a", 'w', encoding="utf-8") as afd:
            for _ in range(CNT_TESTS_PER_FILE):
                matrix, n, m = generate_matrix()
                result, path = min_penalty_path_with_trace(matrix)
                str_matrix = ''
                for row in matrix:
                    str_matrix += (' '.join(map(str, row)) + '\n')
                tfd.write(f"{n} {m}\n{str_matrix}")
                formatted_path = ''.join(f"({row},{col}) " for row, col in path)
                afd.write(f"{result}\n{formatted_path}\n")    

# Entrypoint
if __name__ == "__main__":
    main()