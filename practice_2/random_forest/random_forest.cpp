#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <string>

#define TREES_COUNT           10
#define RANDOM_INDEXES_COUNT  100

void readData(std::string const& fileName, std::vector<int>* data) {
    FILE* f = fopen(fileName.c_str(), "r");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10000; ++j) {
            int x;
            fscanf(f, "%d", &x);
            data[i].push_back(x);
        }
    }
    fclose(f);
}

void readLabels(std::string const& fileName, int* labels) {
    FILE* f = fopen(fileName.c_str(), "r");
    for (int i = 0; i < 100; ++i) {
        int x;
        fscanf(f, "%d", &x);
        labels[i] = x;
    }
    fclose(f);
}

std::vector<int> trainData[100];
int trainLabels[100];

class Tree {
public:
    Tree(std::vector<int> const& sampleIndexes)
        : leaf(false)
        , left(0)
        , right(0)
        , fIndex(-1)
    {
        bool hasMinusOne = false;
        bool hasPlusOne = false;
        for (int i = 0; i < sampleIndexes.size(); ++i) {
            if (trainLabels[sampleIndexes[i]] == 1)
                hasPlusOne = true;
            if (trainLabels[sampleIndexes[i]] == -1)
                hasMinusOne = true;
        }

        assert(hasMinusOne || hasPlusOne);

        leaf = (!hasMinusOne || !hasPlusOne);
        if (leaf)
            return;

        std::vector<int> featureIndexes;
        for (int i = 0; i < RANDOM_INDEXES_COUNT; ++i) {
            featureIndexes.push_back(rand() % 10000);
        }

        double bestGini = 0;
        for (size_t i = 0; i < featureIndexes.size(); ++i) {
            int featureIndex = featureIndexes[i];
            std::set<int> featureValues;
            for (size_t j = 0; j < sampleIndexes.size(); ++j) {
                featureValues.insert(trainData[sampleIndexes[j]][featureIndex]);
            }

            for (std::set<int>::const_iterator it = featureValues.begin(); it != featureValues.end(); ++it) {
                if (gini() > bestGini) {
                    fIndex = featureIndex;
                    fDivVal = *it;
                }
            }
        }

        assert(fIndex != -1);

        left = new Tree(...);
        right = new Tree(...);
    }

private:
    bool leaf;

    // Если не лист
    Tree* left;
    Tree* right;
    int fIndex;
    int fDivVal;
};

int main() {
    srand(time(0));

    readData("arcene_train.data", trainData);
    readLabels("arcene_train.labels", trainLabels);

    std::vector<int> sampleIndexes;
    for (int i = 0; i < 100; ++i) {
        sampleIndexes.push_back(i);
    }

    std::vector<Tree*> trees;
    for (int i = 0; i < TREES_COUNT; ++i) {
        trees.push_back(new Tree(sampleIndexes));
    }

    return 0;
}
