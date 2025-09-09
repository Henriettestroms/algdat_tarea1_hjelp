CXX = g++
CXXFLAGS = -std=c++17 -O2

SORTING_SRC = sorting.cpp
MM_SRC = matrix_multiplication.cpp

sort_std: $(SORTING_SRC) sort.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

sort_merge: $(SORTING_SRC) mergesort.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

sort_quick: $(SORTING_SRC) quicksort.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

sort_insertion: $(SORTING_SRC) insertionsort.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

sort_panda: $(SORTING_SRC) pandasort.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

mm_naive: $(MM_SRC) naive.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

mm_strassen: $(MM_SRC) strassen.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f sort_std sort_merge sort_quick sort_insertion sort_panda mm_naive mm_strassen