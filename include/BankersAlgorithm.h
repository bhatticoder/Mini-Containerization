#ifndef BANKERS_ALGORITHM_H
#define BANKERS_ALGORITHM_H

#include <vector>
#include <iostream>

class BankersAlgorithm {
    int n, m;
    std::vector<std::vector<int>> allocation;
    std::vector<std::vector<int>> max_need;
    std::vector<int> available;

public:
    BankersAlgorithm(int processes, int resources) 
        : n(processes), m(resources) {
        allocation.resize(n, std::vector<int>(m, 0));
        max_need.resize(n, std::vector<int>(m, 0));
        available.resize(m, 0);
    }

    void setAvailable(std::vector<int> avail) { available = avail; }
    void setAllocation(int p, std::vector<int> alloc) { allocation[p] = alloc; }
    void setMaxNeed(int p, std::vector<int> max) { max_need[p] = max; }

    bool isSafe() {
        std::vector<int> work = available;
        std::vector<bool> finish(n, false);
        std::vector<std::vector<int>> need(n, std::vector<int>(m));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                need[i][j] = max_need[i][j] - allocation[i][j];

        int count = 0;
        while (count < n) {
            bool found = false;
            for (int p = 0; p < n; p++) {
                if (!finish[p]) {
                    int j;
                    for (j = 0; j < m; j++)
                        if (need[p][j] > work[j]) break;

                    if (j == m) {
                        for (int k = 0; k < m; k++) work[k] += allocation[p][k];
                        finish[p] = true;
                        found = true;
                        count++;
                    }
                }
            }
            if (!found) return false; // Unsafe state
        }
        return true; // Safe state
    }
};

#endif
