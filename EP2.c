// Nome: Kaique de Jesus Pessoa Santos
// Nº USP: 14677144


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define a ordem mínima da árvore B+.
// Cada nó pode ter no máximo 2t - 1 chaves e no mínimo t - 1 chaves.
#define t 3

#define MAX_CHAVE 2 * t - 1

typedef struct str_no
{
	int chave[2 * t];				  // vetor de chaves
	struct str_no *filhos[2 * t + 1]; // vetor de ponteiros para os filhos
	int numChaves;					  // número de chaves armazenadas no nó
	bool folha;						  // indica se o nó é folha
} NO;

typedef struct
{
	NO *raiz; // ponteiro para a raiz da árvore
} ArvBMais;

// FUNÇÕES DE MANIPULAÇÃO DA ÁRVORE B+
bool criarArvore(ArvBMais *arv);
void imprimirArvore(ArvBMais *arv, NO *x, FILE *saida);
NO *encontraFilhoDireito(NO *y);
NO *encontraFilhoEsquerdo(NO *y);
int retornarPosicao(NO *x, int k);
NO *encontrarSubArvore(NO *x, int k);
NO *criaNo(bool folha);
void split(NO *x, int i, NO *y);
void inserirNCheia(NO *x, int k);
void inserirArvore(ArvBMais *arv, int k);
void removerChave(NO *x, int k);
void removerNo(NO *x, int k);
void removerRaiz(ArvBMais *arv, int k);
void removerArvore(ArvBMais *arv, int k);
void leArquivo(ArvBMais *arv, char arquivoEntrada[], char arquivoSaida[]);
int main(int argc, char *argv[]);

// Variável global que armazena a posição da sub-árvore.
int posicaoSubArvore = 0;

// Função que cria uma árvore B+
bool criarArvore(ArvBMais *arv) {
    NO *x = criaNo(true); // Cria um novo nó como raiz e o define como folha
    if (!x) {
        return false; // Verifica se a alocação de memória foi bem-sucedida
    }
    arv->raiz = x; // Define o nó criado como a raiz da árvore
    return true;
}

void imprimirArvore(ArvBMais *arv, NO *x, FILE *saida)
{
	int i;
	if (x->numChaves == 0 && x == arv->raiz)
	{
		// Caso a raiz seja vazia
		// printf("Vazia\n")
		fprintf(saida, "Vazia");
		return;
	}

	fprintf(saida, "(");

	// Se o nó for um nó folha
	if (x->folha)
	{
		for (i = 1; i <= x->numChaves; i++)
		{
			if (i == x->numChaves)
				fprintf(saida, "%d", x->chave[i]);
			else
				fprintf(saida, "%d ", x->chave[i]);
		}
	}
	else
	{
		for (i = 1; i <= x->numChaves; i++)
		{
			// Imprime a sub-árvore à esquerda da chave.
			imprimirArvore(arv, x->filhos[i], saida);
			fprintf(saida, " %d ", x->chave[i]);
		}
		imprimirArvore(arv, x->filhos[i], saida);
	}

	fprintf(saida, ")");
}

NO *encontraFilhoDireito(NO *y)
{
	if (!y->folha)
		encontraFilhoDireito(y->filhos[y->numChaves + 1]);
	return y;
}

NO *encontraFilhoEsquerdo(NO *y)
{
	if (!y->folha)
		encontraFilhoEsquerdo(y->filhos[1]);
	return y;
}

// Retorna a posição da chave k no nó x
int retornarPosicao(NO *x, int k)
{
	int i = x->numChaves;
	while (i >= 1)
	{
		if (k == x->chave[i])
			return i;
		i--;
	}
	return -1;
}

NO *encontrarSubArvore(NO *x, int k)
{
	NO *subArvore = NULL;
	int i = 1;
	while (i <= x->numChaves)
	{
		if (k < x->chave[i])
		{
			subArvore = x->filhos[i];
			posicaoSubArvore = i;
			return subArvore;
		}
		else if (k == x->chave[i] || (k > x->chave[i] && i == x->numChaves))
		{
			subArvore = x->filhos[i + 1];
			posicaoSubArvore = i + 1;
			return subArvore;
		}
		else
			i++;
	}
	return subArvore;
}

// Função que aloca memória para um novo nó
NO *criaNo(bool folha) {
    NO *novoNo = (NO *)malloc(sizeof(NO));
    novoNo->numChaves = 0;
    novoNo->folha = folha;
    return novoNo;
}

// Função que copia chaves de y para z
void copiaChaves(NO *y, NO *z) {
    int j;
    if (y->folha) {
        z->chave[1] = y->chave[t];
        for (j = 2; j <= t; j++) {
            z->chave[j] = y->chave[j + t - 1];
        }
    } else {
        for (j = 1; j < t; j++) {
            z->chave[j] = y->chave[j + t];
        }
    }
}

// Função que copia filhos de y para z
void copiaFilhos(NO *y, NO *z) {
    int j;
    if (!y->folha) {
        for (j = 1; j <= t; j++) {
            z->filhos[j] = y->filhos[j + t];
        }
    }
}

// Função que insere o novo nó z no nó pai x
void insereNovoNo(NO *x, int i, NO *z) {
    int j;
    for (j = x->numChaves + 1; j >= i + 1; j--) {
        x->filhos[j + 1] = x->filhos[j];
    }
    x->filhos[i + 1] = z;

    for (j = x->numChaves + 1; j >= i; j--) {
        x->chave[j + 1] = x->chave[j];
    }
    x->chave[i] = x->filhos[i]->chave[t];
    x->numChaves++;
}

//Função principal que divide o nó y e move a chave mediana para o nó x
//Dividia em subfunções
void split(NO *x, int i, NO *y) {
    NO *z = criaNo(y->folha);
    z->numChaves = t - 1 + y->folha;

    copiaChaves(y, z);
    copiaFilhos(y, z);

    y->numChaves = t - 1;

    insereNovoNo(x, i, z);
}

// Função que insere a chave k no nó x se a árvore não estiver cheia
void inserirNCheia(NO *x, int k)
{
	int i = x->numChaves;
	if (x->folha)
	{
		while (i >= 1 && k < x->chave[i])
		{
			x->chave[i + 1] = x->chave[i];
			i--;
		}
		x->chave[i + 1] = k;
		x->numChaves++;
	}
	else
	{
		while (i >= 1 && k < x->chave[i])
		{
			i--;
		}
		i++;

		// Verifica se o nó filho está cheio
		if (x->filhos[i]->numChaves == MAX_CHAVE)
		{
			split(x, i, x->filhos[i]);
			if (k > x->chave[i])
				i++;
		}
		inserirNCheia(x->filhos[i], k);
	}
}

// Função que insere a chave k na árvore B+
void inserirArvore(ArvBMais *arv, int k)
{
	NO *raiz = arv->raiz;
	NO *s;

	// Caso a raiz esteja cheia
	if (raiz->numChaves == MAX_CHAVE)
	{
		if ((s = (NO *)malloc(sizeof(NO))))
		{
			arv->raiz = s;
			s->folha = false;
			s->numChaves = 0;
			s->filhos[1] = raiz;
			split(s, 1, raiz);
			inserirNCheia(s, k);
		}
	}
	else
		inserirNCheia(raiz, k);
}

// Função que remove a chave k do nó x
void removerChave(NO *x, int k)
{
	int posicao = retornarPosicao(x, k);
	int j;

	if (posicao == x->numChaves)
	{
		x->numChaves--;
	}
	else
	{
		for (j = posicao; j < x->numChaves; j++)
		{
			x->chave[j] = x->chave[j + 1];
		}
		x->numChaves--;
	}
}

void removerNo(NO *x, int k)
{
	int posicaoK = retornarPosicao(x, k);
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

		// 2a
		if (y->numChaves >= t)
		{
			NO *predecessor = encontraFilhoDireito(y);
			x->chave[posicaoK] = predecessor->chave[predecessor->numChaves];

			if (y->folha)
			{
				NO *sucessor = encontraFilhoEsquerdo(z);
				sucessor->chave[1] = x->chave[posicaoK];
				z->filhos[1] = predecessor->filhos[predecessor->numChaves + 1];
			}

			removerNo(y, predecessor->chave[predecessor->numChaves]);
		}
		// 2b
		else if (z->numChaves >= t)
		{

			NO *sucessor = encontraFilhoEsquerdo(z);
			if (z->folha)
			{
				x->chave[posicaoK] = sucessor->chave[2];
				removerNo(sucessor, sucessor->chave[1]);
			}
			else
			{
				x->chave[posicaoK] = sucessor->chave[1];
				removerNo(sucessor, sucessor->chave[1]);
			}
		}
		// 2c
		else
		{ 
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
			// Enquanto não atingir o número máximo de chaves
			while (j < MAX_CHAVE)
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
			removerNo(y, k);
		}
	}

	// caso 3
	else if (!x->folha && posicaoK == -1)
	{
		int i = 1;
		int j;
		NO *subArvore = encontrarSubArvore(x, k);

		if (!subArvore)
			return;

		if (subArvore->numChaves == t - 1)
		{
			// 3a
			if (posicaoSubArvore + 1 < x->numChaves + 1 && x->filhos[posicaoSubArvore + 1]->numChaves >= t)
			{
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
			{
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
			// 3b
			else if (x->filhos[posicaoSubArvore - 1]->numChaves == t - 1 && x->filhos[posicaoSubArvore + 1]->numChaves == t - 1)
			{
				if (posicaoSubArvore - 1 >= 1)
				{
					int numArvAnterior = x->filhos[posicaoSubArvore - 1]->numChaves;
					int atual = numArvAnterior + 1;
					int prox = 1;

					x->filhos[posicaoSubArvore - 1]->numChaves += subArvore->numChaves;

					while (prox <= subArvore->numChaves)
					{
						x->filhos[posicaoSubArvore - 1]->chave[atual] = subArvore->chave[prox];
						atual++;
						prox++;
					}
					atual = numArvAnterior;
					prox = 1;
					while (prox + 1 <= subArvore->numChaves + 1)
					{
						x->filhos[posicaoSubArvore - 1]->filhos[atual] = subArvore->filhos[prox];
						prox++;
						atual++;
					}
					atual = posicaoSubArvore - 1;
					while (atual + 1 <= x->numChaves)
					{
						x->chave[atual] = x->chave[atual + 1];
						atual++;
					}
					atual = posicaoSubArvore;
					while (atual + 1 <= x->numChaves + 1)
					{
						x->filhos[atual] = x->filhos[atual + 1];
						atual++;
					}
					x->numChaves--;
					x->filhos[posicaoSubArvore - 1]->numChaves--;
				}
				else if (posicaoSubArvore + 1 <= x->numChaves + 1)
				{
					int prox = 1;
					int numChavesSubArvoreOriginal = subArvore->numChaves;
					int atual = subArvore->numChaves;
					subArvore->numChaves += x->filhos[posicaoSubArvore + 1]->numChaves;
					while (prox <= x->filhos[posicaoSubArvore + 1]->numChaves)
					{
						subArvore->chave[atual] = x->filhos[posicaoSubArvore + 1]->chave[prox];
						atual++;
						prox++;
					}
					atual = numChavesSubArvoreOriginal + 1;
					prox = 1;
					while (prox + 1 <= subArvore->numChaves + 1)
					{
						subArvore->filhos[atual] = x->filhos[posicaoSubArvore + 1]->filhos[prox];
						prox++;
						atual++;
					}

					atual = posicaoSubArvore;
					while (atual + 1 <= x->numChaves)
					{
						x->chave[atual] = x->chave[atual + 1];
						atual++;
					}
					atual = posicaoSubArvore + 1;
					while (atual + 1 <= x->numChaves + 1)
					{
						x->filhos[atual] = x->filhos[atual + 1];
						atual++;
					}
					x->numChaves--;
					subArvore->numChaves--;
				}
			}
			else
				return;
		}
		removerNo(subArvore, k);
	}
}

void removerRaiz(ArvBMais *arv, int k)
{
	NO *raiz = arv->raiz;

	if (raiz->numChaves == 0)
		return;
	else
		removerNo(raiz, k);
	if (raiz->numChaves == 0 && !raiz->folha)
	{
		arv->raiz = raiz->filhos[1];
		free(raiz);
	}
}

void removerArvore(ArvBMais *arv, int k)
{
	int i;
	// Verifica se a chave k está na raiz da árvore
	bool excluido = false;
	for (i = 1; i <= arv->raiz->numChaves; i++)
	{
		// Se a chave k estiver na raiz
		if (arv->raiz->chave[i] == k)
		{
			removerRaiz(arv, k);
			excluido = true;
		}
	}
	// Se a chave k não estiver na raiz
	if (!excluido)
		removerNo(arv->raiz, k);
}

void leArquivo(ArvBMais *arv, char arquivoEntrada[], char arquivoSaida[])
{
	char operacao;
	int valor;

	FILE *entrada = NULL;
	// Abre o arquivo de entrada
	entrada = fopen(arquivoEntrada, "r");
	FILE *saida = NULL;
	// Abre o arquivo de saída
	saida = fopen(arquivoSaida, "w");

	// Verifica se o arquivo foi aberto corretamente e cria a árvore
	bool arvore = criarArvore(arv);

	if (entrada && arvore)
	{
		while (fscanf(entrada, "%c %d", &operacao, &valor) != EOF)
		{
			switch (operacao)
			{
			// Imprimir
			case 'i':
				inserirArvore(arv, valor);
				break;
			// Remover
			case 'r':
				removerArvore(arv, valor);
				break;
			// Printar
			case 'p':
				imprimirArvore(arv, arv->raiz, saida);
				fprintf(saida, "\n");
				break;
			// Finalizar
			case 'f':
				return;
				break;
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
		printf("Uso: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
		return 1;
	}
	// Cria a árvore B+
	ArvBMais arvore;
	leArquivo(&arvore, argv[1], argv[2]);
	return 0;
}
