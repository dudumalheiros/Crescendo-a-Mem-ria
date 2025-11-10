#include "memoria.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

static const char* NOMES[] = {
    "Casa da Vovo", "Bicicleta Azul", "Primeiro Dia na Escola", "O Gato de Rua",
    "Aniversario 10", "Noite das Estrelas", "Primeiro Amigo", "Brincar na Chuva",
    "Viagem ao Campo", "Jogo no Patio", "Passaro Ferido", "Album Antigo",
    "Sorvete de Morango", "Cinema com Pais", "Cartinha de Natal", "Tesouro Afetivo"
};

static void geraAleatorio(Memoria v[], int n, int minAfeto, int maxAfeto) {
    for (int i = 0; i < n; i++) {
        int idx = i % (int)(sizeof(NOMES)/sizeof(NOMES[0]));
        strncpy(v[i].nome, NOMES[idx], sizeof(v[i].nome)-1);
        v[i].nome[sizeof(v[i].nome)-1] = '\0';
        v[i].valorAfeto = minAfeto + rand() % (maxAfeto - minAfeto + 1);
        v[i].coord.x = rand()%6; v[i].coord.y = rand()%6;
    }
}

// cria padroes diferentes por fase (para didatica)
void carregarFase(int fase, Memoria vetor[], int *n) {
    srand((unsigned)time(NULL));
    int tam = 12; // tamanho padrao
    *n = tam;

    // base aleatoria
    geraAleatorio(vetor, tam, 5, 95);

    // molda o vetor conforme a fase (para ilustrar casos melhores/piores)
    switch (fase) {
        case 1: // Bubble: bagunçado
            // já está aleatório
            break;
        case 2: // Selection: alguns valores repetidos/espalhados
            for (int i = 0; i < tam; i += 3) vetor[i].valorAfeto = 10 + (i*7)%90;
            break;
        case 3: // Insertion: quase ordenado
            for (int i = 0; i < tam; ++i) vetor[i].valorAfeto = 10 + i*5;
            // injeta pequenas desordens
            if (tam >= 4) {
                int a = rand()%tam, b = rand()%tam;
                int t = vetor[a].valorAfeto; vetor[a].valorAfeto = vetor[b].valorAfeto; vetor[b].valorAfeto = t;
            }
            break;
        case 4: // Quick: bem bagunçado
            for (int i = 0; i < tam; ++i) vetor[i].valorAfeto = rand()%100;
            break;
        case 5: // Merge: intervalo cheio
            for (int i = 0; i < tam; ++i) vetor[i].valorAfeto = (i*7 + rand()%6) % 100;
            break;
        default:
            break;
    }
}
