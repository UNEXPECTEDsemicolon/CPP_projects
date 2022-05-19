import subprocess, os.path
from random import randint


while True:
    inp_list = [str(randint(-1000, 1000)) for _ in range(6)]
    inp_list += inp_list[-2:]
    inp = ' '.join(map(str, inp_list))
    inp_b = inp.encode('utf-8')
    fine_res = subprocess.run([os.getcwd() + r'/a.exe'],
                              input=inp_b, capture_output=True).stdout.decode("utf-8")[:3]
    true_res = subprocess.run([os.getcwd() + r'/b.exe'],
                              input=inp_b, capture_output=True).stdout.decode("utf-8")[:3]
    if fine_res.strip(" \n") != true_res.strip(" \n"):
        print(inp)
        print(f'true_res = \n{true_res}')
        print(f'your_res = \n{fine_res}')
        if not input():
            break
