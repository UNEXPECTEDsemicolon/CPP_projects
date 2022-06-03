class chain_sum:
    def __init__(self, x=0):
        self.x = x

    def __call__(self, x=0):
        self.x += x
        return self

    def __int__(self):
        return self.x

    def __str__(self):
        return str(int(self))

    def __repr__(self):
        return str(self).__repr__()


print(1 + chain_sum(5))
print(chain_sum(5)(2))
print(chain_sum(5)(100)(-10))
