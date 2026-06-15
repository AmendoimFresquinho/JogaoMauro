#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "raylib.h"
#include "corpoFisico.h"

typedef enum {
    FOGO,
    TORRE,
    GLITCH
}TIPOS_INIMIGOS;

typedef enum {
    CARREGANDO,
    PREPARANDO,
    ATIRANDO,
    RESFRIANDO
} ESTADOS_TORRE;

typedef struct{
    float x;
    float y;
    int direcao;
    bool ativo;
    int numrand;
} Fogo;

typedef struct {
    TIPOS_INIMIGOS tipo;
    ESTADOS_TORRE estado;
    int dir, celX, celY;
    float tempoProx, tempoPreTiro, tempoPosTiro, tempoTiro;
    float minCooldown, maxCooldown;
    bool ativo;
} Torre;

static const float Torre_tempoMinCooldown[3] = {4.5,   2.5,    0.5};
static const float Torre_tempoMaxCooldown[3] = {7,     4.5,    2};
static const float Torre_tempoPreTiro[3] =     {2.5,   1.5,    0.5};
static const float Torre_tempoPosTiro[3] =     {5,     3,      0.8};

typedef struct {
    TIPOS_INIMIGOS tipo;
    float posX, posY;
    float alcance, velocidade;
} Glitch;


bool Torre_atirar(Torre torre, CorpoFisico p, char* gridP, int tamanhoCelula, int sX);
void Torre_desenharLaser(Torre t, int offset, int tamanhoCelula);
bool Torre_testarTiro(Torre t, CorpoFisico cf, int tamanhoCelula, int offset);
int Torre_computarEstado(Torre* torre, double tempo);


#endif