#!/usr/bin/env python3

from math import sqrt
import collections, operator
import random

def dist(p1, p2):
    return sqrt((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2)

if __name__ == "__main__":
    points = []
    with open("chips.txt", "r") as f:
        for line in f:
            line = line.replace("\r", "").replace("\n", "").strip()
            if line != "":
                [x, y, v] = line.split(",")[0:3]
                points.append((float(x), float(y), v))

    random.shuffle(points)
    div = int(0.7 * len(points))
    pointsTest = points[div:]
    points = points[0:div]

    print("Size 1 = %d" % len(points))
    print("Size 2 = %d" % len(pointsTest))

    k = int(input("Input K: "))

    correct = 0
    for test in pointsTest:
        x = test[0]
        y = test[1]

        dists = {}
        for p in points:
            dists[dist(p, (x, y))] = p[2]

        distsSorted = collections.OrderedDict(sorted(dists.items()))
        vCount = {}
        i = 0
        for q, v in distsSorted.items():
            if v in vCount:
                vCount[v] += 1
            else:
                vCount[v] = 1
            i += 1
            if i == k:
                break

        vCountSorted = sorted(vCount.items(), key=operator.itemgetter(1), reverse=True)

        if vCountSorted[0][0] == test[2]:
            print("CORRECT: Class of (%f, %f): %s" % (x, y, vCountSorted[0][0]))
            correct += 1
        else:
            print("WRONG:   Class of (%f, %f): %s" % (x, y, vCountSorted[0][0]))

    print("Correct percent: %f" % (correct / len(pointsTest)))
