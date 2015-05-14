#!/usr/bin/env python

import sys

from random import randint

MAX_INT = 100
MIN_INT = 1

def generate_headers(num_of_columns):
    print ",".join(map(lambda x: "column{}".format(x + 1), xrange(0, num_of_columns)))

def generate_rows(num_of_rows, num_of_columns):
    for i in xrange(0, num_of_rows):
        row_data = ",".join(map(lambda _: str(randint(1, 100 + 1)), xrange(0, num_of_columns)))
        print row_data

def err(reason):
    sys.stderr.write(reason)
    sys.stderr.write("Usage: ./csv_generator num_of_columns num_of_rows")
    exit(1)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        err("Invalid number of arguments")

    try:
        num_of_columns = int(sys.argv[1])
        num_of_rows = int(sys.argv[2])
    except:
        err("Invalid argument type")

    generate_headers(num_of_columns)
    generate_rows(num_of_rows, num_of_columns)
