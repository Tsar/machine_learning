#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <string>
#include <limits>

double square(double x) {
    return x * x;
}

template <class T>
void readData(std::string const& fileName, std::vector<T>* d) {
    FILE* f = fopen(fileName.c_str(), "r");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10000; ++j) {
            int x;
            fscanf(f, "%d", &x);
            d[i].push_back(x);
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

std::vector<double> data[100];
int labels[100];
std::vector<int> selected;

double getEuclideanDist(int n1, int n2) {
    double dist = 0.0;
    for (int f = 0; f < 10000; ++f) {
        dist += square(data[n1][f] - data[n2][f]);
    }
    assert(!isnan(dist));
    return dist;
}

int getEuclideanClosestWithLabel(int n, int l) {
    double minDist = std::numeric_limits<double>::max();
    int res = -1;
    for (int i = 0; i < 100; ++i) {
        if (i == n || labels[i] != l)
            continue;

        double dist = getEuclideanDist(n, i);
        if (minDist > dist) {
            minDist = dist;
            res = i;
        }
    }
    return res;
}

void writeSelectedFeatures(std::string const& origFileName, std::string const& fsFileName) {
    std::vector<int> d[100];
    readData(origFileName, d);

    FILE* fd = fopen(fsFileName.c_str(), "w");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < selected.size(); ++j) {
            int f = selected[j];
            fprintf(fd, "%d ", d[i][f]);
        }
        fprintf(fd, "\n");
    }
    fclose(fd);
}

int main() {
    srand(time(0));

    readData("../practice_2/random_forest/arcene_train.data", data);
    readLabels("../practice_2/random_forest/arcene_train.labels", labels);

    /// Scaling each feature to interval [0, 1]
    for (int f = 0; f < 10000; ++f) {
        double min_ = data[0][f];
        double max_ = data[0][f];
        for (int i = 1; i < 100; ++i) {
            if (min_ > data[i][f]) min_ = data[i][f];
            if (max_ < data[i][f]) max_ = data[i][f];
        }

        for (int i = 0; i < 100; ++i) {
            data[i][f] = (min_ != max_) ? (data[i][f] - min_) / (max_ - min_) : 0.0;
        }
    }

    std::vector<double> w;
    w.resize(10000);
    w.assign(10000, 0.0);

    /// Relief algorithm
    int m = 100;
    for (int iter = 0; iter < m; ++iter) {
        int r = rand() % 100;
        int nearHit  = getEuclideanClosestWithLabel(r, labels[r]);
        int nearMiss = getEuclideanClosestWithLabel(r, (labels[r] == -1) ? 1 : -1);

        for (int f = 0; f < 10000; ++f) {
            w[f] = w[f] - square(data[r][f] - data[nearHit][f]) + square(data[r][f] - data[nearMiss][f]);
        }
    }
    for (int f = 0; f < 10000; ++f) {
        w[f] /= m;
    }

    double threshold = 0.01;

    int fCount = 0;
    for (int f = 0; f < 10000; ++f) {
        if (w[f] >= threshold) {
            selected.push_back(f);
            ++fCount;
        }
    }

    printf("Selected features count = %d\n", fCount);

    writeSelectedFeatures("../practice_2/random_forest/arcene_train.data", "arcene_train_FS.data");
    writeSelectedFeatures("../practice_2/random_forest/arcene_valid.data", "arcene_valid_FS.data");

    return 0;
}
