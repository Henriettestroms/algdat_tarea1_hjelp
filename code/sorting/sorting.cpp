#include <algorithm>
#include <iostream>
#include <vector>

#if defined(USE_INSERTION)
#include "insertionsort.cpp"
#elif defined(USE_MERGE)
#include "mergesort.cpp"
#elif defined(USE_QUICK)
#include "quicksort.cpp"
#elif defined(USE_PANDA)
#include "pandasort.cpp"
#else
std::vector<int> sortArray(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());
    return arr;
}
#endif

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t n;
    if (!(std::cin >> n)) return 0;
    std::vector<int> arr(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    auto sorted = sortArray(arr);
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << sorted[i];
    }
    std::cout << '\n';
    return 0;
}
