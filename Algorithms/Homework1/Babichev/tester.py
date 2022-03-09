from random import randint
import subprocess, sys, os, os.path
from time import time

l, r = -10**18, 10**18
n = 10000000
k = 10000
a = [str(randint(l, r)) for i in range(n)]
inp = (f'{n} {k}\n' + '\n'.join(a) + '\0').encode('utf-8')
begin = time()
subprocess.run([os.getcwd() + r'/a.out', '>> dev\null'], input=inp, capture_output=False, text=False)
print(f"time={time()-begin}")
