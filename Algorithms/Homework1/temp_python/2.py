def main():
    a, b = get_bounds()
    while a < b:
        swap(a, b)
        a += 1
        b -= 1

import sys
exec(sys.stdin.read())

