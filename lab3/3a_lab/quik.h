#pragma once
#include<iostream>
using namespace std;
int partition(double arr[], int low, int high, bool show_steps = false, int n = 0) {
    double pivot = arr[low + (high - low) / 2];
    int i = low - 1;
    int j = high + 1;

    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);

        if (i >= j) return j;

        std::swap(arr[i], arr[j]);

        if (show_steps) {
            std::cout << "Swap (" << arr[i] << " <-> " << arr[j] << "): ";
            for (int k = 0; k < n; k++) std::cout << arr[k] << " ";
            std::cout << std::endl;
        }
    }
}

void quickSort(double arr[], int low, int high, bool show_steps = false, int n = 0) {
    if (low < high) {
        int p = partition(arr, low, high, show_steps, n);
        quickSort(arr, low, p, show_steps, n);
        quickSort(arr, p + 1, high, show_steps, n);
    }
}