#include "grafos.h"
#include <stdio.h>

int menu(void);
TG *opcao(int opcao, TG *grafo);
TG *carregaArquivo(TG *grafo);

int main(void){
	int escolha;
	TG *grafo = NULL;
	do
	{
		escolha = menu();
		grafo = opcao(escolha, grafo);
		
	}while(escolha);
	
	return 0;
}

int menu(){
	int opt;
	printf("\n---MENU---\n");
	printf("0 - sair\n");
	printf("1 - exibir\n");
	printf("2 - adcionar no\n");
	printf("3 - retirar no\n");
	printf("4 - buscar no\n");	
	printf("5 - adcionar aresta\n");
	printf("6 - retirar aresta\n");	
	printf("7 - busca aresta\n");
	printf("8 - carrega arquivo\n");
	printf("9 - pintar grafo\n");
	printf("----\nSelecione uma opcao\n");
	scanf("%d", &opt);
	return opt;
}

TG *opcao(int opcao, TG *grafo){
	int info;
	int destino;
	int custo;	
	
	switch(opcao){
		case 0:
			printf("programa finalizado\n");
			break;
		case 1:
			imprimeGrafo(grafo);
			break;
		case 2:
			printf("\n\nAdicionar no:\n \tdigite a informcao do no: ");
			scanf("%d", &info);
			grafo = insereNo(grafo, info);
			break;
		case 3:
			printf("\n\nRetirar no:\n \tdigite a informacao do no: ");
			scanf("%d", &info);
			grafo = retiraNo(grafo, info);
			break;
		case 4:
			printf("\n\nBuscar no:\n \tdigite a informacao do no: ");
			scanf("%d", &info);
			if(buscaNo(grafo, info)){
				imprimeNo(buscaNo(grafo, info));
			}else{
				printf("\n\nNo nao existe \n");
			}
			break;
		case 5:
			printf("\n\nAdicionar aresta:\n \tdigite o no de onde sai a aresta: ");
			scanf("%d", &info);
			if (!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			printf("\n \tdigite o no para onde a aresta aponta: ");
			scanf("%d", &destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			printf("\n \tdigite o custo da aresta: ");
			scanf("%d",&custo);
			insereAresta(grafo, info, destino);
			break;
		case 6:
			printf("\n\nRetira aresta:\n \tdigita o no de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			printf("\n \tdigite o nó para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o no digitado nao foi encontrado =/");
				break;
			}
			retiraAresta(grafo, info, destino);
			break;
		case 7:
			printf("\n\nBusca aresta aresta:\n \tdigita o no de onde sai a aresta: ");
			scanf("%d", &info);
			if(!buscaNo(grafo, info)){
				printf("\n\nDesculpe, o no digitado não foi encontrado =/");
				break;
			}
			printf("\n \tdigite o no para onde a aresta aponta: ");
			scanf("%d",&destino);
			if(!buscaNo(grafo, destino)){
				printf("\n\nDesculpe, o no digitado não foi encontrado =/");
				break;
			}
			imprimeAresta(buscaAresta(grafo, info, destino));
			break;
		case 8:
			printf("\n\nCarregando arquivo\n");
			grafo = carregaArquivo(grafo);
			break;
		case 9:
			printf("\n\nColorindo grafo");
			pintaGrafo(grafo, grafo, contaNos(grafo));
			break;
		default:
			printf("\nOpcao não encontrada");
			break;
	}
	return grafo;
}

TG *carregaArquivo(TG *grafo){
	FILE *arq = fopen("exemplo_EDI.txt", "r");
	if (arq != NULL){
		int qtdNo;
		fscanf(arq,"%d", &qtdNo);
		printf("\nQuantidade de nos: %d\n", qtdNo);
		int i;
		for(i = 1; i <= qtdNo ; i++){
			grafo = insereNo(grafo, i);
		}
		
		while(!feof(arq)){
			int origem;
			int destino;
			
			fscanf(arq,"%d%d", &origem, &destino);
			printf("origem: %d ; destino: %d\n", origem, destino);
			insereAresta(grafo, origem, destino);
		}
		
		
		fclose(arq);
	} else {
		printf("\nErro ao tentar abrir o arquivo");
	}
	return grafo;
}





















