#!/usr/bin/env python3

if __name__ == "__main__":
    with open("arcene_train.data") as f1:
        for line in f1:
            print(len(line.split()))

    with open("arcene_valid.data") as f2:
        for line in f2:
            print(len(line.split()))
