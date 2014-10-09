#!/usr/bin/env python3

# Apriori algorithm (implemented by Ioann Volkov)

import sys
import operator
import arff

LEVEL = 0
MIN_SUPPORT = 10

def removeApostrophes(s):
    if s.startswith("'") and s.endswith("'"):
        return s[1:-1]
    return s

def aprioriGen(F):
    F_ = [frozenset(f) for f in F]
    R = []
    for l1 in F:
        for l2 in F:
            if l1[:-1] == l2[:-1] and l1[-1] < l2[-1]:
                candidate = l1 + [l2[-1]]
                good = True
                for i in range(len(candidate)):
                    if not frozenset(candidate[0:i] + candidate[i + 1:]) in F_:
                        good = False
                        break
                if good:
                    R.append(candidate)
    return R

def walkTree(cur, fs):
    if cur == {}:
        cur[-1] = 1
        return
    if -1 in cur:
        cur[-1] += 1
        return
    for v in cur:
        if v in fs:
            walkTree(cur[v], fs)

def createList(cur, l, L):
    if cur == {}:
        return
    if (-1 in cur) and (cur[-1] >= MIN_SUPPORT):
        L.append(l)
        return
    if -1 in cur:  # and (cur[-1] < MIN_SUPPORT)
        return
    for v in cur:
        createList(cur[v], l + [v], L)

if __name__ == "__main__":
    paramsMap = {}
    paramsBackMap = {}
    lastParamId = 1

    baskets = {}

    for row in arff.load("supermarket.arff"):
        if LEVEL == 0:
            param = row.product_name
        elif LEVEL == 1:
            param = row.product_category
        elif LEVEL == 2:
            param = row.product_department
        else:
            print("Unsupported level")
            sys.exit(1)

        param = str(removeApostrophes(param))
        basketId = int(row.basket_id)

        if not param in paramsMap:
            paramsMap[param] = lastParamId
            paramsBackMap[lastParamId] = param
            lastParamId += 1

        if not basketId in baskets:
            baskets[basketId] = []

        baskets[basketId].append(paramsMap[param])

    baskets = dict((k, frozenset(l)) for k, l in baskets.items())

    count = {}   # id-параметра -> сколько раз встречается в транзакциях
    for k, l in baskets.items():
        for paramId in l:
            if paramId in count:
                count[paramId] += 1
            else:
                count[paramId] = 1

    F = [[p] for p, v in count.items() if v >= MIN_SUPPORT]

    k = 1
    while len(F) > 0 and len(F[0]) > 0:
        print("k = %d" % k)
        print(F)
        for f in F:
            for p in f:
                print(paramsBackMap[p], end="")
                print(", ", end="")
            print("")
        print("\n")

        k += 1
        print("#1")
        L = aprioriGen(F)
        #count = dict((frozenset(x), 0) for x in aprioriGen(F))
        print("#2")

        root = {}
        for l in L:
            cur = root
            for el in l:
                if el in cur:
                    cur = cur[el]
                else:
                    cur[el] = {}
                    cur = cur[el]

        for b, t in baskets.items():
            walkTree(root, t)

        #for b, t in baskets.items():
        #    for f in count.keys():
        #        if f.issubset(t):
        #            count[f] += 1

        print("#3", end="\n\n")

        F = []
        createList(root, [], F)
