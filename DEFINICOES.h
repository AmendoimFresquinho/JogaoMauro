#ifndef DEFINICOES_H  
#define DEFINICOES_H
#include "raylib.h"
#include <stdbool.h>
#include "corpoFisico.h"
#include "inimigos.h"

//Definições

#define LARGURA_TELA 900
#define ALTURA_TELA 900
#define VELOCIDADE 4.0f
#define TAMANHO_JOGADOR 20
#define FORCA_PULO -20.0f  //-15.0f pode ser melhor (testa aí)
#define MAX_INIMIGOS 40

//Definições do mapa

#define LINHAS 30
#define COLUNAS 30
#define TAMANHO_BLOCOS 30

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
    Fogo fogos[MAX_INIMIGOS];
    Torre torres[MAX_INIMIGOS];
    Glitch glitches[MAX_INIMIGOS];
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

typedef struct {
    char nome[30];
    float tempo;
} PLACAR;



typedef struct {
    char grid[LINHAS][COLUNAS];
} Mapa;

//Funções do jogador

void movimento(Jogador *p, Mapa *mapa, Sound fx);

//Funções do Mapa

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo* estado, double* tempoFinal, double* tempoInicio);
void DesenharMapa(Mapa *mapa);
void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos, float *invencivel, Dificuldade* dificuldade, int* numrandom, Sound fx4);
void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos, Dificuldade *dificuldade, EstadosJogo* estado, double* tempoFinal, double* tempoInicio);
void proximafase(Mapa *mapa, Jogador *jog, Opps *inimigos, int *numInimigos, int *faseAtual, char *numMapa, EstadosJogo* estado, double *tempoFinal, double *tempoInicio, Sound fx5, Dificuldade *dificuldade);
void andar(Jogador *p);
void pular(Jogador* p, Sound fx);
void mover(Jogador *p, Mapa* mapa);
void zerarJogador(Jogador* j);
void subirEscada(CorpoFisico *cf, Mapa *mapa);
void inserirRanking(PLACAR rank[10], char *nome, float tempo);
void salvarRanking(PLACAR rank[10]);
void carregarRanking(PLACAR rank[10]);
void caseMenu(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound fx2, Sound fx3, Sound fx6);
void caseJogando(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, float *tempoInvencivel, int *faseAtual, char *numMapa, double *tempoInicio, double *tempoFinal, int *numrandom, Sound fx, Sound fx4, Sound fx5, Sound fx6);
void casePausado(EstadosJogo *estado, Sound fx2, Sound fx6);
void caseMorto(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound fx2, Sound fx3local);
void caseConfigs(EstadosJogo *estado, Dificuldade *dificuldade, Color *cortexto1, Color *cortexto2, Color *cortexto3, Sound fx2);
void caseRanking(EstadosJogo *estado, PLACAR rank[10]);
void caseVitoria(EstadosJogo *estado, PLACAR rank[10], double *tempoFinal, char *nomeJogador, int *letraNome, Sound fx6);
void powerUp(Jogador *j, Mapa *mapa, Sound fx7);


typedef struct {
    Texture2D source; // Spritesheet
    int lar, alt; // Largura e altura de um frame
    int atual, totframes; // Frame atual e total de frames
    float delay, ultimo; // Delay entre cada frame e o frame da ultima animada
} Anim;

void proxFrame(Anim* anim);

#endif



