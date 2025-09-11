#include <bits/stdc++.h>
using namespace std;

// ✱ ENDRING: bruk wrappers fra algoritmene
std::vector<std::vector<int>> multiply_naive(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&);
std::vector<std::vector<int>> multiply_strassen(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&);

using AlgoFn = std::vector<std::vector<int>>(*)(const std::vector<std::vector<int>>&, const std::vector<std::vector<int>>&);
struct Algo { string name; AlgoFn fn; };

static Algo pick_algo(const string& selector) {
    string s = selector; for (auto& c : s) c = (char)tolower(c);
    if (s.find("naive")    != string::npos) return {"naive",    multiply_naive};
    if (s.find("strassen") != string::npos) return {"strassen", multiply_strassen};
    return {"naive", multiply_naive};
}

static bool read_stdin_matrices(vector<vector<int>>& A, vector<vector<int>>& B) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    size_t n;
    if (!(cin >> n)) return false;
    A.assign(n, vector<int>(n));
    B.assign(n, vector<int>(n));
    for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < n; ++j) cin >> A[i][j];
    for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < n; ++j) cin >> B[i][j];
    return true;
}

static void write_matrix_to_file(const vector<vector<int>>& C, const string& path) {
    filesystem::create_directories(filesystem::path(path).parent_path());
    ofstream out(path);
    for (size_t i = 0; i < C.size(); ++i) {
        for (size_t j = 0; j < C[i].size(); ++j) {
            if (j) out << ' ';
            out << C[i][j];
        }
        out << '\n';
    }
}

int main(int argc, char** argv) {
    // 1) velg algoritme via --algo=
    string flag;
    for (int i = 1; i < argc; ++i) {
        string s = argv[i];
        if (s.rfind("--algo=", 0) == 0) flag = s.substr(7);
    }
    Algo algo = flag.empty() ? pick_algo(argv[0]) : pick_algo(flag);

    // 2) les input (n + A + B) fra stdin
    vector<vector<int>> A, B;
    if (!read_stdin_matrices(A, B)) {
        cerr << "No/invalid input on stdin (expected: n, then A, then B)\n";
        return 1;
    }
    const size_t n = A.size();

    // 3) mål tid + kjør
    auto t0 = chrono::high_resolution_clock::now();
    vector<vector<int>> C = algo.fn(A, B);
    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();
    long mem_kb = -1; // Windows

    // 4) skriv ut
    const string out_path = "./data/matrix_output/matrix_" + algo.name + "_" + to_string(n) + ".txt";
    write_matrix_to_file(C, out_path);

    filesystem::create_directories("./data/measurements");
    ofstream m("./data/measurements/matrix_" + algo.name + ".txt", ios::app);
    m << n << " " << ms << " " << mem_kb << "\n";

    cerr << "[matrix:" << algo.name << "] n=" << n << " -> " << ms << " ms, mem=" << mem_kb
         << " kB; output: " << out_path << "\n";
    return 0;
}