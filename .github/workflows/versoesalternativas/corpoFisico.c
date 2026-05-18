#include <math.h>
#include "corpoFisico.h"
#include "raylib.h"

void aplicarForcasMitigantes(CorpoFisico *cf, bool noChao){
    cf->velX *= 1 - CF_RESISTENCIA_AR;
    cf->velY *= 1 - CF_RESISTENCIA_AR;
    if (noChao) cf->velX *= 1 - CF_ATRITO_CHAO;
}
void aplicarGravidade(CorpoFisico *cf){
    cf->velY += CF_GRAVIDADE;
}
void testarColisaoX(CorpoFisico *cf, char* gridP, int tamanhoCelula, int sX/*, char* grupoDeColisao*/){
    int dir = (cf->velX > 0)? 1 : ((cf->velX < 0)? -1 : 0);
    if (!dir) return;
    int r = cf->raio;
    int yCelula = (int) cf->posY / tamanhoCelula;
    int celulaFinal = (cf->posX + cf->velX + (r*dir)) / tamanhoCelula;
    int celulaInicial = cf->posX / tamanhoCelula;
    int dist = abs(celulaFinal - celulaInicial);
    //DrawCircle(colunaFinal * tamanhoCelula, p->corpoFisico.posY, 30, GREEN);
    for (int i = 0; i <= dist; i++){
        //DrawCircle(((i*dir))*tamanhoCelula + (dir*p->tamanho/2), p->corpoFisico.posY, 25, RED);
        if (gridP[(yCelula*sX) + celulaInicial + (i*dir)] == 'Z' || !(celulaInicial + (i*dir) >= 0 && celulaInicial + (i*dir) < sX)){
            int verdadeiraCelulaFinal = celulaInicial + ((i-1)*dir);
            int offset = (dir==1)? tamanhoCelula - r : r;
            cf->posX = (tamanhoCelula*verdadeiraCelulaFinal) + offset;
            cf->velX = 0;
            return;
        };
    }
    cf->posX += cf->velX;
}
void testarColisaoY(CorpoFisico *cf, char* gridP, int tamanhoCelula, int sX, int sY/*, char* grupoDeColisao*/){
    int dir = (cf->velY > 0)? 1 : ((cf->velY < 0)? -1 : 0);
    if (!dir) return;
    int r = cf->raio;
    int xCelula = (int) cf->posX / tamanhoCelula;
    int celulaFinal = (cf->posY + cf->velY + (r*dir)) / tamanhoCelula;
    int celulaInicial = cf->posY / tamanhoCelula;
    int dist = abs(celulaFinal - celulaInicial);
    //DrawCircle(colunaFinal * tamanhoCelula, p->corpoFisico.posY, 30, GREEN);
    for (int i = 0; i <= dist; i++){
        //DrawCircle(((i*dir))*tamanhoCelula + (dir*p->tamanho/2), p->corpoFisico.posY, 25, RED);
        if (gridP[(sX *(celulaInicial + (i*dir))) + xCelula] == 'Z' || !(celulaInicial + (i*dir) >= 0 && celulaInicial + (i*dir) < sY)){
            int verdadeiraCelulaFinal = celulaInicial + ((i-1)*dir);
            int offset = (dir==1)? tamanhoCelula - r : r;
            cf->posY = (tamanhoCelula*verdadeiraCelulaFinal) + offset;
            cf->velY = 0;
            return;
        };
    }
    cf->posY += cf->velY;
}