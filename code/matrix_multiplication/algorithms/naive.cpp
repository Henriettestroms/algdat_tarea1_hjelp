#include <vector>

// ✱ ENDRING: gjør implementasjonen intern for å unngå kollisjon
static std::vector<std::vector<int>> multiply_impl(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B)
{
    std::size_t n = A.size();
    std::size_t m = B.empty() ? 0 : B[0].size();
    std::size_t p = B.size();

    std::vector<std::vector<int>> C(n, std::vector<int>(m, 0));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t k = 0; k < p; ++k)
            for (std::size_t j = 0; j < m; ++j)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// ✱ ENDRING: offentlig wrapper som driveren kaller
std::vector<std::vector<int>> multiply_naive(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B)
{
    return multiply_impl(A,B);
}