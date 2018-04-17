#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INFINITO INT_MAX/2
#define true 1
#define false 0

//Função que conta o número de colunas da matriz
int numDeColunas(FILE *f){
	int vertices = 0;
	char str[1024];
	fgets(str, 1024, f);
	int i=0;
	for(i=0; str[i]!='\0'; i++){
		if(i==0){
			if(str[i]!=' '){
				vertices++;
			}
		}
		else if(str[i]!=' '){
			if(str[i-1]==' '){
				vertices++;
			}
		}
	}
	return vertices;
}

//Função que imprime matriz
void imprimeMatriz(int** m, int linhas, int colunas){
	int i, j;
	for(i=0; i<linhas; i++){
		char zero = '0';
		for(j=0; j<colunas; j++){
			if(m[i][j]==INFINITO) printf(" --- ");
			else if(m[i][j] == 0) printf("%4.c ", zero);
			else printf("%4.d ", m[i][j]);
		}
		puts("");
	}
}

//Função que adiciona um elemento na fila
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

//Função que retira um elemento da fila
void pop(int* vec, int size){
	int i;
	for(i=0; i<size; i++){
		if(i==size-1) vec[i]=-1;
		else vec[i] = vec[i+1];
	}
}

//Função que cria matriz de distancias minimas
void dijkstra(int** g, int** d, int size){
	int i;
	int inicio;
	//Aloca espaço na memoria para o vetor cor e Q
	int* cor = malloc(sizeof(int)*size);
	int* Q = malloc(sizeof(int)*size);

	/*
	Repete o processo de achar a distancia minima
	Uma vez para cada vertice
	Inicio é o vertice de ponto de partida
	*/
	for(inicio=0; inicio<size; inicio++){
		//Inicializa a fila Q e o vetor cor
		for(i=0; i<size; i++){
			Q[i] = -1; //-1 é uma posição da fila vazia
			cor[i] = false; //Todo numero false ainda não foi encontrado
		}

		push(Q, inicio, size);	//Adiciona o numero inicial a fila Q
		cor[inicio] = true;		//Marca o numero inicial como achado
		//d[inicio][inicio] = 0;  //Define a distancia do numero inicial até ele mesmo como 0

		//Enquanto a fila não estiver vazia
		while(Q[0]!=-1){
			int u = Q[0];   //Armazena o elemento da frente da fila na variável u
			pop(Q, size); 	//Retira um elemento da fila
			for(i=0; i<size; i++){
				int peso = g[u][i]; //Armazena o peso de u à i

				//Se o peso do vertice for infinito
				if(peso==INFINITO)
					continue;   //Ignora essa passagem do for

				//Se a distancia até u + o peso for menor que a distancia até i atual
				if(d[inicio][u] + peso < d[inicio][i]){
					d[inicio][i] = d[inicio][u] + peso; //Troca a distancia até i atual pela soma do peso e distancia até u
				}

				//Se o número i ainda não foi encontrado
				if(cor[i] == false){
					cor[i] = true;  	//Marca o número i como encontrado
					push(Q, i, size);   //Adiciona i a fila Q
				}
			}
		}
	}
	//Liberta os espaços alocados da memória
	free(cor);
	free(Q);
}

int main()
{
	int vertices=0;
	int x, y, z;
	int *vec_pesos, *vec_d;
	int **pesos, **d;

	//Abre o arquivo
	FILE *f = fopen("Entrada.txt", "r");
	if(f==NULL) puts("ERRO! O arquivo nao foi aberto!");
	else vertices = numDeColunas(f); //Pega o numero de vertices

	rewind(f);  //Volta para o começo do arquivo

	//Aloca espaço para a matriz de pesos
	vec_pesos = malloc(sizeof(int)*(vertices*vertices));
	pesos = malloc(sizeof(int)*vertices);

	//Aloca espaço para a matriz de distancias minimas
	vec_d = malloc(sizeof(int)*(vertices*vertices));
	d = malloc(sizeof(int)*vertices);
	int k=0;

	int i, j;
	//Inicializa as matrizes
	for(i=0; i<vertices; i++){
		pesos[i] = vec_pesos+k;
		d[i] = vec_d+k;
		for(j=0; j<vertices; j++){
			fscanf(f, "%d", &vec_pesos[k]);
			if(vec_pesos[k]==-1)vec_pesos[k] = INFINITO;   //Se o numero for -1 o peso é infinito
			vec_d[k] = vec_pesos[k];
			k++;
		}
	}
	//IMPRIME a matriz de pesos
	puts("Matriz de pesos:");
	imprimeMatriz(pesos, vertices, vertices);
	puts("");

	//Chama a função de dijkstra
	dijkstra(pesos, d, vertices);

	//IMPRIME a matriz de distancias minimas
	puts("Matriz de distancia minima:");
	imprimeMatriz(d, vertices, vertices);
	puts("");

	int u, v;
	while(fscanf(f, "%d %d", &u, &v)==2){
		if(d[u][v]==INFINITO)
			printf("Nao eh possivel ir de %d ate %d\n", u, v);
		else
			printf("A distancia minima de %d ate %d eh: %d\n", u, v, d[u][v]);
	}

	fclose(f); //Fecha o arquivo

	//Liberta os espaços alocados da memória
	free(vec_pesos);
	free(pesos);

	free(vec_d);
	free(d);

	system("pause");
	return 0;
}
