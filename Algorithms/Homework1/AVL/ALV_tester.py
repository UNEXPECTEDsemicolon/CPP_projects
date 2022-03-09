# import numpy as np
from random import randint, choice
import subprocess, os.path
from time import time

while True:
    m = 5
    g = 10
    acts = [choice(["?", "+"]) for _ in range(m)]
    xs = [randint(0, g) for _ in range(m)]
    inp = f'{m}\n' + '\n'.join([f'{acts[i]} {xs[i]}' for i in range(m)])
    AVL = set()
    true_res = ''
    old = 0
    for i in range(m):
        if acts[i] == '?':
            for j in sorted(list(AVL)):
                if j >= xs[i]:
                    old = j
                    break
            else:
                old = -1
            true_res += f'{old}\n'
        elif i > 0 and acts[i-1] == '?':
            AVL.add(xs[i] + old)
            old = xs[i]
        else:
            AVL.add(xs[i])
            old = xs[i]

    inp_b = inp.encode('utf-8')
    # print(inp)
    # print(true_res)
    fine_res = subprocess.run([os.getcwd() + r'/avl.exe'],
                              input=inp_b, capture_output=True).stdout.decode("utf-8")
    if fine_res.strip(" \n") != true_res.strip(" \n"):
        print(inp)
        print(f'true_res = \n{true_res}')
        print(f'your_res = \n{fine_res}')
        if not input():
            break
