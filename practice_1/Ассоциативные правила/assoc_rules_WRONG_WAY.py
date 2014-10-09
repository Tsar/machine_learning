#!/usr/bin/env python3

# Apriori algorithm (implemented by Ioann Volkov)

import sys
import operator
import arff

LEVEL = 0
MIN_SUPPORT = 46

def removeApostrophes(s):
    if s.startswith("'") and s.endswith("'"):
        return s[1:-1]
    return s

class Node:
    def __init__(self, k, elems):
        self.k = k
        self.elems = elems
        self.children = []

    def addChild(self, node):
        self.children.append(node)

    def addChildren(self, nodesList):
        self.children.extend(nodesList)

    def getElems(self):
        return self.elems

    def getChildren(self):
        return self.children

root = Node(0, [])

def getNodesByK(cur, k):
    if cur.k == k:
        return [cur]
    resultList = []
    for node in cur.getChildren():
        resultList.extend(getNodesByK(node, k))
    return resultList

def aprioriGen(k, T):
    global root

    L = getNodesByK(root, k - 2)
    for node in L:
        for el1 in node.getChildren():
            for el2 in node.getChildren():
                if el1

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
            baskets[basketId] = set()

        baskets[basketId].add(paramsMap[param])

    count = {}   # id-параметра -> сколько раз встречается в транзакциях
    for k in baskets.keys():
        for paramId in baskets[k]:
            if paramId in count:
                count[paramId] += 1
            else:
                count[paramId] = 1

    F = [p for p, v in count.items() if v >= MIN_SUPPORT]

    root.addChildren([Node(1, [p]) for p, v in count.items() if v >= MIN_SUPPORT)])

    k = 1
    while len(F) > 0:
        k += 1
        aprioriGen(k, T)
