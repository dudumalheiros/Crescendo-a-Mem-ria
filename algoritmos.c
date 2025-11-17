// algoritmos.c
#include "algoritmos.h"

// ----------- BUBBLE SORT -----------
void bubbleSort(Memoria vetor[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (vetor[j].valorAfeto > vetor[j + 1].valorAfeto) {
                Memoria tmp  = vetor[j];
                vetor[j]     = vetor[j + 1];
                vetor[j + 1] = tmp;
            }
        }
    }
}

// ----------- SELECTION SORT -----------
void selectionSort(Memoria vetor[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (vetor[j].valorAfeto < vetor[min].valorAfeto) {
                min = j;
            }
        }
        if (min != i) {
            Memoria tmp = vetor[i];
            vetor[i]    = vetor[min];
            vetor[min]  = tmp;
        }
    }
}

// ----------- QUICK SORT -----------
static void quickSortInterno(Memoria vetor[], int inicio, int fim) {
    if (inicio >= fim) return;

    int i = inicio;
    int j = fim;
    int pivo = vetor[(inicio + fim) / 2].valorAfeto;

    while (i <= j) {
        while (vetor[i].valorAfeto < pivo) i++;
        while (vetor[j].valorAfeto > pivo) j--;

        if (i <= j) {
            Memoria tmp = vetor[i];
            vetor[i]    = vetor[j];
            vetor[j]    = tmp;
            i++;
            j--;
        }
    }

    if (inicio < j) quickSortInterno(vetor, inicio, j);
    if (i < fim)    quickSortInterno(vetor, i, fim);
}

void quickSort(Memoria vetor[], int inicio, int fim) {
    quickSortInterno(vetor, inicio, fim);
}

// ----------- DESPACHO GERAL -----------
void ordenarMemorias(Memoria vetor[], int n, int tipoAlgoritmo) {
    switch (tipoAlgoritmo) {
        case 1:
            bubbleSort(vetor, n);
            break;
        case 2:
            selectionSort(vetor, n);
            break;
        case 3:
        default:
            quickSort(vetor, 0, n - 1);
            break;
    }
}
