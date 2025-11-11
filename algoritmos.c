#include "algoritmos.h"

// Tipos: 1 = Bubble, 2 = Selection, 3 = Quick

void bubbleSort(Memoria vetor[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (vetor[j].valorAfeto > vetor[j + 1].valorAfeto) {
                Memoria temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void selectionSort(Memoria vetor[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (vetor[j].valorAfeto < vetor[min].valorAfeto)
                min = j;
        }
        Memoria temp = vetor[i];
        vetor[i] = vetor[min];
        vetor[min] = temp;
    }
}

void quickSort(Memoria vetor[], int inicio, int fim) {
    if (inicio < fim) {
        int pivo = vetor[fim].valorAfeto;
        int i = inicio - 1;
        for (int j = inicio; j < fim; j++) {
            if (vetor[j].valorAfeto < pivo) {
                i++;
                Memoria temp = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = temp;
            }
        }
        Memoria temp = vetor[i + 1];
        vetor[i + 1] = vetor[fim];
        vetor[fim] = temp;
        int p = i + 1;
        quickSort(vetor, inicio, p - 1);
        quickSort(vetor, p + 1, fim);
    }
}

void ordenarMemorias(Memoria vetor[], int n, int tipoAlgoritmo) {
    switch (tipoAlgoritmo) {
        case 1: bubbleSort(vetor, n); break;
        case 2: selectionSort(vetor, n); break;
        case 3: quickSort(vetor, 0, n - 1); break;
        default: bubbleSort(vetor, n); break;
    }
}
