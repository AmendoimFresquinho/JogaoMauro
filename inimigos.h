#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "raylib.h"
#include "corpoFisico.h"

typedef enum {
    FOGO,
    TORRE,
    GLITCH
}TIPOS_INIMIGOS;

typedef struct{
    float x;
    float y;
    int direcao;
    bool ativo;
    int numrand;
} Fogo;

typedef struct {
    TIPOS_INIMIGOS tipo;
    int dir, celX, celY;
    float tempoProx, minCooldown, maxCooldown;
    bool ativo;
} Torre;

typedef struct {
    TIPOS_INIMIGOS tipo;
    float posX, posY;
    float alcance, velocidade;
} Glitch;


bool Torre_atirar(Torre torre, CorpoFisico p, char* gridP, int tamanhoCelula, int sX);
void Torre_desenharLaser(int celX, int celY, int tamanhoCelula);
bool Torre_testarTiro(Torre t, CorpoFisico cf, int tamanhoCelula, int offset);



#endif