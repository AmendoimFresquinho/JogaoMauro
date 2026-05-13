#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>

int main(){
    
    InitWindow(LARGURA_TELA, ALTURA_TELA, "DKINF - Teste de Movimentacao");
    SetTargetFPS(60); 
    
    Jogador mario = {0};
    mario.tamanho = TAMANHO_JOGADOR;
    Mapa mapa = {0};
    LerMapa(&mapa, &mario, "mapa1.txt");

    while (!WindowShouldClose()) { 

        movimento(&mario, &mapa);
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DesenharMapa(&mapa);    
        DrawRectangle(mario.x, mario.y, mario.tamanho, mario.tamanho, BLUE);

        EndDrawing();
}
    CloseWindow();
    return 0;
}

void movimento(Jogador *p, Mapa *mapa) {
    
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        p->x += VELOCIDADE;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        p->x -= VELOCIDADE;
    }
    if (IsKeyPressed(KEY_SPACE) && p->noChao == true) {
        p->velocidadeY = FORCA_PULO;
        p->noChao = false;
    }

    p->velocidadeY += GRAVIDADE;
    p->y += p->velocidadeY;

    int gridLinha  = (int)(p->y + TAMANHO_JOGADOR) / TAMANHO_BLOCOS;
    int gridColuna = (int)(p->x + 10) / TAMANHO_BLOCOS;
    int gridLinhaMeio = (int)(p->y + (TAMANHO_JOGADOR / 2)) / TAMANHO_BLOCOS;
    int gridColunaMeio = (int)(p->x + 10) / TAMANHO_BLOCOS;
    int gridLinhaCima = (int)(p->y) / TAMANHO_BLOCOS;
    int gridColunaCima = (int)(p->x + 10) / TAMANHO_BLOCOS;
    static float xEscada = 0;

    // Deixa ele ficar no chao

    if(gridLinha >= 0 && gridLinha < LINHAS && gridColuna >= 0 && gridColuna < COLUNAS && (mapa->grid[gridLinha][gridColuna] == 'Z' || (mapa->grid[gridLinha][gridColuna] == 'H' && mapa->grid[gridLinhaMeio][gridColunaMeio] == 'D'))) {
        p->y = (gridLinha * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
        p->noChao = true;
        p->velocidadeY = 0.0f;
    } 
    else {
        p->noChao = false;
    }

   // Não deixa ele atravessar o teto

     if (gridLinhaCima >= 0 && gridLinhaCima < LINHAS && gridColunaCima >= 0 && gridColunaCima < COLUNAS && mapa->grid[gridLinhaCima][gridColunaCima] == 'Z') {
        p->y = (gridLinhaCima + 1) * TAMANHO_BLOCOS; 
        p->velocidadeY = 0.0f;} 

    
    //Não deixa ele sair da tela

    if (p->y < 0) p->y = 0;
    if (p->x > LARGURA_TELA - TAMANHO_JOGADOR) p->x = LARGURA_TELA - TAMANHO_JOGADOR;
    if (p->x < 0) p->x = 0;

    // Sobe Escada 

  if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'S' && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))) {
    for (int l = gridLinhaMeio; l >= 0; l--) {
        if (mapa->grid[l][gridColunaMeio] == 'D') {
            p->velocidadeY = 0.0f;
            p->y = ((l + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            p->noChao = true;
            break;
        }
    }
}   

    // Desce Escada 

   if(mapa->grid[gridLinhaMeio][gridColunaMeio] == 'D' && (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))){
    for (int l = gridLinhaMeio; l < LINHAS; l++) {
        if (mapa->grid[l][gridColunaMeio] == 'S') {
            p->y = ((l + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            p->velocidadeY = 0.0f;
            p->noChao = true;
            break;
        }
    }
}
    
}

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo){
    FILE *file = fopen(arquivo, "r"); //Abre o arquivo txt

    if (file == NULL) {
    printf("Erro ao abrir o arquivo %s\n", arquivo);
    return;}

    char c;
    int linha = 0, coluna = 0;
        
    while((c = fgetc(file)) != EOF){ //

        if(c == '\r'){
            continue; //deixar windows proof
        }
        else if(c == '\n'){
            linha++; //Terminou uma linha, vai para a próxima
            coluna = 0; //Zera a coluna, pois nova linha
        }
        else{
            
        if (linha < LINHAS && coluna < COLUNAS) {
            mapa->grid[linha][coluna] = c;

        if (c == 'P') {
            jogador->x = coluna * TAMANHO_BLOCOS;
            jogador->y = linha  * TAMANHO_BLOCOS;
            jogador->ativo = true;
            jogador->noChao = false;
            jogador->velocidadeY = 0.0f;
            mapa->grid[linha][coluna] = ' ';
        }
    }

    coluna++;
    }
}
fclose(file);
}

void DesenharMapa(Mapa *mapa){
  for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            int x = c * TAMANHO_BLOCOS;
            int y = l * TAMANHO_BLOCOS;

            switch (mapa->grid[l][c]) {
                case 'Z':
                    DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, GRAY);
                    break;
                case 'S':
                    DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, BROWN);
                    break;
                case 'D':
                    break;
                case 'H':
                    DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, BROWN);
                    break;
                case 'F':
                    DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, GREEN);
                    break;
                default:
                    break;
            }
        }
    }
}
