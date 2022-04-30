def process(s1, s2):
    return head(s1) == head(s2) and process(tail(s1), tail(s2))

import sys
exec(sys.stdin.read())
