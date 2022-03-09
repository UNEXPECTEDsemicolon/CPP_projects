from random import randint, choice
import subprocess, sys, os, os.path
from time import time
from itertools import permutations

while True:
    n = 10
    coords = [(randint(0, n), randint(0, n)) for i in range(n)]
    if len(set(coords)) < len(coords):
        continue
    w = [randint(0, 100) for i in range(n)]
    table = [[0 for i in range(n + 1)] for j in range(n + 1)]
    for i, (x, y) in enumerate(coords): table[x][y] = w[i]
    a = [f'{coords[i][0]} {coords[i][1]} {w[i]}' for i in range(n)]
    m = 100
    inp = f'{n}\n' + '\n'.join(a) + f'\n{m}\n'
    res = ''
    for i in range(m):
        act = randint(0, 1)
        if act:
            rx, ry = randint(0, n), randint(0, n)
            # rx, ry = choice(coords)
            cur_sum = sum([sum(table[x][:ry+1]) for x in range(rx+1)])
            res += str(cur_sum) + '\n'
            inp += f'get {rx} {ry}\n';
        else:
            ri, rw = randint(0, n-1), randint(0, 100)
            table[coords[ri][0]][coords[ri][1]] = rw
            inp += f'change {ri + 1} {rw}\n'
    inp_b = inp.encode('utf-8')
    fine_res = subprocess.run([os.getcwd() + r'/a.out', '>> dev\null'], input=inp_b, capture_output=True).stdout.decode("utf-8")
    good = fine_res == res
    if not good:
        print(inp)
        print(f'./a.out: <<{fine_res[:20]}>>')
        print(f'true res: <<{res}>>')
        if not input():
            break
