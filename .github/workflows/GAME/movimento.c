#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void movimento(Jogador *p, Mapa *mapa, Sound Pular){
    CorpoFisico *cf = &(p->corpoFisico);
    andar(p);
    aplicarGravidade(cf);
    aplicarForcasMitigantes(cf, p->noChao);
    pular(p, Pular);
    mover(p, mapa);
    subirEscada(cf, mapa);
}
void andar(Jogador *p){
    float tmp = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) * p->aceleracao;
    p->corpoFisico.velX += p->noChao ? tmp : tmp * razaoArChao; // Caso esteja no chão, acelerará normalmente. Se estiver no ar, irá parecer normal mas na verdade tem menos impacto
}
void pular(Jogador *p, Sound Pular){
    if (IsKeyPressed(KEY_SPACE) && p->noChao){
        p->corpoFisico.velY += p->forcaPulo;
        p->noChao = false;
        PlaySound(Pular);
    }
}
void mover(Jogador *p, Mapa *mapa){
    testarColisaoY(&(p->corpoFisico), &mapa->grid[0][0], TAMANHO_BLOCOS, COLUNAS, LINHAS, &(p->noChao));
    testarColisaoX(&(p->corpoFisico), &mapa->grid[0][0], TAMANHO_BLOCOS, COLUNAS);
}
void subirEscada(CorpoFisico *cf, Mapa *mapa){
    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !IsKeyDown(KEY_SPACE)){
        int celX = cf->posX / TAMANHO_BLOCOS;
        int celY = cf->posY / TAMANHO_BLOCOS;
        if (mapa->grid[celY][celX] == 'S'){
            for (int i = 0; i < LINHAS; i++){
                if (mapa->grid[celY - i][celX] == 'D'){
                    cf->posY = (celY - i) * TAMANHO_BLOCOS + cf->raio * 2;
                    return;
                }
            }
        }
    }
    else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && !IsKeyDown(KEY_SPACE)){
        int celX = cf->posX / TAMANHO_BLOCOS;
        int celY = cf->posY / TAMANHO_BLOCOS;
        if (mapa->grid[celY][celX] == 'D'){
            for (int i = 0; i < LINHAS; i++){
                if (mapa->grid[celY + i][celX] == 'S'){
                cf->posY = (celY + i) * TAMANHO_BLOCOS + cf->raio * 2;
                return;
                }
            }
        }
    }
}
