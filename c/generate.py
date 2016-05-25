"""Generates a million entries long test file.
"""
import random, string, sys

fh = open(sys.argv[1], 'w')
fh.write('\n'.join(''.join(random.sample(string.ascii_uppercase, 3), random.sample(string.digits, 3))) for _ in range(1000000)))
fh.close()

