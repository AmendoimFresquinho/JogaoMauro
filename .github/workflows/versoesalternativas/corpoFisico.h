#ifndef CORPO_FISICO
#define CORPO_FISICO

typedef struct {
    float posX, posY;
    float velX, velY;
    float massa, raio; // Em geral, raio = tamanho/2
} CorpoFisico;


#define CF_GRAVIDADE 1.4f   

#define CF_RESISTENCIA_AR 0.1f
#define CF_ATRITO_CHAO 0.4f
static const float razaoArChao = CF_RESISTENCIA_AR/CF_ATRITO_CHAO;

void testarColisaoX(CorpoFisico *cf, char* gridP, int tamanhoCelula, int sX);
void testarColisaoY(CorpoFisico *cf, char* gridP, int tamanhoCelula, int sX, int sY);
void aplicarForcasMitigantes(CorpoFisico *cf, bool noChao);
void aplicarGravidade(CorpoFisico *cf);

#endif