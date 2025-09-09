#include <vector>
#include <algorithm>
#include <random>

// Randomized Quick Sort using an initial shuffle of the array.

namespace {
    int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] <= pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void quicksort(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quicksort(arr, low, pi - 1);
            quicksort(arr, pi + 1, high);
        }
    }
}

std::vector<int> sortArray(std::vector<int>& arr) {
    if (!arr.empty()) {
        std::mt19937 rng(std::random_device{}());
        std::shuffle(arr.begin(), arr.end(), rng);
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
    return arr;
}