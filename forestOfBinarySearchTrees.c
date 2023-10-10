#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct itemNodeStruct{
    char name[32];
    int count;
    struct itemNodeStruct *left, *right;
};

typedef struct itemNodeStruct itemNode;

struct treeNameNodeStruct{
    char treeName[32];
    struct treeNameNodeStruct *left, *right;
    itemNode *theTree;
};

typedef struct treeNameNodeStruct treeNameNode;

void remove_crlf(char *s){
    char *t = s + strlen(s);
    t--;
    while ((t >= s) && (*t == '\n'|| *t == '\r')){
      *t = '\0';
       t--;
    }
}

int readNextLine(FILE *ifp, char *buf, int max_length){
    buf[0] = '\0';    
    while(!feof(ifp) && (buf[0] == '\0')){        
        fgets(buf, max_length, ifp);        
        remove_crlf(buf);    
    }    
    if(buf[0] != '\0') { 
        return 1;  
    }
    else{        
        return 0;    
    }
}


treeNameNode *new_treeNameNode(char *inputName){
    treeNameNode *t = malloc(sizeof(treeNameNode));
    strcpy(t->treeName, inputName);
    t->left = NULL;
    t->right = NULL;
    t->theTree = NULL;

    return t;
}

itemNode *new_itemNode(char *inputItemName, int inputCount){
    itemNode *item = malloc(sizeof(itemNode));
    strcpy(item->name, inputItemName);
    item->count = inputCount;
    item->left = NULL;
    item->right = NULL;

    return  item;
}

void dispose_node (treeNameNode *parent){
    free(parent);
}

void disposeTree(treeNameNode *parent){
    if (parent->left){
        disposeTree(parent->left);
    }
    if (parent->right){
        disposeTree(parent->right);
    }
    dispose_node(parent);
}

treeNameNode *insertTopHalf(treeNameNode *parent, treeNameNode *new){
    
    if (strcmp(parent->treeName, new->treeName) > 0){
        if (parent->left != NULL){
            return(insertTopHalf(parent->left, new));
        }
        else{
            parent->left = new;
            return new;
        }
    }

    else {
        if (parent->right != NULL){
            return(insertTopHalf(parent->right, new));
        }
        else{
            parent->right = new;
            return new;
        }
    }
}

itemNode *insertLowerHalf(itemNode *parent, itemNode *new){
    if (parent == NULL){
        parent = new;
        return new;
    }
    
    if (strcmp(parent->name, new->name) > 0){
        if (parent->left != NULL){
            return(insertLowerHalf(parent->left, new));
        }
        else{
            parent->left = new;
            return new;
        }
    }
    else {
        if (parent->right != NULL){
            return(insertLowerHalf(parent->right, new));
        }
        else{
            parent->right = new;
            return new;
        }
    }
}

treeNameNode *searchTopHalf(treeNameNode *rootNode, char *treeInputName){
    if (rootNode == NULL){
       return NULL;
    }
    else if (strcmp(rootNode->treeName, treeInputName) == 0){
        return(rootNode);
    }
    else if (strcmp(rootNode->treeName, treeInputName) > 0){
        return(searchTopHalf(rootNode->left, treeInputName));
    }
    else {
        return(searchTopHalf(rootNode->right, treeInputName));
    }
    
}
itemNode *searchLowerHalf(itemNode *parent, char *itemInputName){
    if (parent == NULL){
        return parent;
    }
    else{
        if (strcmp(parent->name, itemInputName) == 0){
            return parent;
        }
        if (strcmp(parent->name, itemInputName) > 0){
            return (searchLowerHalf(parent->left, itemInputName));
        }
        else{
            return(searchLowerHalf(parent->right, itemInputName));
        }
    }
}

void deleteSubTrees(itemNode *parent){
    if (parent == NULL){
        return;
    }
    else{
        deleteSubTrees(parent->left);
        deleteSubTrees(parent->right);
        free(parent);
    }
}

treeNameNode *findMinChild_tree(treeNameNode *parent){
    if (parent == NULL){
        return NULL;
    }
    else if (parent->left != NULL){
        return(findMinChild_tree(parent->left));
    }
    return parent;
}

itemNode *findMinChild_item(itemNode *parent){
    if (parent == NULL){
        return NULL;
    }
    else if (parent->left != NULL){
        return(findMinChild_item(parent->left));
    }
    return parent;
}


treeNameNode *deletetreeNode(treeNameNode *parent, char *deletename){
    if (parent == NULL){
        return NULL;
    }
    if(strcmp(parent->treeName, deletename) < 0){
        parent->right = deletetreeNode(parent->right, deletename);
    }
    else if (strcmp(parent->treeName, deletename) > 0){
        parent->left = deletetreeNode(parent->left, deletename);
    }
    else{
        if (parent->left == NULL && parent->right == NULL){ //condition #1: no children
            deleteSubTrees(parent->theTree);
            free(parent);
            return NULL;
        }
        else if(parent->left == NULL || parent->right == NULL){ //condition #2: one child
            treeNameNode *temp;

            if (parent->left == NULL){
                temp = parent->right;
            }
            else{
                temp = parent->left;
            }
            deleteSubTrees(parent->theTree);
            free(parent);
            return temp;
        }
        else{ 
            treeNameNode *temp = findMinChild_tree(parent->right);
            strcpy(temp->treeName, parent->treeName);
            parent->theTree = temp->theTree;
            parent->right = deletetreeNode(parent->right, temp->treeName);
        }
    }
    return parent;

}

void deleteTree(treeNameNode *parent, char *name){
    deletetreeNode(parent, name);
}


void InOrderTraversal(treeNameNode *rootNode, FILE *out){
    if(rootNode != NULL){
        InOrderTraversal(rootNode->left, out);
        fprintf(out, "%s ", rootNode->treeName);
        InOrderTraversal(rootNode->right, out);
    }
}

void itemNodeInOrderTraversal(itemNode *parent, FILE *out){
    if (parent!= NULL){

        itemNodeInOrderTraversal(parent->left, out);

        fprintf(out, "%s ", parent->name);

        itemNodeInOrderTraversal(parent->right, out);
    }
}

void itemsInOrderTraversal(treeNameNode *parent, FILE *out){
    if(parent != NULL){
        itemsInOrderTraversal(parent->left, out);

        fprintf(out, "\n======%s======\n", parent->treeName);
        itemNodeInOrderTraversal(parent->theTree, out);

        itemsInOrderTraversal(parent->right, out);
    }
}


void fullTraversal(treeNameNode *rootNode, FILE *out){
    InOrderTraversal(rootNode, out);
    itemsInOrderTraversal(rootNode, out);
}

void countItems(itemNode *parent, char* item, int *count){
    if (parent!= NULL){
        if(strcmp(parent->name, item) > 0){
            *count += 1;
        }
        countItems(parent->left, item, count);
        countItems(parent->right, item, count);
    }
}


int item_before(treeNameNode *parent, char *tree, char *item){
    int count = 0;

    treeNameNode *temp = searchTopHalf(parent, tree);
    countItems(temp->theTree, item, &count);
    return count;
}

void countTreeItems(itemNode *parent, int *count){
    if (parent!= NULL){
        countTreeItems(parent->left, count);
        countTreeItems(parent->right, count);
        *count += parent->count;
    }
}
itemNode *deleteItemNode(itemNode *parent, char *name){
    if (parent == NULL){
        return NULL;
    }
    if (strcmp(parent->name, name) < 0){
        parent->right = deleteItemNode(parent->right, name);
    }
    else if (strcmp(parent->name, name) > 0){
        parent->left = deleteItemNode(parent->left, name);
    }
    else{
        if (parent->left == NULL && parent->right == NULL){ // no children
            free(parent);
            return NULL;
        }
        else if (parent->left == NULL || parent->right == NULL){ // 1 child
            itemNode *temp;
            if (parent->left == NULL){
                temp = parent->right;
            }
            else{
                temp = parent->left;
            }
            free(parent);
            return temp;
        }
        else{ //2 children
            itemNode *temp = findMinChild_item(parent->right);
            strcpy(temp->name, parent->name);
            parent->count = temp->count;
            parent->right = deleteItemNode(parent->right, temp->name);

        }
    }
    return parent;

}

void deleteItem(treeNameNode *parent, char *treeName, char *itemName){
    treeNameNode *temp = searchTopHalf(parent, treeName);
    temp->theTree = deleteItemNode(temp->theTree, itemName);
}

int count(treeNameNode *parent, char *tree){
    int count = 0;
    treeNameNode *temp = searchTopHalf(parent, tree);
    countTreeItems(temp->theTree, &count);

    return count;
}

int treesHeight(itemNode *parent){
    if (parent == NULL){
        return -1;
    }
    else{
        int ld = treesHeight(parent->left);
        int rd = treesHeight(parent->right);

        if (ld > rd){
            ld++;
            return ld;
        }
        else{
            rd++;
            return rd;
        }
    }
}

void height_balance(treeNameNode *parent, char *name, FILE *out){
    int l, r, heightDiff;
    treeNameNode *temp1 = searchTopHalf(parent, name);
    itemNode *temp2 = temp1->theTree;

    l = treesHeight(temp2->left);
    r = treesHeight(temp2->right);

    heightDiff = abs(l-r);
    if (heightDiff <= 1){
        fprintf(out, "tree %s has a left height of %d, a right height of %d, and a difference of %d. balanced.\n", name, l, r, heightDiff);
    }
    else{
        fprintf(out, "tree %s has a left height of %d, a right height of %d, and a difference of %d. not balanced.\n", name, l, r, heightDiff);
    }

}




void commands(FILE *in, FILE *out, treeNameNode *rootNode, int totalCommandNum){
    fprintf(out, "\n======== Processing Commands ========\n");
    char command[32];
    char tree[32];
    char item[32];
    int delcount = 0;

    int commandcount = totalCommandNum;

    fscanf(in, "%s", command);

    while(commandcount > 0 || !feof(in)){
        if (strcmp(command, "search") == 0){
          fscanf(in, "%s", tree);
          fscanf(in, "%s", item);

         treeNameNode *pt = searchTopHalf(rootNode, tree);

          if (pt == NULL){
             fprintf(out, "tree %s doesn't exist\n", tree);
             commandcount--;
            }
          else{
             itemNode *ipt = searchLowerHalf(pt->theTree, item);
             if (searchLowerHalf(pt->theTree, item) != NULL && delcount < 1){
                  fprintf(out, "%d %s in tree %s\n",ipt->count, ipt->name, tree);
                 commandcount--;
               }
             else{
                fprintf(out, "item %s doesn't exist in tree %s\n", item, tree);
                commandcount--;
              }
            }
        }
        else if (strcmp(command, "delete") == 0){
            fscanf(in, "%s", tree);
            fscanf(in, "%s", item);
            deleteItem(rootNode, tree, item);
            fprintf(out, "item %s from tree %s has been deleted\n", item, tree);
            delcount = 1;
            commandcount--;
        }
        else if (strcmp(command, "delete_tree") == 0){
            fscanf(in, "%s", tree);
            deleteTree(rootNode, tree);
            fprintf(out, "tree %s has been deleted\n", tree);
            commandcount = 0;
        }
        else if (strcmp(command, "item_before") == 0){
            fscanf(in, "%s", tree);
            fscanf(in, "%s", item);
            int count_before;

            count_before = item_before(rootNode, tree, item);
            fprintf(out, "items before %s: %d\n", item, count_before);
            commandcount--;
        }
        else if(strcmp(command, "height_balance") == 0){
            fscanf(in, "%s", tree);
            height_balance(rootNode, tree, out);
            commandcount--;
        }
        else if(strcmp(command, "count") == 0){
            fscanf(in, "%s", tree);
            int totalItemsinTree = count(rootNode, tree);
            fprintf(out, "total number of items in tree %s: %d\n", tree, totalItemsinTree);
            commandcount--;
        }
        
        fscanf(in, "%s", command);
    }

}
int main(void){
    int nTrees, nItems, nCommands;
    char buff[256];
    char inputName[32];
    char item_treeName[32];
    char itemName[32];
    int count;
    
    FILE *in = fopen("sampleInput-2.txt", "r");
    FILE *out = fopen("result2.txt", "w");

    readNextLine(in, buff, 255);
    sscanf(buff, "%d %d %d", &nTrees, &nItems, &nCommands);

    readNextLine(in, buff, 31);
    sscanf(buff, "%s", inputName); 
    treeNameNode *rootNode = new_treeNameNode(inputName);
    
    
    for (int i = 0; i < nTrees - 1; i++){
        readNextLine(in, buff, 31);
        sscanf(buff, "%s", inputName); 
        insertTopHalf(rootNode, new_treeNameNode(inputName));
    }


    for (int j = 0; j < nItems; j++){

        readNextLine(in, buff, 64);
        sscanf(buff, "%s %s %d", item_treeName, itemName, &count); 
        
        if(searchTopHalf(rootNode, item_treeName)->theTree == NULL){
            searchTopHalf(rootNode, item_treeName)->theTree = insertLowerHalf(searchTopHalf(rootNode, item_treeName)->theTree, new_itemNode(itemName, count));
        }
        else{
            insertLowerHalf(searchTopHalf(rootNode, item_treeName)->theTree, new_itemNode(itemName, count));
        }
    }
    fullTraversal(rootNode, out);
    fprintf(out, "\n\n");

    commands(in, out, rootNode, nCommands);

    disposeTree(rootNode);
    return 0;
}