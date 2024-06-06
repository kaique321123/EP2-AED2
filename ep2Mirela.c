/*

Mirela Mei - 11208392

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3
typedef struct str_no{
	int chave[2*t];
	struct str_no* filhos[2*t+1]; 
	int numChaves;
	bool folha;
}NO;

typedef struct{
	NO* raiz;
}ArvBMais;

int main(int argc, char *argv[]);
void leArquivo(ArvBMais* arv, char arquivoEntrada[], char arquivoSaida[]);
void imprimeArvore(ArvBMais* arv, NO* x, FILE* saida);
bool criaArvore(ArvBMais* arv);
void splitFilho(NO* x, int i, NO* y);
void insereNaoCheia(NO* x, int k);
void insereArvore(ArvBMais* arv, int k);
NO* achaPredecessor(NO* y);
NO* achaSucessor(NO* y);
int retornaPosicaoK(NO* x, int k);
NO* encontraSubArvore(NO* x, int k);
void removeChave(NO* x, int k);
void removeNo(NO* x, int k);
void removeRaiz(ArvBMais* arv, int k);
void removeArvore(ArvBMais* arv, int k);

int posicaoSubArvore = 0;


int main(int argc, char *argv[]){
	if (argc != 3) {
        printf("Uso: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
        return 1;
    }
	ArvBMais arvore;
	leArquivo(&arvore, argv[1], argv[2]);
	return 0;
}

void leArquivo(ArvBMais* arv, char arquivoEntrada[], char arquivoSaida[]){
	char operacao;
	int valor;
	
	FILE *entrada = NULL;
	entrada = fopen(arquivoEntrada, "r");
	FILE *saida = NULL;
	saida = fopen(arquivoSaida, "w");
	
	bool arvore = criaArvore(arv);
	
	if(entrada && arvore){
		while(fscanf(entrada,"%c %d", &operacao, &valor) != EOF){
			switch(operacao){
				case 'i':
					insereArvore(arv, valor);
					break;
				case 'r':
					removeArvore(arv, valor);
					break;
				case 'p':
					imprimeArvore(arv, arv->raiz, saida);
					fprintf(saida, "\n");
					break;
				case 'f':
					return;
					break;
			}
		}
	}
	fclose(entrada);
	fclose(saida);
}

void imprimeArvore(ArvBMais* arv, NO* x, FILE* saida){
	int i;
	if(x->numChaves==0 && x==arv->raiz){
		fprintf(saida, "Vazia");
		return;
	}
	
	fprintf(saida, "(");
	if(x->folha){
		for(i=1; i<=x->numChaves; i++){
			if(i==x->numChaves)
				fprintf(saida, "%d", x->chave[i]);
			else
				fprintf(saida, "%d ", x->chave[i]);
		}
	}
	else{
		for(i=1; i<=x->numChaves; i++){
			imprimeArvore(arv, x->filhos[i], saida);
			fprintf(saida, " %d ", x->chave[i]);
		}
		imprimeArvore(arv, x->filhos[i], saida);
	}
	
	fprintf(saida, ")");		
}

bool criaArvore(ArvBMais* arv){
	NO* x;
	if(!(x = (NO*)malloc(sizeof(NO))))
		return false;
	x->folha=true;
	x->numChaves=0;
	arv->raiz=x;
	return true;
}

void splitFilho(NO* x, int i, NO* y){ 
	int j;
	NO* z;
	if((z = (NO*) malloc(sizeof(NO)))){
		z->folha = y->folha;
		z->numChaves = t-1 + y->folha;
		
		if(y->folha){
			z->chave[1]=y->chave[t];
			j=2;	
		} 
		else{
			j=1;	
		} 
		
		for(j; j <= t-1+y->folha; j++){
			if(y->folha) z->chave[j]=y->chave[j+t-1];
			else z->chave[j] = y->chave[j+t];
		
		}
		
		if(!y->folha){
			for(j=1; j<=t; j++)
				z->filhos[j] = y->filhos[j+t];
		}
		
		y->numChaves = t-1;
		
		for(j=x->numChaves+1; j>=i+1; j--){
			x->filhos[j+1] = x->filhos[j];
		}
		
		x->filhos[i+1]=z;
	
		for(j=x->numChaves+1;j>=i;j--){
			x->chave[j+1]=x->chave[j];
		}
		
		x->chave[i]=y->chave[t];
		x->numChaves++;
	}
}

void insereNaoCheia(NO* x, int k){ 
	int i = x->numChaves;
	if(x->folha){
		while(i>=1 && k<x->chave[i]){
			x->chave[i+1] = x->chave[i];
			i--;
		}
		x->chave[i+1] = k;
		x->numChaves++;
	}
	else{
		while(i>=1 && k<x->chave[i]){
			i--;
		}
		i++;
		if(x->filhos[i]->numChaves == 2*t-1){
			splitFilho(x, i, x->filhos[i]);
			if(k>x->chave[i])
				i++;
		}
		insereNaoCheia(x->filhos[i], k);
	}
}

void insereArvore(ArvBMais* arv, int k){
	NO* raiz = arv->raiz;
	NO* s;
	if(raiz->numChaves==2*t-1){
		if((s = (NO*)malloc(sizeof(NO)))){
			arv->raiz=s;
			s->folha=false;
			s->numChaves=0;
			s->filhos[1]=raiz;
			splitFilho(s, 1, raiz);
			insereNaoCheia(s, k);
		}
	}
	else
		insereNaoCheia(raiz, k); 
}

NO* achaPredecessor(NO* y){
	if(!y->folha)
		achaPredecessor(y->filhos[y->numChaves+1]);	
	return y;
}

NO* achaSucessor(NO* y){
	if(!y->folha)
		achaSucessor(y->filhos[1]);
	return y;
}

int retornaPosicaoK(NO* x, int k){
	int i=x->numChaves;
	while(i>=1){
		if(k==x->chave[i])
			return i;
		i--;
	}	
	return -1;
}

NO* encontraSubArvore(NO* x, int k){
	NO* subArvore=NULL;
	int i = 1;
	while(i<=x->numChaves){
		if(k < x->chave[i]){
			subArvore = x->filhos[i];
			posicaoSubArvore = i;
			return subArvore;
		}
		else if(k == x->chave[i] || (k > x->chave[i] && i == x->numChaves)){
			subArvore = x->filhos[i+1];
			posicaoSubArvore = i+1;
			return subArvore;
		}
		else
			i++;
	}
	return subArvore;
}

void removeChave(NO* x, int k){
	int posicao = retornaPosicaoK(x, k);
	int j;
	if(posicao == x->numChaves){
		x->numChaves--;
	}
	else{
		for(j=posicao; j<x->numChaves; j++){
			x->chave[j] = x->chave[j+1];
		}
		x->numChaves--;
	}
}

void removeNo(NO* x, int k){
	int posicaoK = retornaPosicaoK(x, k);
	int j;
	
	//caso 0
	if(x->folha && posicaoK == -1)
		return;
	
	//caso 1
	else if(posicaoK != -1 && x->folha){
		removeChave(x, k);
	}
	
	//caso 2	
	else if(!x->folha && posicaoK!=-1){
		NO* y =  x->filhos[posicaoK];
		NO* z = x->filhos[posicaoK+1];
		
		if(y->numChaves >= t){ //2a
			NO*	predecessor = achaPredecessor(y);
			x->chave[posicaoK] = predecessor->chave[predecessor->numChaves];
			
			if(y->folha){
				NO* sucessor = achaSucessor(z);
				sucessor->chave[1] = x->chave[posicaoK];	
				z->filhos[1] = predecessor->filhos[predecessor->numChaves+1];
			}
			
			removeNo(y, predecessor->chave[predecessor->numChaves]);	
		}
		else if(z->numChaves >= t){ //2b
			
			NO* sucessor = achaSucessor(z);
			if(z->folha){
				x->chave[posicaoK] = sucessor->chave[2];
				removeNo(sucessor, sucessor->chave[1]);	
			}
			else{
				x->chave[posicaoK] = sucessor->chave[1];
				removeNo(sucessor, sucessor->chave[1]);						
			}
		}
		else{ //2c		
			int i=1;
			for(i; i<z->numChaves; i++){
				z->chave[i]=z->chave[i+1];
			}
			i=1;
			for(i;i<=z->numChaves;i++){
				z->chave[i]=z->chave[i+1];
			}
			z->numChaves--;
		
			y->numChaves++;
			y->chave[y->numChaves] = k;
			
			int posAtualZ = 1;
			
			y->numChaves++;
			j=y->numChaves;
			if(!y->folha){
				y->chave[j] = x->chave[posicaoK];
				j=y->numChaves+1;
			}
			while(j < 2*t-1){
				y->chave[j] = z->chave[posAtualZ];
				j++;
				posAtualZ++; 
			}
						
			for(j=posicaoK; j<=x->numChaves; j++){
				x->chave[j] = x->chave[j+1];
			}
			for(j=posicaoK+1; j<=x->numChaves+1; j++){
				x->filhos[j] = x->filhos[j+1];
			}
			x->numChaves--;
			
			free(z);
			removeNo(y, k);
		}
	}
	
	//caso 3
	else if(!x->folha && posicaoK == -1){
		int i = 1;
		int j;
		NO* subArvore = encontraSubArvore(x, k);
		
		if(!subArvore) return;
		
		if(subArvore->numChaves == t-1){

			if(posicaoSubArvore+1 < x->numChaves+1 && x->filhos[posicaoSubArvore+1]->numChaves >= t){ //3a1 
				subArvore->numChaves++;
				subArvore->chave[subArvore->numChaves] = x->chave[posicaoSubArvore];
				subArvore->filhos[subArvore->numChaves+1]=x->filhos[posicaoSubArvore+1]->filhos[1];				
				int aux;
				for(aux=1; aux<x->filhos[posicaoSubArvore+1]->numChaves; aux++)
					x->filhos[posicaoSubArvore+1]->chave[aux]=x->filhos[posicaoSubArvore+1]->chave[aux+1];
				for(aux=1; aux<=x->filhos[posicaoSubArvore+1]->numChaves; aux++) 
					x->filhos[posicaoSubArvore+1]->filhos[aux+1] = x->filhos[posicaoSubArvore+1]->filhos[aux];
				x->filhos[posicaoSubArvore+1]->numChaves--;				
				x->chave[posicaoSubArvore]=x->filhos[posicaoSubArvore+1]->chave[1];
			}
			else if(posicaoSubArvore > 1 && x->filhos[posicaoSubArvore-1]->numChaves >= t){ //3a2
				int ant = x->filhos[posicaoSubArvore-1]->chave[x->filhos[posicaoSubArvore-1]->numChaves];
				x->chave[posicaoSubArvore-1] = ant;
				x->filhos[posicaoSubArvore-1]->numChaves--;
				subArvore->numChaves++;
				int aux;
				for(aux=subArvore->numChaves; aux>=1; aux--) 
					subArvore->chave[aux]=subArvore->chave[aux-1];
				if(x->filhos[posicaoSubArvore+1])
					subArvore->chave[1]=x->chave[posicaoSubArvore-1];
				aux=subArvore->numChaves+1;
				for(aux; aux<=2; aux--)
					subArvore->filhos[aux]=subArvore->filhos[aux-1];
					
				if(x->filhos[posicaoSubArvore+1])
					subArvore->filhos[1] = x->filhos[posicaoSubArvore-1]->filhos[x->filhos[posicaoSubArvore-1]->numChaves+1];
			}
			else if(x->filhos[posicaoSubArvore-1]->numChaves == t-1 && x->filhos[posicaoSubArvore+1]->numChaves == t-1){ //3b
				if(posicaoSubArvore-1 >= 1 ){
					int numArvAnterior = x->filhos[posicaoSubArvore-1]->numChaves;
					int auxAtual = numArvAnterior+1;
					int auxProx=1;
					
					x->filhos[posicaoSubArvore-1]->numChaves += subArvore->numChaves;
					
					while(auxProx <= subArvore->numChaves){ 
						x->filhos[posicaoSubArvore-1]->chave[auxAtual] = subArvore->chave[auxProx];
						auxAtual++;
						auxProx++;
					}
					auxAtual = numArvAnterior;
					auxProx = 1;
					while(auxProx+1 <= subArvore->numChaves+1){
						x->filhos[posicaoSubArvore-1]->filhos[auxAtual] = subArvore->filhos[auxProx];
						auxProx++;
						auxAtual++;
					}
					auxAtual = posicaoSubArvore-1;
					while(auxAtual+1 <= x->numChaves){
						x->chave[auxAtual] = x->chave[auxAtual+1];
						auxAtual++;
					}	
					auxAtual=posicaoSubArvore;
					while(auxAtual+1 <= x->numChaves+1){
						x->filhos[auxAtual] = x->filhos[auxAtual+1];
						auxAtual++;
					}
					x->numChaves--;
					x->filhos[posicaoSubArvore-1]->numChaves--;
				}
				else if(posicaoSubArvore+1 <= x->numChaves+1){
					int auxProx = 1;
					int numChavesSubArvoreOriginal = subArvore->numChaves;
					int auxAtual = subArvore->numChaves;
					subArvore->numChaves += x->filhos[posicaoSubArvore+1]->numChaves;
					while(auxProx <= x->filhos[posicaoSubArvore+1]->numChaves){
						subArvore->chave[auxAtual] = x->filhos[posicaoSubArvore+1]->chave[auxProx];
						auxAtual++;
						auxProx++;
					}
					auxAtual = numChavesSubArvoreOriginal+1;
					auxProx = 1;
					while(auxProx+1 <= subArvore->numChaves+1){
						subArvore->filhos[auxAtual] = x->filhos[posicaoSubArvore+1]->filhos[auxProx];
						auxProx++;
						auxAtual++;
					}
					
					auxAtual=posicaoSubArvore;
					while(auxAtual+1 <= x->numChaves){
						x->chave[auxAtual] = x->chave[auxAtual+1];
						auxAtual++;
					}	
					auxAtual=posicaoSubArvore+1;
					while(auxAtual+1 <= x->numChaves+1){
						x->filhos[auxAtual] = x->filhos[auxAtual+1];
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

void removeRaiz(ArvBMais* arv, int k){
	NO* r = arv->raiz;
	if(r->numChaves == 0) return;
	else
		removeNo(r, k);
	if(r->numChaves==0 && !r->folha){
		arv->raiz=r->filhos[1];
		free(r);
	}
}

void removeArvore(ArvBMais* arv, int k){
	int i;
	bool excluido=false;
	for(i=1;i<=arv->raiz->numChaves; i++){
		if(arv->raiz->chave[i] == k){
			removeRaiz(arv, k);
			excluido=true;
		}
	}
	if(!excluido) removeNo(arv->raiz, k);
		
}