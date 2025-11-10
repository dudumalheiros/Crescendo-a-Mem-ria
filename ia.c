#include "ia.h"
#include <stdlib.h>
#include <time.h>

// IA representa o "Esquecimento": cria o pior caso para o próximo desafio
void acaoIA(Memoria vetor[], int tamanho, int algoritmoUsado) {
    srand(time(NULL));

    if (algoritmoUsado == 1) { 
        // Bubble Sort -> cria vetor em ordem reversa
        for (int i = 0; i < tamanho / 2; i++) {
            Memoria temp = vetor[i];
            vetor[i] = vetor[tamanho - 1 - i];
            vetor[tamanho - 1 - i] = temp;
        }
    } else if (algoritmoUsado == 2) {
        // Selection Sort -> embaralha fortemente
        for (int i = 0; i < tamanho; i++) {
            int j = rand() % tamanho;
            Memoria temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
        }
    } else {
        // Outros algoritmos -> cria pequenos blocos desordenados
        for (int i = 0; i < tamanho - 2; i += 3) {
            Memoria temp = vetor[i];
            vetor[i] = vetor[i + 1];
            vetor[i + 1] = temp;
        }
    }
}
