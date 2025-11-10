#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "memoria.h"
#include <stdbool.h>

void bubbleSort   (Memoria v[], int n);
void selectionSort(Memoria v[], int n);
void insertionSort(Memoria v[], int n);
void quickSort    (Memoria v[], int l, int r);
void mergeSort    (Memoria v[], int l, int r);

// util
bool isSorted(Memoria v[], int n);

#endif
