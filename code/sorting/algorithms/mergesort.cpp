#include <vector>

namespace {
    void merge(std::vector<int>& arr, std::vector<int>& tmp, std::size_t l, std::size_t m, std::size_t r) {
        std::size_t i = l, j = m, k = l;
        while (i < m && j < r) {
            if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
            else tmp[k++] = arr[j++];
        }
        while (i < m) tmp[k++] = arr[i++];
        while (j < r) tmp[k++] = arr[j++];
        for (std::size_t t = l; t < r; ++t) arr[t] = tmp[t];
    }

    void mergesort(std::vector<int>& arr, std::vector<int>& tmp, std::size_t l, std::size_t r) {
        if (r - l <= 1) return;
        std::size_t m = l + (r - l) / 2;
        mergesort(arr, tmp, l, m);
        mergesort(arr, tmp, m, r);
        merge(arr, tmp, l, m, r);
    }
}

std::vector<int> sortArray(std::vector<int> arr) {
    std::vector<int> tmp(arr.size());
    mergesort(arr, tmp, 0, arr.size());
    return arr;
}