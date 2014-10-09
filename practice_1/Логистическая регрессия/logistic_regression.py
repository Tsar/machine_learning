#!/usr/bin/env python3

import math
import random

points = []

teta = [0, 0, 0]

def p(point):
    return 1.0 / (1.0 + math.exp(-(teta[0] * point[0] + teta[1] * point[1] + teta[2])))

def gradDescent():
    global teta

    res = [0.0, 0.0, 0.0]
    for x, y, v in points:
        cf = 0.08 * (v - p([x, y]))
        res[0] += cf * x
        res[1] += cf * y
        res[2] += cf
    res[0] -= 2 * 0.002 * teta[0]
    res[1] -= 2 * 0.002 * teta[1]
    res[2] -= 2 * 0.002 * teta[2]

    return res

if __name__ == "__main__":
    with open("chips.txt", "r") as f:
        for line in f:
            line = line.replace("\r", "").replace("\n", "").strip()
            if line != "":
                [x, y, v] = line.split(",")[0:3]
                points.append((float(x), float(y), int(v)))

    #random.shuffle(points)
    div = int(0.8 * len(points))
    pointsTest = points[div:]
    points = points[0:div]

    print("Size 1 = %d" % len(points))
    print("Size 2 = %d" % len(pointsTest))

    eps = 0.000000005
    maxSteps = 100000

    summ = eps * 2
    step = 0
    while (summ < eps) and (step < maxSteps):
        step += 1
        gd = gradDescent()
        teta[0] += gd[0]
        teta[1] += gd[1]
        teta[2] += gd[2]
        summ = gd[0] * gd[0] + gd[1] * gd[1] + gd[2] * gd[2]

    correct = 0
    for x, y, v in pointsTest:
        if p([x, y]) <= 0.5:
            v2 = 0
        else:
            v2 = 1

        if v == v2:
            correct += 1

    print("Correct: %f %%" % (100 * float(correct) / len(pointsTest)))
