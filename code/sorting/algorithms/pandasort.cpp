#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <climits>

// hold denne intern for å unngå multiple definition
static std::vector<int> sortArray(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return arr;

    int block_size = std::sqrt(n);
    int block_count = (n + block_size - 1) / block_size;
    std::vector<std::queue<int>> block_queue(block_count);

    for (int i = 0; i < block_count; i++) {
        int l = i * block_size;
        int r = std::min((i + 1) * block_size - 1, n - 1);

        for (int j = l; j <= r; j++) {
            int min_idx = j;
            for (int k = j + 1; k <= r; k++) {
                if (arr[k] < arr[min_idx]) min_idx = k;
            }
            if (min_idx != j) std::swap(arr[j], arr[min_idx]);
        }
        for (int j = l; j <= r; j++) block_queue[i].push(arr[j]);
    }

    for (int i = 0; i < n; i++) {
        int mn = INT_MAX, mni = -1;
        for (int j = 0; j < block_count; j++) {
            if (block_queue[j].empty()) continue;
            int cur = block_queue[j].front();
            if (cur < mn) { mni = j; mn = cur; }
        }
        arr[i] = mn;
        block_queue[mni].pop();
    }
    return arr;
}

// offentlig API som sorting.cpp lenker mot
void panda_sort(std::vector<int>& arr) {
    (void)sortArray(arr);
}
