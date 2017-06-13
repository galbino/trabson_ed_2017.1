#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"

#define false 0
#define true 1

//comandos
#define C_CD "cd" //
#define C_LS "ls" //
#define C_MK "mk" //
#define C_RM "rm" //
#define C_RN "rn" //
#define C_MV "mv" //
#define C_DIRTOFILE "dtf" //
#define C_FILETODIR "ftd" //
#define C_EXIT "exit"

#define TYPE_DIRECTORY 0
#define TYPE_BINARY_FILE 10
#define TYPE_TEXT_FILE 100

#define LENGTH_NAME 50
#define LENGTH_DATE 21
#define LENGTH_PATH 255

#define EXTRACT_PATH 1
#define EXTRACT_NAME 2
#define EXTRACT_TIME 3
#define EXTRACT_SIZE 4

int main(void){
	char action[10];
	char line[LENGTH_PATH], path[LENGTH_PATH], date[LENGTH_DATE], name[LENGTH_NAME], auxName[LENGTH_NAME], size[15], linecpy[LENGTH_PATH], linecpy2[LENGTH_PATH], linecpy3[LENGTH_PATH], dump;
	int type;
	TNT *arv = NULL, *node;
    FILE* file = fopen("exemplo_EDI.txt", "r");
    while(!feof(file)){
        fscanf(file,"%[^\n] ", line);
        strcpy(linecpy, line);
        strcpy(linecpy2, line);
        strcpy(linecpy3, line);
        extractFromInput(linecpy, path, EXTRACT_PATH, arv);
        extractFromInput(linecpy2, name, EXTRACT_NAME, arv);
        type = extractTypeFromInput(linecpy3);
        extractFromInput(linecpy, date, EXTRACT_TIME, arv);
        if (!strcmp(path, "NULL")) {
        	arv = inicializa(name);
		} else {
	        if (type == TYPE_DIRECTORY) {
	            if(strcmp(path, "")) {
	        		strcat(path, "/");
				}
	        	strcat(path, name);
	            makeDirByPath(arv, path, date);
	        } else {
	            extractFromInput(linecpy2, size, EXTRACT_SIZE, arv);
	            makeFileByPath(arv, path, type, date, name, atoi(size));
	        }
	    }
    }
	TNT *currentNode = arv;
	char currentPath[100];
	while (1){
		strcpy(currentPath, "");
		strcpy(currentPath, getPath(currentNode, currentPath));
		currentPath[strlen(currentPath)-1] = '\0';
		printf("%s/%s>", arv->name, currentPath);
		scanf("%s", action);
		if (!strcmp(action, C_LS)){		 						//LS Chain working perfectly
			int ch = (int) getc(stdin);
			if (ch == 32) {
				scanf("%s", action);
				if (!strcmp(action, "-la")) {
					listFilesDetailed(currentNode);
				} else {
					printf("Modificador invalido \n");
				}
			} else if (ch == 10){
				listFiles(currentNode);
			}
		} else if (!strcmp(action, C_CD)){						//CD Chain working perfectly
			scanf(" %255[^\n]", path);
			if (!strcmp(path, "..")) {
				if (currentNode->parent){
					currentNode = currentNode->parent;
				} else {
					printf("Operacao invalida\n");
				}
			} else {
				node = searchDirByPath(currentNode, path);
				if (node){
					currentNode = node;
				} else {
					printf("Diretorio invalido\n");
				}
			}
		} else if (!strcmp(action, C_RM)){										//Remove chain working perfectly
			scanf("%s \"%255[^\"] %c", action, path, &dump);
            if (!strcmp(action, "-d")) {
                node = searchDirByPath(arv, path);
                if (node) {
                    if (!removeNode(node->parent, node->name, TYPE_DIRECTORY)) {
                    	printf("operacao invalida!!\n");
					}
                } else {
                	printf("operacao invalida!!\n");
				}
            } else if (!strcmp(action, "-b")) {
                scanf(" \"%255[^\"] %c", name, &dump);
                node = searchFileByPath(arv, path, name, TYPE_BINARY_FILE);
                if (node) {
                    if (!removeNode(node->parent, node->name, TYPE_BINARY_FILE)) {
                    	printf("operacao invalida!!\n");
					}
                } else {
                	printf("operacao invalida!!\n");
				}
            } else if (!strcmp(action, "-t")) {
            	scanf(" \"%255[^\"] %c", name, &dump);
                node = searchFileByPath(arv, path, name, TYPE_TEXT_FILE);
                if (node) {
                    if (!removeNode(node->parent, node->name, TYPE_TEXT_FILE)) {
                    	printf("operacao invalida!!\n");
					}
                } else {
                	printf("operacao invalida!!\n");
				}
            } else {
            	printf("Comando invalido!\n");
			}
		} else if (!strcmp(action, C_MK)) { 									//Make chain working perfectly
            scanf("%s \"%255[^\"] %c", action, path, &dump);
            currentTime(date);
            if (!strcmp(action, "-d")) {
                node = makeDirByPath(arv, path, date);
                if (!node) {
                    printf("Operacao invalida!!\n");
                }
            } else if (!strcmp(action, "-b")) {
                scanf(" \"%255[^\"] %c", name, &dump);
				node = makeFileByPath(arv, path, TYPE_BINARY_FILE, date, name, 0);
                if (!node){
                	printf("operacao invalida!!\n");
				}
			} else if (!strcmp(action, "-t")) {
				scanf(" \"%255[^\"] %c", name, &dump);
				node = makeFileByPath(arv, path, TYPE_TEXT_FILE, date, name, 0);
                if (!node){
                	printf("operacao invalida!!\n");
				}
			} else {
            	printf("Comando invalido!\n");
            }
        } else if (!strcmp(action, C_RN))  { 						// Rename Chain working perfectly
        	scanf("%s \"%255[^\"] %c", action, path, &dump);
            if (!strcmp(action, "-d")) {
                scanf(" \"%255[^\"] %c", name, &dump);
                node = searchDirByPath(arv, path);
                if (node) {
                    if (!renameNode(node, name, TYPE_DIRECTORY)) {
                    	printf("operacao invalida!!\n");
					}
                } else printf("operacao invalida!!\n");
            } else if (!strcmp(action, "-b")) {
                scanf(" \"%255[^\"] %c \"%255[^\"] %c", auxName, &dump, name, &dump);
                node = searchFileByPath(arv, path, auxName, TYPE_BINARY_FILE);
                if (node) {
                    if (!renameNode(node, name, TYPE_BINARY_FILE))
                        printf("operacao invalida!!\n");
                } else {
                	printf("operacao invalida!!\n");
				}
            } else if (!strcmp(action, "-t")) {
            	scanf(" \"%255[^\"] %c \"%255[^\"] %c", auxName, &dump, name, &dump);
                node = searchFileByPath(arv, path, auxName, TYPE_TEXT_FILE);
                if (node) {
                    if (!renameNode(node, name, TYPE_TEXT_FILE))
                        printf("operacao invalida!!\n");
                } else {
                	printf("operacao invalida!!\n");
				}            	
            } else {
        		printf("Comando invalido!\n");
			} 
		} else if (!strcmp(action, C_MV)) {											// Seems complete
			char desPath[LENGTH_PATH];
            scanf("%s \"%255[^\"] %c", action, path, &dump);
            if (!strcmp(action, "-d")) {
            	scanf(" \"%255[^\"] %c", desPath, &dump);
                node = searchDirByPath(arv, path);
                TNT *ver = searchDirByPath(arv, desPath);
                if (node && ver && !searchDependingOnType(node, ver->name, ver->type)) {
                    if (!moveNode(arv, node->parent, node->name, desPath, TYPE_DIRECTORY)){
                    	printf("operacao mal sucedida!!\n");
					}
                }
                else {
                	printf("operacao invalida!!\n");
				}
            } else if (!strcmp(action, "-b")) {
                scanf(" \"%255[^\"] %c \"%255[^\"] %c", name, &dump, desPath, &dump);
                node = searchFileByPath(arv, path, name, TYPE_BINARY_FILE);
                if (node) {
                    if (!moveNode(arv, node->parent, node->name, desPath, TYPE_BINARY_FILE)) {
                        printf("operacao invalida!!\n");
                    }
                } else {
                	printf("operacao invalida!!\n");
				}
            } else if (!strcmp(action, "-t")) {
                scanf(" \"%255[^\"] %c \"%255[^\"] %c", name, &dump, desPath, &dump);
                node = searchFileByPath(arv, path, name, TYPE_TEXT_FILE);
                if (node) {
                    if (!moveNode(arv, node->parent, node->name, desPath, TYPE_TEXT_FILE)) {
                        printf("operacao invalida!!\n");
                    }
                } else {
                	printf("operacao invalida!!\n");
                }	
			} else {
            	printf("Comando invalido!\n");
			}
		} else if (!strcmp(action, C_EXIT)) { 										//Exit 10/10
			break;
		} else {
			printf("Comando inexistente.\n");
		}
		
	}
	return 0;
}
