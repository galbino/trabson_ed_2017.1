#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ROOT "root"
#define false 0
#define true 1

#define TYPE_DIRECTORY 0
#define TYPE_BINARY_FILE 10
#define TYPE_TEXT_FILE 100

#define LENGTH_NAME 50
#define LENGTH_DATE 21
#define LENGTH_PATH 100

#define EXTRACT_PATH 1
#define EXTRACT_NAME 2
#define EXTRACT_TIME 3
#define EXTRACT_SIZE 4


// formatacoa da data Nov 21 2016/10:25:45
// aquivos com o mesmo nome no mesmo diretorio?

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


TNT* inicializa();
TNT* allocNode(int type, const char* name, const char* time, int size);   //aloca um no
void currentTime(char* time);                                //retorna a data atual
int insertNode(TNT* root, TNT* node);               //retorna 1 se teu certo e 0 se nao deu
TNT* search(TNT* tree, char* name);              //retorna um no (retorna NULL se nao achar)
int removeNode(TNT* parent, const char* name);          //retorna 1 se teu certo e 0 se nao deu
TNT* searchDirByPath(TNT* root, const char* path);  //se não achar retorna NULL
TNT* searchFileByPath(TNT* root, const char* path, const char* name);  //se não achar retorna NULL
TNT* searchByPath(TNT* root, const char* path);
void extractFromInput(const char* str, char* info, int action);
int extractTypeFromInput(const char* str);
TNT* makeDirByPath(TNT* root, char* path, const char* time);   //constroi todo o path e retorna o no do ultimo diretorio criado
TNT* makeFileByPath(TNT* root, char* path, int type, const char* time, const char* name, int size);
void dirToFile(TNT* node);
void fileToDir(TNT* node);
void renameNode(TNT* node, const char* name);
int moveNode(TNT* root, TNT* parent, const char* name, const char* path);
void destroy(TNT* tree);
void printTree(TNT* tree);

void printTree(TNT* tree) {
    if (!tree)
        return;
    printf("%s %s\n", tree->name, tree->creation_date);
    if (tree->next)
        printTree(tree->next);
    if (tree->first)
        printTree(tree->first);
}

TNT* inicializa() {
    char time[LENGTH_DATE];
    currentTime(time);
    TNT* root = allocNode(TYPE_DIRECTORY, ROOT,time , 0);
    root->parent = NULL;
    return root;
}

void currentTime(char* time) {
    strcpy(time, __DATE__);
    strcat(time, "/");
    strcat(time, __TIME__);
}

TNT* allocNode(int type, const char* name, const char* time, int size) {
    TNT* node = (TNT*) malloc(sizeof(TNT));
    char* _name = (char*) malloc(LENGTH_NAME * sizeof(char));
    char* modification_date = (char*) malloc(LENGTH_DATE * sizeof(char));
    char* creatrion_date = (char*) malloc(LENGTH_DATE * sizeof(char));
    strcpy(_name, name);
    strcpy(modification_date, time);
    strcpy(creatrion_date, time);
    node->type = type;
    node->parent = NULL;
    node->first = NULL;
    node->next = NULL;

    node->name = _name;
    node->creation_date = creatrion_date; //tem um proplema aqui
    node->modification_date = modification_date;

    if (type == TYPE_DIRECTORY) {
        TDIR* dir = (TDIR*) malloc(sizeof(TDIR));
        dir->n_files = 0;
        dir->n_directories = 0;
        node->data = dir;
    }
    else if (type == TYPE_BINARY_FILE || type == TYPE_TEXT_FILE) {
        TFIL* file = (TFIL*) malloc(sizeof(TFIL));
        file->size = size;
        node->data = file;
    }
    else
        printf("Tipo invalido!\n");
    return node;
}

void destroy(TNT* tree) {
    if (tree->first) {
        destroy(tree->first);
        tree->first = NULL;
    }
    if (tree->next) {
        destroy(tree->next);
        tree->next = NULL;
    }
    free(tree->name);
    free(tree->data);
    free(tree->creation_date);
    free(tree->modification_date);
    free(tree);
}

int insertNode(TNT* root, TNT* node) {
    if (!root)
        return false;
    node->parent = root;
    node->next = root->first;
    root->first = node;
    return true;
}

int removeNode(TNT* parent, const char* name) {
    if (!parent)
        return false;
    TNT *p = parent->first, *last = NULL;
    while (p) {
        if (!strcmp(p->name, name)) {    //pode dar ruim aqui arquivo binario e texto
            if (!last) {    //remocao no comeco
                parent->first = p->next;
                p->next = NULL;
                destroy(p);
            }
            else {
                last->next = p->next;
                p->next = NULL;
                destroy(p);
            }
            return true;
        }
        last = p;
        p = p->next;
    }
    return false;
}

TNT* search(TNT* tree, char* name) {
    if (!strcmp(name, tree->name))
        return tree;
    if (tree->first) {
        TNT* node = search(tree->first, name);
        if (node)
            return node;
    }
    if (tree->next) {
        TNT* node = search(tree->next, name);
        if (node)
            return node;
    }
    return NULL;
}

void dirToFile(TNT* node) {
    if (!node)
        exit(1);
    TFIL* file = (TFIL*) malloc(sizeof(TFIL));
    char time[LENGTH_DATE];
    currentTime(time);
    file->size = 0;
    node->data = file;
    node->modification_date = time;
    node->type = TYPE_BINARY_FILE;
}

void fileToDir(TNT* node) {
    if (!node)
        exit(1);
    TDIR* dir = (TDIR*) malloc(sizeof(TDIR));
    char time[LENGTH_DATE];
    currentTime(time);
    dir->n_directories = 0;
    dir->n_files = 0;
    node->data = dir;
    node->modification_date = time;
    node->type = TYPE_DIRECTORY;
}

void renameNode(TNT* node, const char* name) {
    strcpy(node->name, name);
    currentTime(node->modification_date);
}

int moveNode(TNT* root, TNT* parent, const char* name, const char* path) {
    TNT* newParent = searchDirByPath(root, path);
    if (!newParent)
        return false;
    TNT *p = parent->first, *last = NULL;
    while (p) {
        if (!strcmp(p->name, name)) {
            if (!last)    //p esta no comeco
                parent->first = p->next;
            else
                last->next = p->next;
            if (newParent) {
                insertNode(newParent, p);
                return true;
            }
        }
        last = p;
        p = p->next;
    }
    return false;
}

void extractFromInput(const char* str, char* info, int action) {
    int i = 2;      //inguinora os dois primeiro caracteres
    int lastBars[4] = {0, 0, 0, 0}; //guarda as posicoes das ultimas '/'
    while (str[i] != '\0') {   //procurar o ultimo e penultimo "/"
        if (str[i] == '/') {
            lastBars[0] = lastBars[1];
            lastBars[1] = lastBars[2];
            lastBars[2] = lastBars[3];
            lastBars[3] = i;
        }
        i++;
    }
    //printf("lastBars[0] = %d ; lastBars[1] = %d ; lastBars[2] = %d ; lastBars[3] = %d\n",
     //                  lastBars[0], lastBars[1], lastBars[2], lastBars[3]);
    if (action == EXTRACT_PATH) {
        if (extractTypeFromInput(str) == TYPE_DIRECTORY) {
            lastBars[2] -= 2;
            strncpy(info, &str[2], lastBars[2]);
            info[lastBars[2]] = '\0';     //fecha a string
        }
        else {
            if(lastBars[0] == 0) {  //o diretorio pai e a root
                strcpy(info, ROOT);
            }
            else {
                lastBars[0] -= 2;
                strncpy(info, &str[2], lastBars[0]);
                info[lastBars[0]] = '\0';     //fecha a string
            }
        }
    }
    if (action == EXTRACT_NAME) {
        if (lastBars[0] == 0)   //o diretorio pai e a root
            lastBars[0]++;
        i = lastBars[1] - lastBars[0] - 1;
        strncpy(info, &str[lastBars[0] + 1], i);
        info[i] = '\0';
    }
    if (action == EXTRACT_TIME) {
        strcpy(info, &str[lastBars[2] + 1]);
    }
    if (action == EXTRACT_SIZE) {
        i = lastBars[2] - lastBars[1] - 1;
        strncpy(info, &str[lastBars[1] + 1], i);
        info[i] = '\0';
    }
}

int extractTypeFromInput(const char* str) {
    char c = str[0];
    if (c == 'D')
        return TYPE_DIRECTORY;
    if (c == 'T')
        return TYPE_TEXT_FILE;
    if (c == 'B')
        return TYPE_BINARY_FILE;
    return -1;
}

TNT* searchDir(const TNT* root, const char* name) {
    if (!root)
        return NULL;
    TNT* node = root->first;
    while (node) {
        if (!strcmp(name, node->name))
            break;
        node = node->next;
    }
    return node;
}

TNT* searchDirByPath(TNT* root, const char* path) {
    if (!root)
        return NULL;
    int i = 0, j = 0;
    while (1) {
        if (path[i] == '/' || path[i] == '\0') {
            char name[LENGTH_NAME];
            strncpy(name, &path[j], i - j);
            name[i - j] = '\0';
            root = search(root, name);
            j = i + 1;
            if (!root)
                return NULL;
            if (path[i] == '\0')
                break;
        }
        i++;

    }
    return root;
}

TNT* searchFileByPath(TNT* root, const char* path, const char* name) {
    root = searchDirByPath(root, path);
    if (!root)
        return NULL;
    TNT* child = root->first;
    while(child) {
        if (!strcmp(child->name, name))
            return child;
        child = child->next;
    }
    return NULL;
}

TNT* makeDirByPath(TNT* root, char* path, const char* time){
    int i = 0;
    char name[50];
    if (path[0] == '\0')
        return root;
    else if (path[0] == '/')
        path = &path[1];
    while (path[i] != '\0') {
        if (path[i] == '/')
            break;
        i++;
    }
    strncpy(name, path, i);
    name[i] = '\0';
    TNT* node = searchDir(root, name);
    if (!node) {
        node = allocNode(TYPE_DIRECTORY, name, time, 0);
        insertNode(root, node);
    }
    return makeDirByPath(node, &path[i], time);
}

TNT* makeFileByPath(TNT* root, char* path, int type, const char* time, const char* name, int size) {
    if (strcmp(path, ROOT))
        root = makeDirByPath(root, path, time);
    TNT* node = allocNode(type, name, time, size);
    insertNode(root, node);
    return node;
}

int main(void){

    char linha[100], s[100], date[50], name[100];
    currentTime(date);
    TNT* tree = inicializa();
    TNT* node = allocNode(TYPE_BINARY_FILE, "test.txt", date, 20);
    insertNode(tree, node);
    FILE* file = fopen("exemplo_EDI.txt", "r");
    //printf("%s\n", s);
    while(!feof(file)){
        fscanf(file,"%[^\n] ", linha);
        extractFromInput(linha, s, EXTRACT_PATH);
        printf("%s\n", s);
        if (extractTypeFromInput(linha) == TYPE_DIRECTORY) {
            currentTime(date);
            makeDirByPath(tree, s, date);
        }
        else {
            extractFromInput(linha, date, EXTRACT_TIME);
            extractFromInput(linha, name, EXTRACT_NAME);
            makeFileByPath(tree, s, extractTypeFromInput(linha),date, name, 5000);
        }
    }
    if (search(tree, "Nicolas Almagro"))
        printf("achou!\n");
    node = searchFileByPath(tree, "Downloads/musicas", "song.mp3");
    //node = searchDirByPath(tree, "Downloads/musicas");
    moveNode(tree, node->parent, node->name, "Downloads/musicas/pop/katy perry");
    printf("moveu para! %s\n", node->parent->name);
    /*
    if (node) {
        printf("achou! %s\n", node->name);
        removeNode(node->parent, node->name);
        printf("removeu\n");
    }
    else
        printf("nao achou\n");
    */
    printTree(tree);
    destroy(tree);
    fclose(file);
    return 0;
}
