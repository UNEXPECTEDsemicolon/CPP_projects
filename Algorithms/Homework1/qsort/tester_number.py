from random import randint
import subprocess, sys, os, os.path
from time import time
from itertools import permutations

while(True):
    a = [''.join([str(randint(0, 9)) for j in range(randint(1, 5))]) for i in range(5)]
    inp = ('\n'.join(a))
    # print(inp)
    inp_b = inp.encode('utf-8')
    res = subprocess.run([os.getcwd() + r'/a.out', '>> dev\null'], input=inp_b, capture_output=True).stdout.decode("utf-8")
    maxnum = -1
    for perm in permutations(a):
        num = int(''.join(perm))
        maxnum = max(maxnum, num)
    # print(f"maxnum={maxnum}")
    if int(res) != maxnum:
        print(f"res={res}")
        print(f"maxnum={maxnum}")
        print(f"a = {a}")
        break

