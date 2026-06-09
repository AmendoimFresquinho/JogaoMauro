#include "inimigos.h"
#include "raylib.h"
#include "corpoFisico.h"
#include <stdio.h>
void Torre_desenharLaser(int celX, int celY, int tamanhoCelula){
    DrawRectangle(celX * tamanhoCelula, celY * tamanhoCelula, tamanhoCelula/2, tamanhoCelula/2, RED);
}

bool Torre_testarTiro(Torre t, CorpoFisico cf, int tamanhoCelula, int offset){
    int celY_p = (int) cf.posY / tamanhoCelula;
    if (celY_p != t.celY) return false;
    int celX_p = (int) cf.posX / tamanhoCelula;
    //printf("[Testando Colisão: (%3d, %3d), (%3d, %3d) = ", t.celX+offset, t.celY, celX_p, celY_p);
    //printf("<MESMO_Y> <MESMO_X?(%d)>]", t.celX+offset == celX_p);

    return (t.celX + offset == celX_p);
}

bool Torre_atirar(Torre t, CorpoFisico cf, char* gridP, int tamanhoCelula, int sX){
    // sX representa a quantidade de colunas, é usado para separar as linhas, visto que gridP é um ponteiro e não uma matriz
    int linha = t.celY * sX;
    int gridPos = linha + t.celX;
    //printf("[1:(%d) 2:(%d) 3:(%d) | l:(%-3d) gP:(%-3d) c:(%c)]", gridP[gridPos] != 'Z', gridPos > linha, gridPos < linha+sX, linha, gridPos, gridP[gridPos]);
    bool acerto = false;
    for (int offset = 0; ((gridP[gridPos + offset] != 'Z') && (gridPos + offset > linha) && (gridPos + offset < linha+sX)); offset+=t.dir){
        //                      testa colisão          testa se chegou na esquerda     testa se chegou na direita
        Torre_desenharLaser(t.celX + offset, t.celY, tamanhoCelula);
        if (!acerto) acerto = Torre_testarTiro(t, cf, tamanhoCelula, offset); // Se ainda não acertou, verifica se acertaria agora
    }
    return acerto;    
}