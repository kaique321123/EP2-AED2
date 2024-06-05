#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3

typedef int TipoChave;

typedef struct s
{
    TipoChave chave[2 * t - 1]; // num de chaves
    struct s *filhos[2 * t];    // num de filhos
    int numChaves;              // num de ch
    bool folha;                 // true se for folha
    struct s *proxFolha;        // prox folha
} NO;

typedef struct
{
    NO *raiz;
} ArvBMais;

NO *criaNo()
{
    NO *novo = NULL;
    if (!(novo = (NO *)malloc(sizeof(NO))))
    {
        printf("Memoria insuficiente!\n");
        return NULL;
    }
    for (int i = 0; i < 2 * t; i++)
    {
        novo->filhos[i] = NULL;
    }
    novo->numChaves = 0;
    novo->folha = true; // inicialmente é folha
    novo->proxFolha = NULL;

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

void inserirNaoCheio(NO *arv, int ch)
{
    int i = arv->numChaves - 1;
    if (arv->folha)
    {
        while (i >= 0 && ch < arv->chave[i])
        {
            arv->chave[i + 1] = arv->chave[i];
            i--;
        }
        arv->chave[i + 1] = ch;
        arv->numChaves++;
    }
    else
    {
        while (i >= 0 && ch < arv->chave[i])
        {
            i--;
        }
        i++;
        if (arv->filhos[i]->numChaves == 2 * t - 1)
        {
            split(arv, i);
            if (ch > arv->chave[i])
            {
                i++;
            }
        }
        inserirNaoCheio(arv->filhos[i], ch);
    }
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

void removerAux(NO *no, TipoChave chave) {
    int i = 0;

    // Encontre a chave no nó ou o filho que pode conter a chave
    while (i < no->numChaves && chave > no->chave[i]) {
        i++;
    }

    // Se a chave está neste nó
    if (i < no->numChaves && chave == no->chave[i]) {
        if (no->folha) {
            // Remova a chave e desloque as outras chaves
            for (int j = i; j < no->numChaves - 1; j++) {
                no->chave[j] = no->chave[j + 1];
            }
            no->numChaves--;
        } else {
            // Substitua a chave pela chave máxima do filho à esquerda ou pela chave mínima do filho à direita,
            // e remova a chave substituída recursivamente
            // Esta parte foi omitida por simplicidade
        }
    } else if (!no->folha) {
        // A chave pode estar no filho i
        removerAux(no->filhos[i], chave);
    }
}

void remover(ArvBMais *arv, TipoChave chave)
{
    NO *raiz = arv->raiz;
    if (raiz->numChaves == 0)
    {
        return;
    }
    if (raiz->numChaves == 1 && raiz->folha)
    {
        if (raiz->chave[0] == chave)
        {
            raiz->numChaves = 0;
        }
        return;
    }
    removerAux(raiz, chave);
}

void percorrerInOrdem(NO *no)
{
    if (no == NULL)
    {
        return;
    }

    printf("(");

    for (int i = 0; i < no->numChaves; i++)
    {
        percorrerInOrdem(no->filhos[i]);
        printf(" %d ", no->chave[i]);
    }

    percorrerInOrdem(no->filhos[no->numChaves]);

    printf(")");
}

void imprimirArvore(ArvBMais *arv)
{
    percorrerInOrdem(arv->raiz);
    printf("\n");
}

void leArquivo(const char *filename, ArvBMais *arv)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return;
    }

    char command;
    int value;
    while (fscanf(file, " %c", &command) != EOF)
    {
        switch (command)
        {
        case 'i':
            fscanf(file, "%d", &value);
            inserir(arv, value);
            break;
        case 'r':
            fscanf(file, "%d", &value);
            remover(arv, value);
            break;
        case 'p':
            imprimirArvore(arv);
            break;
        case 'f':
            fclose(file);
            return;
        default:
            printf("Unknown command: %c\n", command);
            break;
        }
    }

    fclose(file);
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <nome_arquivo_entrada>\n", argv[0]);
        return 1;
    }

    ArvBMais arv;
    iniciaArv(&arv);
    leArquivo(argv[1], &arv);

    return 0;
}