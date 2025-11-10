#include "algoritmos.h"
#include <stdio.h>

// Tipos: 1 = Bubble, 2 = Selection, 3 = Insertion, 4 = Quick, 5 = Merge

void bubbleSort(Memoria vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (vetor[j].valorAfeto > vetor[j + 1].valorAfeto) {
                Memoria temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void selectionSort(Memoria vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        int min = i;
        for (int j = i + 1; j < tamanho; j++) {
            if (vetor[j].valorAfeto < vetor[min].valorAfeto)
                min = j;
        }
        Memoria temp = vetor[i];
        vetor[i] = vetor[min];
        vetor[min] = temp;
    }
}

void insertionSort(Memoria vetor[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        Memoria key = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j].valorAfeto > key.valorAfeto) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = key;
    }
}

void ordenarMemorias(Memoria vetor[], int tamanho, int tipo) {
    switch (tipo) {
        case 1: bubbleSort(vetor, tamanho); break;
        case 2: selectionSort(vetor, tamanho); break;
        case 3: insertionSort(vetor, tamanho); break;
        default: bubbleSort(vetor, tamanho); break;
    }
}
