// code/sorting/sorting.cpp
#include <bits/stdc++.h>
using namespace std;

// --- algorithm declarations (provided in your separate .cpp files) ---
void insertion_sort(vector<int>& a);
void merge_sort_vec(vector<int>& a);
void quick_sort_vec(vector<int>& a);
void panda_sort(vector<int>& a);

// --- choose algorithm by flag or executable name ---
using AlgoFn = void(*)(vector<int>&);
struct Algo { string name; AlgoFn fn; };

static Algo pick_algo(const string& selector) {
    string s = selector;
    for (auto& c : s) c = (char)tolower(c);
    if (s.find("insertion") != string::npos) return {"insertion", insertion_sort};
    if (s.find("merge")     != string::npos) return {"merge",     merge_sort_vec};
    if (s.find("quick")     != string::npos) return {"quick",     quick_sort_vec};
    if (s.find("panda")     != string::npos) return {"panda",     panda_sort};
    if (s.find("stdsort")   != string::npos || s.find("std") != string::npos) {
        return {"stdsort", [](vector<int>& a){ sort(a.begin(), a.end()); }};
    }
    // default
    return {"quick", quick_sort_vec};
}

// --- memory (Linux). On Windows returns -1 ---
static long get_memory_kb() {
#if defined(_WIN32)
    return -1;
#else
    ifstream f("/proc/self/status");
    string key; long val; string unit;
    while (f >> key) {
        if (key == "VmRSS:") { f >> val >> unit; return val; }
        f.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return -1;
#endif
}

// --- IO helpers ---
static vector<int> read_stdin_array() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    size_t n;
    if (!(cin >> n)) return {};
    vector<int> a(n);
    for (size_t i = 0; i < n; ++i) cin >> a[i];
    return a;
}
static void write_array_to_file(const vector<int>& a, const string& path) {
    filesystem::create_directories(filesystem::path(path).parent_path());
    ofstream out(path);
    for (size_t i = 0; i < a.size(); ++i) {
        if (i) out << ' ';
        out << a[i];
    }
    out << '\n';
}

int main(int argc, char** argv) {
    // 1) choose algorithm
    string flag;
    for (int i = 1; i < argc; ++i) {
        string s = argv[i];
        if (s.rfind("--algo=", 0) == 0) flag = s.substr(7);
    }
    Algo algo = flag.empty() ? pick_algo(argv[0]) : pick_algo(flag);

    // 2) read input
    vector<int> a = read_stdin_array();
    if (a.empty()) { cerr << "No input on stdin\n"; return 1; }
    const size_t n = a.size();

    // 3) measure + sort
    (void)get_memory_kb(); // prime
    auto t0 = chrono::high_resolution_clock::now();
    algo.fn(a);
    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();
    long mem_kb = get_memory_kb();

    // 4) write outputs
    const string out_path = "./data/array_output/" + algo.name + "_" + to_string(n) + ".txt"; // ✱
    write_array_to_file(a, out_path);

    filesystem::create_directories("./data/measurements"); // ✱
    ofstream m("./data/measurements/" + algo.name + ".txt", ios::app); // ✱
    m << n << " " << ms << " " << mem_kb << "\n";


    cerr << "[" << algo.name << "] n=" << n << " -> " << ms << " ms, mem=" << mem_kb
         << " kB; output: " << out_path << "\n";
    return 0;
}
