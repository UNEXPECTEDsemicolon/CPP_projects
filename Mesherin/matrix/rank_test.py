import numpy as np
from random import randint, choice
import subprocess, os.path
from time import time

iteration = 1
while True:
    if iteration % 1000 == 0: print(iteration)
    iteration += 1
    # m, n = randint(1, 10), randint(1, 10)
    m, n = 100, 100
    g = 100
    matrix = np.array([[randint(-g, g) for j in range(n)] for i in range(m)])
    # true_res = np.linalg.matrix_rank(matrix)
    # begin = time()
    true_res = round(np.linalg.det(matrix))
    # true_res = np.linalg.inv(matrix)
    # print(f"numpy time = {time() - begin}")
    inp = ''
    for i in range(m):
        inp += ' '.join(map(str, matrix[i])) + '\n'
    inp_b = inp.encode('utf-8')
    begin = time()
    fine_res = subprocess.run([os.getcwd() + r'/a.out', '>> dev\null'],
                              input=inp_b, capture_output=True).stdout.decode("utf-8")
    end = time()
    if int(fine_res.strip("\n ")) != true_res:
        print(inp)
        print(f'true_res = {true_res}')
        print(f'your_res = {fine_res}')
    print(f'python time = {end - begin}')
    break
