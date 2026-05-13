#ifndef DEFINICOES_H  
#define DEFINICOES_H
#include "raylib.h"
#include <stdbool.h>

//Struct jogador

typedef struct {
    float x; //Posição x do mario
    float y; //Posição x do mario
    int tamanho; //tamanho (por enquanto) do mario
    float velocidadeY; //força da gravidade
    bool noChao; //ele esta no chao ou nao?
    bool ativo; //ta vivo?
} Jogador;

//Definições

#define LARGURA_TELA 900
#define ALTURA_TELA 900
#define VELOCIDADE 4.0f
#define TAMANHO_JOGADOR 20
#define GRAVIDADE 0.5f   
#define FORCA_PULO -7.0f  
#define LINHA_DO_CHAO 500  
#define VELOCIDADE_ESCADA 30.0f

//Definições do mapa

#define LINHAS 30
#define COLUNAS 30
#define TAMANHO_BLOCOS 30

typedef struct {
    char grid[LINHAS][COLUNAS];
} Mapa;

//Funções do jogador

void movimento(Jogador *p, Mapa *mapa);

//Funções do Mapa

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo);
void DesenharMapa(Mapa *mapa);


#endif
