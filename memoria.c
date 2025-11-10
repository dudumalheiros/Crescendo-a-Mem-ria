#include "memoria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void trocarMemorias(Memoria *a, Memoria *b) {
    Memoria temp = *a;
    *a = *b;
    *b = temp;
}

void embaralharMemorias(Memoria vetor[], int tamanho) {
    srand(time(NULL));
    for (int i = 0; i < tamanho; i++) {
        int j = rand() % tamanho;
        trocarMemorias(&vetor[i], &vetor[j]);
    }
}

void inicializarMemorias(Memoria vetor[], int tamanho) {
    const char *nomes[] = {
        "Casa da Vovo", "Primeiro Dia de Aula", "O Gato de Rua",
        "Brincadeira na Chuva", "O Aniversario", "Viagem ao Campo",
        "Primeiro Amigo", "Noite das Estrelas", "A Bicicleta Azul",
        "O Tesouro Afetivo"
    };

    srand(time(NULL));
    for (int i = 0; i < tamanho; i++) {
        strcpy(vetor[i].nome, nomes[i % 10]);
        vetor[i].valorAfeto = rand() % 100;
        vetor[i].coordX = rand() % 600 + 100;
        vetor[i].coordY = rand() % 400 + 100;
    }
    embaralharMemorias(vetor, tamanho);
}

void mostrarMemorias(Memoria vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d - %s (Afeto: %d)\n", i + 1, vetor[i].nome, vetor[i].valorAfeto);
    }
}
