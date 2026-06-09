#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo *estado, double *tempoFinal, double *tempoInicio){
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
                    inimigo[*numInimigos].x = ((coluna + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
                    inimigo[*numInimigos].y = ((linha + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
                    inimigo[*numInimigos].direcao = 1;
                    inimigo[*numInimigos].ativo = true;
                    inimigo[*numInimigos].numrand = 1;
                    (*numInimigos)++; // Fundamental para a leitura do array de inimigos
                }
            }

            coluna++;
        }
    }
    fclose(file);
}

void DesenharMapa(Mapa *mapa, Texture2D groundTXT, Texture2D ladderTXT){
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
                DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, GREEN);
                break;
            case 'K':
                DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, YELLOW);
                break;
            case 'V':
                DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, PINK);
                break;
            default:
                break;
            }
        }
    }
}
