a__ = [1, 2, 3, 4, 5, 6, 7]


def length():
    return len(a__)


def get(i):
    return a__[i - 1]


def swap(i, j):
    a__[i - 1], a__[j - 1] = a__[j - 1], a__[i - 1]


def get_bounds():
    return 1, 3


def get_params():
    return 2, 5, 2
