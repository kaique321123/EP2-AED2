#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3

typedef int TipoChave;

int posSubArvore = 0;
typedef struct s
{
    TipoChave chave[2 * t - 1]; // num de chaves
    struct s *filhos[2 * t];    // num de filhos
    int numChaves;              // num de chaves
    bool folha;                 // true se for folha
    struct s *proxFolha;        // prox folha
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

NO *encontrarPredecessor(NO *y)
{
    if (!y->folha)
        encontrarPredecessor(y->filhos[y->numChaves + 1]);
    return y;
}

NO *encontrarSucessor(NO *y)
{
    if (!y->folha)
        encontrarSucessor(y->filhos[1]);
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
            subArv = x->filhos[i];
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

void removeNo(NO *x, int k)
{
    int posicaoK = retornaPos(x, k);
    int j;

    // Caso 0
    if (x->folha && posicaoK == -1)
        return;

    // Caso 1
    else if (posicaoK != -1 && x->folha)
    {
        removeCh(x, k);
    }

    // Caso 2
    else if (!x->folha && posicaoK != -1)
    {
        NO *y = x->filhos[posicaoK];
        NO *z = x->filhos[posicaoK + 1];

        if (y->numChaves >= t)
        { // 2a
            NO *predecessor = encontrarPredecessor(y);
            x->chave[posicaoK] = predecessor->chave[predecessor->numChaves];

            if (y->folha)
            {
                NO *sucessor = encontrarSucessor(z);
                sucessor->chave[1] = x->chave[posicaoK];
                z->filhos[1] = predecessor->filhos[predecessor->numChaves + 1];
            }

            removeNo(y, predecessor->chave[predecessor->numChaves]);
        }
        else if (z->numChaves >= t)
        { // 2b

            NO *sucessor = encontrarSucessor(z);
            if (z->folha)
            {
                x->chave[posicaoK] = sucessor->chave[2];
                removeNo(sucessor, sucessor->chave[1]);
            }
            else
            {
                x->chave[posicaoK] = sucessor->chave[1];
                removeNo(sucessor, sucessor->chave[1]);
            }
        }
        else
        { // 2c
            int i = 1;
            for (i; i < z->numChaves; i++)
            {
                z->chave[i] = z->chave[i + 1];
            }
            i = 1;
            for (i; i <= z->numChaves; i++)
            {
                z->chave[i] = z->chave[i + 1];
            }
            z->numChaves--;

            y->numChaves++;
            y->chave[y->numChaves] = k;

            int posAtualZ = 1;

            y->numChaves++;
            j = y->numChaves;
            if (!y->folha)
            {
                y->chave[j] = x->chave[posicaoK];
                j = y->numChaves + 1;
            }
            while (j < 2 * t - 1)
            {
                y->chave[j] = z->chave[posAtualZ];
                j++;
                posAtualZ++;
            }

            for (j = posicaoK; j <= x->numChaves; j++)
            {
                x->chave[j] = x->chave[j + 1];
            }
            for (j = posicaoK + 1; j <= x->numChaves + 1; j++)
            {
                x->filhos[j] = x->filhos[j + 1];
            }
            x->numChaves--;

            free(z);
            removeNo(y, k);
        }
    }

    // Caso 3
    else if (!x->folha && posicaoK == -1)
    {
        int i = 1;
        int j;
        NO *subArvore = encontraSubArv(x, k);

        if (!subArvore)
            return;

        if (subArvore->numChaves == t - 1)
        {

            if (posSubArvore + 1 < x->numChaves + 1 && x->filhos[posSubArvore + 1]->numChaves >= t)
            { // 3a1
                subArvore->numChaves++;
                subArvore->chave[subArvore->numChaves] = x->chave[posSubArvore];
                subArvore->filhos[subArvore->numChaves + 1] = x->filhos[posSubArvore + 1]->filhos[1];
                int aux;
                for (aux = 1; aux < x->filhos[posSubArvore + 1]->numChaves; aux++)
                    x->filhos[posSubArvore + 1]->chave[aux] = x->filhos[posSubArvore + 1]->chave[aux + 1];
                for (aux = 1; aux <= x->filhos[posSubArvore + 1]->numChaves; aux++)
                    x->filhos[posSubArvore + 1]->filhos[aux + 1] = x->filhos[posSubArvore + 1]->filhos[aux];
                x->filhos[posSubArvore + 1]->numChaves--;
                x->chave[posSubArvore] = x->filhos[posSubArvore + 1]->chave[1];
            }
            else if (posSubArvore > 1 && x->filhos[posSubArvore - 1]->numChaves >= t)
            { // 3a2
                int ant = x->filhos[posSubArvore - 1]->chave[x->filhos[posSubArvore - 1]->numChaves];
                x->chave[posSubArvore - 1] = ant;
                x->filhos[posSubArvore - 1]->numChaves--;
                subArvore->numChaves++;
                int aux;
                for (aux = subArvore->numChaves; aux >= 1; aux--)
                    subArvore->chave[aux] = subArvore->chave[aux - 1];
                if (x->filhos[posSubArvore + 1])
                    subArvore->chave[1] = x->chave[posSubArvore - 1];
                aux = subArvore->numChaves + 1;
                for (aux; aux <= 2; aux--)
                    subArvore->filhos[aux] = subArvore->filhos[aux - 1];

                if (x->filhos[posSubArvore + 1])
                    subArvore->filhos[1] = x->filhos[posSubArvore - 1]->filhos[x->filhos[posSubArvore - 1]->numChaves + 1];
            }
            else if (x->filhos[posSubArvore - 1]->numChaves == t - 1 && x->filhos[posSubArvore + 1]->numChaves == t - 1)
            { // 3b
                if (posSubArvore - 1 >= 1)
                {
                    int numArvAnterior = x->filhos[posSubArvore - 1]->numChaves;
                    int auxAtual = numArvAnterior + 1;
                    int auxProx = 1;

                    x->filhos[posSubArvore - 1]->numChaves += subArvore->numChaves;

                    while (auxProx <= subArvore->numChaves)
                    {
                        x->filhos[posSubArvore - 1]->chave[auxAtual] = subArvore->chave[auxProx];
                        auxAtual++;
                        auxProx++;
                    }
                    auxAtual = numArvAnterior;
                    auxProx = 1;
                    while (auxProx + 1 <= subArvore->numChaves + 1)
                    {
                        x->filhos[posSubArvore - 1]->filhos[auxAtual] = subArvore->filhos[auxProx];
                        auxProx++;
                        auxAtual++;
                    }
                    auxAtual = posSubArvore - 1;
                    while (auxAtual + 1 <= x->numChaves)
                    {
                        x->chave[auxAtual] = x->chave[auxAtual + 1];
                        auxAtual++;
                    }
                    auxAtual = posSubArvore;
                    while (auxAtual + 1 <= x->numChaves + 1)
                    {
                        x->filhos[auxAtual] = x->filhos[auxAtual + 1];
                        auxAtual++;
                    }
                    x->numChaves--;
                    x->filhos[posSubArvore - 1]->numChaves--;
                }
                else if (posSubArvore + 1 <= x->numChaves + 1)
                {
                    int auxProx = 1;
                    int numChavesSubArvoreOriginal = subArvore->numChaves;
                    int auxAtual = subArvore->numChaves;
                    subArvore->numChaves += x->filhos[posSubArvore + 1]->numChaves;
                    while (auxProx <= x->filhos[posSubArvore + 1]->numChaves)
                    {
                        subArvore->chave[auxAtual] = x->filhos[posSubArvore + 1]->chave[auxProx];
                        auxAtual++;
                        auxProx++;
                    }
                    auxAtual = numChavesSubArvoreOriginal + 1;
                    auxProx = 1;
                    while (auxProx + 1 <= subArvore->numChaves + 1)
                    {
                        subArvore->filhos[auxAtual] = x->filhos[posSubArvore + 1]->filhos[auxProx];
                        auxProx++;
                        auxAtual++;
                    }

                    auxAtual = posSubArvore;
                    while (auxAtual + 1 <= x->numChaves)
                    {
                        x->chave[auxAtual] = x->chave[auxAtual + 1];
                        auxAtual++;
                    }
                    auxAtual = posSubArvore + 1;
                    while (auxAtual + 1 <= x->numChaves + 1)
                    {
                        x->filhos[auxAtual] = x->filhos[auxAtual + 1];
                        auxAtual++;
                    }
                    x->numChaves--;
                    subArvore->numChaves--;
                }
            }
            else
                return;
        }
        removeNo(subArvore, k);
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
                removeArv(arv, ch);
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