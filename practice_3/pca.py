#!/usr/bin/env python3

import sys
import numpy

if __name__ == "__main__":
    for fileName in ["newBasis" + str(i) for i in range(1, 4)]:
        print("=== %s ===" % fileName)
        L = []
        with open(fileName, "r") as f:
            for line in f:
                L.append([float(x) for x in line.split()])
        M = numpy.matrix(L)

        for i in range(200):
            M[:,i] -= numpy.mean(M[:,i])

        MT = M.transpose()
        U, s, V = numpy.linalg.svd(MT)

        print(s)
        print()
