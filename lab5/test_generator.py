#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import random

# Tests file counter
TESTS_CNT = 10
# Tests in file counter
CNT_TESTS_PER_FILE = 10000

ALPHABET = 'abcd'#string.ascii_lowercase 

def get_random_text(text_len=None):
    if text_len is None:
        text_len = random.randint(1, 1000)
    return "".join( [ random.choice( ALPHABET ) for _ in range( text_len ) ] )

def get_all_occurrences( text, pattern ):
    last_found = -1
    pos = list()
    while True:
        # Найдём следующую позицию, начиная с прошлого вхождения.
        last_found = text.find( pattern, last_found + 1 )
        # Все вхождения были найдены.
        if last_found == -1:  
            break
        pos.append( str(last_found+1) )
    return pos


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
                text = get_random_text()
                tfd.write( "{}\n".format( text ) )
                for cnt in range(CNT_TESTS_PER_FILE):
                    # Определяем, использовать ли настоящую подстроку или 
                    # воображаемую.
                    use_real_pattern = random.choice( [ True, False ] )
                    if use_real_pattern and len( text ) > 3:
                        # Вычисляем позиции среза.
                        start_pos = random.randint(0, len(text) - 3)
                        end_pos= random.randint( start_pos+1, len(text) - 1)
                        pattern = text[start_pos:end_pos]
                    else:
                        # Берём рандомную подстроку. Она может совпасть с 
                        # существующей.
                        pattern = get_random_text( random.randint( 1, 100 ) )
                    # Определяем позиции, на которых встретился наш шаблон в тексте.
                    #pos = [ str(m.start()+1) for m in re.finditer(pattern, text) ]
                    pos = get_all_occurrences( text, pattern )
                    tfd.write( "{}\n".format( pattern ) )
                    # Если не нашли ни одного вхождения, то и записывать в файл с ответом не будем.
                    if len(pos):
                        afd.write( "{}: {}\n".format( cnt+1, ", ".join(pos) ) )

# Entrypoint
if __name__ == "__main__":
    main()