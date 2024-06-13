#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

// Linear search function
int linearSearch(const std::vector<int>& arr, int x) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == x) {
            return i;
        }
    }
    return -1;
}

// Function to measure the time taken by linear search
double measureTime(const std::vector<int>& arr, int x) {
    auto start = std::chrono::high_resolution_clock::now();
    linearSearch(arr, x);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

int main() {
    std::ofstream resultsFile("linear_search_times.csv");
    if (!resultsFile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return 1;
    }
    resultsFile << "Size,Best Case,Average Case,Worst Case\n";

    std::vector<int> sizes = {10, 100, 1000, 5000, 10000, 50000, 100000};

    for (int size : sizes) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }

        // Best case: element is at the first position
        double bestTime = measureTime(arr, arr[0]);

        // Average case: element is in the middle
        double averageTime = measureTime(arr, arr[size / 2]);

        // Worst case: element is at the last position or not present
        double worstTime = measureTime(arr, -1);

        // Write results to file
        resultsFile << size << "," << bestTime << "," << averageTime << "," << worstTime << "\n";

        std::cout << "Size: " << size 
                  << " Best Case: " << bestTime << " microseconds"
                  << " Average Case: " << averageTime << " microseconds"
                  << " Worst Case: " << worstTime << " microseconds\n";
    }

    resultsFile.close();
    return 0;
}
