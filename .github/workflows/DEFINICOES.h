#ifndef DEFINICOES_H  
#define DEFINICOES_H
#include "raylib.h"
#include <stdbool.h>
#include "corpoFisico.h"

    //Struct jogador
typedef struct {
    CorpoFisico corpoFisico;
    float aceleracao; // Taxa em que ele aumenta a velocidade
    float forcaPulo; // Força em que ele pula
    int tamanho; //tamanho (por enquanto) do mario
    bool noChao; //ele esta no chao ou nao?
    bool ativo; //ta vivo?
    int vidas;
} Jogador;

typedef struct {
    float x;
    float y;
    int direcao;
    bool ativo;
    int numrand;
} Opps;

typedef enum {
    MENU,       
    JOGANDO,   
    PAUSADO,    
    MORTO,  
    CONFIGS,
    RANKING,
    VITORIA
} EstadosJogo;

typedef enum{
    FACIL,
    NORMAL,
    DIFICIL
} Dificuldade;

//Definições

#define LARGURA_TELA 900
#define ALTURA_TELA 900
#define VELOCIDADE 4.0f
#define TAMANHO_JOGADOR 20
#define GRAVIDADE 0.5f   
#define FORCA_PULO -20.0f  
#define LINHA_DO_CHAO 500  
#define VELOCIDADE_ESCADA 30.0f
#define VELOCIDADE_OPPS 3
#define VELOCIDADE_OPPS_DIFICIL 6
#define MAX_INIMIGOS 40

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

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo* estado);
void DesenharMapa(Mapa *mapa);
void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos, float *invencivel, Dificuldade* dificuldade, int* numrandom);
void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos, Dificuldade *dificuldade, EstadosJogo* estado);
void proximafase(Mapa *mapa, Jogador *jog, Opps *inimigos, int *numInimigos, int *faseAtual, char *numMapa, EstadosJogo* estado);
void andar(Jogador *p);
void pular(Jogador* p);
void mover(Jogador *p, Mapa* mapa);
void zerarJogador(Jogador* j);
void subirEscada(CorpoFisico *cf, Mapa *mapa);
void inserirRanking(PLACAR rank[10], char *nome, float tempo);
void salvarRanking(PLACAR rank[10]);
void carregarRanking(PLACAR rank[10]);

#endif



