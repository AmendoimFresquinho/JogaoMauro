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

typedef struct {
    char nome[30];
    float tempo;
} PLACAR;

//Definições gerais

#define LARGURA_TELA 900
#define ALTURA_TELA 900
#define VELOCIDADE 4.0f
#define TAMANHO_JOGADOR 20
#define FORCA_PULO -15.0f  
#define MAX_INIMIGOS 40
#define TEMPO_INVENCIVEL 1.0f
#define ACELERACAO 5
#define MASSA 10

//Definições dificuldades

#define VELOCIDADE_INIMIGOS_FACIL 3.0f
#define VELOCIDADE_INIMIGOS_MEDIO 4.0f
#define VELOCIDADE_INIMIGOS_DIFICIL 6.0f
#define VELOCIDADE_INIMIGOS_LENTO 1.0f

#define VIDAS_FACIL 3
#define VIDAS_MEDIO 2
#define VIDAS_DIFICIL 1

//Definições do mapa

#define LINHAS 30
#define COLUNAS 30
#define TAMANHO_BLOCOS 30

//Definições Menu

#define X_MENU_TITULO 75
#define X_MENU_OPCOES 75
#define Y_OPCAO_JOGAR ALTURA_TELA - 550
#define Y_OPCAO_CONFIGS ALTURA_TELA - 475
#define Y_OPCAO_RANKING ALTURA_TELA - 400
#define Y_OPCAO_SAIR ALTURA_TELA - 325
#define Y_TITULO 75


typedef struct {
    char grid[LINHAS][COLUNAS];
} Mapa;

//Funções do jogador

void movimento(Jogador *p, Mapa *mapa, Sound fx);

//Funções do Mapa

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo *estado, double *tempoFinal, double *tempoInicio);
void DesenharMapa(Mapa *mapa, Texture2D groundTXT, Texture2D ladderTXT);
void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos, float *invencivel, Dificuldade* dificuldade, int* numrandom, Sound fx4);
void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos, Dificuldade *dificuldade, EstadosJogo *estados, double *tempoFinal, double *tempoInicio);
void proximafase(Mapa *mapa, Jogador *jog, Opps *inimigos, int *numInimigos, int *faseAtual, char *numMapa, EstadosJogo *estados, double *tempoFinal, double *tempoInicio, Sound fx5, Dificuldade *dificuldade);
void andar(Jogador *p);
void pular(Jogador* p, Sound fx);
void mover(Jogador *p, Mapa* mapa);
void zerarJogador(Jogador* j, Dificuldade *dificuldade);
void subirEscada(CorpoFisico *cf, Mapa *mapa);
void inserirRanking(PLACAR rank[10], char *nome, float tempo);
void salvarRanking(PLACAR rank[10]);
void carregarRanking(PLACAR rank[10]);
void caseMenu(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound fx2, Sound fx3, Sound fx6, Font Menu, Font Titulo);
void caseJogando(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, float *tempoInvencivel, int *faseAtual, char *numMapa, double *tempoInicio, double *tempoFinal, int *numrandom, Sound fx, Sound fx4, Sound fx5, Sound fx6, double *tempoPausado);
void casePausado(EstadosJogo *estado, Sound fx2, Sound fx6, double *tempoPausado, double *tempoInicio, Font Titulo, Font Menu);
void caseMorto(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound fx2, Sound fx3local);
void caseConfigs(EstadosJogo *estado, Dificuldade *dificuldade, Color *cortexto1, Color *cortexto2, Color *cortexto3, Sound fx2, Font Menu, Font titulo);
void caseRanking(EstadosJogo *estado, PLACAR rank[10], Sound fx2, Font Titulo, Font Nomes);
void caseVitoria(EstadosJogo *estado, PLACAR rank[10], double *tempoFinal, char *nomeJogador, int *letraNome, Sound fx6, Font Nomes);
void powerUp(Jogador *j, Mapa *mapa, Sound fx7);
void zerarRanking(PLACAR rank[10]);
void corrigirPosicao(Jogador *j);

#endif
