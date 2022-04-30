import subprocess, os.path
from random import randint
from time import time


def test(inp, true_res, name=r'/a__.out'):
    while True:
        inp_b = inp.encode('utf-8')
        begin = time()
        fine_res = subprocess.run([os.getcwd() + name],
                                  input=inp_b, capture_output=True).stdout.decode("utf-8")
        print('time:', time() - begin)
        if fine_res.strip(" \n") != true_res.strip(" \n"):
            # print(inp)
            # print(f'true_res = \n{true_res}')
            # print(f'your_res = \n{fine_res}')
            if not input():
                break

n = 239017
inp = f'{n}\n' + ' '.join([str(randint(0, (1 << 10) - 2)) for _ in range(n)]) + '\n'
# with open('input.txt', 'w') as f:
#     f.write(inp)
test(inp, '')
