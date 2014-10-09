#!/usr/bin/env python3

import numpy
import random

prices = []

def x(m, n):
    if n > 0:
        return prices[m][n - 1]
    return 1

if __name__ == "__main__":
    N = 3  # Y = b0 + b1 * X1 + b2 * X2

    with open("prices.txt", "r") as f:
        for line in f:
            line = line.replace("\r", "").replace("\n", "").strip()
            if line != "":
                prices.append([float(val) for val in line.split(",")[0:N]])

    random.shuffle(prices)
    div = int(0.8 * len(prices))
    pricesTest = prices[div:]
    prices = prices[0:div]

    print("Size 1 = %d" % len(prices))
    print("Size 2 = %d" % len(pricesTest))

    A = []
    for j in range(N):
        L = []
        for i in range(N):
            val = 0
            for k in range(len(prices)):
                val += x(k, i) * x(k, j)
            L.append(val)
        A.append(L)

    B = []
    for j in range(N):
        val = 0
        for k in range(len(prices)):
            val += prices[k][N - 1] * x(k, j)
        B.append(val)

    # Now solving: A x X = B
    A = numpy.array(A)
    B = numpy.array(B)
    X = numpy.linalg.solve(A, B).tolist()

    print("Y = %f + %f * X1 + %f * X2" % tuple(X), end="\n\n")

    for test in pricesTest:
        y = X[0] + X[1] * test[0] + X[2] * test[1]
        print("y = %f; real y = %f" % (y, test[2]))
