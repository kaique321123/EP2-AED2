#include <stdio.h>
#include <stdlib.h>


#define t 3

typedef int TipoChave;

typedef struct s
{
    TipoChave chave[2 * t - 1]; // num de epaços no vetor
    struct s *filhos[2 * t];    // num de ponteiros
    int numChaves;              // num de ch armazenadas neste nó
    bool folha;                 // true se nao tem mais filhos
    struct s *proxFolha;        // aponta para a folha irma a direita
} NO;

typedef struct
{
    NO *raiz;
} ArvBMais;

NO *criaNo()
{
    NO *novo = null;
   if (!(novo = (NO *)malloc(sizeof(NO))))
    {
        printf("Memoria insuficiente!\n");
        return NULL;
    }
    for(int i = 0; i < 2*t; i++)
    {
        novo->filhos[i] = NULL;
        novo->numChaves = 0;
        novo->folha = true; // inicialmente é folha
        novo->proxFolha = NULL;
    }

    return novo;
}
void iniciaArv(ArvBMais *arv)
{
    arv->raiz = criaNo();
}

void split(NO *arv, int i)
{
    NO *novo = criaNo();
    NO *filho = arv->filhos[i];
    novo->folha = filho->folha;
    novo->numChaves = t - 1;

    for (int j = 0; j < t - 1; j++)
    {
        novo->chave[j] = filho->chave[j + t];
    }

    if (!filho->folha)
    {
        for (int j = 0; j < t; j++)
        {
            novo->filhos[j] = filho->filhos[j + t];
        }
    }

    filho->numChaves = t - 1;

    for (int j = arv->numChaves; j > i; j--)
    {
        arv->filhos[j + 1] = arv->filhos[j];
    }

    arv->filhos[i + 1] = novo;

    for (int j = arv->numChaves - 1; j >= i; j--)
    {
        arv->chave[j + 1] = arv->chave[j];
    }

    arv->chave[i] = filho->chave[t - 1];
    arv->numChaves++;
}


void inserirNaoCheio(NO *arv, int ch){
    int i = arv->numChaves; //iterador

    if(arv->folha){ // se for folha
        while((i > 0) && (ch < arv->chave[i-1])){
            arv->chave[i] = arv->chave[i-1];
            i--;
        }
        arv->chave[i] = ch;
        arv->numChaves++;
}
    else{
        while((i > 0) && (ch < arv->chave[i-1])){
            i--;
        }
        if(arv->filhos[i]->numChaves == 2*t-1){
            split(arv, i);
            if(ch > arv->chave[i]){
                i++;
            }
        }
        inserirNaoCheio(arv->filhos[i], ch); //buscas o lugar correto
    }





void inserir(ArvBMais *arv, TipoChave chave)
{
    NO *raiz = arv->raiz;
    if (raiz->numChaves == 2 * t - 1)
    {
        NO *novo = criaNo();
        novo->folha = false;
        novo->filhos[0] = raiz;
        arv->raiz = novo;
        split(novo, 0);
        inserirNaoCheio(novo, chave);
    }
    else
    {
        inserirNaoCheio(raiz, chave);
    }
}


void main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Uso: %s <nome_arquivo_entrada> <nome_arquivo_saida>\n", argv[0]);
        return 1;
    }
}
