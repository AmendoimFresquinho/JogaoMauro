#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>

int main(){
    
    InitWindow(LARGURA_TELA, ALTURA_TELA, "DKINF - Teste de Movimentacao");
    SetTargetFPS(60); 
        
    int numInimigos = 0;
    Jogador mario = {0};
    Opps inimigo[MAX_INIMIGOS] = {0};
    mario.tamanho = TAMANHO_JOGADOR;
    Mapa mapa = {0};
    LerMapa(&mapa, &mario, "mapa1.txt", inimigo, &numInimigos);
    EstadosJogo estado = JOGANDO;

    while (!WindowShouldClose()) { 
       BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if(mario.ativo == false){
            estado = MORTO;
            }
         if (estado == JOGANDO)
            HideCursor();
                else 
                    ShowCursor();
        
        switch(estado){

        case MENU:

        break;
        
        case JOGANDO:
        
        if(IsKeyPressed(KEY_TAB)){
            estado = PAUSADO;
        }
            
        Inimigos(&mapa, &mario, inimigo, &numInimigos);
        
        movimento(&mario, &mapa);
        
        DesenharMapa(&mapa);  
        DrawRectangle(mario.x, mario.y, mario.tamanho, mario.tamanho, BLUE);
       

        for (int i = 0; i < numInimigos; i++) {
            if (inimigo[i].ativo) {
                DrawRectangle(inimigo[i].x, inimigo[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
            }   
        }

        break;

        case PAUSADO:
            int larguraVoltar = MeasureText("VOLTAR AO JOGO", 20);
            int larguraMenu = MeasureText("IR AO MENU", 20);
            int larguraSair = MeasureText("SAIR DO JOGO", 20);
            int larguraPausado = MeasureText("JOGO PAUSADO, ", 60);
            int larguraSelecione = MeasureText("SELECIONE UMA OPCAO", 60);

            int xVoltar = 50;
            int xMenu = (LARGURA_TELA - larguraMenu) / 2;
            int xSair = LARGURA_TELA - larguraSair - 50;
            int xPausado = (LARGURA_TELA - larguraPausado) / 2;
            int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;

            Rectangle voltaJogo = {xVoltar, 700, larguraVoltar, 20};
            Rectangle menu = {xMenu, 700, larguraMenu, 20};
            Rectangle vazaJogo = {xSair, 700, larguraSair, 20};

            DrawText("JOGO PAUSADO, ", xPausado, 300, 60, RED);
            DrawText("SELECIONE UMA OPCAO", xSelecione, 400, 60, RED);
            DrawText("VOLTAR AO JOGO", xVoltar, 700, 20, CheckCollisionPointRec(GetMousePosition(), voltaJogo) ? RED : BLACK);
            DrawText("IR AO MENU", xMenu, 700, 20, CheckCollisionPointRec(GetMousePosition(), menu) ? RED : BLACK);
            DrawText("SAIR DO JOGO", xSair, 700, 20, CheckCollisionPointRec(GetMousePosition(), vazaJogo) ? RED : BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), voltaJogo)) estado = JOGANDO;
                if (CheckCollisionPointRec(GetMousePosition(), menu)) estado = MENU;
                if (CheckCollisionPointRec(GetMousePosition(), vazaJogo)) CloseWindow();
            }
        break;

        case MORTO:

            int larguraReiniciar = MeasureText("REINICIAR JOGO", 20);
            int larguraMenu2 = MeasureText("IR AO MENU", 20);
            int larguraSair2 = MeasureText("SAIR DO JOGO", 20);
            int larguraMorto = MeasureText("VOCE MORREU, ", 60);
           
            int xMorto = (LARGURA_TELA - larguraMorto) / 2;;
            int xMenu2 = (LARGURA_TELA - larguraMenu2) / 2;
            int xSair2 = LARGURA_TELA - larguraSair2 - 50;
            int xReiniciar = 50;
           
            Rectangle reiniciaJogo = {xReiniciar, 700, larguraReiniciar, 20};
            Rectangle menu2 = {xMenu2, 700, larguraMenu2, 20};
            Rectangle vazaJogo2 = {xSair2, 700, larguraSair2, 20};
                        
            DrawText("VOCE MORREU", xMorto, 400, 70, RED);
            DrawText("REINICIAR JOGO", xReiniciar, 700, 20, CheckCollisionPointRec(GetMousePosition(), reiniciaJogo) ? RED : BLACK);
            DrawText("IR AO MENU", xMenu2, 700, 20, CheckCollisionPointRec(GetMousePosition(), menu2) ? RED : BLACK);
            DrawText("SAIR DO JOGO", xSair2, 700, 20, CheckCollisionPointRec(GetMousePosition(), vazaJogo2) ? RED : BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), reiniciaJogo)){
                reiniciarJogo(&mapa, &mario, inimigo, &numInimigos);                    
                estado = JOGANDO;}
                if (CheckCollisionPointRec(GetMousePosition(), menu2)) estado = MENU;
                if (CheckCollisionPointRec(GetMousePosition(), vazaJogo2)) CloseWindow();
            }
        break;
        
    }   
EndDrawing();} 

CloseWindow();
return 0;}

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
    int gridColuna = (int)(p->x + (TAMANHO_JOGADOR/2)) / TAMANHO_BLOCOS;
    int gridLinhaMeio = (int)(p->y + (TAMANHO_JOGADOR / 2)) / TAMANHO_BLOCOS;
    int gridColunaMeio = (int)(p->x + (TAMANHO_JOGADOR/2)) / TAMANHO_BLOCOS;
    int gridLinhaCima = (int)(p->y) / TAMANHO_BLOCOS;
    int gridColunaCima = (int)(p->x + (TAMANHO_JOGADOR/2)) / TAMANHO_BLOCOS;
    
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

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos){
    FILE *file = fopen(arquivo, "r"); //Abre o arquivo txt

    if (file == NULL) {
    printf("Erro ao abrir o arquivo %s\n", arquivo);
    return;}

    char c;
    int linha = 0, coluna = 0;
        
    while((c = fgetc(file)) != EOF){ 

        if(c == '\r'){
            continue; //deixar windows proof
        }
        else if(c == '\n'){
            linha++; 
            coluna = 0; 
        }
        else{
            
        if (linha < LINHAS && coluna < COLUNAS) {
            mapa->grid[linha][coluna] = c;

        if (c == 'P') {
            jogador->x = ((coluna + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            jogador->y = ((linha + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            jogador->ativo = true;
            jogador->noChao = false;
            jogador->velocidadeY = 0.0f;
        }

        if (c == 'E') {
            inimigo[*numInimigos].x = ((coluna + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            inimigo[*numInimigos].y = ((linha + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            inimigo[*numInimigos].direcao = 1;
            inimigo[*numInimigos].ativo = true;
            (*numInimigos)++;
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

void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos){

for (int i = 0; i < *numInimigos; i++) {
    int gridLinha  = (int)(inimigo[i].y + TAMANHO_JOGADOR) / TAMANHO_BLOCOS;
    int gridColuna = (int)(inimigo[i].x + (TAMANHO_JOGADOR/2)) / TAMANHO_BLOCOS;

if(mapa->grid[gridLinha][gridColuna] == 'Z' || mapa->grid[gridLinha][gridColuna] == 'H'){
    if(inimigo[i].y < 300)
        inimigo[i].x += VELOCIDADE_OPPS_DIFICIL * inimigo[i].direcao;
    else
        inimigo[i].x += VELOCIDADE_OPPS * inimigo[i].direcao;
}
else{
    inimigo[i].direcao *= -1;  
    if(inimigo[i].y < 300)
        inimigo[i].x += VELOCIDADE_OPPS_DIFICIL * inimigo[i].direcao;
    else
        inimigo[i].x += VELOCIDADE_OPPS * inimigo[i].direcao;
}

Rectangle player = {j->x, j->y, TAMANHO_JOGADOR, TAMANHO_JOGADOR};
Rectangle opps = {inimigo[i].x, inimigo[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR};
 
if(CheckCollisionRecs(player, opps)){
    j->ativo = false;
}}}

void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos) {
    *numInimigos = 0;
    *jogador = (Jogador){0};
    jogador->tamanho = TAMANHO_JOGADOR;
    *mapa = (Mapa){0};
    LerMapa(mapa, jogador, "mapa1.txt", inimigos, numInimigos);
}
