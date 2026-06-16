#include "inimigos.h"
#include "raylib.h"
#include "corpoFisico.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Desenha laser em uma célula
 * @param t A torre que atirou
 * @param offset Offset de células em relação à torre
 * @param tamanhoCelula O tamanho de uma célula
 * @return void
*/
void Torre_desenharLaser(Torre t, int offset, int tamanhoCelula){
    Color cor = t.estado == ATIRANDO? RED : YELLOW;
    DrawRectangle((t.celX + offset) * tamanhoCelula, t.celY * tamanhoCelula + (float)(tamanhoCelula/ALTURA_LASER), tamanhoCelula, tamanhoCelula/FINURA_LASER, cor);
}

/**
 * @brief Testa se o tiro colide com o jogador
 * @param t A torre que atirou
 * @param cf O CorpoFisico do jogador em questão
 * @param tamanhoCelula O tamanho de uma célula
 * @param offset Offset de células em relação à torre
 * @return true se colide, false caso contrário
*/
bool Torre_testarTiro(Torre t, CorpoFisico cf, int tamanhoCelula, int offset){
    int celY_p = (int) cf.posY / tamanhoCelula;
    if (celY_p != t.celY) return false;
    int celX_p = (int) cf.posX / tamanhoCelula;
    //printf("[Testando Colisão: (%3d, %3d), (%3d, %3d) = ", t.celX+offset, t.celY, celX_p, celY_p);
    //printf("<MESMO_Y> <MESMO_X?(%d)>]", t.celX+offset == celX_p);

    return (t.celX + offset == celX_p);
}

/**
 * @brief Executa a lógica de atirar para uma torre
 * @param t A torre para testar
 * @param cf O CorpoFisico (do jogador) para testar as colisões com
 * @param gridP Um ponteiro para a grid do mapa
 * @param tamanhoCelula O tamanho em pixels de uma célula
 * @param sX A quantidade de colunas no mapa
 * @return true se o laser colidir com o CorpoFisico, false caso contrário
*/
bool Torre_atirar(Torre t, CorpoFisico cf, char* gridP, int tamanhoCelula, int sX){
    if (t.estado != PREPARANDO && t.estado != ATIRANDO) return false;
    // sX representa a quantidade de colunas, é usado para separar as linhas, visto que gridP é um ponteiro e não uma matriz
    int linha = t.celY * sX;
    int gridPos = linha + t.celX;
    //printf("[1:(%d) 2:(%d) 3:(%d) | l:(%-3d) gP:(%-3d) c:(%c)]", gridP[gridPos] != 'Z', gridPos > linha, gridPos < linha+sX, linha, gridPos, gridP[gridPos]);
    bool acerto = false;
    for (int offset = 0; ((gridP[gridPos + offset] != 'Z') && (gridPos + offset >= linha) && (gridPos + offset < linha+sX)); offset+=t.dir){
        //                      testa colisão          testa se chegou na esquerda     testa se chegou na direita
        Torre_desenharLaser(t, offset, tamanhoCelula);
        if (!acerto && t.estado == ATIRANDO) acerto = Torre_testarTiro(t, cf, tamanhoCelula, offset); // Se ainda não acertou, verifica se acertaria agora
    }
    return acerto;    
}

/**
 * @brief Computa o estado da torre e muda se passar tempo o bastante
 * @param torre Ponteiro para a torre à testar
 * @param tempo GetFrameTime() em que a função é chamada
 * @return Inteiro representando o id do sprite certo para desenhar
*/
int Torre_computarEstado(Torre* torre, double tempo){

    //printf("[%.2f = %.2f? %d]", tempo, GetFrameTime(), tempo == GetFrameTime());

    // torre.tempoProx = tempo em que ocorrerá a próxima mudança de estado
    // torre.minCooldown e torre.maxCooldown é o range em que o próximo tiro pode ocorrer
    // quando o tiro começar, a torre deve estar no modo "atirando"
    // modos: {0 = Carregado; 1 = Preparando; 2 = Atirando; 3 = Resfriando}

    switch (torre->estado){
        case CARREGANDO:
        torre->tempoProx -= tempo;
        if (torre->tempoProx <= 0){
            torre->estado = PREPARANDO;
            torre->tempoProx = torre->tempoPreTiro;
            //printf("Troca -> PREPARANDO\n");
        }
        break;
        case PREPARANDO:
        torre->tempoProx -= tempo;
        if (torre->tempoProx <= 0){
            torre->estado = ATIRANDO;
            torre->tempoProx = torre->tempoTiro;
            //printf("Troca -> ATIRANDO\n");
        }
        break;
        case ATIRANDO:
        torre->tempoProx -= tempo;
        if (torre->tempoProx <= 0){
            torre->estado = RESFRIANDO;
            torre->tempoProx = torre->tempoPosTiro;
            //printf("Troca -> RESFRIANDO\n");
        }
        break;
        case RESFRIANDO:
        torre->tempoProx -= tempo;
        if (torre->tempoProx <= 0){
            torre->estado = CARREGANDO;
            torre->tempoProx = ( (float) (rand() % (int) (100 *(torre->maxCooldown - torre->minCooldown))) / 100.0) + torre->minCooldown;
            //printf("Troca -> CARREGANDO\n");
        }
        break;
    }

    return (torre->estado > 1)? torre->estado - 1 : torre->estado;
    // CARREGANDO = 0, PREPARANDO e ATIRANDO = 1, RESFIRANDO = 2; Esses números são para que sejam usados os sprites corretos
}
