#pragma once
#include<iostream>
#include<vector>

void merge(double arr[], int low, int mid, int high, bool show_steps = false, int n = 0) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    double* left = new double[n1];
    double* right = new double[n2];

    for (int i = 0; i < n1; i++) left[i] = arr[low + i];
    for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
    }

    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];

    delete[] left;
    delete[] right;

    if (show_steps) {
        std::cout << "Merge step: ";
        for (int m = 0; m < n; m++) std::cout << arr[m] << " ";
        std::cout << std::endl;
    }
}

void mergeSort(double arr[], int low, int high, bool show_steps = false, int n = 0) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSort(arr, low, mid, show_steps, n);
        mergeSort(arr, mid + 1, high, show_steps, n);
        merge(arr, low, mid, high, show_steps, n);
    }
}