#include "algoritmos.h"
#include <string.h>
#include <stdlib.h>

static void swapM(Memoria *a, Memoria *b) {
    Memoria t = *a; *a = *b; *b = t;
}

void bubbleSort(Memoria v[], int n) {
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-1-i; ++j) {
            if (v[j].valorAfeto > v[j+1].valorAfeto) swapM(&v[j], &v[j+1]);
        }
    }
}

void selectionSort(Memoria v[], int n) {
    for (int i = 0; i < n-1; ++i) {
        int min = i;
        for (int j = i+1; j < n; ++j) {
            if (v[j].valorAfeto < v[min].valorAfeto) min = j;
        }
        if (min != i) swapM(&v[i], &v[min]);
    }
}

void insertionSort(Memoria v[], int n) {
    for (int i = 1; i < n; ++i) {
        Memoria key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j].valorAfeto > key.valorAfeto) {
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = key;
    }
}

// Quick Sort
static int partition(Memoria v[], int l, int r) {
    int pivot = v[r].valorAfeto; // pivot final
    int i = l - 1;
    for (int j = l; j < r; ++j) {
        if (v[j].valorAfeto <= pivot) {
            i++;
            swapM(&v[i], &v[j]);
        }
    }
    swapM(&v[i+1], &v[r]);
    return i + 1;
}

void quickSort(Memoria v[], int l, int r) {
    if (l < r) {
        int p = partition(v, l, r);
        quickSort(v, l, p - 1);
        quickSort(v, p + 1, r);
    }
}

// Merge Sort
static void merge(Memoria v[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    Memoria *L = (Memoria*)malloc(n1 * sizeof(Memoria));
    Memoria *R = (Memoria*)malloc(n2 * sizeof(Memoria));
    for (int i = 0; i < n1; ++i) L[i] = v[l + i];
    for (int j = 0; j < n2; ++j) R[j] = v[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].valorAfeto <= R[j].valorAfeto) v[k++] = L[i++];
        else v[k++] = R[j++];
    }
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];

    free(L); free(R);
}

void mergeSort(Memoria v[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(v, l, m);
        mergeSort(v, m + 1, r);
        merge(v, l, m, r);
    }
}

bool isSorted(Memoria v[], int n) {
    for (int i = 1; i < n; ++i)
        if (v[i-1].valorAfeto > v[i].valorAfeto) return false;
    return true;
}
