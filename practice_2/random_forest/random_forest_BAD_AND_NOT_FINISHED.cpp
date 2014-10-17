#include <stdio.h>
#include <string>

#define TREES_COUNT           100
#define RANDOM_INDEXES_COUNT  100

std::vector<int> v[100];

class Tree {
public:
private:
    Tree* left;
    Tree* right;
    int index;  // индекс признака
    int div;    // значение, по которому производится разделение
};

std::vector<int> genRandomIndexes() {
    std::vector<int> t;
    for (int i = 0; i < 10000; ++i) {
        t.push_back(i);
    }
    for (int i = 0; i < RANDOM_INDEXES_COUNT; ++i) {
        int x = (rand() % (10000 - i)) + i;
        std::swap(t[x], t[i]);
    }
    t.resize(RANDOM_INDEXES_COUNT);
    return t;
}

Tree* buildTree() {
    std::vector indexes = genRandomIndexes();
    for (int i = 0; i < indexes.size(); ++i) {
        int ind = indexes[i];
        std::set<int> valuesOnInd;
        for (int l = 0; l < 100; ++l) {
            valuesOnInd.insert(v[l][ind]);
        }
        for (std::set<int>::const_iterator it = valuesOnInd.begin(); it != valuesOnInd.end(); ++it) {
            
        }
    }
}

int main() {
    srand(time(0));

    FILE* f = fopen("arcene_train.data", "r");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10000; ++j) {
            int x;
            fscanf("%d", &x);
            v[i].push_back(x);
        }
    }
    fclose(f);

    std::vector<Tree*> trees;
    for (int i = 0; i < TREES_COUNT; ++i) {
        trees.push_back(buildTree());
    }

    return 0;
}
