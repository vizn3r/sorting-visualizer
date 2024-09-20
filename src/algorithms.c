#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

#ifdef BUBBLE
void sort(int *arr, int n) {
  int i, j, temp;
  for (i = 0; i < n - 1; i++) {
    // Last i elements are already in place
    for (j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        // Swap arr[j] and arr[j+1]
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
        update(arr);
      }
    }
  }
}
#endif
#ifdef INSERTION
void sort(int *arr, int n) {
  for (int i = 1; i < n; ++i) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      update(arr);
      j = j - 1;
    }
    arr[j + 1] = key;
    update(arr);
  }
}
#endif
#ifdef MERGE
void merge(int arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  int L[n1], R[n2];
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];
  i = 0;
  j = 0;
  k = l;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    update(arr);
    k++;
  }
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
    update(arr);
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
    update(arr);
  }
}
void mergeSort(int arr[], int l, int r) {
  if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}
void sort(int *arr, int n) {
  mergeSort(arr, 0, n - 1);
  update(arr);
}
#endif
#ifdef QUICK
int partition(int arr[], int low, int high) {
  int pivot = arr[high];
  int i = low - 1;
  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivot) {
      i++;
      swap(&arr[i], &arr[j]);
      update(arr);
    }
  }

  swap(&arr[i + 1], &arr[high]);
  update(arr);
  return i + 1;
}
void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}
void sort(int *arr, int n) {
  quickSort(arr, 0, n - 1);
  update(arr);
}
#endif
#ifdef HEAP
void heapify(int arr[], int n, int i) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && arr[left] > arr[largest])
    largest = left;
  if (right < n && arr[right] > arr[largest])
    largest = right;
  if (largest != i) {
    swap(&arr[i], &arr[largest]);
    update(arr);
    heapify(arr, n, largest);
  }
}
void sort(int *arr, int n) {
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  for (int i = n - 1; i >= 0; i--) {
    swap(&arr[0], &arr[i]);
    update(arr);
    heapify(arr, i, 0);
  }
  update(arr);
}
#endif
#ifdef COUNTING
void sort(int *arr, int n) {
  int M = 0;
  for (int i = 0; i < n; i++)
    if (arr[i] > M) {
      M = arr[i];
      update(arr);
    }
  int *countArray = (int *)calloc(M + 1, sizeof(int));
  for (int i = 0; i < n; i++)
    countArray[arr[i]]++;
  for (int i = 1; i <= M; i++)
    countArray[i] += countArray[i - 1];
  int *outputArray = (int *)malloc(n * sizeof(int));
  for (int i = n - 1; i >= 0; i--) {
    outputArray[countArray[arr[i]] - 1] = arr[i];
    countArray[arr[i]]--;
  }
  for (int i = 0; i < n; i++) {
    arr[i] = outputArray[i];
    update(arr);
  }
  free(countArray);
  free(outputArray);
  update(arr);
}
#endif
#ifdef RADIX
int getMax(int arr[], int n) {
  int mx = arr[0];
  for (int i = 1; i < n; i++)
    if (arr[i] > mx)
      mx = arr[i];
  return mx;
}
void countSort(int arr[], int n, int exp) {
  int output[n];       // Output array
  int count[10] = {0}; // Initialize count array as 0
  for (int i = 0; i < n; i++)
    count[(arr[i] / exp) % 10]++;
  for (int i = 1; i < 10; i++)
    count[i] += count[i - 1];
  for (int i = n - 1; i >= 0; i--) {
    output[count[(arr[i] / exp) % 10] - 1] = arr[i];
    count[(arr[i] / exp) % 10]--;
  }
  for (int i = 0; i < n; i++) {
    arr[i] = output[i];
    update(arr);
  }
}
void sort(int *arr, int n) {
  int m = getMax(arr, n);
  for (int exp = 1; m / exp > 0; exp *= 10)
    countSort(arr, n, exp);
  update(arr);
}
#endif
#ifdef SELECTION
void sort(int arr[], int n) {
  for (int step = 0; step < n - 1; step++) {
    int min_idx = step;
    for (int i = step + 1; i < n; i++) {
      if (arr[i] < arr[min_idx])
        min_idx = i;
    }
    swap(&arr[min_idx], &arr[step]);
    update(arr);
  }
  update(arr);
}
#endif
#ifdef BUCKET
void sort(int *arr, int n) { printf("NOT IMPLEMENTED\n\n"); }
#endif
#ifdef SHELL
void sort(int arr[], int n) {
  for (int interval = n / 2; interval > 0; interval /= 2) {
    for (int i = interval; i < n; i += 1) {
      int temp = arr[i];
      int j;
      for (j = i; j >= interval && arr[j - interval] > temp; j -= interval) {
        arr[j] = arr[j - interval];
        update(arr);
      }
      arr[j] = temp;
      update(arr);
    }
  }
}
#endif
#ifdef COMB
int getNextGap(int gap) {
  gap = (gap * 10) / 13;
  if (gap < 1)
    return 1;
  return gap;
}
void sort(int *arr, int n) {
  int gap = n;
  int swapped = 1;
  while (gap != 1 || swapped == 1) {
    gap = getNextGap(gap);
    swapped = 0;
    for (int i = 0; i < n - gap; i++) {
      if (arr[i] > arr[i + gap]) {
        swap(&arr[i], &arr[i + gap]);
        update(arr);
        swapped = 1;
      }
    }
  }
  update(arr);
}
#endif
