#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

#define INFINITO INT_MAX/2
#define true 1
#define false 0

//Função que conta o número de colunas da matriz e verifica se a matriz é quadrada
int numDeColunas(FILE *f){
	int vertices = 0, linhas = 0, colunas = 0, ProxColuna;
	char str[2048];
	fgets(str, 2048, f);
	int i=0, digitos, num, total=0;
	while(sscanf(str+total, "%d%n", &num, &digitos)!=-1){
		vertices++;
		total+=digitos;
	}
	colunas = vertices;
	linhas = vertices-1;
	for(linhas=vertices-1; linhas>0; linhas--){
		fgets(str, 2048, f);
		total = 0;
        ProxColuna = 0;
		while(sscanf(str+total, "%d%n", &num, &digitos)!=-1){
			ProxColuna++;
			total+=digitos;
		}
		if(ProxColuna!=colunas) break;
	}
	
	if(linhas!=0){
		printf("A matriz lida não é uma matriz quadrada\n");
		return 0;
	}
	return vertices;
}

//Função que imprime matriz
int imprimeMatriz(int** m, int linhas, int colunas, char c[100]){
	if(linhas==0 || colunas==0) return 0;
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

//Função que lê matriz do arquivo f, e guarda nas matrizes pesos e d
void lerMatriz(FILE *f, int linhas, int colunas, int** pesos, int** d){
	int i, j;
	for(i=0; i<linhas; i++){
		for(j=0; j<colunas; j++){
			fscanf(f, "%d", &pesos[i][j]);
			if(pesos[i][j]==-1)pesos[i][j] = INFINITO;   //Se o numero for -1 o peso é infinito
			d[i][j] = pesos[i][j];
		}
	}
}

//Função que imprime o caminho minimo entre dois vertices lidos do arquivo f
int caminhoMinimo(FILE *f, int** d, int max){
	if(max==0) return 0;
	int u, v;
	while(fscanf(f, "%d %d", &u, &v)==2){
		if((u>=max && v>=max) || (u<0 && v<0)) printf("O grafo não tem os vertices %d e %d\n", u, v);
		else if(u>=max || u<0) printf("O grafo não tem vertice %d\n", u);
		else if(v>=max || v<0) printf("O grafo não tem vertice %d\n", v);
		else{
			if(d[u][v]==INFINITO)
				printf("Não é possivel ir de %d até %d\n", u, v);
			else
				printf("A distância mínima de %d até %d é: %d\n", u, v, d[u][v]);
		}
	}
}

//Função que aloca espaço para uma matriz
int** alocaMatriz(int linhas, int colunas){
	int** m;
	m = malloc(sizeof(int*)*linhas);
	int i;
	for(i=0; i<linhas; i++){
		m[i] = malloc(sizeof(int)*colunas);
	}
	return m;
}

//Função que destroi uma matriz
void freeMatriz(int** m, int linhas){
	int i;
	for(i=0; i<linhas; i++){
		free(m[i]);
	}
	free(m);
}

//Função que cria matriz de distancias minimas
void dijkstra(int** g, int** d, int size){
	int i;
	int inicio;
	//Aloca espaço na memoria para o vetor cor e Q
	int* cor = malloc(sizeof(int)*size);
	int* Q = malloc(sizeof(int)*size);

	/*
	*Repete o processo de achar a distancia minima
	*Uma vez para cada vertice
	*Inicio é o vertice de ponto de partida
	*/
	for(inicio=0; inicio<size; inicio++){
		//Inicializa a fila Q e o vetor cor
		for(i=0; i<size; i++){
			Q[i] = -1; //-1 é uma posição da fila vazia
			cor[i] = false; //Todo numero false ainda não foi encontrado
		}

		push(Q, inicio, size);	//Adiciona o numero inicial a fila Q
		cor[inicio] = true;		//Marca o numero inicial como achado
		d[inicio][inicio] = 0;  //Consideramos que o peso do vertice até ele mesmo é 0

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
	setlocale(LC_ALL,"Portuguese"); //Muda os tipos de caracteres para poder usar acentos e ç
	int vertices=0;
	int x, y, z;
	int **pesos, **d;

	//Abre o arquivo
	FILE *f = fopen("Entrada.txt", "r");
	if(f==NULL) puts("ERRO! O arquivo nao foi aberto!");
	else{
		vertices = numDeColunas(f); //Pega o numero de vertices
		rewind(f);  //Volta para o começo do arquivo
	}

	//Aloca espaço para as matrizes
	pesos = alocaMatriz(vertices, vertices);
	d = alocaMatriz(vertices, vertices);

	lerMatriz(f, vertices, vertices, pesos, d);	//Inicializa as matrizes
	
	imprimeMatriz(pesos, vertices, vertices, "pesos"); //IMPRIME a matriz de pesos

	dijkstra(pesos, d, vertices); //Chama a função de dijkstra

	imprimeMatriz(d, vertices, vertices, "distância mínima"); //IMPRIME a matriz de distancias minimas

	caminhoMinimo(f, d, vertices); //IMPRIME caminho minimo de um vertice a outro

	if(f!=NULL) fclose(f); //Fecha o arquivo
	//Liberta os espaços alocados da memória
	freeMatriz(pesos, vertices);
	freeMatriz(d, vertices);

	system("pause");
	return 0;
}
