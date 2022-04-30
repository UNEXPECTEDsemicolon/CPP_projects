from random import randint
import subprocess, sys, os, os.path
from time import time

def test(inp, true_res, name=r'/a__.out'):
    inp_b = inp.encode('utf-8')
    begin = time()
    fine_res = subprocess.run([os.getcwd() + name],
                              input=inp_b, capture_output=True).stdout.decode("utf-8")
    # print('time:', time() - begin)
    if fine_res.strip(" \n") != true_res.strip(" \n"):
        print(inp)
        print(f'true_res = \n{true_res}')
        print(f'your_res = \n{fine_res}')
        if not input():
            exit()

n = randint(1, 100000)
k = randint(1, 15)
# a__ = [str(randint(1, s)) for i in range(n)]
# inp = (f'{n}\n' + ' '.join(a__) + f' {s}\n\0')
inp = f'{n} {k}\n'
# print(inp)
# inp = inp.encode('utf-8')
# begin = time()
# subprocess.run([os.getcwd() + r'/a__.out'], input=inp, capture_output=False, text=False)
# print(f"time={time()-begin}")
while True:
    test(inp, )