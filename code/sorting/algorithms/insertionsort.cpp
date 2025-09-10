#include <vector>

// offentlig API som sorting.cpp lenker mot (IKKE static, IKKE namespace)
void insertion_sort(std::vector<int>& arr){
    if (arr.size() <= 1) return;

    for (std::size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            --j;
        }
        arr[j+1] = key;
    }
}
