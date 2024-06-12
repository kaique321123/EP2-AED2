#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3
typedef struct str_no
{
	int chave[2 * t];
	struct str_no *filhos[2 * t + 1];
	int numChaves;
	bool folha;
} NO;

typedef struct
{
	NO *raiz;
} ArvBMais;

// FUNÇÕES DE MANIPULAÇÃO DA ÁRVORE B+
bool criarArvore(ArvBMais *arv);
void imprimirArvore(ArvBMais *arv, NO *x, FILE *saida);
NO *encontrarPredecessor(NO *y);
NO *encontrarSucessor(NO *y);
int retornaPos(NO *x, int k);
NO *encontraSubArv(NO *x, int k);
void split(NO *x, int i, NO *y);
void inserirNONaoCheio(NO *x, int k);
void inserirArvore(ArvBMais *arv, int k);
void removeChave(NO *x, int k);
void removeChaveaveFolha(NO *x, int posChave);
NO *encontrarSubArvore(NO *x, int k);
void removeNo(NO *x, int k);
void removeRaiz(ArvBMais *arv, int k);
void removeArv(ArvBMais *arv, int k);
void leArquivo(ArvBMais *arv, char arquivoEntrada[], char arquivoSaida[]);
int main(int argc, char *argv[]);

int posSubArv = 0;

bool criarArvore(ArvBMais *arv)
{
	NO *x;
	if (!(x = (NO *)malloc(sizeof(NO))))
		return false;
	x->folha = true;
	x->numChaves = 0;
	arv->raiz = x;
	return true;
}

void imprimirArvore(ArvBMais *arv, NO *x, FILE *saida)
{
	int i;
	if (x->numChaves == 0 && x == arv->raiz)
	{
		fprintf(saida, "Vazia");
		return;
	}

	fprintf(saida, "(");
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
			imprimirArvore(arv, x->filhos[i], saida);
			fprintf(saida, " %d ", x->chave[i]);
		}
		imprimirArvore(arv, x->filhos[i], saida);
	}

	fprintf(saida, ")");
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

// Função para retornar a posição de uma chave em um nó
int retornaPos(NO *x, int k)
{
	int pos = 1;
	while (pos <= x->numChaves && x->chave[pos] < k)
	{
		pos++;
	}
	return pos;
}

NO *encontraSubArv(NO *x, int k)
{
	NO *subArvore = NULL;
	int i = 1;
	while (i <= x->numChaves)
	{
		if (k < x->chave[i])
		{
			subArvore = x->filhos[i];
			posSubArv = i;
			return subArvore;
		}
		else if (k == x->chave[i] || (k > x->chave[i] && i == x->numChaves))
		{
			subArvore = x->filhos[i + 1];
			posSubArv = i + 1;
			return subArvore;
		}
		else
			i++;
	}
	return subArvore;
}

// Função para dividir o nó
void split(NO *x, int i, NO *y)
{
	int j;
	NO *z;
	if ((z = (NO *)malloc(sizeof(NO))))
	{
		z->folha = y->folha;
		z->numChaves = t - 1 + y->folha;

		// copia metade das chaves de y para z
		if (y->folha)
		{
			z->chave[1] = y->chave[t];
			j = 2;
		}

		else
		{
			j = 1;
		}

		for (j; j <= t - 1 + y->folha; j++)
		{
			if (y->folha)
				z->chave[j] = y->chave[j + t - 1];
			else
				z->chave[j] = y->chave[j + t];
		}

		if (!y->folha)
		{
			for (j = 1; j <= t; j++)
				z->filhos[j] = y->filhos[j + t];
		}

		y->numChaves = t - 1;

		for (j = x->numChaves + 1; j >= i + 1; j--)
		{
			x->filhos[j + 1] = x->filhos[j];
		}

		x->filhos[i + 1] = z;

		for (j = x->numChaves + 1; j >= i; j--)
		{
			x->chave[j + 1] = x->chave[j];
		}

		x->chave[i] = y->chave[t];
		x->numChaves++;
	}
}

// Função para inserir a chave em um nó não cheio
void inserirNONaoCheio(NO *x, int k)
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
		if (x->filhos[i]->numChaves == 2 * t - 1)
		{
			split(x, i, x->filhos[i]);
			if (k > x->chave[i])
				i++;
		}
		inserirNONaoCheio(x->filhos[i], k);
	}
}

// Função para inserir uma chave na árvore B+
void inserirArvore(ArvBMais *arv, int k)
{
	NO *raiz = arv->raiz;
	NO *s;
	if (raiz->numChaves == 2 * t - 1)
	{
		if ((s = (NO *)malloc(sizeof(NO))))
		{
			arv->raiz = s;
			s->folha = false;
			s->numChaves = 0;
			s->filhos[1] = raiz;
			split(s, 1, raiz);
			inserirNONaoCheio(s, k);
		}
	}
	else
		inserirNONaoCheio(raiz, k);
}
// Função para remover a chave
void removeChave(NO *x, int k)
{
	int posicao = retornaPos(x, k);
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

// Função para encontrar a subárvore que contém a chave
NO *encontrarSubArvore(NO *x, int k)
{
	posSubArv = 0;
	while (posSubArv <= x->numChaves && k > x->chave[posSubArv + 1])
	{
		posSubArv++;
	}
	return x->filhos[posSubArv + 1];
}

void removeNo(NO *x, int k)
{
	int pos = retornaPos(x, k);
	int j;

	// caso 0
	if (x->folha && pos == -1)
		return;

	// caso 1
	else if (pos != -1 && x->folha)
	{
		removeChave(x, k);
	}

	// caso 2
	else if (!x->folha && pos != -1)
	{
		NO *y = x->filhos[pos];
		NO *z = x->filhos[pos + 1];

		if (y->numChaves >= t)
		{ // 2a
			NO *predecessor = encontrarPredecessor(y);
			x->chave[pos] = predecessor->chave[predecessor->numChaves];

			if (y->folha)
			{
				NO *sucessor = encontrarSucessor(z);
				sucessor->chave[1] = x->chave[pos];
				z->filhos[1] = predecessor->filhos[predecessor->numChaves + 1];
			}

			removeNo(y, predecessor->chave[predecessor->numChaves]);
		}
		else if (z->numChaves >= t)
		{ // 2b

			NO *sucessor = encontrarSucessor(z);
			if (z->folha)
			{
				x->chave[pos] = sucessor->chave[2];
				removeNo(sucessor, sucessor->chave[1]);
			}
			else
			{
				x->chave[pos] = sucessor->chave[1];
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
				y->chave[j] = x->chave[pos];
				j = y->numChaves + 1;
			}
			while (j < 2 * t - 1)
			{
				y->chave[j] = z->chave[posAtualZ];
				j++;
				posAtualZ++;
			}

			for (j = pos; j <= x->numChaves; j++)
			{
				x->chave[j] = x->chave[j + 1];
			}
			for (j = pos + 1; j <= x->numChaves + 1; j++)
			{
				x->filhos[j] = x->filhos[j + 1];
			}
			x->numChaves--;

			free(z);
			removeNo(y, k);
		}
	}

	// caso 3
	else if (!x->folha && pos == -1)
	{
		int i = 1;
		int j;
		NO *subArvore = encontrarSubArvore(x, k);

		if (!subArvore)
			return;

		if (subArvore->numChaves == t - 1)
		{

			if (posSubArv + 1 < x->numChaves + 1 && x->filhos[posSubArv + 1]->numChaves >= t)
			{ // 3a1
				subArvore->numChaves++;
				subArvore->chave[subArvore->numChaves] = x->chave[posSubArv];
				subArvore->filhos[subArvore->numChaves + 1] = x->filhos[posSubArv + 1]->filhos[1];
				int aux;
				for (aux = 1; aux < x->filhos[posSubArv + 1]->numChaves; aux++)
					x->filhos[posSubArv + 1]->chave[aux] = x->filhos[posSubArv + 1]->chave[aux + 1];
				for (aux = 1; aux <= x->filhos[posSubArv + 1]->numChaves; aux++)
					x->filhos[posSubArv + 1]->filhos[aux + 1] = x->filhos[posSubArv + 1]->filhos[aux];
				x->filhos[posSubArv + 1]->numChaves--;
				x->chave[posSubArv] = x->filhos[posSubArv + 1]->chave[1];
			}
			else if (posSubArv > 1 && x->filhos[posSubArv - 1]->numChaves >= t)
			{ // 3a2
				int ant = x->filhos[posSubArv - 1]->chave[x->filhos[posSubArv - 1]->numChaves];
				x->chave[posSubArv - 1] = ant;
				x->filhos[posSubArv - 1]->numChaves--;
				subArvore->numChaves++;
				int aux;
				for (aux = subArvore->numChaves; aux >= 1; aux--)
					subArvore->chave[aux] = subArvore->chave[aux - 1];
				if (x->filhos[posSubArv + 1])
					subArvore->chave[1] = x->chave[posSubArv - 1];
				aux = subArvore->numChaves + 1;
				for (aux; aux <= 2; aux--)
					subArvore->filhos[aux] = subArvore->filhos[aux - 1];

				if (x->filhos[posSubArv + 1])
					subArvore->filhos[1] = x->filhos[posSubArv - 1]->filhos[x->filhos[posSubArv - 1]->numChaves + 1];
			}
			else if (x->filhos[posSubArv - 1]->numChaves == t - 1 && x->filhos[posSubArv + 1]->numChaves == t - 1)
			{ // 3b
				if (posSubArv - 1 >= 1)
				{
					int numArvAnterior = x->filhos[posSubArv - 1]->numChaves;
					int auxAtual = numArvAnterior + 1;
					int auxProx = 1;

					x->filhos[posSubArv - 1]->numChaves += subArvore->numChaves;

					while (auxProx <= subArvore->numChaves)
					{
						x->filhos[posSubArv - 1]->chave[auxAtual] = subArvore->chave[auxProx];
						auxAtual++;
						auxProx++;
					}
					auxAtual = numArvAnterior;
					auxProx = 1;
					while (auxProx + 1 <= subArvore->numChaves + 1)
					{
						x->filhos[posSubArv - 1]->filhos[auxAtual] = subArvore->filhos[auxProx];
						auxProx++;
						auxAtual++;
					}
					auxAtual = posSubArv - 1;
					while (auxAtual + 1 <= x->numChaves)
					{
						x->chave[auxAtual] = x->chave[auxAtual + 1];
						auxAtual++;
					}
					auxAtual = posSubArv;
					while (auxAtual + 1 <= x->numChaves + 1)
					{
						x->filhos[auxAtual] = x->filhos[auxAtual + 1];
						auxAtual++;
					}
					x->numChaves--;
					x->filhos[posSubArv - 1]->numChaves--;
				}
				else if (posSubArv + 1 <= x->numChaves + 1)
				{
					int auxProx = 1;
					int numChavesSubArvoreOriginal = subArvore->numChaves;
					int auxAtual = subArvore->numChaves;
					subArvore->numChaves += x->filhos[posSubArv + 1]->numChaves;
					while (auxProx <= x->filhos[posSubArv + 1]->numChaves)
					{
						subArvore->chave[auxAtual] = x->filhos[posSubArv + 1]->chave[auxProx];
						auxAtual++;
						auxProx++;
					}
					auxAtual = numChavesSubArvoreOriginal + 1;
					auxProx = 1;
					while (auxProx + 1 <= subArvore->numChaves + 1)
					{
						subArvore->filhos[auxAtual] = x->filhos[posSubArv + 1]->filhos[auxProx];
						auxProx++;
						auxAtual++;
					}

					auxAtual = posSubArv;
					while (auxAtual + 1 <= x->numChaves)
					{
						x->chave[auxAtual] = x->chave[auxAtual + 1];
						auxAtual++;
					}
					auxAtual = posSubArv + 1;
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

void removeRaiz(ArvBMais *arv, int k)
{
	NO *r = arv->raiz;
	if (r->numChaves == 0)
		return;
	else
		removeNo(r, k);
	if (r->numChaves == 0 && !r->folha)
	{
		arv->raiz = r->filhos[1];
		free(r);
	}
}

void removeArv(ArvBMais *arv, int k)
{
	int i;
	bool excluido = false;
	for (i = 1; i <= arv->raiz->numChaves; i++)
	{
		if (arv->raiz->chave[i] == k)
		{
			removeRaiz(arv, k);
			excluido = true;
		}
	}
	if (!excluido)
		removeNo(arv->raiz, k);
}

void leArquivo(ArvBMais *arv, char arquivoEntrada[], char arquivoSaida[])
{
	char op;
	int i;

	FILE *entrada = NULL;
	// Abre o arquivo de entrada
	entrada = fopen(arquivoEntrada, "r");
	FILE *saida = NULL;
	// Abre o arquivo de saída
	saida = fopen(arquivoSaida, "w");

	bool arvore = criarArvore(arv);

	if (entrada && arvore)
	{
		while (fscanf(entrada, "%c %d", &op, &i) != EOF)
		{
			switch (op)
			{
			// inserir
			case 'i':
				inserirArvore(arv, i);
				break;
			// remover
			case 'r':
				removeArv(arv, i);
				break;
			// imprimir
			case 'p':
				imprimirArvore(arv, arv->raiz, saida);
				fprintf(saida, "\n");
				break;
			// finalizar
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

	ArvBMais arvore;
	leArquivo(&arvore, argv[1], argv[2]);
	return 0;
}