#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
float velocidadeInimigo;

Texture2D groundTXT, ladderTXT, portalTXT;
Sound fx, fx2, fx3, fx4, fx5, fx6, fx7;
Anim portalANIM;

int main(){

    InitWindow(LARGURA_TELA, ALTURA_TELA, "DKINF");
    SetTargetFPS(60);
    // Inicia as structs zeradas (para evitar lixo de memória) ou no "default"

    Jogador mario = {0};
    Mapa mapa = {0};
    Dificuldade dificuldade = NORMAL;
    Opps inimigo = {0};
    Torre torres[MAX_INIMIGOS] = {0};
    EstadosJogo estado = MENU;

    mario.tamanho = TAMANHO_JOGADOR;
    mario.aceleracao = 5;
    mario.corpoFisico = criarCorpoFisico();
    mario.corpoFisico.raio = mario.tamanho / 2;
    mario.corpoFisico.massa = 10;
    char numMapa[20];
    Color cortexto1 = WHITE;
    Color cortexto2 = RED;
    Color cortexto3 = WHITE;
    float tempoInvencivel = 2.0f;
    int numInimigos = 0, faseAtual = 1, numrandom = 1, letraNome = 0;
    double tempoInicio = 0, tempoFinal = 0;
    char nomeJogador[31] = {};
    PLACAR rank[10] = {};

    srand(time(NULL));

    // Inicializa o primeiro mapa

    sprintf(numMapa, "mapa%d.txt", faseAtual);
    LerMapa(&mapa, &mario, numMapa, &inimigo, &numInimigos, &estado, &tempoFinal, &tempoInicio);

    carregarRanking(rank); // Só puxa uma vez, pois o que muda vai para o arquivo, mas não precisa ler toda vez do arquivo, pode só atualizar direto na memória, mas tem que ler no começo senão ele não lembra se fechamos o jogo

    // Carrega as texturas
    Image groundIMG = LoadImage("Sprites/ground.png");
    ImageResize(&groundIMG, 30, 30);
    groundTXT = LoadTextureFromImage(groundIMG);
    UnloadImage(groundIMG);

    Image ladderIMG = LoadImage("Sprites/ladder.png");
    ImageResize(&ladderIMG, 30, 30);
    ladderTXT = LoadTextureFromImage(ladderIMG);
    UnloadImage(ladderIMG);

    portalANIM.lar = 60; portalANIM.alt = 60;
    portalANIM.atual = 0; portalANIM.totframes = 4;
    portalANIM.delay = 0.2; portalANIM.ultimo = 0;
    Image portalIMG = LoadImage("Sprites/portal.png");
    ImageResize(&portalIMG, portalANIM.lar * portalANIM.totframes, portalANIM.alt);
    portalTXT = LoadTextureFromImage(portalIMG);
    UnloadImage(portalIMG);

    InitAudioDevice();
    Sound fx = LoadSound("Sons/Pulo.wav");
    SetSoundVolume(fx, 0.1f /10);
    Sound fx2 = LoadSound("Sons/Click.wav");
    SetSoundVolume(fx2, 0.5f);
    Sound fx3 = LoadSound("Sons/Morreu.mp3");
    SetSoundVolume(fx3, 0.1f);
    Sound fx4 = LoadSound("Sons/Dano.wav");
    SetSoundVolume(fx4, 0.2f);
    Sound fx5 = LoadSound("Sons/LevelUp.wav");
    SetSoundVolume(fx5, 0.1f);
    Sound fx6 = LoadSound("Sons/Andar.mp3");
    SetSoundVolume(fx6, 1.3f);
    Sound fx7 = LoadSound("Sons/Power.wav");
    SetSoundVolume(fx7, 0.1f);
    
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
    int xMorto = (LARGURA_TELA - larguraMorto) / 2;
    int xReiniciar = 50;

    while (!WindowShouldClose()){

        proxFrame(&portalANIM);

        BeginDrawing();

        if (!mario.ativo)
            estado = MORTO;
        estado == JOGANDO ? HideCursor() : ShowCursor();

        switch (estado){
        case MENU:
            caseMenu(&estado, &mapa, &mario, &inimigo, &numInimigos, &dificuldade, &faseAtual, &tempoInicio, &tempoFinal, fx2, fx3, fx6);
            break;
        case JOGANDO:
            caseJogando(&estado, &mapa, &mario, &inimigo, &numInimigos, &dificuldade, &tempoInvencivel, &faseAtual, numMapa, &tempoInicio, &tempoFinal, &numrandom, fx, fx4, fx5, fx6);
            powerUp(&mario, &mapa, fx7);
            break;
        case PAUSADO:
            casePausado(&estado, fx2, fx6);
            break;
        case MORTO:
            caseMorto(&estado, &mapa, &mario, &inimigo, &numInimigos, &dificuldade, &faseAtual, &tempoInicio, &tempoFinal, fx2, fx3);
            break;
        case CONFIGS:
            caseConfigs(&estado, &dificuldade, &cortexto1, &cortexto2, &cortexto3, fx2);
            break;
        case RANKING:
            caseRanking(&estado, rank);
            break;
        case VITORIA:
            caseVitoria(&estado, rank, &tempoFinal, nomeJogador, &letraNome, fx6);
            break;
        }

        EndDrawing();
    }
    UnloadSound(fx);
    UnloadSound(fx2);
    UnloadSound(fx3);
    UnloadSound(fx4);
    UnloadSound(fx5);
    UnloadSound(fx6);
    UnloadSound(fx7);

    UnloadTexture(groundTXT);
    UnloadTexture(ladderTXT);
    UnloadTexture(portalTXT);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void movimento(Jogador *p, Mapa *mapa, Sound fx){
    CorpoFisico *cf = &(p->corpoFisico);
    andar(p);
    aplicarGravidade(cf);
    aplicarForcasMitigantes(cf, p->noChao);
    pular(p, fx);
    mover(p, mapa);
    subirEscada(cf, mapa);
}
void andar(Jogador *p){
    float tmp = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) * p->aceleracao;
    p->corpoFisico.velX += p->noChao ? tmp : tmp * razaoArChao; // Caso esteja no chão, acelerará normalmente. Se estiver no ar, irá parecer normal mas na verdade tem menos impacto
}
void pular(Jogador *p, Sound fx){
    if (IsKeyPressed(KEY_SPACE) && p->noChao){
        p->corpoFisico.velY += p->forcaPulo;
        p->noChao = false;
        PlaySound(fx);
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
        }
        for (int i = 0; i < LINHAS; i++){
            if (mapa->grid[celY + i][celX] == 'S'){
                cf->posY = (celY + i) * TAMANHO_BLOCOS + cf->raio * 2;
                return;
            }
        }
    }
}

void LerMapa(Mapa *mapa, Jogador *jogador, const char *arquivo, Opps *inimigo, int *numInimigos, EstadosJogo *estado, double* tempoFinal, double* tempoInicio){
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
            torre->maxCooldown = 5; torre->minCooldown = 2.5;
            torre->tempoProx = 5;
            torre->ativo = true;
            (*numInimigos)++;
         }

         if (c == 't') {
            Torre* torre = &inimigo->torres[*numInimigos];
            torre->tipo = TORRE;
            torre->celX = coluna;
            torre->celY = linha;
            torre->dir = -1;
            torre->maxCooldown = 5; torre->minCooldown = 2.5;
            torre->tempoProx = 5;
            torre->ativo = true;
            (*numInimigos)++;
         }
    }

    coluna++;
    }
}
fclose(file);
}

void DesenharMapa(Mapa *mapa){
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
                Rectangle rect = {portalANIM.atual * portalANIM.lar, 0, portalANIM.lar, portalANIM.alt};
                DrawTextureRec(portalTXT, rect, pos, WHITE);
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
void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos, float *invencivel, Dificuldade *dif, int *numrand, Sound fx4){
    for (int i = 0; i < *numInimigos; i++){
        float proximoX = inimigo->fogos[i].x + velocidadeInimigo * inimigo->fogos[i].direcao * inimigo->fogos[i].numrand;
        int gridLinhaFuturo = (int)(inimigo->fogos[i].y + TAMANHO_JOGADOR) / TAMANHO_BLOCOS;
        int gridColunaFuturo = (int)(proximoX + (TAMANHO_JOGADOR / 2)) / TAMANHO_BLOCOS;

        if (*dif == DIFICIL){
            if (rand() % 150 == 0){
                inimigo->fogos[i].numrand *= -1;
            }
            if (mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'Z' || mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'H'){
                inimigo->fogos[i].x = proximoX;
            }
            else{
                inimigo->fogos[i].direcao *= -1;
                inimigo->fogos[i].numrand = 1;
            }
        }
        else{
            if (mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'Z' || mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'H'){
                inimigo->fogos[i].x += velocidadeInimigo * inimigo->fogos[i].direcao;
            }
            else{
                inimigo->fogos[i].direcao *= -1;
                inimigo->fogos[i].x += velocidadeInimigo * inimigo->fogos[i].direcao;
            }
        }

        if (inimigo->fogos[i].x < 0){
            inimigo->fogos[i].x = 0;
            inimigo->fogos[i].direcao *= -1;
        } // Esses dois if nao deixam que visualmente os inimigos saiam da tela
        if (inimigo->fogos[i].x > LARGURA_TELA - TAMANHO_JOGADOR){
            inimigo->fogos[i].x = LARGURA_TELA - TAMANHO_JOGADOR;
            inimigo->fogos[i].direcao *= -1;
        }

        Rectangle player = {j->corpoFisico.posX - j->corpoFisico.raio, j->corpoFisico.posY - j->corpoFisico.raio, TAMANHO_JOGADOR, TAMANHO_JOGADOR}; // Retangulos invisiveis que servem para checar a colisao
        Rectangle opps = {inimigo->fogos[i].x, inimigo->fogos[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR};

        if (CheckCollisionRecs(player, opps) && (*invencivel) <= 0){ // Tira uma vida do jogador se toca no inimigo e da um segundo de invencibilidade para ele nao morrer inta
            *invencivel += 1.0f;
            j->vidas -= 1;
            if (j->vidas > 0)
            {
                PlaySound(fx4);
            }
        }
        if (j->vidas == 0){ // Se nao tem mais vidas, morre
            j->ativo = false;
        }
    }
}

void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos, Dificuldade *dificuldade, EstadosJogo *estados, double* tempoFinal, double* tempoInicio) {
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
    LerMapa(mapa, jogador, "mapa1.txt", inimigos, numInimigos, estados, tempoFinal, tempoInicio); // Refaz o primeiro mapa 
}

void proximafase(Mapa *mapa, Jogador *jog, Opps *inimigos, int *numInimigos, int *faseAtual, char *numMapa, EstadosJogo *estados, double *tempoFinal, double *tempoInicio, Sound fx5, Dificuldade *dificuldade){

    int gridLinhaMeio = (int)(jog->corpoFisico.posY + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;
    int gridColunaMeio = (int)(jog->corpoFisico.posX + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;

    if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'F'){
        if (*dificuldade == FACIL)
        velocidadeInimigo = 3.0f;
        if (*dificuldade == NORMAL)
        velocidadeInimigo = 4.0f;
        if (*dificuldade == DIFICIL)
        velocidadeInimigo = 6.0f;
        *numInimigos = 0;
        *mapa = (Mapa){0};
        (*faseAtual)++;
        PlaySound(fx5);
        sprintf(numMapa, "mapa%d.txt", *faseAtual); // Muda o numero do mapa de acordo com a variavel faseAtual, que aumenta se tu passa de nivel
        LerMapa(mapa, jog, numMapa, inimigos, numInimigos, estados, tempoFinal, tempoInicio);
    }
}
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
void powerUp(Jogador *j, Mapa *mapa, Sound fx7){
int gridLinhaMeio = (int)(j->corpoFisico.posY + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;
int gridColunaMeio = (int)(j->corpoFisico.posX + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;

    if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'K'){
        velocidadeInimigo = 1.0f;
        PlaySound(fx7);
        mapa->grid[gridLinhaMeio][gridColunaMeio] = ' ';
    }
      if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'V'){
        j->vidas++;
        PlaySound(fx7);
        mapa->grid[gridLinhaMeio][gridColunaMeio] = ' ';
    }
}

void caseMenu(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound fx2, Sound fx3, Sound fx6) {
ClearBackground(BLACK);
    StopSound(fx3);
    StopSound(fx6);
    *faseAtual = 1;

    // Calcula tamanhos e posições dos textos
    int larguraMorto = MeasureText("VOCÊ MORREU, ", 60);
    int larguraSelecione = MeasureText("SELECIONE UMA OPÇÃO", 60);
    int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;
    int xMorto = (LARGURA_TELA - larguraMorto) / 2;

    // Define as áreas clicáveis de cada botão
    Rectangle iniciaJogo = {xSelecione + 95, 400, larguraMorto - 60, 60};
    Rectangle configs = {xSelecione + 95, 500, larguraMorto + 40, 60};
    Rectangle ranking = {xSelecione + 95, 600, larguraMorto - 160, 60};
    Rectangle sairJogo = {xSelecione + 95, 700, larguraMorto - 40, 60};

    // Desenha os fundos dos botões
    DrawRectangleRounded(iniciaJogo, 0.1, 1, DARKGRAY);
    DrawRectangleRounded(configs, 0.1, 1, DARKGRAY);
    DrawRectangleRounded(ranking, 0.1, 1, DARKGRAY);
    DrawRectangleRounded(sairJogo, 0.1, 1, DARKGRAY);

    // Desenha os textos — fica vermelho se o mouse estiver em cima
    DrawText("BEM VINDO AO DKINF!", xSelecione + 25, 100, 60, RED);
    DrawText("OPÇÕES:", xMorto + 100, 200, 60, RED);
    DrawText("INICIAR JOGO", xSelecione + 100, 400, 60, CheckCollisionPointRec(GetMousePosition(), iniciaJogo) ? RED : WHITE);
    DrawText("CONFIGURAÇÕES", xSelecione + 100, 500, 60, CheckCollisionPointRec(GetMousePosition(), configs) ? RED : WHITE);
    DrawText("RANKINGS", xSelecione + 100, 600, 60, CheckCollisionPointRec(GetMousePosition(), ranking) ? RED : WHITE);
    DrawText("SAIR DO JOGO", xSelecione + 100, 700, 60, CheckCollisionPointRec(GetMousePosition(), sairJogo) ? RED : WHITE);

    // Define a velocidade dos inimigos de acordo com a dificuldade
    if (*dificuldade == FACIL)
        velocidadeInimigo = 3.0f;
    if (*dificuldade == NORMAL)
        velocidadeInimigo = 4.0f;
    if (*dificuldade == DIFICIL)
        velocidadeInimigo = 6.0f;

    // Processa os cliques
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (CheckCollisionPointRec(GetMousePosition(), iniciaJogo)){
            reiniciarJogo(mapa, mario, inimigo, numInimigos, dificuldade, estado, tempoFinal, tempoInicio);
            *tempoInicio = GetTime();
            *tempoFinal = 0;
            *estado = JOGANDO;
            PlaySound(fx2);}
        if (CheckCollisionPointRec(GetMousePosition(), configs)){
            PlaySound(fx2);
            *estado = CONFIGS;}
        if (CheckCollisionPointRec(GetMousePosition(), ranking)){
            PlaySound(fx2);
            *estado = RANKING;}
        if (CheckCollisionPointRec(GetMousePosition(), sairJogo)){
            PlaySound(fx2);
            CloseWindow();}
    }
}

void caseJogando(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, float *tempoInvencivel, int *faseAtual, char *numMapa, double *tempoInicio, double *tempoFinal, int *numrandom, Sound fx, Sound fx4, Sound fx5, Sound fx6){

    ClearBackground(BLACK);
    StopSound(fx3);

    // Pausa com TAB
    if (IsKeyPressed(KEY_TAB))
        *estado = PAUSADO;

    // Conta o tempo de invencibilidade após tomar dano
    if (*tempoInvencivel > 0)
        *tempoInvencivel -= GetFrameTime();
    else
        *tempoInvencivel = 0;

    // Som de andar
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)){
        if (!IsSoundPlaying(fx6))
            PlaySound(fx6);
    }
    else{
        if (IsSoundPlaying(fx6))
            StopSound(fx6);
    }

    // HUD de vidas
    char texto[50];
    sprintf(texto, "VIDAS: %d", mario->vidas);
    DrawText(texto, 10, 10, 30, RED);

    // Atualiza lógica
    Inimigos(mapa, mario, inimigo, numInimigos, tempoInvencivel, dificuldade, numrandom, fx4);
    movimento(mario, mapa, fx);
    proximafase(mapa, mario, inimigo, numInimigos, faseAtual, numMapa, estado, tempoFinal, tempoInicio, fx5, dificuldade);

    // Desenha o mapa, o jogador e os inimigos
    DesenharMapa(mapa);
    DrawRectangle(mario->corpoFisico.posX - mario->corpoFisico.raio, mario->corpoFisico.posY - mario->corpoFisico.raio, mario->tamanho, mario->tamanho, BLUE);

    for (int i = 0; i < *numInimigos; i++){
        if (inimigo->fogos[i].ativo)
            DrawRectangle(inimigo->fogos[i].x, inimigo->fogos[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
        else if (inimigo->torres[i].ativo){
            DrawRectangle((inimigo->torres[i].celX) * TAMANHO_BLOCOS, (inimigo->torres[i].celY) * TAMANHO_BLOCOS, TAMANHO_JOGADOR, TAMANHO_JOGADOR, GREEN);
            Torre_atirar(inimigo->torres[i], mario->corpoFisico, &mapa->grid[0][0], TAMANHO_BLOCOS, COLUNAS);
        }
    }
}

void casePausado(EstadosJogo *estado, Sound fx2, Sound fx6){
StopSound(fx6);

    // Retângulo semitransparente por cima do jogo
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.2f));

    // Calcula posições dos textos
    int larguraVoltar = MeasureText("VOLTAR AO JOGO", 20);
    int larguraMenu = MeasureText("IR AO MENU", 20);
    int larguraSair = MeasureText("SAIR DO JOGO", 20);
    int larguraPausado = MeasureText("JOGO PAUSADO, ", 60);
    int larguraSelecione = MeasureText("SELECIONE UMA OPÇÃO", 60);
    int xVoltar = 50;
    int xMenu = (LARGURA_TELA - larguraMenu) / 2;
    int xSair = LARGURA_TELA - larguraSair - 50;
    int xPausado = (LARGURA_TELA - larguraPausado) / 2;
    int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;

    Rectangle voltaJogo = {xVoltar, 700, larguraVoltar, 20};
    Rectangle menu = {xMenu, 700, larguraMenu, 20};
    Rectangle vazaJogo = {xSair, 700, larguraSair, 20};

    DrawText("JOGO PAUSADO, ", xPausado, 300, 60, RED);
    DrawText("SELECIONE UMA OPÇÃO", xSelecione, 400, 60, RED);
    DrawText("VOLTAR AO JOGO", xVoltar, 700, 20, CheckCollisionPointRec(GetMousePosition(), voltaJogo) ? RED : WHITE);
    DrawText("IR AO MENU", xMenu, 700, 20, CheckCollisionPointRec(GetMousePosition(), menu) ? RED : WHITE);
    DrawText("SAIR DO JOGO", xSair, 700, 20, CheckCollisionPointRec(GetMousePosition(), vazaJogo) ? RED : WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (CheckCollisionPointRec(GetMousePosition(), voltaJogo)){
            *estado = JOGANDO;
            PlaySound(fx2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), menu)){
            *estado = MENU;
            PlaySound(fx2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), vazaJogo)){
            PlaySound(fx2);
            CloseWindow();
        }
    }
}

void caseMorto(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound fx2, Sound fx3){

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(RED, 0.5f));

    if (mario->vidas == 0)
        PlaySound(fx3);

    *faseAtual = 1;
    mario->vidas = 1;

    int larguraReiniciar = MeasureText("REINICIAR JOGO", 20);
    int larguraMenu = MeasureText("IR AO MENU", 20);
    int larguraSair = MeasureText("SAIR DO JOGO", 20);
    int larguraMorto = MeasureText("VOCÊ MORREU, ", 60);
    int xReiniciar = 50;
    int xMenu = (LARGURA_TELA - larguraMenu) / 2;
    int xSair = LARGURA_TELA - larguraSair - 50;
    int xMorto = (LARGURA_TELA - larguraMorto) / 2;

    Rectangle reiniciaJogo = {xReiniciar, 700, larguraReiniciar, 20};
    Rectangle menu2 = {xMenu, 700, larguraMenu, 20};
    Rectangle vazaJogo2 = {xSair, 700, larguraSair, 20};

    DrawText("VOCÊ MORREU", xMorto - 20, 400, 70, BLACK);
    DrawText("REINICIAR JOGO", xReiniciar, 700, 20, CheckCollisionPointRec(GetMousePosition(), reiniciaJogo) ? WHITE : BLACK);
    DrawText("IR AO MENU", xMenu, 700, 20, CheckCollisionPointRec(GetMousePosition(), menu2) ? WHITE : BLACK);
    DrawText("SAIR DO JOGO", xSair, 700, 20, CheckCollisionPointRec(GetMousePosition(), vazaJogo2) ? WHITE : BLACK);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (CheckCollisionPointRec(GetMousePosition(), reiniciaJogo)){
            reiniciarJogo(mapa, mario, inimigo, numInimigos, dificuldade, estado, tempoFinal, tempoInicio);
            *tempoInicio = GetTime();
            *tempoFinal = 0;
            PlaySound(fx2);
            StopSound(fx3);
            *estado = JOGANDO;
        }
        if (CheckCollisionPointRec(GetMousePosition(), menu2)){
            mario->ativo = true;
            *estado = MENU;
            PlaySound(fx2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), vazaJogo2)){
            PlaySound(fx2);
            CloseWindow();
        }
    }
}

void caseConfigs(EstadosJogo *estado, Dificuldade *dificuldade, Color *cortexto1, Color *cortexto2, Color *cortexto3, Sound fx2){
ClearBackground(BLACK);

    // A dificuldade atual fica vermelha, as outras ficam brancas
    if (*dificuldade != FACIL)
        *cortexto1 = WHITE;
    if (*dificuldade != NORMAL)
        *cortexto2 = WHITE;
    if (*dificuldade != DIFICIL)
        *cortexto3 = WHITE;

    int larguraMorto = MeasureText("VOCÊ MORREU, ", 60);
    int larguraSelecione = MeasureText("SELECIONE UMA OPÇÃO", 60);
    int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;

    Rectangle facil = {xSelecione + 100, 300, larguraMorto - 110, 60};
    Rectangle medio = {xSelecione + 100, 400, larguraMorto - 100, 60};
    Rectangle dificil = {xSelecione + 100, 500, larguraMorto - 60, 60};
    Rectangle menu3 = {xSelecione + 100, 700, larguraMorto - 110, 60};

    DrawText("CONFIGURAÇÕES:", xSelecione + 100, 150, 60, RED);

    // Hover — fica vermelho ao passar o mouse
    if (CheckCollisionPointRec(GetMousePosition(), facil))
        *cortexto1 = RED;
    else if (CheckCollisionPointRec(GetMousePosition(), medio))
        *cortexto2 = RED;
    else if (CheckCollisionPointRec(GetMousePosition(), dificil))
        *cortexto3 = RED;

    DrawText("MODO FÁCIL", xSelecione + 100, 300, 60, *cortexto1);
    DrawText("MODO MÉDIO", xSelecione + 100, 400, 60, *cortexto2);
    DrawText("MODO DIFÍCIL", xSelecione + 100, 500, 60, *cortexto3);
    DrawText("IR AO MENU", xSelecione + 100, 700, 60, CheckCollisionPointRec(GetMousePosition(), menu3) ? RED : WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (CheckCollisionPointRec(GetMousePosition(), facil)){
            *cortexto1 = RED;
            *cortexto2 = WHITE;
            *cortexto3 = WHITE;
            *dificuldade = FACIL;
            PlaySound(fx2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), medio)){
            *cortexto1 = WHITE;
            *cortexto2 = RED;
            *cortexto3 = WHITE;
            *dificuldade = NORMAL;
            PlaySound(fx2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), dificil)){
            *cortexto1 = WHITE;
            *cortexto2 = WHITE;
            *cortexto3 = RED;
            *dificuldade = DIFICIL;
            PlaySound(fx2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), menu3)){
            *estado = MENU;
            PlaySound(fx2);
        }
    }
}

void caseRanking(EstadosJogo *estado, PLACAR rank[10]){

    Color bronze = {205, 127, 50, 255};
    ClearBackground(BLACK);

    DrawText("RANKING", (LARGURA_TELA - MeasureText("RANKING", 60)) / 2, 100, 60, RED);

    // Fundo de cada posição do ranking (ouro, prata, bronze, cinza)
    DrawRectangle(25, 190, 850, 45, GOLD);
    DrawRectangle(25, 235 + 5, 850, 45, LIGHTGRAY);
    DrawRectangle(25, 280 + 10, 850, 45, bronze);
    DrawRectangle(25, 325 + 15, 850, 45, DARKGRAY);
    DrawRectangle(25, 370 + 20, 850, 45, DARKGRAY);
    DrawRectangle(25, 415 + 25, 850, 45, DARKGRAY);
    DrawRectangle(25, 460 + 30, 850, 45, DARKGRAY);
    DrawRectangle(25, 505 + 35, 850, 45, DARKGRAY);
    DrawRectangle(25, 550 + 40, 850, 45, DARKGRAY);
    DrawRectangle(25, 640, 850, 45, DARKGRAY);

    // Desenha cada entrada do ranking que estiver preenchida
    for (int i = 0; i < 10; i++){
        if (rank[i].nome[0] != '\0' && rank[i].tempo != 0){
            char nomeDoCara[500];
            char tempoDemorado[500];
            sprintf(nomeDoCara, "%d. %s", i + 1, rank[i].nome);
            sprintf(tempoDemorado, "- %.2f segundos", rank[i].tempo);
            DrawText(nomeDoCara, 35, 200 + i * 50, 29, BLACK);
            DrawText(tempoDemorado, MeasureText(nomeDoCara, 29) + 50, 200 + i * 50, 29, LIME);
        }
    }
    DrawText("CLIQUE QUALQUER TECLA PARA VOLTAR", 230, 730, 20, RED);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || GetKeyPressed() > 0){
        *estado = MENU;
    }
}

void caseVitoria(EstadosJogo *estado, PLACAR rank[10], double *tempoFinal, char *nomeJogador, int *letraNome, Sound fx6){
    ClearBackground(BLACK);
    StopSound(fx6);

    char textoTempo[50];
    sprintf(textoTempo, "SEU TEMPO: %.2f segundos", *tempoFinal);

    DrawText("VOCÊ VENCEU!", 65, 200, 100, RED);
    DrawText(textoTempo, 260, 310, 30, BLUE);
    DrawText("DIGITE SEU NOME:", 300, 500, 30, WHITE);
    Rectangle nomes = {130, 540, 645, 50};
    DrawRectangleRounded(nomes, 0.1, 1, DARKGRAY);
    DrawText(nomeJogador, 140, 550, 30, GREEN);
    DrawText("PRESSIONE ENTER PARA CONFIRMAR", 255, 605, 20, RED);

  // Lê uma letra por frame
    int letra = GetCharPressed();
    if (letra > 0 && *letraNome < 30){
        nomeJogador[*letraNome] = (char)letra;
        (*letraNome)++;
        nomeJogador[*letraNome] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE) && *letraNome > 0){ // Backspace apaga a última letra
        (*letraNome)--;
        nomeJogador[*letraNome] = '\0';
    }
    if (IsKeyPressed(KEY_ENTER) && *letraNome > 0){ // Enter confirma e vai pro ranking
        inserirRanking(rank, nomeJogador, (float)*tempoFinal);
        nomeJogador[0] = '\0';
        *letraNome = 0;
        *estado = RANKING;
    }
}

void proxFrame(Anim* anim){
    float tmp = GetTime();
    if (tmp - anim->ultimo >= anim->delay){
        int qntFrames = (int) ((tmp - anim->ultimo) / anim->delay);
        printf("%d\n", qntFrames);
        anim->atual = (anim->atual + qntFrames) % anim->totframes;
        anim->ultimo = tmp;
    }
    
}
