#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INFINITO INT_MAX/2
#define true 1
#define false 0

//Fun��o que conta o n�mero de colunas da matriz
int numDeColunas(FILE *f){
	int vertices = 0;
	char str[2048];
	fgets(str, 2048, f);
	int i=0, digitos, num, total=0;
	while(sscanf(str+total, "%d%n", &num, &digitos)!=-1){
		vertices++;
		total+=digitos;
	}
	return vertices;
}

//Fun��o que imprime matriz
void imprimeMatriz(int** m, int linhas, int colunas, char c[100]){
	int i, j;
	printf("Matriz de %s:\n", c);
	for(i=0; i<linhas; i++){
		char zero = '0';
		for(j=0; j<colunas; j++){
			if(m[i][j]==INFINITO) printf(" --- ");
			else if(m[i][j] == 0) printf("%4.c ", zero);
			else printf("%4.d ", m[i][j]);
		}
		puts("");
	}
	puts("");
}

//Fun��o que adiciona um elemento na fila
void push(int* vec, int num, int size){
	int i;
	for(i=0; i<size; i++){
		if(vec[i]==-1){
			vec[i] = num;
			break;
		}
		else if(i==size-1) puts("Falha no push");
	}
}

//Fun��o que retira um elemento da fila
void pop(int* vec, int size){
	int i;
	for(i=0; i<size; i++){
		if(i==size-1) vec[i]=-1;
		else vec[i] = vec[i+1];
	}
}

//Fun��o que l� matriz do arquivo f, e guarda nas matrizes pesos e d
void lerMatriz(FILE *f, int linhas, int colunas, int** pesos, int** d){
	int i, j;
	for(i=0; i<linhas; i++){
		for(j=0; j<colunas; j++){
			fscanf(f, "%d", &pesos[i][j]);
			if(pesos[i][j]==-1)pesos[i][j] = INFINITO;   //Se o numero for -1 o peso � infinito
			d[i][j] = pesos[i][j];
		}
	}
}

//Fun��o que imprime o caminho minimo entre dois vertices lidos do arquivo f
void caminhoMinimo(FILE *f, int** d){
	int u, v;
	while(fscanf(f, "%d %d", &u, &v)==2){
		if(d[u][v]==INFINITO)
			printf("Nao eh possivel ir de %d ate %d\n", u, v);
		else
			printf("A distancia minima de %d ate %d eh: %d\n", u, v, d[u][v]);
	}
}

//Fun��o que aloca espa�o para uma matriz
int** alocaMatriz(int linhas, int colunas){
	int** m;
	m = malloc(sizeof(int*)*linhas);
	int i;
	for(i=0; i<linhas; i++){
		m[i] = malloc(sizeof(int)*colunas);
	}
	return m;
}

//Fun��o que destroi uma matriz
void freeMatriz(int** m, int linhas){
	int i;
	for(i=0; i<linhas; i++){
		free(m[i]);
	}
	free(m);
}

//Fun��o que cria matriz de distancias minimas
void dijkstra(int** g, int** d, int size){
	int i;
	int inicio;
	//Aloca espa�o na memoria para o vetor cor e Q
	int* cor = malloc(sizeof(int)*size);
	int* Q = malloc(sizeof(int)*size);

	/*
	Repete o processo de achar a distancia minima
	Uma vez para cada vertice
	Inicio � o vertice de ponto de partida
	*/
	for(inicio=0; inicio<size; inicio++){
		//Inicializa a fila Q e o vetor cor
		for(i=0; i<size; i++){
			Q[i] = -1; //-1 � uma posi��o da fila vazia
			cor[i] = false; //Todo numero false ainda n�o foi encontrado
		}

		push(Q, inicio, size);	//Adiciona o numero inicial a fila Q
		cor[inicio] = true;		//Marca o numero inicial como achado

		//Enquanto a fila n�o estiver vazia
		while(Q[0]!=-1){
			int u = Q[0];   //Armazena o elemento da frente da fila na vari�vel u
			pop(Q, size); 	//Retira um elemento da fila
			for(i=0; i<size; i++){
				int peso = g[u][i]; //Armazena o peso de u � i

				//Se o peso do vertice for infinito
				if(peso==INFINITO)
					continue;   //Ignora essa passagem do for

				//Se a distancia at� u + o peso for menor que a distancia at� i atual
				if(d[inicio][u] + peso < d[inicio][i]){
					d[inicio][i] = d[inicio][u] + peso; //Troca a distancia at� i atual pela soma do peso e distancia at� u
				}

				//Se o n�mero i ainda n�o foi encontrado
				if(cor[i] == false){
					cor[i] = true;  	//Marca o n�mero i como encontrado
					push(Q, i, size);   //Adiciona i a fila Q
				}
			}
		}
	}
	//Liberta os espa�os alocados da mem�ria
	free(cor);
	free(Q);
}

int main()
{
	int vertices=0;
	int x, y, z;
	int **pesos, **d;

	//Abre o arquivo
	FILE *f = fopen("Entrada.txt", "r");
	if(f==NULL) puts("ERRO! O arquivo nao foi aberto!");
	else{
		vertices = numDeColunas(f); //Pega o numero de vertices
		rewind(f);  //Volta para o come�o do arquivo
	}

	//Aloca espa�o para as matrizes
	pesos = alocaMatriz(vertices, vertices);
	d = alocaMatriz(vertices, vertices);

	lerMatriz(f, vertices, vertices, pesos, d);	//Inicializa as matrizes
	
	imprimeMatriz(pesos, vertices, vertices, "pesos"); //IMPRIME a matriz de pesos

	dijkstra(pesos, d, vertices); //Chama a fun��o de dijkstra

	imprimeMatriz(d, vertices, vertices, "distancia minima"); //IMPRIME a matriz de distancias minimas

	caminhoMinimo(f, d); //IMPRIME caminho minimo de um vertice a outro

	fclose(f); //Fecha o arquivo
	//Liberta os espa�os alocados da mem�ria
	freeMatriz(pesos, vertices);
	freeMatriz(d, vertices);

	system("pause");
	return 0;
}
