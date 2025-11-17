// algoritmos.h
#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "memoria.h"

// 1 = Bubble, 2 = Selection, 3 = Quick
void bubbleSort   (Memoria vetor[], int n);
void selectionSort(Memoria vetor[], int n);
void quickSort    (Memoria vetor[], int inicio, int fim);

// escolhe o algoritmo de acordo com o tipoAlgoritmo
void ordenarMemorias(Memoria vetor[], int n, int tipoAlgoritmo);

#endif
