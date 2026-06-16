#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo *estado, double *tempoFinal, double *tempoInicio, Dificuldade dificuldade){
    FILE *file = fopen(arquivo, "r"); // Abre o arquivo txt

    if (file == NULL){
        *tempoFinal = GetTime() - *tempoInicio;
        *estado = VITORIA;
        return;
    }

    char c;
    int linha = 0, coluna = 0;
    
    while ((c = fgetc(file)) != EOF){ // Vai pegando os caracteres do arquivo e colocando na matriz

        if (c == '\r'){
            continue; // deixar windows proof
        }
        else if (c == '\n'){
            linha++;
            coluna = 0;
        }
        else{

            if (linha < LINHAS && coluna < COLUNAS){
                mapa->grid[linha][coluna] = c;

                if (c == 'P'){ // Se é P spawna o player
                    jogador->corpoFisico.posX = ((coluna + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
                    jogador->corpoFisico.posY = ((linha + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
                    jogador->ativo = true;
                    jogador->noChao = true;
                    jogador->corpoFisico.velY = 0.0f;
                }

                if (c == 'E'){ // Se é E spawna os monstros
                    Fogo* fogo = &inimigo->fogos[*numInimigos];
                    fogo->x = ((coluna + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
                    fogo->y = ((linha + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
                    fogo->direcao = 1;
                    fogo->ativo = true;
                    fogo->numrand = 1;
                    (*numInimigos)++; // Fundamental para a leitura do array de inimigos 
                }

                if (c == 'T') {
                    Torre* torre = &inimigo->torres[*numInimigos];
                    torre->tipo = TORRE;
                    torre->celX = coluna;
                    torre->celY = linha;
                    torre->dir = 1;
                    torre->maxCooldown = Torre_tempoMaxCooldown[dificuldade]; torre->minCooldown = Torre_tempoMinCooldown[dificuldade];
                    torre->tempoProx = 5; torre->tempoTiro = 0.1;
                    torre->tempoPreTiro = Torre_tempoPreTiro[dificuldade]; torre->tempoPosTiro = Torre_tempoPosTiro[dificuldade];
                    torre->estado = CARREGANDO; torre->ativo = true;
                    (*numInimigos)++;
                }

                if (c == 't') {
                    Torre* torre = &inimigo->torres[*numInimigos];
                    torre->tipo = TORRE;
                    torre->celX = coluna;
                    torre->celY = linha;
                    torre->dir = -1;
                    torre->maxCooldown = Torre_tempoMaxCooldown[dificuldade]; torre->minCooldown = Torre_tempoMinCooldown[dificuldade];
                    torre->tempoProx = 5; torre->tempoTiro = 0.1;
                    torre->tempoPreTiro = Torre_tempoPreTiro[dificuldade]; torre->tempoPosTiro = Torre_tempoPosTiro[dificuldade];
                    torre->estado = CARREGANDO; torre->ativo = true;
                    (*numInimigos)++;
                }
            }

            coluna++;
        }
    }
    fclose(file);
}

void DesenharMapa(Mapa *mapa, Texture2D groundTXT, Texture2D ladderTXT, Texture2D portalTXT, Anim portalANIM, Texture2D heartTXT, Texture2D slowTXT){
    for (int l = 0; l < LINHAS; l++){
        for (int c = 0; c < COLUNAS; c++){
            int x = c * TAMANHO_BLOCOS;
            int y = l * TAMANHO_BLOCOS;

            switch (mapa->grid[l][c]){ // Le a matriz e desenha os blocos em seus devidos lugares
            case 'Z':
                // DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, LIGHTGRAY);
                DrawTexture(groundTXT, x, y, WHITE);
                break;
            case 'S':
                // DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, BROWN);
                DrawTexture(ladderTXT, x, y, WHITE);
                break;
            case 'D':
                DrawTexture(groundTXT, x, y + TAMANHO_BLOCOS, WHITE);
                break;
            case 'H':
                // DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, BROWN);
                DrawTexture(ladderTXT, x, y, WHITE);
                break;
            case 'F':
                Vector2 pos; pos.x = x - portalANIM.lar/2; pos.y = y - portalANIM.alt/2;
                int padding = portalANIM.atual * 2 + 1; // Cada frame tem 1 pixel de pading para todos os lados para prevenir erros de renderização
                Rectangle rect = {portalANIM.atual * portalANIM.lar + padding, 1, portalANIM.lar, portalANIM.alt};
                DrawTextureRec(portalTXT, rect, pos, WHITE);
                break;
            case 'K':
                DrawTexture(slowTXT, x, y, WHITE);
                break;
            case 'V':
                DrawTexture(heartTXT, x, y, WHITE);
                //DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, PINK);
                break;
            default:
                break;
            }
        }
    }
}
