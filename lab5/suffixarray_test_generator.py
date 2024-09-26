#! /usr/bin/env python3

import sys
import random

# Tests file counter
TESTS_CNT = 1
# Tests in file counter
CNT_TESTS_PER_FILE = 10

ALPHABET = 'abcd' #string.ascii_lowercase 

def get_random_text(text_len=None):
    if text_len is None:
        text_len = random.randint(1, 1000)
    return "".join([random.choice(ALPHABET) for _ in range(text_len)])

def get_suffix_array(text):
    # Создаем список суффиксов
    suffixes = [(text[i:], i) for i in range(len(text))]
    # Сортируем суффиксы в лексикографическом порядке
    suffixes.sort()
    # Извлекаем индексы отсортированных суффиксов
    suffix_array = [suffix[1] for suffix in suffixes]
    return suffix_array


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
            arr = []
            for _ in range(CNT_TESTS_PER_FILE):
                text = get_random_text()
                suffix_array = ' '.join(map(str, get_suffix_array(text)))
                tfd.write(f"{text}\n")
                afd.write(f"{suffix_array} \n")    

# Entrypoint
if __name__ == "__main__":
    main()