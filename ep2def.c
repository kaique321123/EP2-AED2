#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3
#define MAX_CHAVE 2 * t - 1
#define MAX_FILHOS 2 * t

typedef int TipoChave;

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