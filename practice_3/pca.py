#!/usr/bin/env python3

import numpy

if __name__ == "__main__":
    for fileName in ["newBasis" + str(i) for i in range(1, 4)]:
        L = []
        with open(fileName, "r") as f:
            for line in f:
                L.append([float(x) for x in line.split()])
        M = numpy.matrix(L)
        MT = M.transpose()
        print(M)
        print("------------")
        print(MT)
        print("============================")
