#include <vector>
#include <cmath>

namespace {
    using Matrix = std::vector<std::vector<int>>;

    Matrix add(const Matrix& A, const Matrix& B) {
        std::size_t n = A.size();
        Matrix C(n, std::vector<int>(n));
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                C[i][j] = A[i][j] + B[i][j];
        return C;
    }

    Matrix sub(const Matrix& A, const Matrix& B) {
        std::size_t n = A.size();
        Matrix C(n, std::vector<int>(n));
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                C[i][j] = A[i][j] - B[i][j];
        return C;
    }

    Matrix strassenRec(const Matrix& A, const Matrix& B) {
        std::size_t n = A.size();
        if (n == 1) return {{A[0][0] * B[0][0]}};
        std::size_t k = n / 2;
        Matrix a11(k, std::vector<int>(k)), a12(k, std::vector<int>(k)),
               a21(k, std::vector<int>(k)), a22(k, std::vector<int>(k));
        Matrix b11(k, std::vector<int>(k)), b12(k, std::vector<int>(k)),
               b21(k, std::vector<int>(k)), b22(k, std::vector<int>(k));
        for (std::size_t i = 0; i < k; ++i) {
            for (std::size_t j = 0; j < k; ++j) {
                a11[i][j] = A[i][j];
                a12[i][j] = A[i][j + k];
                a21[i][j] = A[i + k][j];
                a22[i][j] = A[i + k][j + k];
                b11[i][j] = B[i][j];
                b12[i][j] = B[i][j + k];
                b21[i][j] = B[i + k][j];
                b22[i][j] = B[i + k][j + k];
            }
        }
        Matrix m1 = strassenRec(add(a11, a22), add(b11, b22));
        Matrix m2 = strassenRec(add(a21, a22), b11);
        Matrix m3 = strassenRec(a11, sub(b12, b22));
        Matrix m4 = strassenRec(a22, sub(b21, b11));
        Matrix m5 = strassenRec(add(a11, a12), b22);
        Matrix m6 = strassenRec(sub(a21, a11), add(b11, b12));
        Matrix m7 = strassenRec(sub(a12, a22), add(b21, b22));
        Matrix c11 = add(sub(add(m1, m4), m5), m7);
        Matrix c12 = add(m3, m5);
        Matrix c21 = add(m2, m4);
        Matrix c22 = add(sub(add(m1, m3), m2), m6);
        Matrix C(n, std::vector<int>(n));
        for (std::size_t i = 0; i < k; ++i) {
            for (std::size_t j = 0; j < k; ++j) {
                C[i][j] = c11[i][j];
                C[i][j + k] = c12[i][j];
                C[i + k][j] = c21[i][j];
                C[i + k][j + k] = c22[i][j];
            }
        }
        return C;
    }
}

std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& A,
                                       const std::vector<std::vector<int>>& B) {
    std::size_t n = A.size();
    std::size_t m = 1;
    while (m < n) m <<= 1; // next power of two
    Matrix A_pad(m, std::vector<int>(m, 0)), B_pad(m, std::vector<int>(m, 0));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j) {
            A_pad[i][j] = A[i][j];
            B_pad[i][j] = B[i][j];
        }
    Matrix C_pad = strassenRec(A_pad, B_pad);
    Matrix C(n, std::vector<int>(n));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            C[i][j] = C_pad[i][j];
    return C;
}