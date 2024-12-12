#! /usr/bin/env python3

import sys
import random
import networkx as nx

# Tests file counter
TESTS_CNT = 10
# Tests in file counter
CNT_TESTS_PER_FILE = 1

def generate_random_graph(num_vertices, num_edges, max_capacity):
    edges = set()

    # Убедимся, что источник и сток будут в графе
    source = 1
    sink = num_vertices
    
    # Добавляем ребро от источника к случайной вершине
    if num_vertices > 1:
        edges.add((source, random.randint(2, num_vertices - 1)))
    
    # Добавляем ребро от случайной вершины к стоку
    if num_vertices > 1:
        edges.add((random.randint(2, num_vertices - 1), sink))
    
    # Проверка на возможность добавления ребер
    max_possible_edges = num_vertices * (num_vertices - 1)  # Максимальное количество уникальных направленных ребер
    num_edges = min(num_edges, max_possible_edges)

    while len(edges) < num_edges:
        u = random.randint(1, num_vertices)  # Включаем источник и сток
        v = random.randint(1, num_vertices)
        
        # Избегаем петель
        if u != v:
            edges.add((u, v))  # Добавляем только вершины

    # Генерируем веса для уникальных ребер
    weighted_edges = [(u, v, random.randint(1, max_capacity)) for u, v in edges]

    return weighted_edges

def generate_test_case(num_vertices, num_edges, max_capacity):
    """
    Генерирует тестовый случай для алгоритма Эдмонса-Карпа.
    
    :param num_vertices: Количество вершин в графе.
    :param num_edges: Количество ребер в графе.
    :param max_capacity: Максимальная пропускная способность ребер.
    :return: Строка, представляющая тестовый случай и ожидаемый ответ.
    """
    edges = generate_random_graph(num_vertices, num_edges, max_capacity)
    
    # Создаем граф с помощью networkx
    G = nx.DiGraph()
    for u, v, capacity in edges:
        G.add_edge(u, v, capacity=capacity)
    
    # Вычисляем максимальный поток
    source = 1  # Предположим, что источник - это вершина 1
    sink = num_vertices  # Сток - последняя вершина
    flow_value, flow_dict = nx.maximum_flow(G, _s=source, _t=sink)
    
    # Форматируем тестовый случай
    test_case = f"{num_vertices} {num_edges}\n"
    for u, v, capacity in edges:
        test_case += f"{u} {v} {capacity}\n"
    
    return test_case, flow_value

def main():
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
                num_vertices = random.randint(1, 2000)
                num_edges = random.randint(1, 10000)  # Количество ребер
                max_capacity = 10 ** 9
                test_case, expected_flow = generate_test_case(num_vertices, num_edges, max_capacity)
                tfd.write(test_case)
                afd.write(f"{expected_flow}\n")

if __name__ == "__main__":
    main()
