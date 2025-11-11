#ifndef MEMORIA_H
#define MEMORIA_H

typedef struct {
    char nome[50];
    int valorAfeto;
    int coordX;
    int coordY;
    int encontrada; // 0 = escondida, 1 = revelada
} Memoria;

void criarMemorias(Memoria vetor[], int n);
void desenharMemorias(Memoria vetor[], int n);
void verificarOrdem(Memoria vetor[], int n);
void atualizarTela();
void executarJogo();

#endif
