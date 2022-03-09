from random import choice

n = int(input())
for i in range(n):
    print(''.join([choice(['+', '-']) for _ in range(i)]) + '#')

