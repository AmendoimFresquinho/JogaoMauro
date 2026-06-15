#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void aplicarForcasMitigantes(CorpoFisico *cf, bool noChao){
    cf->velX *= 1 - CF_RESISTENCIA_AR;
    cf->velY *= 1 - CF_RESISTENCIA_AR;
    if (noChao)
        cf->velX *= 1 - CF_ATRITO_CHAO;
}
void aplicarGravidade(CorpoFisico *cf){
    cf->velY += CF_GRAVIDADE;
}
void testarColisaoX(CorpoFisico *cf, char *gridP, int tamanhoCelula, int sX /*, char* grupoDeColisao*/){
    int dir = (cf->velX > 0) ? 1 : ((cf->velX < 0) ? -1 : 0);
    if (!dir)
        return;
    int r = cf->raio;
    int yCelula = (int)cf->posY / tamanhoCelula;
    int celulaFinal = (cf->posX + cf->velX + (r * dir)) / tamanhoCelula;
    int celulaInicial = cf->posX / tamanhoCelula;
    int dist = abs(celulaFinal - celulaInicial);
    // DrawCircle(colunaFinal * tamanhoCelula, p->corpoFisico.posY, 30, GREEN);
    for (int i = 0; i <= dist; i++){
        // DrawCircle(((i*dir))*tamanhoCelula + (dir*p->tamanho/2), p->corpoFisico.posY, 25, RED);
        if (gridP[(yCelula * sX) + celulaInicial + (i * dir)] == 'Z' || !(celulaInicial + (i * dir) >= 0 && celulaInicial + (i * dir) < sX)){
            int verdadeiraCelulaFinal = celulaInicial + ((i - 1) * dir);
            int offset = (dir == 1) ? tamanhoCelula - r : r;
            cf->posX = (tamanhoCelula * verdadeiraCelulaFinal) + offset;
            cf->velX = 0;
            return;
        }
    }
    cf->posX += cf->velX;
}
void testarColisaoY(CorpoFisico *cf, char *gridP, int tamanhoCelula, int sX, int sY, bool *noChao /*, char* grupoDeColisao*/){
    int dir = (cf->velY > 0) ? 1 : ((cf->velY < 0) ? -1 : 0);
    if (!dir)
        return;
    // char* grupoDeColisao = (*noChao)? "ZH" : "Z";
    int r = cf->raio;
    int xCelulaE = (int)(cf->posX - cf->raio + 0.001) / tamanhoCelula;
    int xCelulaD = (int)(cf->posX + cf->raio - 0.001) / tamanhoCelula;
    int celulaFinal = (cf->posY + cf->velY + (r * dir)) / tamanhoCelula;
    int celulaInicial = cf->posY / tamanhoCelula;
    int dist = abs(celulaFinal - celulaInicial);
    // DrawCircle(colunaFinal * tamanhoCelula, p->corpoFisico.posY, 30, GREEN);
    for (int i = 0; i <= dist; i++)
    {
        if (!(celulaInicial + (i * dir) >= 0 && celulaInicial + (i * dir) < sY))
        {
            int verdadeiraCelulaFinal = celulaInicial + ((i - 1) * dir);
            int offset = (dir == 1) ? tamanhoCelula - r : r;
            cf->posY = (tamanhoCelula * verdadeiraCelulaFinal) + offset;
            cf->velY = 0;
            *noChao = (dir < 0) ? false : true;
            return;
        }
        // DrawCircle(((i*dir))*tamanhoCelula + (dir*p->tamanho/2), p->corpoFisico.posY, 25, RED);
        // for (int j = 0; grupoDeColisao[j] != 0; j++){
        // char c = grupoDeColisao[j];
        if (gridP[(sX * (celulaInicial + (i * dir))) + xCelulaE] == 'Z' || gridP[(sX * (celulaInicial + (i * dir))) + xCelulaD] == 'Z')
        {
            int verdadeiraCelulaFinal = celulaInicial + ((i - 1) * dir);
            int offset = (dir == 1) ? tamanhoCelula - r : r;
            cf->posY = (tamanhoCelula * verdadeiraCelulaFinal) + offset;
            cf->velY = 0;
            *noChao = (dir < 0) ? false : true;
            return;
        }
        if (dir > 0 && (gridP[(sX * (celulaInicial + (i * dir) - 1)) + xCelulaE] == 'D' || gridP[(sX * (celulaInicial + (i * dir) - 1)) + xCelulaD] == 'D'))
        {
            int verdadeiraCelulaFinal = celulaInicial + ((i - 1) * dir);
            int offset = (dir == 1) ? tamanhoCelula - r : r;
            cf->posY = (tamanhoCelula * verdadeiraCelulaFinal) + offset;
            cf->velY = 0;
            *noChao = (dir < 0) ? false : true;
            return;
        }
    }
    cf->posY += cf->velY;
    return;
}
CorpoFisico criarCorpoFisico(void){
    CorpoFisico cf;
    cf.posX = 0;
    cf.posY = 0;
    cf.velX = 0;
    cf.velY = 0;
    cf.massa = 0;
    cf.raio = 0;
    return cf;
}
