#! /usr/bin/env python3

import sys
import random
import string

class TPhoneNumber:
    def __init__(self, country, region, number, phonenumber) -> None:
        self.country = country
        self.region = region
        self.number = number
        self.phonenumber = phonenumber

# Tests file counter
TESTS_CNT = 10
# Tests in file counter
CNT_TESTS_PER_FILE = 1000

def generate_kv() -> tuple[TPhoneNumber, str]:
    key = "+"
    country = str(random.randint(1, 999))
    key += country
    key += "-"
    region = str(random.randint(1, 999))
    key += region
    key += "-"
    number = str(random.randint(10000, 999999))
    key += number
    phone = TPhoneNumber(country=int(country), region=int(region), number=int(number), phonenumber=key)
    value = ""
    length = random.randint(1, 64)
    for _ in range(length):
        value += random.choice(string.ascii_letters + "0123456789")
    for _ in range(64 - length):
        value += "\0"
    return phone, value

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
            # Generating random key-value and writing to test file
            for _ in range(CNT_TESTS_PER_FILE):
                key, value = generate_kv()
                arr.append((key, value))
                tfd.write(f"{key.phonenumber}\t{value}\n")
            # Sorting of random pairs: key-value
            arr.sort(key=lambda x: x[0].number)
            arr.sort(key=lambda x: x[0].region)
            arr.sort(key=lambda x: x[0].country)
            # Writing sorted elements in answer file
            for elem in arr:
                afd.write(f"{elem[0].phonenumber}\t{elem[1]}\n")
    

# Entrypoint
if __name__ == "__main__":
    main()
    