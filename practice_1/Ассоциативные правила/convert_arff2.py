#!/usr/bin/env python3

import arff

def removeApostrophes(s):
    if s.startswith("'") and s.endswith("'"):
        return s[1:-1]
    return s

if __name__ == "__main__":
    with open("supermarket2.txt", "w") as f:
        for row in arff.load("supermarket2.arff"):
            f.write("%s\n%d\n" % (str(removeApostrophes(row.product_name)), int(row.basket_id)))
    f.close()
