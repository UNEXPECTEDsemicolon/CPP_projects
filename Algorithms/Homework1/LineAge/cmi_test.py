import numpy as np
from random import randint, choice
from random import uniform
import subprocess, os.path
from time import time

while True:
    # m = n = randint(1, 10)
    m = n = 1
    # m, n = 20, 20
    matrix = np.array([[randint(-10, 10) + randint(-10, 10) * 1j for j in range(n)] for i in range(m)])
    # if round(np.linalg.det(matrix), 5) == 0.0:
    #     continue
    # true_res = np.linalg.matrix_rank(matrix)
    # begin = time()
    # true_res = round(np.linalg.det(matrix))
    # print(f"numpy time = {time() - begin}")
    inp = f'{n}\n'
    for i in range(m):
        inp += ' '.join(map(lambda x: f'{x.real} {x.imag}', matrix[i])) + '\n'
    inp_b = inp.encode('utf-8')
    with open("matrix.in", 'wb') as f:
        f.write(inp_b)
    # begin = time()
    err = subprocess.run([os.getcwd() + r'/a__.out', '>> dev\null']).returncode
    if round(np.linalg.det(matrix), 5) == 0.0:
        continue
    else:
        true_res = np.linalg.inv(matrix)
    # print(err)
    with open("matrix.out") as f:
        fine_res = f.read()
    res_matr = np.array(list(map(lambda s: [float(s.split()[i]) + 1j * float(s.split()[i+1]) for i in range(0, len(s.split()), 2)], fine_res.split('\n')[:-1])))
    # end = time()
    cond = (abs((res_matr - true_res)) < np.array([[0.000001] * n] * n))
    if not cond.all():
        print(inp)
        print(f'true_res = {true_res}')
        print(f'your_res = {res_matr}')
        print(cond)
        # print(np.linalg.det(matrix))
        # print(f'python time = {end - begin}')
        break

