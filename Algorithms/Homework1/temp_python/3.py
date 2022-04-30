def main():
    a, b, k = get_params()
    for _ in range(k):
        for i in range(a, b):
            swap(i, i+1)

import sys
exec(sys.stdin.read())

