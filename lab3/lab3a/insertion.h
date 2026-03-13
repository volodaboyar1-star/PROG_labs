#pragma once

void insertionSort(double arr[], int n)

{

	for (int i = 1; i < n; ++i) {

		double key = arr[i];

		int j = i - 1;

		while (j >= 0 && arr[j] > key) {

			arr[j + 1] = arr[j];

			j--;

		}

		arr[j + 1] = key;

	}

}