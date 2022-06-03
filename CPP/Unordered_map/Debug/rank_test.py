import numpy as np
from random import randint, choice
import subprocess, os.path
from time import time

begin = time()
fine_res = subprocess.run([os.getcwd() + r'\a.exe'],
                          capture_output=True).stdout.decode("utf-8")
end = time()
print(end - begin)
