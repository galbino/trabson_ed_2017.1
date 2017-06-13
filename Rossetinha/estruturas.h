#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct dir {
    int n_files;
    int n_directories;
}TDIR;

typedef struct fil {
    int size;
}TFIL;

typedef struct n_tree {
    int type;
    void* data;
    char* name;
    char* creation_date;
    char* modification_date;
    struct n_tree* parent;
    struct n_tree* first;   //primeiro dos filhos
    struct n_tree* next;    //proximo irmao
}TNT;


TNT* inicializa(char *rootName);
TNT* allocNode(int type, const char* name, const char* time, int size);   //aloca um no
void currentTime(char* time);                                //retorna a data atual
int insertNode(TNT* arv, TNT* node);               //retorna 1 se teu certo e 0 se nao deu
TNT* search(TNT* tree, char* name);              //retorna um no (retorna NULL se nao achar)
int removeNode(TNT* parent, const char* name, int type);          //retorna 1 se teu certo e 0 se nao deu
TNT* searchDirByPath(TNT* arv, const char* path);  //se não achar retorna NULL
TNT* searchFileByPath(TNT* arv, const char* path, const char* name, int type);  //se não achar retorna NULL
TNT* searchByPath(TNT* arv, const char* path);
TNT* searchDir(TNT* arv, const char* name);     //pesquisa em um diretorio
void extractFromInput(const char* str, char *info, int action, TNT *arv);
int extractTypeFromInput(const char* str);
TNT* makeDirByPath(TNT* arv, char* path, const char* time);   //constroi todo o path e retorna o no do ultimo diretorio criado
TNT* makeFileByPath(TNT* arv, char* path, int type, const char* time, const char* name, int size);
void dirToFile(TNT* node);
void fileToDir(TNT* node);
int renameNode(TNT* node, char* name, int type);
int moveNode(TNT* arv, TNT* parent, const char* name, const char* path, int type);
void destroy(TNT* tree);
void printTree(TNT* tree);
TNT *treeNavigating(TNT* tree, char* folder);
char *getPath(TNT *node, char *path);
TNT *dirByName(TNT *node, char *name);
char *getRootName(TNT *arv);
void listFiles(TNT *arv);
void listFilesDetailed(TNT *arv);
TNT *searchDependingOnType(TNT* arv, char* name, int type);
int *getTotalSubDirAndFile(TNT *root, TNT* arv, int *total);
