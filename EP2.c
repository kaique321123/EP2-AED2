#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3

typedef int TipoChave;
int posicaoSubArvore = 0;
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


int main(int argc, char *argv[]); //feito
void leArquivo(ArvBMais* arv, char arquivoEntrada[], char arquivoSaida[]); //feito
void imprimeArvore(ArvBMais* arv, NO* x, FILE* saida); //feito
bool criaArvore(ArvBMais* arv); //feito
void splitFilho(NO* x, int i, NO* y); //feito
void insereNaoCheia(NO* x, int k); //feito
void insereArvore(ArvBMais* arv, int k); //feito
NO* achaPredecessor(NO* y); //feito
NO* achaSucessor(NO* y); //feito
int retornaPosicaoK(NO* x, int k); //feito
NO* encontraSubArvore(NO* x, int k); //feito
void removeChave(NO* x, int k); //feito
void removeNo(NO* x, int k); //tem que terminar
void removeRaiz(ArvBMais* arv, int k); //feito
void removeArvore(ArvBMais* arv, int k); //feito



NO *criaNo()
{
    NO *novo = (NO *)malloc(sizeof(NO));
    if (!novo)
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

bool criarArvore(ArvBMais *arv)
{
    NO *novo = criaNo();
    if (!novo)
        return false;
    novo->folha = true;
    novo->numChaves = 0;
    arv->raiz = novo;
    return true;
}

NO *encontraSucessor(NO *s)
{
    if (!s->folha)
    {
        return encontraSucessor(s->filhos[0]);
    }
    return s;
}

NO *encontraPredecessor(NO *p)
{
    if (!p->folha)
    {
        return encontraPredecessor(p->filhos[p->numChaves]);
    }
    return p;
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

void inserirArvore(ArvBMais *arv, int i)
{
    NO *raiz = arv->raiz;

    if (raiz->numChaves == 2 * t - 1)
    {
        NO *novo = criaNo();
        novo->folha = false;
        novo->filhos[0] = raiz;
        arv->raiz = novo;
        split(novo, 0);
        inserirNaoCheio(novo, i);
    }
    else
    {
        inserirNaoCheio(raiz, i);
    }
}

int retornarPos(NO *x, int k)
{
    int i = 0;
    while (i < x->numChaves && k > x->chave[i])
    {
        i++;
    }
    return i;
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

void imprimirArvore(ArvBMais *arv, NO *x, FILE *saida)
{
    if (x != NULL)
    {
        fprintf(saida, "(");
        for (int i = 0; i < x->numChaves; i++)
        {
            imprimirArvore(arv, x->filhos[i], saida);
            fprintf(saida, " %d ", x->chave[i]);
        }
        imprimirArvore(arv, x->filhos[x->numChaves], saida);
        fprintf(saida, ")");
    }
}

NO *encontraSubArvore(NO *x, int k)
{
    int i = 0;
    while (i < x->numChaves && k > x->chave[i])
    {
        i++;
    }
    if (i < x->numChaves && k == x->chave[i])
    {
        posicaoSubArvore = i;
        return x;
    }
    if (x->folha)
    {
        return NULL;
    }
    return encontraSubArvore(x->filhos[i], k);
}


void removerChave(NO *x, int k)
{
    int i = 0;
    while (i < x->numChaves && k > x->chave[i])
    {
        i++;
    }
    if (i < x->numChaves && k == x->chave[i])
    {
        for (int j = i; j < x->numChaves - 1; j++)
        {
            x->chave[j] = x->chave[j + 1];
        }
        x->numChaves--;
    }
}

void removeNo(NO *x, int k)
{
    int posicaoK = retornarPos(x, k);
    int j;

    // caso 0
    if (x->folha && posicaoK == -1)
        return;

    // caso 1
    else if (posicaoK != -1 && x->folha)
    {
        removerChave(x, k);
    }

    // caso 2
    else if (!x->folha && posicaoK != -1)
    {
        NO *y = x->filhos[posicaoK];
        NO *z = x->filhos[posicaoK + 1];

        if (y->numChaves >= t)
        { // 2a
            NO *predecessor = encontraPredecessor(y);
            x->chave[posicaoK] = predecessor->chave[predecessor->numChaves];

            if (y->folha)
            {
                NO *sucessor = encontraSucessor(z);
                sucessor->chave[1] = x->chave[posicaoK];
                z->filhos[1] = predecessor->filhos[predecessor->numChaves + 1];
            }

            removeNo(y, predecessor->chave[predecessor->numChaves]);
        }
        else if (z->numChaves >= t)
        { // 2b

            NO *sucessor = encontraSucessor(z);
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

    // caso 3
    else if (!x->folha && posicaoK == -1)
    {
        int i = 1;
        int j;
        NO *subArvore = encontraSubArvore(x, k);

        if (!subArvore)
            return;

        if (subArvore->numChaves == t - 1)
        {

            if (posicaoSubArvore + 1 < x->numChaves + 1 && x->filhos[posicaoSubArvore + 1]->numChaves >= t)
            { // 3a1
                subArvore->numChaves++;
                subArvore->chave[subArvore->numChaves] = x->chave[posicaoSubArvore];
                subArvore->filhos[subArvore->numChaves + 1] = x->filhos[posicaoSubArvore + 1]->filhos[1];
                int aux;
                for (aux = 1; aux < x->filhos[posicaoSubArvore + 1]->numChaves; aux++)
                    x->filhos[posicaoSubArvore + 1]->chave[aux] = x->filhos[posicaoSubArvore + 1]->chave[aux + 1];
                for (aux = 1; aux <= x->filhos[posicaoSubArvore + 1]->numChaves; aux++)
                    x->filhos[posicaoSubArvore + 1]->filhos[aux + 1] = x->filhos[posicaoSubArvore + 1]->filhos[aux];
                x->filhos[posicaoSubArvore + 1]->numChaves--;
                x->chave[posicaoSubArvore] = x->filhos[posicaoSubArvore + 1]->chave[1];
            }
            else if (posicaoSubArvore > 1 && x->filhos[posicaoSubArvore - 1]->numChaves >= t)
            { // 3a2
                int ant = x->filhos[posicaoSubArvore - 1]->chave[x->filhos[posicaoSubArvore - 1]->numChaves];
                x->chave[posicaoSubArvore - 1] = ant;
                x->filhos[posicaoSubArvore - 1]->numChaves--;
                subArvore->numChaves++;
                int aux;
                for (aux = subArvore->numChaves; aux >= 1; aux--)
                    subArvore->chave[aux] = subArvore->chave[aux - 1];
                if (x->filhos[posicaoSubArvore + 1])
                    subArvore->chave[1] = x->chave[posicaoSubArvore - 1];
                aux = subArvore->numChaves + 1;
                for (aux; aux <= 2; aux--)
                    subArvore->filhos[aux] = subArvore->filhos[aux - 1];

                if (x->filhos[posicaoSubArvore + 1])
                    subArvore->filhos[1] = x->filhos[posicaoSubArvore - 1]->filhos[x->filhos[posicaoSubArvore - 1]->numChaves + 1];
            }
            else if (x->filhos[posicaoSubArvore - 1]->numChaves == t - 1 && x->filhos[posicaoSubArvore + 1]->numChaves == t - 1)
            { // 3b
                if (posicaoSubArvore - 1 >= 1)
                {
                    int numArvAnterior = x->filhos[posicaoSubArvore - 1]->numChaves;
                    int auxAtual = numArvAnterior + 1;
                    int auxProx = 1;

                    x->filhos[posicaoSubArvore - 1]->numChaves += subArvore->numChaves;

                    while (auxProx <= subArvore->numChaves)
                    {
                        x->filhos[posicaoSubArvore - 1]->chave[auxAtual] = subArvore->chave[auxProx];
                        auxAtual++;
                        auxProx++;
                    }
                    auxAtual = numArvAnterior;
                    auxProx = 1;
                    while (auxProx + 1 <= subArvore->numChaves + 1)
                    {
                        x->filhos[posicaoSubArvore - 1]->filhos[auxAtual] = subArvore->filhos[auxProx];
                        auxProx++;
                        auxAtual++;
                    }
                    auxAtual = posicaoSubArvore - 1;
                    while (auxAtual + 1 <= x->numChaves)
                    {
                        x->chave[auxAtual] = x->chave[auxAtual + 1];
                        auxAtual++;
                    }
                    auxAtual = posicaoSubArvore;
                    while (auxAtual + 1 <= x->numChaves + 1)
                    {
                        x->filhos[auxAtual] = x->filhos[auxAtual + 1];
                        auxAtual++;
                    }
                    x->numChaves--;
                    x->filhos[posicaoSubArvore - 1]->numChaves--;
                }
                else if (posicaoSubArvore + 1 <= x->numChaves + 1)
                {
                    int auxProx = 1;
                    int numChavesSubArvoreOriginal = subArvore->numChaves;
                    int auxAtual = subArvore->numChaves;
                    subArvore->numChaves += x->filhos[posicaoSubArvore + 1]->numChaves;
                    while (auxProx <= x->filhos[posicaoSubArvore + 1]->numChaves)
                    {
                        subArvore->chave[auxAtual] = x->filhos[posicaoSubArvore + 1]->chave[auxProx];
                        auxAtual++;
                        auxProx++;
                    }
                    auxAtual = numChavesSubArvoreOriginal + 1;
                    auxProx = 1;
                    while (auxProx + 1 <= subArvore->numChaves + 1)
                    {
                        subArvore->filhos[auxAtual] = x->filhos[posicaoSubArvore + 1]->filhos[auxProx];
                        auxProx++;
                        auxAtual++;
                    }

                    auxAtual = posicaoSubArvore;
                    while (auxAtual + 1 <= x->numChaves)
                    {
                        x->chave[auxAtual] = x->chave[auxAtual + 1];
                        auxAtual++;
                    }
                    auxAtual = posicaoSubArvore + 1;
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

void removerArvore(ArvBMais *arv, int chave)
{
    NO *raiz = arv->raiz;
    removeNo(raiz, chave);
    if (raiz->numChaves == 0)
    {
        arv->raiz = raiz->filhos[0];
        free(raiz);
    }
}

void leArquivo(char arquivoEntrada[], char arquivoSaida[], ArvBMais *arv)
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
            case 'i':
                inserirArvore(arv, ch);
                break;
            case 'r':
                removerArvore(arv, ch);
                break;
            case 'p':
                imprimirArvore(arv, arv->raiz, saida);
                fprintf(saida, "\n");
                break;
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

void desalocaArvore(NO *raiz)
{
    if (raiz != NULL)
    {
        for (int i = 0; i <= raiz->numChaves; i++)
        {
            desalocaArvore(raiz->filhos[i]);
        }
        free(raiz);
    }
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
    desalocaArvore(arv.raiz);

    return 0;
}
