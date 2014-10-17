#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <string>
#include <set>

#define TREES_COUNT           11
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
        int minusOnesCount = 0;
        int plusOnesCount = 0;
        for (int i = 0; i < sampleIndexes.size(); ++i) {
            if (trainLabels[sampleIndexes[i]] == -1)
                ++minusOnesCount;
            else if (trainLabels[sampleIndexes[i]] == 1)
                ++plusOnesCount;
            else
                assert(false);
        }

        assert(minusOnesCount > 0 || plusOnesCount > 0);

        leaf = (minusOnesCount == 0 || plusOnesCount == 0);
        if (leaf) {
            fMult = (minusOnesCount > plusOnesCount) ? -1 : 1;
            return;
        }

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
                for (int featureMult = -1; featureMult < 2; featureMult += 2) {
                    int correctMinusOnes = 0;
                    int wrongMinusOnes = 0;
                    int correctPlusOnes = 0;
                    int wrongPlusOnes = 0;
                    for (size_t k = 0; k < sampleIndexes.size(); ++k) {
                        int sampleIndex = sampleIndexes[k];
                        if (trainLabels[sampleIndex] == -1) {
                            if (trainData[sampleIndex][featureIndex] * featureMult < (*it) * featureMult)
                                ++correctMinusOnes;
                            else
                                ++wrongMinusOnes;
                        } else if (trainLabels[sampleIndex] == 1) {
                            if (trainData[sampleIndex][featureIndex] * featureMult >= (*it) * featureMult)
                                ++correctPlusOnes;
                            else
                                ++wrongPlusOnes;
                        } else {
                            assert(false);
                        }
                    }

                    double pCm1 = static_cast<double>(minusOnesCount) / sampleIndexes.size();  // p(c_-1)
                    double pCp1 = static_cast<double>(plusOnesCount)  / sampleIndexes.size();  // p(c_+1)
                    double pTl = static_cast<double>(correctMinusOnes + wrongPlusOnes) / sampleIndexes.size();  // p(t_left)
                    double pTr = static_cast<double>(correctPlusOnes + wrongMinusOnes) / sampleIndexes.size();  // p(t_right)
                    double pCm1Tl = (static_cast<double>(correctMinusOnes) / sampleIndexes.size()) / pTl;  // p(c_-1 | t_left)
                    double pCp1Tl = (static_cast<double>(correctPlusOnes)  / sampleIndexes.size()) / pTl;  // p(c_+1 | t_left)
                    double pCm1Tr = (static_cast<double>(correctMinusOnes) / sampleIndexes.size()) / pTr;  // p(c_-1 | t_right)
                    double pCp1Tr = (static_cast<double>(correctPlusOnes)  / sampleIndexes.size()) / pTr;  // p(c_+1 | t_right)
                    double gini = 1.0 - pCm1 * pCm1 - pCp1 * pCp1 - pTl * (pCm1Tl * (1 - pCm1Tl) + pCp1Tl * (1 - pCp1Tl)) - pTr * (pCm1Tr * (1 - pCm1Tr) + pCp1Tr * (1 - pCp1Tr));
                    if (gini > bestGini) {
                        bestGini = gini;
                        fIndex = featureIndex;
                        fDivVal = *it;
                        fMult = featureMult;
                    }
                }
            }
        }

        assert(fIndex != -1);

        std::vector<int> leftSampleIndexes;
        std::vector<int> rightSampleIndexes;

        for (size_t i = 0; i < sampleIndexes.size(); ++i) {
            int sampleIndex = sampleIndexes[i];
            if (trainData[sampleIndex][fIndex] * fMult < fDivVal * fMult)
                leftSampleIndexes.push_back(sampleIndex);
            else
                rightSampleIndexes.push_back(sampleIndex);
        }

        left = new Tree(leftSampleIndexes);
        right = new Tree(rightSampleIndexes);
    }

    ~Tree() {
        if (left)
            delete left;
        if (right)
            delete right;
    }

    int classify(std::vector<int> const& test) {
        if (leaf)
            return fMult;

        if (test[fIndex] * fMult < fDivVal * fMult)
            return left->classify(test);
        else
            return right->classify(test);
    }

private:
    bool leaf;

    // Если не лист
    Tree* left;
    Tree* right;
    int fIndex;   // индекс фичи, по которой происходит разделение
    int fDivVal;  // значение фичи, по которому происходит разделение
    int fMult;    // определяет, значение фичи должно быть больше или меньше (а если лист, то хранит результат)
};

int main() {
    srand(time(0));

    printf("Training...\n");

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

    printf("Testing...\n");

    std::vector<int> testData[100];
    int testLabels[100];
    readData("arcene_valid.data", testData);
    readLabels("arcene_valid.labels", testLabels);

    int correct = 0;
    for (int test = 0; test < 100; ++test) {
        int treesVotedMinusOne = 0;
        int treesVotedPlusOne = 0;
        for (int i = 0; i < TREES_COUNT; ++i) {
            int res = trees[i]->classify(testData[test]);
            if (res == -1) {
                ++treesVotedMinusOne;
            } else if (res == 1) {
                ++treesVotedPlusOne;
            } else {
                assert(false);
            }
        }

        int finalRes = (treesVotedMinusOne > treesVotedPlusOne) ? -1 : 1;
        if (testLabels[test] == finalRes) {
            ++correct;
        }
    }

    printf("Correct = %d\nAll = %d\nResult = %lf\n", correct, 100, correct / 100.0);

    for (int i = 0; i < TREES_COUNT; ++i)
        delete trees[i];

    return 0;
}
