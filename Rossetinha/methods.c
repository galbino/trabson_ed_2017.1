#include "estruturas.h"

#define false 0
#define true 1

#define TYPE_DIRECTORY 0
#define TYPE_BINARY_FILE 10
#define TYPE_TEXT_FILE 100

#define LENGTH_NAME 50
#define LENGTH_DATE 21
#define LENGTH_PATH 300

#define EXTRACT_PATH 1
#define EXTRACT_NAME 2
#define EXTRACT_TIME 3
#define EXTRACT_SIZE 4

void printTree(TNT* arv) {
	TNT *tree = arv;
    if (!tree)
        return;
    printf("%s %s\n", tree->name, tree->creation_date);
    if (tree->next)
        printTree(tree->next);
    if (tree->first)
        printTree(tree->first);
}

TNT* inicializa(char *rootName) {
    char time[LENGTH_DATE];
    currentTime(time);
    TNT* root = allocNode(TYPE_DIRECTORY, rootName, time, 0);
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

int insertNode(TNT* arv, TNT* node) {
	TNT *root = arv;
	char time[LENGTH_DATE];
	currentTime(time);
	char *mod_date = (char *) malloc (sizeof(char) * LENGTH_DATE);
	strcpy(mod_date, time);
    if (!root)
        return false;
    node->parent = root;
    node->next = root->first;
    root->first = node;
    TDIR *dir = (TDIR *) node->parent->data;
    root->modification_date = mod_date;
    if (node->type == TYPE_DIRECTORY) {
    	dir->n_directories++;
	} else {
		dir->n_files++;
	}
    return true;
}

int removeNode(TNT* pai, const char* name, int type) {
	TNT *parent = pai;
	char time[LENGTH_DATE];
	currentTime(time);
	char *mod_date = (char *) malloc (sizeof(char) * LENGTH_DATE);
	strcpy(mod_date, time);
    if (!parent)
        return false;
    TNT *p = parent->first, *last = NULL;
    while (p) {
        if (!strcmp(p->name, name) && p->type == type) {
        	TDIR *dir = (TDIR *) parent->data;
        	if (type == TYPE_DIRECTORY) {
        		dir->n_directories--;
			} else {
				dir->n_files--;
			}
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
            parent->modification_date = mod_date;
            return true;
        }
        last = p;
        p = p->next;
    }
    return false;
}

TNT* search(TNT* arv, char* name) {
	TNT *tree = arv;
	if (!tree) {
		return NULL;
	}
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

int renameNode(TNT* node, char* name, int type) {
    if (!node)
        return false;
    TNT* p = searchDependingOnType(node->parent, name, type);
    if (p) {
        printf("Ja existe um arquivo desse tipo com esse nome!\n");
        return false;
    }
    strcpy(node->name, name);
    currentTime(node->modification_date);
    return true;
}

int moveNode(TNT* arv, TNT* parent, const char* name, const char* path, int type) {
	char time[LENGTH_DATE];
	currentTime(time);
	char *mod_date = (char *) malloc (sizeof(char) * LENGTH_DATE);
	strcpy(mod_date, time);
	TNT *root = arv;
    TNT* newParent = searchDirByPath(root, path);
    if (!newParent)
        return false;
    TNT *p = parent->first, *last = NULL;
    while (p) {
		if (!strcmp(p->name, name)) {
			TNT *cpy = searchFileByPath(arv, path, name, type);
			if (cpy) {
				printf("Arquivo igual, deseja sobrescreve-lo? S/N: ");
				char c;
				scanf(" %c", &c);
				if (c == 'S') {
					if (!last) {
            		    parent->first = p->next;
		            } else {
		            	last->next = p->next;
					}
					removeNode(cpy->parent, cpy->name, cpy->type);
					p->modification_date = mod_date;
					insertNode(newParent, p);
					printf("Sobrescrito.\n");
					return true;
				} else if (c == 'N') {
					return false;
				} else {
					printf("Opcao invalida.\n");
				}
			} else {
				if (!last) {
					parent->first = p->next;
		      	} else {
					last->next = p->next;
				}
				p->modification_date = mod_date;
				insertNode(newParent, p);
				return true;
			}
		}
        last = p;
        p = p->next;
    }
    return false;
}

void extractFromInput(const char* str, char* info, int action, TNT *arv) {
	TNT *tree = arv;
	int i = 2;      //ignora os dois primeiro caracteres
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
	if (action == EXTRACT_PATH) {
		if (lastBars[0] == 0) {
			i = lastBars[2] - lastBars[1] - 1;
			strncpy(info, &str[lastBars[1] + 1], i);
		} else {
			i = lastBars[1] - lastBars[0] - 1;
			strncpy(info, &str[lastBars[0] + 1], i);
		}
		info[i] = '\0';
		char t[LENGTH_PATH] = "";
		info[strlen(info)] = '\0';
		if (strcmp(info, "NULL")) {
			strcpy(info, getPath(search(tree, info), t));
			info[strlen(info)-1] = '\0';
		}
	} if (action == EXTRACT_NAME) {
		if (lastBars[0] == 0){
			i = lastBars[1] - 1;	
			strncpy(info, &str[2], i);
		} else {
			i = lastBars[0] - 1;	
			strncpy(info, &str[2], i);
		}
        info[i-1] = '\0';
	} if (action == EXTRACT_TIME) {
        strcpy(info, &str[lastBars[2] + 1]);
    } if (action == EXTRACT_SIZE) {
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

TNT* searchDir(TNT* arv, const char* name) {
	TNT *root = arv;
    if (!root)
        return NULL;
    TNT* node = root->first;
    while (node) {
        if (!strcmp(name, node->name) && node->type == TYPE_DIRECTORY)
            break;
        node = node->next;
    }
    return node;
}

TNT* searchDirByPath(TNT* arv, const char* path) {
	TNT *root = arv;
    if (!root)
        return NULL;
    if (!strcmp(path, getRootName(root))) {
        while (root->parent) {
        	root = root->parent;
		}
		return root;
	}
    int i = 0, j = 0;
    while (1) {
        if (path[i] == '/' || path[i] == '\0') {
            char name[LENGTH_NAME];
            strncpy(name, &path[j], i - j);
            name[i - j] = '\0';
            root = searchDir(root, name);
            j = i + 1;
            if (!root || root->type != TYPE_DIRECTORY)
                return NULL;
            if (path[i] == '\0')
                break;
        }
        i++;

    }
    return root;
}

TNT* searchFileByPath(TNT* arv, const char* path, const char* name, int type) {
	TNT *root = arv;
    root = searchDirByPath(root, path);
    if (!root)
        return NULL;
    TNT* child = root->first;
    while(child) {
        if (!strcmp(child->name, name) && child->type == type)
            return child;
        child = child->next;
    }
    return NULL;
}

TNT* makeDirByPath(TNT* arv, char* path, const char* time){
	TNT *root = arv;
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

TNT* makeFileByPath(TNT* arv, char* path, int type, const char* time, const char* name, int size) {
	TNT *root = arv;
    if (strcmp(path, getRootName(root)))
        root = makeDirByPath(root, path, time);
    TNT* node = allocNode(type, name, time, size);
    TNT *check = searchDependingOnType(root, node->name, type);
    if (!check || check->type != type){
    	insertNode(root, node);
    	return node;
	} 
	return NULL;
}
void listFiles(TNT *arv){
	TNT *tree = arv;
	if (!tree)
        return;
    tree = tree->first;
    while (tree) {
        printf("%s   ", tree->name);
        tree = tree->next;
    }
    printf("\n");
}
void listFilesDetailed(TNT *arv){
	TNT *tree = arv;
	char frs[255], siz[20];
	if (!tree)
        return;
    tree = tree->first;
    printf("\n");
    while (tree) {
    	strcpy(frs, "");
    	strcpy(siz, "");
    	if (tree->type == TYPE_DIRECTORY) {
    		strcat(frs, "DIR");
		} else if (tree->type == TYPE_TEXT_FILE) {
			TFIL *file = (TFIL*) tree->data;
			sprintf(siz, "%ld", (long int) file->size);
			strcat(frs, "TEXT");
			strcat(frs, " ");
			strcat(frs, siz);
		} else {
			TFIL *file = (TFIL*) tree->data;
			sprintf(siz, "%ld", (long int) file->size);
 			strcat(frs, "BIN");
 			strcat(frs, " ");
			strcat(frs, siz);
		}
		printf("%s", frs);
		strcpy(frs, " ");
		strcat(frs, tree->name);
		strcat(frs, "	");
		strcat(frs, tree->creation_date);
		//strcat(frs, "	");
		//strcat(frs, tree->modification_date);
        printf("%s\n", frs);
        printf("%s\n", tree->modification_date);
        tree = tree->next;
    }
    int *total = (int *) malloc (sizeof(int) * 2);
    total[0] = total[1] = 0;
    getTotalSubDirAndFile(arv, arv, total);
    printf("\n");
    printf("Sub-diretorios: %d\n", total[0]);
    printf("Arquivos: %d\n", total[1]);
    printf("\n");
}
int *getTotalSubDirAndFile(TNT *root, TNT* arv, int *total) {
	TNT *tree = arv;
	if (!tree) {
		return total;
	}
	if (tree->first) {
		getTotalSubDirAndFile(root, tree->first, total);
	} if (tree->next && tree != root) {
		getTotalSubDirAndFile(root, tree->next, total);
	}
	if (tree->type == TYPE_DIRECTORY) {
		TDIR *dir = (TDIR *) tree->data;
		total[0] += dir->n_directories;
		total[1] += dir->n_files;
	}
	return total;
}

char *getPath(TNT *arv, char *path){
	TNT *node = arv;
	if (!node) {
		return "";
	} if (!strcmp(node->name, getRootName(arv))) {
		return path;
	}
	if (node->parent){
		getPath(node->parent, path);
	}
	strcat(path, node->name);
	strcat(path, "/");
	return path;
}

char *getRootName(TNT *arv) {
	TNT *node = arv;
	while (node->parent) {
		node = node->parent;
	}
	return node->name;
}

TNT *searchDependingOnType(TNT* arv, char* name, int type){
	TNT *tree = arv;
	if (!tree) {
		return NULL;
	}
    if (!strcmp(name, tree->name) && type == tree->type)
        return tree;
    if (tree->first) {
        TNT* node = searchDependingOnType(tree->first, name, type);
        if (node)
            return node;
    }
    if (tree->next) {
        TNT* node = searchDependingOnType(tree->next, name, type);
        if (node)
            return node;
    }
    return NULL;
}
