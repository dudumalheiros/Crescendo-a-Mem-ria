#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "memoria.h"

void bubbleSort(Memoria vetor[], int n);
void selectionSort(Memoria vetor[], int n);
void quickSort(Memoria vetor[], int inicio, int fim);
void ordenarMemorias(Memoria vetor[], int n, int tipoAlgoritmo);

#endif
