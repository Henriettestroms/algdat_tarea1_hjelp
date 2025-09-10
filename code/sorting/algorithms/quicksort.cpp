#include <vector>
#include <random>
#include <algorithm>
using namespace std;

static int partition(vector<int>& A, int p, int r) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(p, r);
    int pivotIndex = dist(gen);
    int pivot = A[pivotIndex];
    swap(A[pivotIndex], A[r]);

    int i = p - 1;
    for (int j = p; j < r; ++j) {
        if (A[j] <= pivot) {
            ++i;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);
    return i + 1;
}

static void quickSortRec(vector<int>& A, int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        quickSortRec(A, p, q - 1);
        quickSortRec(A, q + 1, r);
    }
}

// offentlig API (IKKE static)
void quick_sort_vec(vector<int>& A) {
    if (A.empty()) return;
    quickSortRec(A, 0, static_cast<int>(A.size()) - 1);
}
