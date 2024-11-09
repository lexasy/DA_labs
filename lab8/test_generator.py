#! /usr/bin/env python3

import sys
import random
import math

# Tests file counter
TESTS_CNT = 10
# Tests in file counter
CNT_TESTS_PER_FILE = 1

def heron_area(a, b, c):
    s = (a + b + c) / 2.0  # Полупериметр
    return math.sqrt(s * (s - a) * (s - b) * (s - c))  # Площадь по формуле Герона

def find_max_area_triangle(lengths):
    # Сортируем отрезки по длине в порядке убывания
    lengths.sort(reverse=True)

    max_area = 0.0
    max_a, max_b, max_c = 0.0, 0.0, 0.0

    # Ищем три отрезка, которые могут образовать треугольник
    for i in range(len(lengths) - 2):
        a, b, c = lengths[i], lengths[i + 1], lengths[i + 2]
        if a < b + c:  # Проверка неравенства треугольника
            area = heron_area(a, b, c)
            if area > max_area:
                max_area = area
                max_a = a
                max_b = b
                max_c = c
    return max_area, max_a, max_b, max_c

def generate_triangle_sides(num_sides, min_length=1, max_length=10000):
    # Генерация случайных длин сторон
    sides = [random.randint(min_length, max_length) for _ in range(num_sides)]
    return sides

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
                num_sides = random.randint(3, 100)
                sides = generate_triangle_sides(num_sides)
                tfd.write(f"{num_sides}\n")
                for side in sides:
                    tfd.write(f'{side}\n')
                max_area, max_a, max_b, max_c = find_max_area_triangle(sides)
                best_triangle = [max_a, max_b, max_c]
                if (max_area == 0):
                    afd.write(f"0\n")
                else:
                    afd.write(f"{max_area:.3f}\n")
                    afd.write(f"{' '.join(f'{x}' for x in sorted(best_triangle))}")
                    afd.write("\n")    

# Entrypoint
if __name__ == "__main__":
    main()