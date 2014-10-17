#!/usr/bin/env python3

import os, math

LEARNING_PARTS = [1, 2, 3, 4, 5, 6, 7, 8]
TESTING_PARTS  = [9, 10]

def messages(partsList):
    for path in ["part" + str(x) for x in partsList]:
        for fileName in os.listdir(path):
            if "legit" in fileName:
                spam = False
            elif "spmsg" in fileName:
                spam = True
            else:
                print("Message file '%s' can not be classified to spam or legit message!" % fileName)
                continue
            #print(path + "/" + fileName)
            with open(path + "/" + fileName) as f:
                msgContents = f.read()
                contents = msgContents.split('\n')
                subject = []
                message = []
                for line in contents:
                    if line == "":
                        continue
                    words = line.split(' ')
                    if words[0].lower() == "subject:":
                        subject += [int(w) for w in words[1:] if w != ""]
                    else:
                        message += [int(w) for w in words if w != ""]
                yield((spam, subject, message))

if __name__ == "__main__":
    occSpam = {}  # появления в спаме
    occAll = {}   # появления всего
    for (spam, subject, message) in messages(LEARNING_PARTS):
        for w in subject + message:
            if w in occAll:
                occAll[w] += 1
            else:
                occAll[w] = 1
            if spam:
                if w in occSpam:
                    occSpam[w] += 1
                else:
                    occSpam[w] = 1

    averageOcc = float(sum(occAll.values())) / len(occAll)

    weight = {}
    for w in occAll.keys():
        if occAll[w] >= averageOcc / 5.0:
            if w in occSpam.keys():
                weight[w] = float(occSpam[w]) / occAll[w]
            else:
                weight[w] = 0.0

    correct = 0
    countAll = 0
    for (spam, subject, message) in messages(TESTING_PARTS):
        countAll += 1
        logSum = 0.0
        for w in subject + message:
            if not w in weight.keys():
                continue
            if weight[w] <= 0.0000001:
                weight[w] = 0.0000001
            if weight[w] >= 0.9999999:
                weight[w] = 0.9999999
            logSum += (math.log(1.0 - weight[w]) - math.log(weight[w]))

        if logSum > 700:
            p = 0.0
        else:
            p = 1.0 / (1.0 + math.exp(logSum))
        if spam == (p >= 0.8):
            correct += 1

    print("Learning parts:", LEARNING_PARTS)
    print("Testing parts: ", TESTING_PARTS)
    print("\nTEST RESULTS:")
    print("Correct = %d\nAll = %d\nCorrect / All = %f" % (correct, countAll, float(correct) / countAll))
