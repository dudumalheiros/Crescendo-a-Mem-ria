# 🎮 Crescendo na Memória – Pique-Esconde dos Algoritmos

**Crescendo na Memória** é um jogo educativo feito em **C com Raylib**, cujo objetivo é ensinar algoritmos de ordenação (Bubble Sort, Selection Sort e Quick Sort) de forma divertida e afetiva.

Você controla um personagem que explora labirintos e coleta memórias importantes da vida. Depois disso, deve **atribuir pesos afetivos** a cada memória, definindo o que é mais marcante para você. A partir desses pesos, o jogo usa um algoritmo de ordenação diferente em cada fase, mostrando como ele organiza suas escolhas.

---

## 📜 **História do Jogo**

Você está em uma viagem pelas suas lembranças mais marcantes:  
os primeiros momentos que moldaram quem você é hoje.

Ao longo de três fases, você encontra memórias espalhadas em labirintos — desde a primeira viagem com amigos até o primeiro filme de terror. Ao restaurá-las, você decide o **peso afetivo** de cada uma.

Então o jogo usa um algoritmo de ordenação para colocar suas memórias em ordem crescente de importância — mostrando, de maneira clara e visual, como cada método funciona.

---

## 🧠 **Algoritmos Usados por Fase**

### **🌿 Fase 1 — Bubble Sort**
O algoritmo mais simples: compara pares vizinhos e troca se estiverem fora de ordem.  
Ideal para introduzir o conceito de ordenação.

### **🌅 Fase 2 — Selection Sort**
Procura sempre o menor valor restante e o coloca na posição correta.  
Mais organizado, mas ainda percorre bastante o vetor.

### **🌌 Fase 3 — Quick Sort**
Escolhe um pivô e divide o vetor em menores e maiores que ele.  
Rápido, eficiente e muito usado na prática.

Ao final de cada fase, o jogador vê suas memórias **ordenadas** e uma **explicação amigável** do algoritmo usado.

---

## 🕹️ **Como Jogar**

- Mova o personagem com **as setas do teclado**  
- Toque uma memória e pressione **ESPAÇO** para restaurá-la  
- Após restaurar todas, você define o **peso afetivo**  
- O algoritmo da fase organiza as memórias com base no peso  
- Pressione **ENTER** para ver a explicação de cada algoritmo

---

## 🛠️ **Como Compilar o Jogo**

### Pré-requisitos
- **GCC** ou MinGW
- **Raylib instalada**
- Make (opcional)

### Compilação manual:
```bash
gcc main.c memoria.c algoritmos.c ia.c -lraylib -lwinmm -lgdi32 -o jogo.exe
