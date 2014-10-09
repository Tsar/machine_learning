#!/usr/bin/env python3

import math
import random

points = []

teta = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

def p(point):
    return 1.0 / (1.0 + math.exp(-(teta[0] * point[0] + teta[1] * point[1] + teta[2] * point[0] * point[0] + teta[3] * point[1] * point[1] + teta[4] * point[0] * point[1] + teta[5])))

def gradDescent():
    global teta

    res = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    for x, y, v in points:
        cf = 0.08 * (v - p([x, y]))
        res[0] += cf * x
        res[1] += cf * y
        res[2] += cf * x * x
        res[3] += cf * y * y
        res[4] += cf * x * y
        res[5] += cf

    for i in range(6):
        res[i] -= 2 * 0.002 * teta[i]

    return res

if __name__ == "__main__":
    with open("chips.txt", "r") as f:
        for line in f:
            line = line.replace("\r", "").replace("\n", "").strip()
            if line != "":
                [x, y, v] = line.split(",")[0:3]
                points.append((float(x), float(y), int(v)))

    random.shuffle(points)
    div = int(0.8 * len(points))
    pointsTest = points[div:]
    points = points[0:div]

    print("Size 1 = %d" % len(points))
    print("Size 2 = %d" % len(pointsTest))

    eps = 0.0000000001
    maxSteps = 1000000

    summ = eps * 2
    step = 0
    while (summ > eps) and (step < maxSteps):
        step += 1
        gd = gradDescent()
        for i in range(6):
            teta[i] += gd[i]
        summ = float(0)
        for i in range(6):
            summ += gd[i] * gd[i]

    print(summ)
    print(step)

    correct = 0
    for x, y, v in pointsTest:
        if p([x, y]) <= 0.5:
            v2 = 0
        else:
            v2 = 1

        if v == v2:
            correct += 1

    print("Correct: %f %%" % (100 * float(correct) / len(pointsTest)))
