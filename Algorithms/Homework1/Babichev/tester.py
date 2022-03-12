from random import randint
import subprocess, sys, os, os.path
from time import time

n = 100
s = 10**6
a = [str(randint(1, s)) for i in range(n)]
inp = (f'{n}\n' + '\n'.join(a) + f'{s}\n\0').encode('utf-8')
begin = time()
subprocess.run([os.getcwd() + r'/a.out', '>> dev\null'], input=inp, capture_output=False, text=False)
print(f"time={time()-begin}")
