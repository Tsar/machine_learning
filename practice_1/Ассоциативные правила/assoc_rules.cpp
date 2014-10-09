#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#define MAX_TRANSACTIONS_COUNT  11000
#define MAX_PRODUCTS_COUNT      1600

std::vector<bool> transactions;

void setTransaction(int tId, int pId, bool value) {
    transactions[tId * MAX_PRODUCTS_COUNT + pId] = value;
}

bool getTransaction(int tId, int pId) {
    return transactions[tId * MAX_PRODUCTS_COUNT + pId];
}

class Tree {
public:
    Tree(int val, Tree* parent) : val(val), parent(parent), leaf(false) {}

    ~Tree() {
        for (std::map<int, Tree*>::const_iterator it = ch.begin(); it != ch.end(); ++it) {
            delete it->second;
        }
    }

    void add(std::vector<int> const& v, int offset = 0) {
        if (offset == v.size()) {
            leaf = true;
            counter = 0;
            return;
        }

        if (ch.find(v[offset]) == ch.end()) {
            ch[v[offset]] = new Tree(v[offset], this);
        }
        ch[v[offset]]->add(v, offset + 1);
    }

    void walkWith(int transactionId) {
        if (leaf) {
            ++counter;
            return;
        }
        for (std::map<int, Tree*>::const_iterator it = ch.begin(); it != ch.end(); ++it) {
            if (getTransaction(transactionId, it->first))
                it->second->walkWith(transactionId);
        }
    }

    /// Chechs for existance of v without element on position skipPosInV
    bool exists(std::vector<int> const& v, int skipPosInV, int offset = 0) {
        if (offset == skipPosInV)
            return exists(v, skipPosInV, offset + 1);
        if (offset == v.size())
            return true;
        if (ch.find(v[offset]) == ch.end())
            return false;
        return ch[v[offset]]->exists(v, skipPosInV, offset + 1);
    }

    void debugPrint() {
        if (leaf) {
            std::cout << "LEAF[" << counter << "], ";
            return;
        }
        std::cout << "{";
        for (std::map<int, Tree*>::const_iterator it = ch.begin(); it != ch.end(); ++it) {
            std::cout << it->first << ": ";
            it->second->debugPrint();
        }
        std::cout << "}, ";
    }

    void createDataByMinSupport(std::vector<std::vector<int> >& res, std::vector<int>& supports, int minSupport) const {
        if (leaf && counter >= minSupport) {
            Tree const* cur = this;
            std::vector<int> ans;
            while (cur->parent != 0) {
                ans.push_back(cur->val);
                cur = cur->parent;
            }
            std::reverse(ans.begin(), ans.end());
            res.push_back(ans);
            supports.push_back(counter);
            return;
        }
        if (leaf)
            return;

        for (std::map<int, Tree*>::const_iterator it = ch.begin(); it != ch.end(); ++it) {
            it->second->createDataByMinSupport(res, supports, minSupport);
        }
    }

private:
    std::map<int, Tree*> ch;
    int val;
    Tree* parent;
    bool leaf;
    int counter;
};

int countEqualFromBeginning(std::vector<int> const& a, std::vector<int> const& b) {
    int r = 0;
    while (a[r] == b[r])
        ++r;
    return r;
}

std::vector<std::vector<int> > aprioriGen(int k, std::vector<std::vector<int> > const& F, Tree* oldRoot) {
    std::vector<std::vector<int> > res;

    int i = 0;
    int j = 1;
    while (i < F.size()) {
        while (j < F.size() && countEqualFromBeginning(F[i], F[j]) == k - 2)
            ++j;
        if (j - i > 1)
        {
            for (int w = i; w < j - 1; ++w) {
                std::vector<int> candidate = F[w];
                candidate.push_back(0);
                for (int q = w + 1; q < j; ++q) {
                    candidate[k - 1] = F[q][k - 2];

                    bool good = true;
                    for (int z = 0; z < candidate.size() && oldRoot != 0; ++z) {
                        if (!oldRoot->exists(candidate, z)) {
                            good = false;
                            break;
                        }
                    }

                    if (good) {
                        res.push_back(candidate);
                    }
                }
            }
        }
        i = j;
        j = i + 1;
    }

    return res;
}

void genNextTake(std::vector<bool>& take, bool& success) {
    int p = take.size() - 1;
    int c = 0;
    while (p >= 0 && take[p]) {
        take[p] = false;
        --p;
        ++c;
    }
    while (p >= 0 && !take[p])
        --p;

    if (p < 0) {
        success = false;
        return;
    }

    take[p] = false;
    for (int k = p + 1; k <= p + 1 + c; ++k) {
        take[k] = true;
    }
    success = true;
}

int main() {
    /*
    /// TEST 1: function aprioriGen
    std::vector<std::vector<int> > test;
    std::vector<int> test2;
    test2.push_back(1);test2.push_back(2);test2.push_back(3);test2.push_back(4);
    test.push_back(test2);
    test2[3] = 5;
    test.push_back(test2);
    test2[3] = 6;
    test.push_back(test2);
    test2[2] = 4;
    test2[3] = 5;
    test.push_back(test2);
    test2[3] = 7;
    test.push_back(test2);
    for (int i = 0; i < test.size(); ++i) {
        for (int j = 0; j < test[0].size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    Tree* root = new Tree();
    for (int i = 0; i < test.size(); ++i) {
        root->add(test[i]);
    }
    root->debugPrint();
    std::cout << std::endl << std::endl;

    std::cout << root->exists(test2) << std::endl;
    test2[0] = 0;
    std::cout << root->exists(test2) << std::endl << std::endl;

    delete root;

    test = aprioriGen(5, test);

    for (int i = 0; i < test.size(); ++i) {
        for (int j = 0; j < test[0].size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
    /// END OF TEST 1
    */

    /*
    /// TEST 2: function genNextTake
    std::vector<bool> t;
    t.push_back(true);
    t.push_back(true);
    t.push_back(true);
    t.push_back(false);
    t.push_back(false);
    t.push_back(false);
    bool working = true;
    int count = 0;
    while (working) {
        ++count;
        for (int i = 0; i < t.size(); ++i) {
            if (t[i])
                std::cout << i + 1 << " ";
        }
        std::cout << std::endl;

        genNextTake(t, working);
    }
    std::cout << "Count = " << count << std::endl;
    return 0;
    /// END OF TEST 2
    */

    int minSupport, minConf;
    std::cout << "Input min support: ";
    std::cin >> minSupport;
    std::cout << "Input min conf (%): ";
    std::cin >> minConf;

    std::map<std::string, int> tIdToNewTId;
    int transactionsCount = 0;

    std::map<std::string, int> nameToId;
    std::map<int, std::string> idToName;
    int productsCount = 0;

    transactions.resize(MAX_TRANSACTIONS_COUNT * MAX_PRODUCTS_COUNT);
    transactions.assign(MAX_TRANSACTIONS_COUNT * MAX_PRODUCTS_COUNT, false);

    std::ifstream f("supermarket.txt");
    std::string s1, s2, s3, s4;
    while (f.good()) {
        getline(f, s1);
        getline(f, s2);
        getline(f, s3);
        getline(f, s4);

        if (nameToId.find(s1) == nameToId.end()) {
            if (productsCount >= MAX_PRODUCTS_COUNT) {
                std::cerr << "Too many products!" << std::endl;
                return 1;
            }
            nameToId.insert(std::make_pair(s1, productsCount));
            idToName.insert(std::make_pair(productsCount, s1));
            ++productsCount;
        }

        if (tIdToNewTId.find(s4) == tIdToNewTId.end()) {
            if (transactionsCount >= MAX_TRANSACTIONS_COUNT) {
                std::cerr << "Too many transactions!" << std::endl;
                return 2;
            }
            tIdToNewTId.insert(std::make_pair(s4, transactionsCount++));
        }

        setTransaction(tIdToNewTId[s4], nameToId[s1], true);
    }
    f.close();

    std::cout << productsCount << " products" << std::endl;
    std::cout << transactionsCount << " transactions" << std::endl << std::endl;

    int F1[MAX_PRODUCTS_COUNT] = {};

    for (int i = 0; i < productsCount; ++i) {
        for (int j = 0; j < transactionsCount; ++j) {
            if (getTransaction(j, i))
                ++F1[i];
        }
    }

    std::vector<std::vector<int> > F;
    std::vector<int> supports;

    std::map<std::vector<int>, int> allSupports;

    for (int i = 0; i < productsCount; ++i) {
        if (F1[i] >= minSupport) {
            std::vector<int> inside;
            inside.push_back(i);
            F.push_back(inside);
            supports.push_back(F1[i]);
            allSupports.insert(std::make_pair(inside, F1[i]));
        }
    }

    Tree* root = 0;
    int k = 1;
    while (!F.empty()) {
        std::cout << "k = " << k << "; count = " << F.size() << std::endl;
        for (int i = 0; i < F.size(); ++i) {
            std::cout << "support = " << supports[i] << ": ";
            for (int j = 0; j < F[i].size(); ++j) {
                std::cout << idToName[F[i][j]] << ((j + 1 != F[i].size()) ? ", " : "");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        ++k;
        F = aprioriGen(k, F, root);

        delete root;
        root = new Tree(0, 0);
        for (int i = 0; i < F.size(); ++i) {
            root->add(F[i]);
        }

        for (int j = 0; j < transactionsCount; ++j) {
            root->walkWith(j);
        }

        F.clear();
        supports.clear();
        root->createDataByMinSupport(F, supports, minSupport);
        for (int i = 0; i < F.size(); ++i) {
            allSupports.insert(std::make_pair(F[i], supports[i]));
        }
    }

    delete root;

    std::cout << std::endl << "== Associative Rules ==" << std::endl;

    for (std::map<std::vector<int>, int>::const_iterator it = allSupports.begin(); it != allSupports.end(); ++it) {
        if (it->first.size() >= 2) {

            /*
            // <debug>
            std::cout << "[SET]: ";
            for (int d = 0; d < it->first.size(); ++d) {
                std::cout << it->first[d] << " ";
            }
            std::cout << std::endl;
            // </debug>
            */

            for (int subsetSize = 1; subsetSize <= it->first.size() - 1; ++subsetSize) {
                // Теперь нужно перебрать все подмножества набора it->first размером subsetSize
                std::vector<bool> take;
                take.resize(it->first.size());
                take.assign(it->first.size(), false);
                for (int k = 0; k < subsetSize; ++k)
                    take[k] = true;
                bool working = true;

                while (working) {
                    std::vector<int> subset, oth;
                    for (int z = 0; z < take.size(); ++z) {
                        if (take[z])
                            subset.push_back(it->first[z]);
                        else
                            oth.push_back(it->first[z]);
                    }

                    /*
                    // <debug>
                    std::cout << "[SuS]: ";
                    for (int d = 0; d < subset.size(); ++d) {
                        std::cout << subset[d] << " ";
                    }
                    std::cout << std::endl;
                    // </debug>
                    */

                    double conf = 100.0 * static_cast<double>(it->second) / allSupports[subset];
                    if (conf >= minConf) {
                        std::cout << "[conf = " << conf << "] ";
                        for (int u = 0; u < subset.size(); ++u) {
                            std::cout << idToName[subset[u]] << ((u < subset.size() - 1) ? ", " : "");
                        }
                        std::cout << " => ";
                        for (int u = 0; u < oth.size(); ++u) {
                            std::cout << idToName[oth[u]] << ((u < oth.size() - 1) ? ", " : "");
                        }
                        std::cout << std::endl;
                    }

                    genNextTake(take, working);
                }
            }

            /*
            // <debug>
            std::cout << std::endl;
            // </debug>
            */
        }
    }

    return 0;
}
