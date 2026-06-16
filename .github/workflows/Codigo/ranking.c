#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void carregarRanking(PLACAR rank[10]){
    FILE *f = fopen("placar.bin", "rb");
    if (f == NULL)
        return; // arquivo ainda não existe, tudo bem
    fread(rank, sizeof(PLACAR), 10, f);
    fclose(f);
}
void salvarRanking(PLACAR rank[10]){
    FILE *f = fopen("placar.bin", "wb");
    fwrite(rank, sizeof(PLACAR), 10, f);
    fclose(f);
}
void inserirRanking(PLACAR rank[10], char *nome, float tempo){
    // acha a posição correta (menor tempo = melhor)
    int pos = -1;
    for (int i = 0; i < 10; i++){
        if (rank[i].tempo == 0 || tempo < rank[i].tempo){
            pos = i;
            break;
        }
    }
    if (pos == -1)
        return; // não entrou no top 10

    // empurra os piores pra baixo
    for (int i = 9; i > pos; i--){
        rank[i] = rank[i - 1];}

    // insere na posição correta
    strncpy(rank[pos].nome, nome, 30);
    rank[pos].tempo = tempo;
    salvarRanking(rank);
}
void zerarRanking(PLACAR rank[10]) {
    memset(rank, 0, sizeof(PLACAR) * 10); // zera o array na memória
    salvarRanking(rank); // salva o array zerado no arquivo
}
