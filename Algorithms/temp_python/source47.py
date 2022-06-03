def empty():
    return []

def is_empty(s):
    return len(s) == 0

def join(x, s):
    return [x] + s if x != empty() else s

def head(s):
    return s[0]

def tail(s):
    return s[1:]
