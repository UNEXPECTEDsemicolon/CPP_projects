def process(s):
    return empty() if is_empty(s) else join(head(s) if head(s) % 2 else empty(), process(tail(s)))

import sys
exec(sys.stdin.read())
