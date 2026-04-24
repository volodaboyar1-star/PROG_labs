#include <iostream>
#include <iomanip>
#include <chrono>   // For time measurement
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include "quik.h"
#include "marge.h"
#include "insertion.h"
#include "mixed.h"

using namespace std;
using namespace std::chrono;

// Helper function to print the array
void printArray(double arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Helper function to copy the array
void copyArray(double source[], double destination[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}

// Function to generate an array of random double numbers (from 0 to 100)
void generateRandomArray(double arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = (rand() % 10000) / 100.0;
    }
}

// ---------------- DEMONSTRATION MODE ----------------
void demoMode() {
    int n;
    cout << "\n=== Demonstration Mode ===" << endl;
    cout << "Enter array size (recommended 10-20): ";
    cin >> n;

    if (n <= 0) {
        cout << "Error: array size must be greater than 0." << endl;
        return;
    }

    double* originalArr = new double[n];
    double* arr = new double[n];

    generateRandomArray(originalArr, n);

    cout << "\nInitial generated array:" << endl;
    printArray(originalArr, n);
    cout << "----------------------------------------\n";

    bool show = true;

    cout << "1. Quick Sort:" << endl;
    copyArray(originalArr, arr, n);
    quickSort(arr, 0, n - 1, show, n);
    cout << "Result: "; printArray(arr, n);
    cout << "----------------------------------------\n";

    cout << "2. Merge Sort:" << endl;
    copyArray(originalArr, arr, n);
    mergeSort(arr, 0, n - 1, show, n);
    cout << "Result: "; printArray(arr, n);
    cout << "----------------------------------------\n";

    cout << "3. Insertion Sort (without step-by-step output):" << endl;
    copyArray(originalArr, arr, n);
    insertionSort(arr, n);
    cout << "Result: "; printArray(arr, n);
    cout << "----------------------------------------\n";

    cout << "4. Hybrid Sort (Quick + Insertion):" << endl;
    copyArray(originalArr, arr, n);
    hybridSort(arr, 0, n - 1, show, n);
    cout << "Result: "; printArray(arr, n);
    cout << "----------------------------------------\n";

    delete[] originalArr;
    delete[] arr;
}

// ---------------- AUTOMATIC MODE (BENCHMARK) ----------------
void benchmarkMode() {
    int n;
    cout << "\n=== Automatic Mode (Benchmark) ===" << endl;
    cout << "Enter array size (recommended 10000+): ";
    cin >> n;

    if (n <= 0) {
        cout << "Error: array size must be greater than 0." << endl;
        return;
    }

    double* originalArr = new double[n];
    double* arr = new double[n];

    cout << "Generating array of " << n << " elements..." << endl;
    generateRandomArray(originalArr, n);

    bool show = false; // Disable step output for accurate time measurement

    cout << "\nStarting benchmark...\n" << endl;

    // 1. Quick Sort
    copyArray(originalArr, arr, n);
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, n - 1, show, n);
    auto stop = high_resolution_clock::now();
    auto durationQuick = duration_cast<milliseconds>(stop - start);
    cout << left << setw(20) << "Quick Sort:" << durationQuick.count() << " ms" << endl;

    // 2. Merge Sort
    copyArray(originalArr, arr, n);
    start = high_resolution_clock::now();
    mergeSort(arr, 0, n - 1, show, n);
    stop = high_resolution_clock::now();
    auto durationMerge = duration_cast<milliseconds>(stop - start);
    cout << left << setw(20) << "Merge Sort:" << durationMerge.count() << " ms" << endl;

    // 3. Insertion Sort
    copyArray(originalArr, arr, n);
    start = high_resolution_clock::now();
    insertionSort(arr, n);
    stop = high_resolution_clock::now();
    auto durationInsertion = duration_cast<milliseconds>(stop - start);
    cout << left << setw(20) << "Insertion Sort:" << durationInsertion.count() << " ms" << endl;

    // 4. Hybrid Sort
    copyArray(originalArr, arr, n);
    start = high_resolution_clock::now();
    hybridSort(arr, 0, n - 1, show, n);
    stop = high_resolution_clock::now();
    auto durationHybrid = duration_cast<milliseconds>(stop - start);
    cout << left << setw(20) << "Hybrid Sort:" << durationHybrid.count() << " ms" << endl;

    delete[] originalArr;
    delete[] arr;
}

// ---------------- MAIN MENU ----------------
int main() {
    srand(static_cast<unsigned>(time(0))); // Initialize random number generator

    int choice;
    do {
        cout << "\n===================================" << endl;
        cout << "           TESTING MENU" << endl;
        cout << "===================================" << endl;
        cout << "1. Demonstration Mode" << endl;
        cout << "2. Automatic Mode (Benchmark)" << endl;
        cout << "0. Exit" << endl;
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            demoMode();
            break;
        case 2:
            benchmarkMode();
            break;
        case 0:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}