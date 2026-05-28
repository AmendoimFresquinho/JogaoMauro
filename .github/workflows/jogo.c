#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
float velocidadeInimigo;


Texture2D groundTXT, ladderTXT;

int main(){
    
    InitWindow(LARGURA_TELA, ALTURA_TELA, "DKINF");
    SetTargetFPS(60); 
        
    // Inicia as structs zeradas (para evitar lixo de memória) ou no "default"

    Jogador mario = {0};
    Mapa mapa = {0};
    Dificuldade dificuldade = NORMAL;
    Opps inimigo[MAX_INIMIGOS] = {0};
    EstadosJogo estado = MENU;

    mario.tamanho = TAMANHO_JOGADOR;
    mario.aceleracao = 5;
    mario.corpoFisico = criarCorpoFisico();
    mario.corpoFisico.raio = mario.tamanho/2;
    mario.corpoFisico.massa = 10;
    char numMapa[20]; 
    Color cortexto1 = WHITE;
    Color cortexto2 = RED;
    Color cortexto3 = WHITE;
    float tempoInvencivel = 2.0f;
    int numInimigos = 0, faseAtual = 1, numrandom = 1;

    srand(time(NULL));

    // Carrega as texturas
    Image groundIMG = LoadImage("sprites/ground.png");
    ImageResize(&groundIMG, 30, 30);
    groundTXT = LoadTextureFromImage(groundIMG);
    UnloadImage(groundIMG);

    Image ladderIMG = LoadImage("sprites/ladder.png");
    ImageResize(&ladderIMG, 30, 30);
    ladderTXT = LoadTextureFromImage(ladderIMG);
    UnloadImage(ladderIMG);


    // Inicializa o primeiro mapa

    sprintf(numMapa, "mapa%d.txt", faseAtual);
    LerMapa(&mapa, &mario, numMapa, inimigo, &numInimigos, &estado);

    carregarRanking(rank);

    // Variaveis de tamanho de texto e centralização de texto (puramente visual)

    int larguraVoltar = MeasureText("VOLTAR AO JOGO", 20);
    int larguraMenu = MeasureText("IR AO MENU", 20);
    int larguraSair = MeasureText("SAIR DO JOGO", 20);
    int larguraPausado = MeasureText("JOGO PAUSADO, ", 60);
    int larguraSelecione = MeasureText("SELECIONE UMA OPÇÃO", 60);
    int larguraReiniciar = MeasureText("REINICIAR JOGO", 20);
    int larguraMorto = MeasureText("VOCÊ MORREU, ", 60);
    int xVoltar = 50;
    int xMenu = (LARGURA_TELA - larguraMenu) / 2;
    int xSair = LARGURA_TELA - larguraSair - 50;
    int xPausado = (LARGURA_TELA - larguraPausado) / 2;
    int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;
    int xMorto = (LARGURA_TELA - larguraMorto) / 2;;
    int xReiniciar = 50;


     while (!WindowShouldClose()) { 
       BeginDrawing();
       
        if(mario.ativo == false){ // Quando o Mario morre, estado = MORTO
            estado = MORTO;
            }
        if (estado == JOGANDO) // Esconde o cursor
            HideCursor();
                else 
                    ShowCursor();
        
        switch(estado){

        // MENU

        case MENU:
        ClearBackground(BLACK);

            faseAtual = 1;

            Rectangle iniciaJogo = {xSelecione + 95, 400, larguraMorto - 60, 60};
            Rectangle configs = {xSelecione + 95, 500, larguraMorto + 40, 60};           
            Rectangle ranking = {xSelecione + 95, 600, larguraMorto - 160, 60};
            Rectangle sairJogo = {xSelecione + 95, 700, larguraMorto - 40, 60};

            DrawRectangleRounded(iniciaJogo, 0.1, 1, DARKGRAY);
            DrawRectangleRounded(configs, 0.1, 1, DARKGRAY);
            DrawRectangleRounded(ranking, 0.1, 1, DARKGRAY);
            DrawRectangleRounded(sairJogo, 0.1, 1, DARKGRAY);

            DrawText("BEM VINDO AO DKINF!", xSelecione + 25, 100, 60, RED);
            DrawText("OPÇÕES:", xMorto + 100, 200, 60, RED);
                        
            DrawText("INICIAR JOGO", xSelecione + 100, 400, 60, CheckCollisionPointRec(GetMousePosition(),iniciaJogo) ? RED : WHITE);
            DrawText("CONFIGURAÇÕES", xSelecione + 100, 500, 60, CheckCollisionPointRec(GetMousePosition(), configs) ? RED : WHITE);
            DrawText("RANKINGS", xSelecione + 100, 600, 60, CheckCollisionPointRec(GetMousePosition(), ranking) ? RED : WHITE);
            DrawText("SAIR DO JOGO", xSelecione + 100, 700, 60, CheckCollisionPointRec(GetMousePosition(), sairJogo) ? RED : WHITE);

            if(dificuldade == FACIL) // Define a velocidade dos inimigos dependendo da dificuldade
                velocidadeInimigo = 3.0f;
            if(dificuldade == NORMAL)
                velocidadeInimigo = 4.0f;
            if(dificuldade == DIFICIL)
                velocidadeInimigo = 6.0f;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (CheckCollisionPointRec(GetMousePosition(), iniciaJogo)){              
                        reiniciarJogo(&mapa, &mario, inimigo, &numInimigos, &dificuldade, &estado);                    
                        estado = JOGANDO;}
                    if (CheckCollisionPointRec(GetMousePosition(), configs)) estado = CONFIGS;
                    if (CheckCollisionPointRec(GetMousePosition(), ranking)) estado = RANKING;
                    if (CheckCollisionPointRec(GetMousePosition(), sairJogo)) CloseWindow();
                }

        break;

        // JOGANDO
        
        case JOGANDO:

        ClearBackground(BLACK);

        if(IsKeyPressed(KEY_TAB)){ // Se clica TAB pausa o game
            estado = PAUSADO;
        }
        if (tempoInvencivel > 0) {
            tempoInvencivel -= GetFrameTime();
        }
        else {
            tempoInvencivel = 0; 
        }
        char texto[50];
        sprintf(texto, "VIDAS: %d", mario.vidas);
        DrawText(texto, 10, 10, 30, RED);
    
        Inimigos(&mapa, &mario, inimigo, &numInimigos, &tempoInvencivel, &dificuldade, &numrandom);
        movimento(&mario, &mapa);
        proximafase(&mapa, &mario, inimigo, &numInimigos, &faseAtual, numMapa, &estado);
    
        DesenharMapa(&mapa);  
        DrawRectangle(mario.corpoFisico.posX - mario.corpoFisico.raio, mario.corpoFisico.posY - mario.corpoFisico.raio, mario.tamanho, mario.tamanho, BLUE);
       
        for (int i = 0; i < numInimigos; i++) {
            if (inimigo[i].ativo) {
                DrawRectangle(inimigo[i].x, inimigo[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
            }   
        }

        break;

        // PAUSADO

        case PAUSADO:

            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.2f)); // Desenha um retangulo preto em cima da tela para o texto ficar visivel

            Rectangle voltaJogo = {xReiniciar, 700, larguraVoltar, 20};
            Rectangle menu = {xMenu, 700, larguraMenu, 20};
            Rectangle vazaJogo = {xSair, 700, larguraSair, 20};
         
            DrawText("JOGO PAUSADO, ", xPausado, 300, 60, RED);
            DrawText("SELECIONE UMA OPÇÃO", xSelecione, 400, 60, RED);
            DrawText("VOLTAR AO JOGO", xVoltar, 700, 20, CheckCollisionPointRec(GetMousePosition(), voltaJogo) ? RED : WHITE);
            DrawText("IR AO MENU", xMenu, 700, 20, CheckCollisionPointRec(GetMousePosition(), menu) ? RED : WHITE);
            DrawText("SAIR DO JOGO", xSair, 700, 20, CheckCollisionPointRec(GetMousePosition(), vazaJogo) ? RED : WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), voltaJogo)) estado = JOGANDO;
                if (CheckCollisionPointRec(GetMousePosition(), menu)) estado = MENU;
                if (CheckCollisionPointRec(GetMousePosition(), vazaJogo)) CloseWindow();
            }
        break;

        // MORTO

        case MORTO:
            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(RED, 0.5f));

        faseAtual = 1;
     
            Rectangle reiniciaJogo = {xReiniciar, 700, larguraReiniciar, 20};
            Rectangle menu2 = {xMenu, 700, larguraMenu, 20};
            Rectangle vazaJogo2 = {xSair, 700, larguraSair, 20};
                        
            DrawText("VOCÊ MORREU ", xMorto - 20, 400, 70, BLACK);
            DrawText("REINICIAR JOGO", xReiniciar, 700, 20, CheckCollisionPointRec(GetMousePosition(), reiniciaJogo) ? WHITE : BLACK);
            DrawText("IR AO MENU", xMenu, 700, 20, CheckCollisionPointRec(GetMousePosition(), menu2) ? WHITE : BLACK);
            DrawText("SAIR DO JOGO", xSair, 700, 20, CheckCollisionPointRec(GetMousePosition(), vazaJogo2) ? WHITE : BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), reiniciaJogo)){                     
                reiniciarJogo(&mapa, &mario, inimigo, &numInimigos, &dificuldade, &estado);
                estado = JOGANDO;}
                if (CheckCollisionPointRec(GetMousePosition(), menu2)) {mario.ativo = true; estado = MENU;}
                if (CheckCollisionPointRec(GetMousePosition(), vazaJogo2)) CloseWindow();
            }
        break;

        // CONFIGURAÇÕES    

        case CONFIGS:
            ClearBackground(BLACK);

            if(dificuldade != FACIL){
                cortexto1 = WHITE;
            }
            if(dificuldade != NORMAL){
                cortexto2 = WHITE;
            }
            if(dificuldade != DIFICIL){
                cortexto3 = WHITE;
            }

            Rectangle facil = {xSelecione + 100, 300, larguraMorto - 110, 60};
            Rectangle medio = {xSelecione + 100, 400, larguraMorto - 100, 60};           
            Rectangle dificil = {xSelecione + 100, 500, larguraMorto - 60, 60};
            Rectangle menu3 = {xSelecione + 100, 700, larguraMorto - 110, 60};

            DrawText("CONFIGURAÇÕES:", xSelecione + 100, 150, 60, RED);
            
            if (CheckCollisionPointRec(GetMousePosition(), facil)){
                cortexto1 = RED;
            }
            else if(CheckCollisionPointRec(GetMousePosition(), medio)){
                cortexto2 = RED;
            }
            else if(CheckCollisionPointRec(GetMousePosition(), dificil)){
                cortexto3 = RED;
            }
    
            DrawText("MODO FÁCIL", xSelecione + 100, 300, 60, cortexto1);
            DrawText("MODO MÉDIO", xSelecione + 100, 400, 60, cortexto2);
            DrawText("MODO DIFÍCIL", xSelecione + 100, 500, 60, cortexto3);
            DrawText("IR AO MENU", xSelecione + 100, 700, 60, CheckCollisionPointRec(GetMousePosition(), menu3) ? RED : WHITE);

             if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), facil)){ //Muda a dificuldade do jogo, deixando a dificuldade escolhida em vermelho
                    cortexto1 = RED;
                    cortexto2 = WHITE;
                    cortexto3 = WHITE;
                    dificuldade = FACIL;
                 } 
                if (CheckCollisionPointRec(GetMousePosition(), medio)){
                    cortexto1 = WHITE;
                    cortexto2 = RED;
                    cortexto3 = WHITE;
                    dificuldade = NORMAL;
                }
                if (CheckCollisionPointRec(GetMousePosition(), dificil)){
                    cortexto1 = WHITE;
                    cortexto2 = WHITE;
                    cortexto3 = RED;
                    dificuldade = DIFICIL;
                }
                if(CheckCollisionPointRec(GetMousePosition(), menu3)){
                    estado = MENU;
                }
            }
        break;

        //RANKING

         case RANKING:

        Color bronze = { 205, 127, 50, 255 };
            ClearBackground(BLACK);
            DrawText("RANKING", (LARGURA_TELA - MeasureText("RANKING", 60)) / 2, 100, 60, RED);
            DrawRectangle(25, 190, 850, 45, GOLD);
            DrawRectangle(25, 235 + 5, 850, 45, LIGHTGRAY);
            DrawRectangle(25, 280 + 10, 850, 45, bronze);
            DrawRectangle(25, 325+ 15, 850, 45, DARKGRAY);
            DrawRectangle(25, 370+ 20, 850, 45, DARKGRAY);
            DrawRectangle(25, 415+ 25, 850, 45, DARKGRAY);
            DrawRectangle(25, 460+ 30, 850, 45, DARKGRAY);
            DrawRectangle(25, 505+ 35, 850, 45, DARKGRAY);
            DrawRectangle(25, 550+ 40, 850, 45, DARKGRAY);
            DrawRectangle(25, 640, 850, 45, DARKGRAY);

        for (int i = 0; i < 10; i++) {
            if (rank[i].nome[0] != '\0' && rank[i].tempo != 0) {
                char nomeDoCara[500];
                char tempoDemorado[500];
                sprintf(nomeDoCara, "%d. %s", i + 1, rank[i].nome);
                sprintf(tempoDemorado,"- %.2f segundos", rank[i].tempo);
                DrawText(nomeDoCara, 35, 200 + i * 50, 29, BLACK);
                DrawText(tempoDemorado, MeasureText(nomeDoCara, 29) + 50, 200 + i * 50, 29, LIME);
        }}

        DrawText("CLIQUE QUALQUER TECLA PARA VOLTAR", 230, 730, 20, RED);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || GetKeyPressed() > 0) {
                estado = MENU;
    }
    break;
        break;

        case VITORIA:
          ClearBackground(BLACK);
          DrawText("VOCÊ VENCEU", 80, ALTURA_TELA/2 - 80, 100, RED);
          DrawText("CLIQUE QUALQUER TECLA PARA IR AO MENU", 220, 480, 20, RED);
          if (GetKeyPressed() > 0){
            estado = MENU;
          }
        break;
        
    }   
EndDrawing();
} 



UnloadTexture(groundTXT);
UnloadTexture(ladderTXT);
CloseWindow();
return 0;}

void movimento(Jogador *p, Mapa *mapa) {
    CorpoFisico* cf = &(p->corpoFisico);
    andar(p);
    aplicarGravidade(cf);
    aplicarForcasMitigantes(cf, p->noChao);
    pular(p);
    mover(p, mapa);
    subirEscada(cf, mapa);
}
void andar(Jogador *p){
    float tmp = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) * p->aceleracao;
    p->corpoFisico.velX += p->noChao? tmp : tmp * razaoArChao; // Caso esteja no chão, acelerará normalmente. Se estiver no ar, irá parecer normal mas na verdade tem menos impacto
}
void pular(Jogador *p){
    if (IsKeyPressed(KEY_SPACE) && p->noChao) {
        p->corpoFisico.velY += p->forcaPulo;
        p->noChao = false;
    }
}
void mover(Jogador *p, Mapa* mapa){
    testarColisaoY(&(p->corpoFisico), &mapa->grid[0][0], TAMANHO_BLOCOS, COLUNAS, LINHAS, &(p->noChao));
    testarColisaoX(&(p->corpoFisico), &mapa->grid[0][0], TAMANHO_BLOCOS, COLUNAS);
}
void subirEscada(CorpoFisico* cf, Mapa *mapa){
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
        int celX = cf->posX / TAMANHO_BLOCOS;
        int celY = cf->posY / TAMANHO_BLOCOS;
        if (mapa->grid[celY][celX] == 'S'){
            for(int i = 0; i < LINHAS; i++){
                if (mapa->grid[celY - i][celX] == 'D'){
                    cf->posY = (celY - i) * TAMANHO_BLOCOS + cf->raio*2;
                    return;
                }
            }
        }
    } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)){
        int celX = cf->posX / TAMANHO_BLOCOS;
        int celY = cf->posY / TAMANHO_BLOCOS;
        if (mapa->grid[celY][celX] == 'D'){}
         for(int i = 0; i < LINHAS; i++){
                if (mapa->grid[celY + i][celX] == 'S'){
                    cf->posY = (celY + i) * TAMANHO_BLOCOS + cf->raio*2;
                    return;
                }
            }
    }
}

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo *estado){
    FILE *file = fopen(arquivo, "r"); //Abre o arquivo txt

    if (file == NULL) { 
    *estado = VITORIA;
    return;}

    char c;
    int linha = 0, coluna = 0;
        
    while((c = fgetc(file)) != EOF){ // Vai pegando os caracteres do arquivo e colocando na matriz 

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

        if (c == 'P') { // Se é P spawna o player 
            jogador->corpoFisico.posX = ((coluna + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            jogador->corpoFisico.posY = ((linha + 1) * TAMANHO_BLOCOS) - TAMANHO_JOGADOR;
            jogador->ativo = true;
            jogador->noChao = false;
            jogador->corpoFisico.velY = 0.0f;
        }

        if (c == 'E') { // Se é E spawna os monstros
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

void DesenharMapa(Mapa *mapa){
  for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            int x = c * TAMANHO_BLOCOS;
            int y = l * TAMANHO_BLOCOS;

            switch (mapa->grid[l][c]) { // Le a matriz e desenha os blocos em seus devidos lugares
                case 'Z':
                    //DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, LIGHTGRAY);
                    DrawTexture(groundTXT, x, y, WHITE);
                    break;
                case 'S':
                    //DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, BROWN);
                    DrawTexture(ladderTXT, x, y, WHITE);
                    break;
                case 'D':
                DrawTexture(groundTXT, x, y + TAMANHO_BLOCOS, WHITE);
                    break;
                case 'H':
                    //DrawRectangle(x, y, TAMANHO_BLOCOS, TAMANHO_BLOCOS, BROWN);
                    DrawTexture(ladderTXT, x, y, WHITE);
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

void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos, float *invencivel, Dificuldade *dif, int *numrand){

for (int i = 0; i < *numInimigos; i++) {
float proximoX = inimigo[i].x + velocidadeInimigo * inimigo[i].direcao * inimigo[i].numrand;
int gridLinhaFuturo  = (int)(inimigo[i].y + TAMANHO_JOGADOR) / TAMANHO_BLOCOS;
int gridColunaFuturo = (int)(proximoX + (TAMANHO_JOGADOR / 2)) / TAMANHO_BLOCOS;

if(*dif == DIFICIL){
    if (rand() % 150 == 0) 
    inimigo[i].numrand *= -1;

    if (mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'Z' || mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'H') {
        inimigo[i].x = proximoX;
    } else {
        inimigo[i].direcao *= -1;
        inimigo[i].numrand = 1;
    }
} 
else{
    if (mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'Z' || mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'H'){
        inimigo[i].x += velocidadeInimigo * inimigo[i].direcao;
    } 
    else {
        inimigo[i].direcao *= -1;
        inimigo[i].x += velocidadeInimigo * inimigo[i].direcao;
    }
}

if (inimigo[i].x < 0) {inimigo[i].x = 0; inimigo[i].direcao *= -1;} // Esses dois if nao deixam que visualmente os inimigos saiam da tela
if (inimigo[i].x > LARGURA_TELA - TAMANHO_JOGADOR){inimigo[i].x = LARGURA_TELA - TAMANHO_JOGADOR; inimigo[i].direcao *= -1;} 

Rectangle player = {j->corpoFisico.posX - j->corpoFisico.raio, j->corpoFisico.posY - j->corpoFisico.raio, TAMANHO_JOGADOR, TAMANHO_JOGADOR}; // Retangulos invisiveis que servem para checar a colisao
Rectangle opps = {inimigo[i].x, inimigo[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR};
 
if(CheckCollisionRecs(player, opps) && (*invencivel) <= 0){  // Tira uma vida do jogador se toca no inimigo e da um segundo de invencibilidade para ele nao morrer inta
    *invencivel += 1.0f;
    j->vidas -= 1;
}
if(j->vidas == 0){ // Se nao tem mais vidas, morre
j->ativo = false;
}}}

void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos, Dificuldade *dificuldade, EstadosJogo *estados) {
    *numInimigos = 0; // Zera tudo para nada duplicar
    *jogador = (Jogador){0};
    zerarJogador(jogador);
    *mapa = (Mapa){0};
    if(*dificuldade == FACIL) // Da as vidas ao jogador dependendo da dificuldade.
    jogador->vidas = 3;
    if(*dificuldade == NORMAL)
    jogador->vidas = 2;
    if(*dificuldade == DIFICIL)
    jogador->vidas = 1;
    LerMapa(mapa, jogador, "mapa1.txt", inimigos, numInimigos, estados); // Refaz o primeiro mapa 
}

void proximafase(Mapa *mapa, Jogador *jog, Opps *inimigos, int *numInimigos, int *faseAtual, char *numMapa, EstadosJogo *estados) {
    
    int gridLinhaMeio = (int)(jog->corpoFisico.posY + (TAMANHO_JOGADOR / 2)) / TAMANHO_BLOCOS;
    int gridColunaMeio = (int)(jog->corpoFisico.posX + (TAMANHO_JOGADOR/2)) / TAMANHO_BLOCOS;
    
    if(mapa->grid[gridLinhaMeio][gridColunaMeio] == 'F'){
    *numInimigos = 0;   
    *mapa = (Mapa){0};
    (*faseAtual)++;
    sprintf(numMapa, "mapa%d.txt", *faseAtual); // Muda o numero do mapa de acordo com a variavel faseAtual, que aumenta se tu passa de nivel
    LerMapa(mapa, jog, numMapa, inimigos, numInimigos, estados);
}}
void zerarJogador(Jogador* j){
    j->tamanho = TAMANHO_JOGADOR;
    j->forcaPulo = FORCA_PULO;
    j->aceleracao = 3;
    j->corpoFisico = criarCorpoFisico();
    j->corpoFisico.raio = j->tamanho/2;
    j->corpoFisico.massa = 10;
    j->noChao = false;
}
void carregarRanking(PLACAR rank[10]) {
    FILE *f = fopen("placar.bin", "rb");
    if (f == NULL) return; // arquivo ainda não existe, tudo bem
    fread(rank, sizeof(PLACAR), 10, f);
    fclose(f);
}
void salvarRanking(PLACAR rank[10]) {
    FILE *f = fopen("placar.bin", "wb");
    fwrite(rank, sizeof(PLACAR), 10, f);
    fclose(f);
}
void inserirRanking(PLACAR rank[10], char *nome, float tempo) {
    // acha a posição correta (menor tempo = melhor)
    int pos = -1;
    for (int i = 0; i < 10; i++) {
        if (rank[i].tempo == 0 || tempo < rank[i].tempo) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return; // não entrou no top 10

    // empurra os piores pra baixo
    for (int i = 9; i > pos; i--) {
        rank[i] = rank[i-1];
    }

    // insere na posição correta
    strncpy(rank[pos].nome, nome, 30);
    rank[pos].tempo = tempo;
    salvarRanking(rank);
}
