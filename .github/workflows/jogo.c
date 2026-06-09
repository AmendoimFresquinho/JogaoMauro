#include "raylib.h"
#include "DEFINICOES.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
float velocidadeInimigo;
Texture2D groundTXT, ladderTXT, FUNDOTXT;
Sound Pular, Clique, Morrer, Dano, LevelUp, Andar, Poder;
Font Menu, Titulo, Letra_pequena;

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
    mario.aceleracao = ACELERACAO;
    mario.corpoFisico = criarCorpoFisico();
    mario.corpoFisico.raio = TAMANHO_JOGADOR / 2;
    mario.corpoFisico.massa = MASSA;
    char numMapa[20];
    Color cortexto1 = WHITE;
    Color cortexto2 = GRAY;
    Color cortexto3 = WHITE;
    float tempoInvencivel = TEMPO_INVENCIVEL;
    int numInimigos = 0, faseAtual = 1, numrandom = 1, letraNome = 0;
    double tempoInicio = 0, tempoFinal = 0, tempoPausado = 0;
    char nomeJogador[21] = {};
    PLACAR rank[10] = {};

    srand(time(NULL));

    // Inicializa o primeiro mapa

    sprintf(numMapa, "mapa%d.txt", faseAtual);
    LerMapa(&mapa, &mario, numMapa, inimigo, &numInimigos, &estado, &tempoFinal, &tempoInicio);

    carregarRanking(rank); // Só puxa uma vez, pois o que muda vai para o arquivo, mas não precisa ler toda vez do arquivo, pode só atualizar direto na memória, mas tem que ler no começo senão ele não lembra se fechamos o jogo

    // Carrega as texturas
    Image groundIMG = LoadImage("Sprites/ground.png");
    ImageResize(&groundIMG, TAMANHO_BLOCOS, TAMANHO_BLOCOS);
    groundTXT = LoadTextureFromImage(groundIMG);
    UnloadImage(groundIMG);

    Image ladderIMG = LoadImage("Sprites/ladder.png");
    ImageResize(&ladderIMG, TAMANHO_BLOCOS, TAMANHO_BLOCOS);
    ladderTXT = LoadTextureFromImage(ladderIMG);
    UnloadImage(ladderIMG);

    Image fundoIMG = LoadImage("Sprites/jogo.png");
    ImageResize(&fundoIMG, LARGURA_TELA + 10, ALTURA_TELA);
    FUNDOTXT = LoadTextureFromImage(fundoIMG);
    UnloadImage(fundoIMG);

    Font Menu = LoadFontEx("Fontes/menu.ttf", 60, 0, 0);
    Font Menu_Maior = LoadFontEx("Fontes/menu.ttf", 100, 0, 0);
    Font Titulo = LoadFontEx("Fontes/titulo.ttf", 150, 0, 0);
    Font Letra_pequena = LoadFontEx("Fontes/Pequena.otf", 40, 0, 0);

    InitAudioDevice();
    Sound Pular = LoadSound("Sons/Pulo.wav");
    SetSoundVolume(Pular, 0.1f);
    Sound Clique = LoadSound("Sons/Click.wav");
    Sound Morrer = LoadSound("Sons/Morreu.mp3");
    Sound Dano = LoadSound("Sons/Dano.wav");
    Sound LevelUp = LoadSound("Sons/LevelUp.wav");
    SetSoundVolume(LevelUp, 0.3f);
    Sound Andar = LoadSound("Sons/Andar.mp3");
    SetSoundVolume(Andar, 1.3f);
    Sound Poder = LoadSound("Sons/Power.wav");
    
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
        BeginDrawing();

        if (!mario.ativo)
            estado = MORTO;
        estado == JOGANDO ? HideCursor() : ShowCursor();

        switch (estado){
        case MENU:
            caseMenu(&estado, &mapa, &mario, inimigo, &numInimigos, &dificuldade, &faseAtual, &tempoInicio, &tempoFinal, Clique, Morrer, Andar, Menu, Titulo);
            break;
        case JOGANDO:
            caseJogando(&estado, &mapa, &mario, inimigo, &numInimigos, &dificuldade, &tempoInvencivel, &faseAtual, numMapa, &tempoInicio, &tempoFinal, &numrandom, Pular, Dano, LevelUp, Andar, &tempoPausado);
            powerUp(&mario, &mapa, Poder);
            char TEMPOAGORA[50];
            sprintf(TEMPOAGORA, "TEMPO: %.2f", GetTime() - tempoInicio);
            DrawText(TEMPOAGORA, 10, 50, 30, BLUE);
            break;
        case PAUSADO:
            casePausado(&estado, Clique, Andar, &tempoPausado, &tempoInicio, Titulo, Menu);
            break;
        case MORTO:
            caseMorto(&estado, &mapa, &mario, inimigo, &numInimigos, &dificuldade, &faseAtual, &tempoInicio, &tempoFinal, Clique, Morrer);
            break;
        case CONFIGS:
            caseConfigs(&estado, &dificuldade, &cortexto1, &cortexto2, &cortexto3, Clique, Menu_Maior, Titulo);
            break;
        case RANKING:
            caseRanking(&estado, rank, Clique, Titulo, Letra_pequena);
            break;
        case VITORIA:
            caseVitoria(&estado, rank, &tempoFinal, nomeJogador, &letraNome, Andar, Letra_pequena);
            break;
        }

        EndDrawing();
    }
    UnloadSound(Pular);
    UnloadSound(Clique);
    UnloadSound(Morrer);
    UnloadSound(Dano);
    UnloadSound(LevelUp);
    UnloadSound(Andar);
    UnloadSound(Poder);
    UnloadFont(Menu);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void Inimigos(Mapa *mapa, Jogador *j, Opps *inimigo, int *numInimigos, float *invencivel, Dificuldade *dif, int *numrand, Sound Dano){
    for (int i = 0; i < *numInimigos; i++){
        float proximoX = inimigo[i].x + velocidadeInimigo * inimigo[i].direcao * inimigo[i].numrand;
        int gridLinhaFuturo = (int)(inimigo[i].y + TAMANHO_JOGADOR) / TAMANHO_BLOCOS;
        int gridColunaFuturo = (int)(proximoX + (TAMANHO_JOGADOR / 2)) / TAMANHO_BLOCOS;

        if (*dif == DIFICIL){
            if (rand() % 150 == 0){
                inimigo[i].numrand *= -1;
            }
            if (mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'Z' || mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'H'){
                inimigo[i].x = proximoX;
            }
            else{
                inimigo[i].direcao *= -1;
                inimigo[i].numrand = 1;
            }
        }
        else{
            if (mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'Z' || mapa->grid[gridLinhaFuturo][gridColunaFuturo] == 'H'){
                inimigo[i].x += velocidadeInimigo * inimigo[i].direcao;
            }
            else{
                inimigo[i].direcao *= -1;
                inimigo[i].x += velocidadeInimigo * inimigo[i].direcao;
            }
        }

        if (inimigo[i].x < 0){
            inimigo[i].x = 0;
            inimigo[i].direcao *= -1;
        } // Esses dois if nao deixam que visualmente os inimigos saiam da tela
        if (inimigo[i].x > LARGURA_TELA - TAMANHO_JOGADOR){
            inimigo[i].x = LARGURA_TELA - TAMANHO_JOGADOR;
            inimigo[i].direcao *= -1;
        }

        Rectangle player = {j->corpoFisico.posX - j->corpoFisico.raio, j->corpoFisico.posY - j->corpoFisico.raio, TAMANHO_JOGADOR, TAMANHO_JOGADOR}; // Retangulos invisiveis que servem para checar a colisao
        Rectangle opps = {inimigo[i].x, inimigo[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR};

        if (CheckCollisionRecs(player, opps) && (*invencivel) <= 0){ // Tira uma vida do jogador se toca no inimigo e da um segundo de invencibilidade para ele nao morrer inta
            *invencivel += TEMPO_INVENCIVEL;
            j->vidas -= 1;
            if (j->vidas > 0){
                PlaySound(Dano);
            }
        }
        if (j->vidas == 0){ // Se nao tem mais vidas, morre
            j->ativo = false;
        }
    }
}

void reiniciarJogo(Mapa *mapa, Jogador *jogador, Opps *inimigos, int *numInimigos, Dificuldade *dificuldade, EstadosJogo *estados, double *tempoFinal, double *tempoInicio){
    *numInimigos = 0; // Zera tudo para nada duplicar
    *jogador = (Jogador){0};
    zerarJogador(jogador, dificuldade);
    *mapa = (Mapa){0};
    if (*dificuldade == FACIL) // Da as vidas ao jogador dependendo da dificuldade.
        jogador->vidas = VIDAS_FACIL;
    if (*dificuldade == NORMAL)
        jogador->vidas = VIDAS_MEDIO;
    if (*dificuldade == DIFICIL)
        jogador->vidas = VIDAS_DIFICIL;
    LerMapa(mapa, jogador, "mapa1.txt", inimigos, numInimigos, estados, tempoInicio, tempoFinal); // Refaz o primeiro mapa
}

void proximafase(Mapa *mapa, Jogador *jog, Opps *inimigos, int *numInimigos, int *faseAtual, char *numMapa, EstadosJogo *estados, double *tempoFinal, double *tempoInicio, Sound LevelUp, Dificuldade *dificuldade){

    int gridLinhaMeio = (int)(jog->corpoFisico.posY + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;
    int gridColunaMeio = (int)(jog->corpoFisico.posX + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;

    if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'F'){
        if (*dificuldade == FACIL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_FACIL;
        if (*dificuldade == NORMAL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_MEDIO;
        if (*dificuldade == DIFICIL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_DIFICIL;
        *numInimigos = 0;
        *mapa = (Mapa){0};
        (*faseAtual)++;
        PlaySound(LevelUp);
        sprintf(numMapa, "mapa%d.txt", *faseAtual); // Muda o numero do mapa de acordo com a variavel faseAtual, que aumenta se tu passa de nivel
        LerMapa(mapa, jog, numMapa, inimigos, numInimigos, estados, tempoFinal, tempoInicio);
    }
}
void zerarJogador(Jogador *j, Dificuldade *dificuldade){
    j->tamanho = TAMANHO_JOGADOR;
    j->forcaPulo = FORCA_PULO;
    j->aceleracao = 3;
    j->corpoFisico = criarCorpoFisico();
    j->corpoFisico.raio = j->tamanho / 2;
    j->corpoFisico.massa = 10;
    j->noChao = false;
     if (*dificuldade == FACIL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_FACIL;
        if (*dificuldade == NORMAL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_MEDIO;
        if (*dificuldade == DIFICIL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_DIFICIL;
}
void powerUp(Jogador *j, Mapa *mapa, Sound Poder){
    int gridLinhaMeio = (int)(j->corpoFisico.posY + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;
    int gridColunaMeio = (int)(j->corpoFisico.posX + ((TAMANHO_JOGADOR / 2) - 10)) / TAMANHO_BLOCOS;

    if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'K'){
        PlaySound(Poder);
        velocidadeInimigo = VELOCIDADE_INIMIGOS_LENTO;
        mapa->grid[gridLinhaMeio][gridColunaMeio] = ' ';
    }
      if (mapa->grid[gridLinhaMeio][gridColunaMeio] == 'V'){
        PlaySound(Poder);
        j->vidas++;
        mapa->grid[gridLinhaMeio][gridColunaMeio] = ' ';
    }
}
void caseMenu(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound Clique, Sound Morrer, Sound Andar, Font Menu, Font Titulo) {
    ClearBackground(BLACK);
    StopSound(Morrer);
    StopSound(Andar);
    *faseAtual = 1;
    DrawTexture(FUNDOTXT, 0, 0, Fade(WHITE, 0.4f));

    // Define as áreas clicáveis de cada botão
    Rectangle iniciaJogo = {X_MENU_OPCOES - 5, Y_OPCAO_JOGAR, MeasureTextEx(Menu, "INICIAR JOGO", 60, 3.0f).x + 10, 60};
    Rectangle configs = {X_MENU_OPCOES - 5, Y_OPCAO_CONFIGS, MeasureTextEx(Menu, "CONFIGURACOES", 60, 3.0f).x + 10, 60};
    Rectangle ranking = {X_MENU_OPCOES - 5, Y_OPCAO_RANKING, MeasureTextEx(Menu, "RANKINGS", 60, 3.0f).x + 10, 60};
    Rectangle sairJogo = {X_MENU_OPCOES - 5, Y_OPCAO_SAIR, MeasureTextEx(Menu, "SAIR DO JOGO", 60, 3.0f).x + 10, 60};
    Rectangle titulo = {X_MENU_TITULO - 5, Y_TITULO, MeasureTextEx(Menu, "DKINF", 100, 3.0f).x + 10, 100};

    if(CheckCollisionPointRec(GetMousePosition(), iniciaJogo))
        DrawRectangleRec(iniciaJogo, Fade(DARKGRAY, 0.8));
    else if(CheckCollisionPointRec(GetMousePosition(), configs))
        DrawRectangleRec(configs, Fade(DARKGRAY, 0.8f));
    else if(CheckCollisionPointRec(GetMousePosition(), ranking))
          DrawRectangleRec(ranking, Fade(DARKGRAY, 0.8f));  
    else if(CheckCollisionPointRec(GetMousePosition(), sairJogo))
        DrawRectangleRec(sairJogo, Fade(DARKGRAY, 0.8f)); 

    DrawTextEx(Titulo, "DKINF", (Vector2){ X_MENU_TITULO, Y_TITULO }, 150, 3.0f, WHITE);
    DrawTextEx(Menu, "INICIAR JOGO", (Vector2){ X_MENU_OPCOES, Y_OPCAO_JOGAR }, 60, 3.0f, CheckCollisionPointRec(GetMousePosition(), iniciaJogo) ? WHITE : LIGHTGRAY);
    DrawTextEx(Menu, "CONFIGURACOES", (Vector2){ X_MENU_OPCOES, Y_OPCAO_CONFIGS }, 60, 3.0f, CheckCollisionPointRec(GetMousePosition(), configs) ? WHITE : LIGHTGRAY);
    DrawTextEx(Menu, "RANKINGS", (Vector2){ X_MENU_OPCOES, Y_OPCAO_RANKING }, 60, 3.0f, CheckCollisionPointRec(GetMousePosition(), ranking) ? WHITE : LIGHTGRAY);
    DrawTextEx(Menu, "SAIR DO JOGO", (Vector2){ X_MENU_OPCOES, Y_OPCAO_SAIR }, 60, 3.0f, CheckCollisionPointRec(GetMousePosition(), sairJogo) ? WHITE : LIGHTGRAY);

    // Define a velocidade dos inimigos de acordo com a dificuldade
    if (*dificuldade == FACIL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_FACIL;
    if (*dificuldade == NORMAL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_MEDIO;
    if (*dificuldade == DIFICIL)
        velocidadeInimigo = VELOCIDADE_INIMIGOS_DIFICIL;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (CheckCollisionPointRec(GetMousePosition(), iniciaJogo)){
            reiniciarJogo(mapa, mario, inimigo, numInimigos, dificuldade, estado, tempoFinal, tempoInicio);
            *tempoInicio = GetTime();
            *tempoFinal = 0;
            *estado = JOGANDO;
            PlaySound(Clique);}
        if (CheckCollisionPointRec(GetMousePosition(), configs)){
            PlaySound(Clique);
            *estado = CONFIGS;}
        if (CheckCollisionPointRec(GetMousePosition(), ranking)){
            PlaySound(Clique);
            *estado = RANKING;}
        if (CheckCollisionPointRec(GetMousePosition(), sairJogo)){
            PlaySound(Clique);
            CloseWindow();}
    }
}

void caseJogando(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, float *tempoInvencivel, int *faseAtual, char *numMapa, double *tempoInicio, double *tempoFinal, int *numrandom, Sound Pular, Sound Dano, Sound LevelUp, Sound Andar, double *tempoPausado){

    ClearBackground(BLACK);
    StopSound(Morrer);

    // Pausa com TAB
    if (IsKeyPressed(KEY_TAB)) {
        *tempoPausado = GetTime();
        *estado = PAUSADO;
    }

    // Conta o tempo de invencibilidade após tomar dano
    if (*tempoInvencivel > 0)
        *tempoInvencivel -= GetFrameTime();
    else
        *tempoInvencivel = 0;

    // Som de andar
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)){
        if (!IsSoundPlaying(Andar))
            PlaySound(Andar);
    }
    else{
        if (IsSoundPlaying(Andar))
            StopSound(Andar);
    }

    // HUD de vidas
    char texto[50];
    sprintf(texto, "VIDAS: %d", mario->vidas);
    DrawText(texto, 10, 10, 30, RED);

    // Atualiza lógica
    Inimigos(mapa, mario, inimigo, numInimigos, tempoInvencivel, dificuldade, numrandom, Dano);
    movimento(mario, mapa, Pular);
    proximafase(mapa, mario, inimigo, numInimigos, faseAtual, numMapa, estado, tempoFinal, tempoInicio, LevelUp, dificuldade);

    // Desenha o mapa, o jogador e os inimigos
    DesenharMapa(mapa, groundTXT, ladderTXT);
    DrawRectangle(mario->corpoFisico.posX - mario->corpoFisico.raio, mario->corpoFisico.posY - mario->corpoFisico.raio, mario->tamanho, mario->tamanho, BLUE);

    for (int i = 0; i < *numInimigos; i++){
        if (inimigo[i].ativo)
            DrawRectangle(inimigo[i].x, inimigo[i].y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
    }
}

void casePausado(EstadosJogo *estado, Sound Clique, Sound Andar, double *tempoPausado, double *tempoInicio, Font Titulo, Font Menu){
    StopSound(Andar);

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, BLACK);
    // Calcula posições dos textos
    int larguraVoltar = MeasureTextEx(Menu, "VOLTAR AO JOGO", 40, 3.0f).x + 10;
    int larguraMenu = MeasureTextEx(Menu, "IR AO MENU", 40, 3.0f).x + 10;
    int larguraSair = MeasureTextEx(Menu, "SAIR DO JOGO", 40, 3.0f).x + 10;
    int larguraPausado = MeasureText("JOGO PAUSADO, ", 60);
    int larguraSelecione = MeasureText("SELECIONE UMA OPÇÃO", 60);
    int xVoltar = 50;
    int xMenu = (LARGURA_TELA - larguraMenu) / 2;
    int xSair = LARGURA_TELA - larguraSair - 50;
    int xPausado = (LARGURA_TELA - larguraPausado) / 2;
    int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;

    Rectangle voltaJogo = {xVoltar - 5, 700, larguraVoltar, 40};
    Rectangle menu = {xMenu - 5, 700, larguraMenu, 40};
    Rectangle vazaJogo = {xSair - 5, 700, larguraSair, 40};

    if(CheckCollisionPointRec(GetMousePosition(), voltaJogo))
        DrawRectangleRec(voltaJogo, Fade(DARKGRAY, 0.8));
    else if(CheckCollisionPointRec(GetMousePosition(), menu))
        DrawRectangleRec(menu, Fade(DARKGRAY, 0.8f));
    else if(CheckCollisionPointRec(GetMousePosition(), vazaJogo))
          DrawRectangleRec(vazaJogo, Fade(DARKGRAY, 0.8f));  

    DrawText("JOGO PAUSADO, ", xPausado, 300, 60, RED);
    DrawText("SELECIONE UMA OPÇÃO", xSelecione, 400, 60, RED);
    DrawTextEx(Menu, "VOLTAR AO JOGO", (Vector2) { xVoltar, 700}, 40, 3.0f, CheckCollisionPointRec(GetMousePosition(), voltaJogo) ? WHITE : LIGHTGRAY);
    DrawTextEx(Menu, "IR AO MENU", (Vector2) { xMenu, 700 }, 40, 3.0f, CheckCollisionPointRec(GetMousePosition(), menu) ? WHITE : LIGHTGRAY);
    DrawTextEx(Menu, "SAIR DO JOGO", (Vector2) { xSair, 700 }, 40, 3.0f, CheckCollisionPointRec(GetMousePosition(), vazaJogo) ? WHITE : LIGHTGRAY);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_TAB)){
        if (CheckCollisionPointRec(GetMousePosition(), voltaJogo) || IsKeyPressed(KEY_TAB)){
            *tempoInicio += GetTime() - *tempoPausado; // compensa o tempo pausado
            *estado = JOGANDO;
            PlaySound(Clique);
        }
        if (CheckCollisionPointRec(GetMousePosition(), menu) && !(IsKeyPressed(KEY_TAB))){
            *estado = MENU;
            PlaySound(Clique);
        }
        if (CheckCollisionPointRec(GetMousePosition(), vazaJogo) && !(IsKeyPressed(KEY_TAB))){
            PlaySound(Clique);
            CloseWindow();
        }
    }
}

void caseMorto(EstadosJogo *estado, Mapa *mapa, Jogador *mario, Opps *inimigo, int *numInimigos, Dificuldade *dificuldade, int *faseAtual, double *tempoInicio, double *tempoFinal, Sound Clique, Sound Morrer){

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(RED, 0.5f));

    if (mario->vidas == 0)
        PlaySound(Morrer);

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
            PlaySound(Clique);
            StopSound(Morrer);
            *estado = JOGANDO;
        }
        if (CheckCollisionPointRec(GetMousePosition(), menu2)){
            mario->ativo = true;
            *estado = MENU;
            PlaySound(Clique);
        }
        if (CheckCollisionPointRec(GetMousePosition(), vazaJogo2)){
            PlaySound(Clique);
            CloseWindow();
        }
    }
}

void caseConfigs(EstadosJogo *estado, Dificuldade *dificuldade, Color *cortexto1, Color *cortexto2, Color *cortexto3, Sound Clique, Font Menu, Font titulo){
ClearBackground(BLACK);

    if (*dificuldade != FACIL)
        *cortexto1 = WHITE;
    if (*dificuldade != NORMAL)
        *cortexto2 = WHITE;
    if (*dificuldade != DIFICIL)
        *cortexto3 = WHITE;

    int larguraSelecione = MeasureText("SELECIONE UMA OPÇÃO", 60);
    int xSelecione = (LARGURA_TELA - larguraSelecione) / 2;

    Rectangle facil = {xSelecione + 100, 300, MeasureTextEx(Menu, "MODO FACIL", 100, 3.0f).x, 80};
    Rectangle medio = {xSelecione + 100, 400, MeasureTextEx(Menu, "MODO MEDIO", 100, 3.0f).x, 80};
    Rectangle dificil = {xSelecione + 100, 500, MeasureTextEx(Menu, "MODO DIFICIL", 100, 3.0f).x, 80};
    Rectangle menu = {(LARGURA_TELA - MeasureTextEx(Menu, "IR AO MENU", 60, 3.0f).x) / 2, 700, MeasureTextEx(Menu, "IR AO MENU", 60, 3.0f).x, 60};
    Rectangle Desc_facil = {xSelecione + 95, 380, 355, 95};
    Rectangle Desc_medio = {xSelecione + 95, 480, 555, 95};
    Rectangle Desc_dificl = {xSelecione + 95, 580, 565, 135};

    DrawTextEx(titulo, "CONFIGURACOES:", (Vector2){xSelecione + 100, 150}, 120, 3.0f, RED);

    if (CheckCollisionPointRec(GetMousePosition(), facil))
        *cortexto1 = GREEN;
    else if (CheckCollisionPointRec(GetMousePosition(), medio))
        *cortexto2 = GRAY;
    else if (CheckCollisionPointRec(GetMousePosition(), dificil))
        *cortexto3 = RED;

    DrawTextEx(Menu, "MODO FACIL", (Vector2){xSelecione + 100, 300}, 100, 3.0f, *cortexto1);
    DrawTextEx(Menu, "MODO MEDIO", (Vector2){xSelecione + 100, 400}, 100, 3.0f, *cortexto2);
    DrawTextEx(Menu, "MODO DIFICIL", (Vector2){xSelecione + 100, 500}, 100, 3.0f, *cortexto3);
    DrawTextEx(Menu, "IR AO MENU", (Vector2){(LARGURA_TELA - MeasureTextEx(Menu, "IR AO MENU", 60, 3.0f).x) / 2, 700}, 60, 3.0f, CheckCollisionPointRec(GetMousePosition(), menu) ? RED : WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), facil)){
        DrawRectangleRounded(Desc_facil, 0.1f, 1, DARKGRAY);
        DrawTextEx(Menu, "3 Vidas", (Vector2){xSelecione + 110, 390}, 35, 3.0f, GREEN);
        DrawTextEx(Menu, "Inimigos: Velocidade Lenta", (Vector2){xSelecione + 110, 430}, 35, 3.0f, GREEN);
    }
    else if (CheckCollisionPointRec(GetMousePosition(), medio)){
        DrawRectangleRounded(Desc_medio, 0.1f, 1, DARKGRAY);
        DrawTextEx(Menu, "2 Vidas", (Vector2){xSelecione + 110, 490}, 35, 3.0f, LIGHTGRAY);
        DrawTextEx(Menu, "Inimigos: Velocidade similar a do jogador", (Vector2){xSelecione + 110, 530}, 35, 3.0f, LIGHTGRAY);
    }
    else if (CheckCollisionPointRec(GetMousePosition(), dificil)){
        DrawRectangleRounded(Desc_dificl, 0.1f, 1, DARKGRAY);
        DrawTextEx(Menu, "1 Vida", (Vector2){xSelecione + 110, 590}, 35, 3.0f, RED);
        DrawTextEx(Menu, "Inimigos: Velocidade Rapida", (Vector2){xSelecione + 110, 630}, 35, 3.0f, RED);
        DrawTextEx(Menu, "Inimigos aleatoriamente mudam de direcao", (Vector2){xSelecione + 110, 670}, 35, 3.0f,RED);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (CheckCollisionPointRec(GetMousePosition(), facil)){
            *cortexto1 = GREEN;
            *cortexto2 = WHITE;
            *cortexto3 = WHITE;
            *dificuldade = FACIL;
            PlaySound(Clique);
        }
        if (CheckCollisionPointRec(GetMousePosition(), medio)){
            *cortexto1 = WHITE;
            *cortexto2 = GRAY;
            *cortexto3 = WHITE;
            *dificuldade = NORMAL;
            PlaySound(Clique);
        }
        if (CheckCollisionPointRec(GetMousePosition(), dificil)){
            *cortexto1 = WHITE;
            *cortexto2 = WHITE;
            *cortexto3 = RED;
            *dificuldade = DIFICIL;
            PlaySound(Clique);
        }
        if (CheckCollisionPointRec(GetMousePosition(), menu)){
            *estado = MENU;
            PlaySound(Clique);
        }
    }
}

void caseRanking(EstadosJogo *estado, PLACAR rank[10], Sound Clique, Font Titulo, Font Nomes){

    Color BRONZE = {205, 127, 50, 255};
    ClearBackground(BLACK);

    DrawTextEx(Titulo, "RANKING", (Vector2){40 + MeasureTextEx(Titulo, "RANKING", 110, 3.0f).x, 90 }, 120, 3.0f, RED);

    // Fundo de cada posição do ranking (ouro, prata, bronze, cinza)
    DrawRectangle(160, 200, 600 - 10, 45, GOLD);
    DrawRectangle(160, 235 + 15, 600 - 10, 45, LIGHTGRAY);
    DrawRectangle(160, 280 + 20, 600 - 10, 45, BRONZE);
    DrawRectangle(160, 325 + 25, 600 - 10, 45, DARKGRAY);
    DrawRectangle(160, 370 + 30, 600 - 10, 45, DARKGRAY);
    DrawRectangle(160, 415 + 35, 600 - 10, 45, DARKGRAY);
    DrawRectangle(160, 460 + 40, 600 - 10, 45, DARKGRAY);
    DrawRectangle(160, 505 + 45, 600 - 10, 45, DARKGRAY);
    DrawRectangle(160, 550 + 50, 600 - 10, 45, DARKGRAY);
    DrawRectangle(160, 650, 600 - 10, 45, DARKGRAY);

    // Desenha cada entrada do ranking que estiver preenchida
    for (int i = 0; i < 10; i++){
        if (rank[i].nome[0] != '\0' && rank[i].tempo != 0){
            char nomeDoCara[50];
            char tempoDemorado[50];
            sprintf(nomeDoCara, "%d. %s", i + 1, rank[i].nome);
            sprintf(tempoDemorado, "- %.2f Segundos", rank[i].tempo);
            DrawTextEx(Nomes, nomeDoCara, (Vector2){175, 202 + i * 50}, 40, 3.0f, BLACK);
            DrawTextEx(Nomes, tempoDemorado, (Vector2){MeasureTextEx(Nomes, nomeDoCara, 40, 3.0f).x + 185, 202 + i * 50}, 40, 3.0f, BLUE);
        }
    }
    DrawText("CLIQUE QUALQUER TECLA PARA VOLTAR", 230, 710, 20, RED);
    if (GetKeyPressed() > 0){
        *estado = MENU;
    }
    int larguraZerar = MeasureText("ZERAR RANKING", 20);
    Rectangle botaoZerar = {LARGURA_TELA - larguraZerar - 20, ALTURA_TELA - 40, larguraZerar, 20};

    DrawText("ZERAR RANKING", LARGURA_TELA - larguraZerar - 20, ALTURA_TELA - 40, 20, 
         CheckCollisionPointRec(GetMousePosition(), botaoZerar) ? RED : DARKGRAY);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), botaoZerar)) {
            zerarRanking(rank);
            PlaySound(Clique);
        }
    }
}

void caseVitoria(EstadosJogo *estado, PLACAR rank[10], double *tempoFinal, char *nomeJogador, int *letraNome, Sound Andar, Font Nomes){
    ClearBackground(BLACK);
    StopSound(Andar);

    char textoTempo[50];
    sprintf(textoTempo, "SEU TEMPO: %.2f segundos", *tempoFinal);

    DrawText("VOCÊ VENCEU!", 65, 200, 100, RED);
    DrawText(textoTempo, 260, 310, 30, BLUE);
    DrawText("DIGITE SEU NOME:", 310, 500, 30, WHITE);
    Rectangle nomes = {260, 540, 380, 50};
    DrawRectangleRounded(nomes, 0.1, 1, DARKGRAY);
    DrawTextEx(Nomes, nomeJogador, (Vector2){ 270, 545 }, 40, 3.0f, GREEN);
    DrawText("PRESSIONE ENTER PARA CONFIRMAR", 255, 605, 20, RED);

  // Lê uma letra por frame
    int letra = GetCharPressed();
    if (letra > 0 && *letraNome < 20){
        nomeJogador[*letraNome] = (char)letra;
        (*letraNome)++;
        nomeJogador[*letraNome] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE) && *letraNome > 0){ // Backspace apaga a última letra
        (*letraNome)--;
        nomeJogador[*letraNome]  = '\0';
    }
    if (IsKeyPressed(KEY_ENTER) && *letraNome > 0){ // Enter confirma e vai pro ranking
        inserirRanking(rank, nomeJogador, (float)*tempoFinal);
        nomeJogador[0] = '\0';
        *letraNome = 0;
        *estado = RANKING;
    }
}
void corrigirPosicao(Jogador *j){
    if(j->corpoFisico.posX < 0){
        j->corpoFisico.posX = 0;
    }
}
