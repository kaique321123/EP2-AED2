#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3
#define MAX_CHAVE 2 * t - 1
#define MAX_FILHOS 2 * t

typedef int TipoChave;

int posSubArvore = 0;
typedef struct s
{
    TipoChave chave[MAX_CHAVE];   // num de chaves
    struct s *filhos[MAX_FILHOS]; // num de filhos
    int numChaves;                // num de chaves
    bool folha;                   // true se for folha
    struct s *proxFolha;          // prox folha
} NO;

typedef struct
{
    NO *raiz;
} ArvBMais;

bool criarArvore(ArvBMais *arv)
{
    NO *novo = (NO *)malloc(sizeof(NO));
    novo->numChaves = 0;
    novo->folha = true;
    arv->raiz = novo;
    return true;
}

void split(NO *pai, int i, NO *filho)
{

    int j;
    NO *novo;

    if (novo = (NO *)malloc(sizeof(NO)))
    {
        novo->folha = filho->folha;
        novo->numChaves = t - 1 + filho->numChaves;

        if (filho->folha)
        {
            // copia metade das chaves de filho para novo
            novo->chave[1] = filho->chave[t];

            j = 2;
        }
        else
        {
            j = 1;
        }

        for (j; j < t - 1 + filho->folha; j++)
        {
            if (filho->folha)
            {
                novo->chave[j] = filho->chave[j + t - 1];
            }
            else
            {
                pai->chave[j] = filho->chave[j + t];
            }
            if (!filho->folha)
            {
                for (j = 1; j <= t; j++)
                    novo->filhos[j] = filho->filhos[j + t];
            }
            filho->numChaves = t - 1;
            for (j = pai->numChaves + 1; j > i; j--)
                pai->filhos[j + 1] = pai->filhos[j];

            pai->filhos[i + 1] = novo;
            for (j = pai->numChaves + 1; j >= i; j--)
                pai->chave[j + 1] = pai->chave[j];
        }
        pai->chave[i] = filho->chave[t];
        pai->numChaves++;
    }
}

void imprimirArvore(ArvBMais *arv, NO *x, FILE *saida)
{
    if (x == NULL)
    {
        return;
    }

    fprintf(saida, "(");

    for (int i = 0; i < x->numChaves; i++)
    {
        imprimirArvore(arv, x->filhos[i], saida);
        fprintf(saida, " %d ", x->chave[i]);
    }

    imprimirArvore(arv, x->filhos[x->numChaves], saida);

    fprintf(saida, ")");
}

void inserirNoNaoCheio(NO *x, TipoChave k)
{
    int i = x->numChaves;

    if (x->folha)
    {
        while (i >= 1 && k < x->chave[i - 1])
        {
            x->chave[i] = x->chave[i - 1];
            i--;
        }
        x->chave[i] = k;
        x->numChaves++;
    }
    else
    {
        while (i >= 1 && k < x->chave[i - 1])
        {
            i--;
        }
        i++;
        if (x->filhos[i]->numChaves == MAX_CHAVE)
        {
            split(x, i, x->filhos[i]);
            if (k > x->chave[i - 1])
            {
                i++;
            }
        }
        inserirNoNaoCheio(x->filhos[i], k);
    }
}

void inserirArvore(ArvBMais *arv, TipoChave k)
{
    NO *r = arv->raiz;
    NO *s;
    int i;

    if (r->numChaves == MAX_CHAVE)
    {
        s = (NO *)malloc(sizeof(NO));
        s->folha = false;
        s->numChaves = 0;
        s->filhos[0] = r;
        arv->raiz = s;
        split(s, 0, r);
        inserirNoNaoCheio(s, k);
    }
    else
    {
        inserirNoNaoCheio(r, k);
    }
}

NO *encontrarPredecessor(NO *x, int i)
{
    NO *y = x->filhos[i];
    while (!y->folha)
    {
        y = y->filhos[y->numChaves];
    }
    return y;
}

NO *encontrarSucessor(NO *x, int i)
{
    NO *y = x->filhos[i + 1];
    while (!y->folha)
    {
        y = y->filhos[0];
    }
    return y;
}

int retornaPos(NO *x, int i)
{
    int j = x->numChaves;
    while (j >= 1)
    {
        if (i == x->chave[i])
        {
            return i;
        }
        j--;
    }
    return -1;
}

NO *encontraSubArv(NO *x, int i)
{
    NO *subArv = NULL;
    int j = 1;

    while (j < x->numChaves)
    {
        if (i < x->chave[i])
        {
            subArv = x->chave[i];
            posSubArvore = i;
            return subArv;
        }
        else if (i == x->chave[i] || (i > x->chave[i] && i == x->numChaves))
        {
            subArv = x->filhos[i + 1];
            posSubArvore = i + 1;
            return subArv;
        }
        else
        {
            j++;
        }
    }
}

void removeCh(NO *x, int ch)
{
    int pos = retornaPos(x, ch);
    int i;
    if (pos == x->numChaves)
    {
        x->numChaves--;
    }
    else
    {
        for (i = 0; i < x->numChaves; i++)
        {
            x->chave[i] = x->chave[i + 1];
        }
        x->numChaves--;
    }
}


void removeRaiz(ArvBMais *arv, int i)
{
    NO *raiz = arv->raiz;
    if (raiz->numChaves == 0)
    {
        return;
    }
    else
    {
        removeNo(raiz, i);
    }
    if (raiz->numChaves == 0 && !(raiz->folha))
    {
        arv->raiz = raiz->filhos[1];
        free(raiz);
    }
}

/*
Caso 1: Remoção de uma chave de uma folha
Caso 2: Redistribuição de chaves
Caso 3: Fusão de nós
Caso 4: Remoção de uma chave de um nó interno
Caso 5: Manutenção da raiz
*/

// terminar ainda
void removeNo(NO *x, int i)
{
    int pos = retornaPos(x, i);
    int j;

    // caso 1
    if (x->folha && pos == -1)
    {
        return;
    }
    // caso 2
    else if (x->folha && pos != -1)
    {
        removeCh(x, i);
    }
}

void removeArv(ArvBMais *arv, int i)
{

    bool excluido = false;
    for (int j = 1; j <= arv->raiz->numChaves; j++)
    {
        if (arv->raiz->chave[j] == i)
        {
            removeRaiz(arv, i);
            excluido = true;
        }
    }
    if (!excluido)
    {
        removeNo(arv->raiz, i);
    }
}


void leArquivo(char *arquivoEntrada, char *arquivoSaida, ArvBMais *arv)
{
    char op;
    int ch;

    FILE *entrada = fopen(arquivoEntrada, "r");
    FILE *saida = fopen(arquivoSaida, "w");

    if (entrada && criarArvore(arv))
    {
        while (fscanf(entrada, "%c %d", &op, &ch) != EOF)
        {
            switch (op)
            {
            // Inserir
            case 'i':
                inserirArvore(arv, ch);
                break;
            // Remover
            case 'r':
                removerArvore(arv, ch);
                break;
            // Buscar
            case 'p':
                imprimirArvore(arv, arv->raiz, saida);
                fprintf(saida, "\n");
                break;
            // Finalizar
            case 'f':
                fclose(entrada);
                fclose(saida);
                return;
            }
        }
    }
    fclose(entrada);
    fclose(saida);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <nome_arquivo_entrada> <nome_arquivo_saida>\n", argv[0]);
        return 1;
    }

    ArvBMais arv;
    leArquivo(argv[1], argv[2], &arv);
    return 0;
}