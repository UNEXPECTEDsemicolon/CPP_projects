import subprocess, os.path
from time import time
from random import randint

name = '/a__.out'
n = 1000
a = randint(1, n)
matr = '\n'.join([' '.join([str(int(randint(0, 100) < 70)) for j in range(n)]) for i in range(n)]) + '\n'
inp = f'{n} {a}\n{matr}'

inp_b = inp.encode('utf-8')
begin = time()
fine_res = subprocess.run(['valgrind', os.getcwd() + name],
                          input=inp_b, capture_output=True).stdout.decode("utf-8")
print(time() - begin)
# print(fine_res)
# if fine_res.strip(" \n") != true_res.strip(" \n"):
#     print(inp)
#     print(f'true_res = \n{true_res}')
#     print(f'your_res = \n{fine_res}')
#     if not input():
#
