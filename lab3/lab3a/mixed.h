#pragma once

#include "quik.h"

using namespace std;

void hybridSort(double arr[], int low, int high, bool show_steps = false, int n = 0) {

    const int THRESHOLD = 10;



    if (high - low + 1 <= THRESHOLD) {

        for (int i = low + 1; i <= high; ++i) {

            double key = arr[i];

            int j = i - 1;

            while (j >= low && arr[j] > key) {

                arr[j + 1] = arr[j];

                j--;

            }

            arr[j + 1] = key;

        }

        if (show_steps) {

            cout << "Hybrid (Insertion for " << high - low + 1 << " elements): ";

            for (int k = 0; k < n; k++) cout << arr[k] << " ";

            cout << endl;

        }
    }
}