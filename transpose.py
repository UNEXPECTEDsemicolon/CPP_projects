def transpose(A):
    A = [[A[i][j] for i in range(len(A))] for j in range(len(A[0]))]
    print(A)

A = [[1, 2, 3], [4, 5, 6]]
transpose(A)
print(A)