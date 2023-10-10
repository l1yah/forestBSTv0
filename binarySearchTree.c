#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct item_node_struct{
    char name[32];
    int count;
    struct item_node_struct *left, *right;
};

typedef struct item_node_struct item_node;

struct tree_name_node_struct{
    char treeName[32];
    struct tree_name_node_struct *left, *right;
    item_node *theTree;
};

typedef struct tree_name_node_struct tree_name_node;

void remove_crlf(char *s){
    char *t = s + strlen(s);
    t--;
    while ((t >= s) && (*t == '\n'|| *t == '\r')){
      *t = '\0';
       t--;
    }
}

// a function to read the next nonblank line of an input file. credits to Doctor Gerber for this function.
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length){
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


tree_name_node *new_tree_name_node(char *inputName){
    tree_name_node *t = malloc(sizeof(tree_name_node));
    strcpy(t->treeName, inputName);
    t->left = NULL;
    t->right = NULL;
    t->theTree = NULL;

    return t;
}

item_node *new_item_node(char *inputItemName, int inputCount){
    item_node *item = malloc(sizeof(item_node));
    strcpy(item->name, inputItemName);
    item->count = inputCount;
    item->left = NULL;
    item->right = NULL;

    return  item;
}

void dispose_node (tree_name_node *parent){
    free(parent);
}

void dispose_tree(tree_name_node *parent){
    if (parent->left){
        dispose_tree(parent->left);
    }
    if (parent->right){
        dispose_tree(parent->right);
    }
    dispose_node(parent);
}

tree_name_node *insertTopHalf(tree_name_node *parent, tree_name_node *new){
    
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

item_node *insertLowerHalf(item_node *parent, item_node *new){
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

tree_name_node *search_top_half(tree_name_node *rootNode, char *treeInputName){
    if (rootNode == NULL){
       return NULL;
    }
    else if (strcmp(rootNode->treeName, treeInputName) == 0){
        return(rootNode);
    }
    else if (strcmp(rootNode->treeName, treeInputName) > 0){
        return(search_top_half(rootNode->left, treeInputName));
    }
    else {
        return(search_top_half(rootNode->right, treeInputName));
    }
    
}
item_node *search_lower_half(item_node *parent, char *itemInputName){
    if (parent == NULL){
        return parent;
    }
    else{
        if (strcmp(parent->name, itemInputName) == 0){
            return parent;
        }
        if (strcmp(parent->name, itemInputName) > 0){
            return (search_lower_half(parent->left, itemInputName));
        }
        else{
            return(search_lower_half(parent->right, itemInputName));
        }
    }
}

void deleteSubTrees(item_node *parent){
    if (parent == NULL){
        return;
    }
    else{
        deleteSubTrees(parent->left);
        deleteSubTrees(parent->right);
        free(parent);
    }
}

tree_name_node *findMinChild_tree(tree_name_node *parent){
    if (parent == NULL){
        return NULL;
    }
    else if (parent->left != NULL){
        return(findMinChild_tree(parent->left));
    }
    return parent;
}

item_node *findMinChild_item(item_node *parent){
    if (parent == NULL){
        return NULL;
    }
    else if (parent->left != NULL){
        return(findMinChild_item(parent->left));
    }
    return parent;
}


tree_name_node *deletetreeNode(tree_name_node *parent, char *deletename){
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
            tree_name_node *temp;

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
        else{ //condition #3: two children
            tree_name_node *temp = findMinChild_tree(parent->right);
            strcpy(temp->treeName, parent->treeName);
            parent->theTree = temp->theTree;
            parent->right = deletetreeNode(parent->right, temp->treeName);
        }
    }
    return parent;

}

void delete_tree(tree_name_node *parent, char *name){
    deletetreeNode(parent, name);
}


void tree_traverse_in_order(tree_name_node *rootNode, FILE *out){
    if(rootNode != NULL){
        tree_traverse_in_order(rootNode->left, out);
        fprintf(out, "%s ", rootNode->treeName);
        tree_traverse_in_order(rootNode->right, out);
    }
}

void item_node_traverse_in_order(item_node *parent, FILE *out){
    if (parent!= NULL){

        item_node_traverse_in_order(parent->left, out);

        fprintf(out, "%s ", parent->name);

        item_node_traverse_in_order(parent->right, out);
    }
}

void item_traverse_in_order(tree_name_node *parent, FILE *out){
    if(parent != NULL){
        item_traverse_in_order(parent->left, out);

        fprintf(out, "\n======%s======\n", parent->treeName);
        item_node_traverse_in_order(parent->theTree, out);

        item_traverse_in_order(parent->right, out);
    }
}


void traverse_entire_tree(tree_name_node *rootNode, FILE *out){
    tree_traverse_in_order(rootNode, out);
    item_traverse_in_order(rootNode, out);
}

void countItems(item_node *parent, char* item, int *count){
    if (parent!= NULL){
        if(strcmp(parent->name, item) > 0){
            *count += 1;
        }
        countItems(parent->left, item, count);
        countItems(parent->right, item, count);
    }
}


int item_before(tree_name_node *parent, char *tree, char *item){
    int count = 0;

    tree_name_node *temp = search_top_half(parent, tree);
    countItems(temp->theTree, item, &count);
    return count;
}

void countTreeItems(item_node *parent, int *count){
    if (parent!= NULL){
        countTreeItems(parent->left, count);
        countTreeItems(parent->right, count);
        *count += parent->count;
    }
}
item_node *deleteItemNode(item_node *parent, char *name){
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
            item_node *temp;
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
            item_node *temp = findMinChild_item(parent->right);
            strcpy(temp->name, parent->name);
            parent->count = temp->count;
            parent->right = deleteItemNode(parent->right, temp->name);

        }
    }
    return parent;

}

void deleteItem(tree_name_node *parent, char *treeName, char *itemName){
    tree_name_node *temp = search_top_half(parent, treeName);
    temp->theTree = deleteItemNode(temp->theTree, itemName);
}

int count(tree_name_node *parent, char *tree){
    int count = 0;
    tree_name_node *temp = search_top_half(parent, tree);
    countTreeItems(temp->theTree, &count);

    return count;
}

int height_subtrees(item_node *parent){
    if (parent == NULL){
        return -1;
    }
    else{
        int ld = height_subtrees(parent->left);
        int rd = height_subtrees(parent->right);

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

void height_balance(tree_name_node *parent, char *name, FILE *out){
    int l, r, heightDiff;
    tree_name_node *temp1 = search_top_half(parent, name);
    item_node *temp2 = temp1->theTree;

    l = height_subtrees(temp2->left);
    r = height_subtrees(temp2->right);

    heightDiff = abs(l-r);
    if (heightDiff <= 1){
        fprintf(out, "tree %s has a left height of %d, a right height of %d, and a difference of %d. balanced.\n", name, l, r, heightDiff);
    }
    else{
        fprintf(out, "tree %s has a left height of %d, a right height of %d, and a difference of %d. not balanced.\n", name, l, r, heightDiff);
    }

}




void commands(FILE *in, FILE *out, tree_name_node *rootNode, int totalCommandNum){
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

         tree_name_node *pt = search_top_half(rootNode, tree);

          if (pt == NULL){
             fprintf(out, "tree %s doesn't exist\n", tree);
             commandcount--;
            }
          else{
             item_node *ipt = search_lower_half(pt->theTree, item);
             if (search_lower_half(pt->theTree, item) != NULL && delcount < 1){
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
            delete_tree(rootNode, tree);
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
    
    FILE *in = fopen("sampleInput.txt", "r");
    FILE *out = fopen("result.txt", "w");

    get_next_nonblank_line(in, buff, 255);
    sscanf(buff, "%d %d %d", &nTrees, &nItems, &nCommands);

    get_next_nonblank_line(in, buff, 31);
    sscanf(buff, "%s", inputName); 
    tree_name_node *rootNode = new_tree_name_node(inputName);
    
    
    for (int i = 0; i < nTrees - 1; i++){
        get_next_nonblank_line(in, buff, 31);
        sscanf(buff, "%s", inputName); 
        insertTopHalf(rootNode, new_tree_name_node(inputName));
    }


    for (int j = 0; j < nItems; j++){

        get_next_nonblank_line(in, buff, 64);
        sscanf(buff, "%s %s %d", item_treeName, itemName, &count); 
        
        if(search_top_half(rootNode, item_treeName)->theTree == NULL){
            search_top_half(rootNode, item_treeName)->theTree = insertLowerHalf(search_top_half(rootNode, item_treeName)->theTree, new_item_node(itemName, count));
        }
        else{
            insertLowerHalf(search_top_half(rootNode, item_treeName)->theTree, new_item_node(itemName, count));
        }
    }
    traverse_entire_tree(rootNode, out);
    fprintf(out, "\n\n");

    commands(in, out, rootNode, nCommands);

    dispose_tree(rootNode);
    return 0;
}