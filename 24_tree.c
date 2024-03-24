#include <stdio.h>
#include <stdlib.h>
#define M 4
#include "24_tree.h"
 
void insert_24_tree(int key){
    struct node *newNode;
    int upKey;
    enum temp value;
    
    value = ins(root, key, &upKey, &newNode);
    
    if (value == Duplicate)
        printf("Value already inserted\n");
    if (value == InsertThis){
        
        struct node *uproot = root;
        root = malloc(sizeof(struct node));
        
        root->n = 1;
        root->keys[0] = upKey;
        root->p[0] = uproot;
        root->p[1] = newNode;
       
    }
}

enum temp ins(struct node *ptr, int key, int *upKey, struct node **newNode){
    struct node *newPtr, *lastPtr;
    int pos, i, n, splitPos;
    int newKey, lastKey;
    
    enum temp value;
    
    if (ptr == NULL){
        *newNode = NULL;
        *upKey = key;
        return InsertThis;
    }

    n = ptr->n;
    pos = findPos(key, ptr->keys, n);
    
    if (pos < n && key == ptr->keys[pos])
        return Duplicate;
    value = ins(ptr->p[pos], key, &newKey, &newPtr);
    
    if (value != InsertThis)
        return value;
    
    if (n < M - 1){
        pos = findPos(newKey, ptr->keys, n);
        
        for (i = n; i > pos; i--){
            ptr->keys[i] = ptr->keys[i - 1]; /* Shifting key and pointer to insert new key */
            ptr->p[i + 1] = ptr->p[i];
        }
        
        ptr->keys[pos] = newKey;
        ptr->p[pos + 1] = newPtr;
        ++ptr->n; /* Incrementing the number of keys in the node */
        return Success;
    }
     
    if (pos == M - 1){
        lastKey = newKey;
        lastPtr = newPtr;
    }

    else { /* If the keys in the nodes are at maximum and the position to insert node is the last one */
        lastKey = ptr->keys[M - 2];
        lastPtr = ptr->p[M - 1];
        
        for (i = M - 2; i > pos; i--){
            ptr->keys[i] = ptr->keys[i - 1];
            ptr->p[i + 1] = ptr->p[i];
        }
        ptr->keys[pos] = newKey;
        ptr->p[pos + 1] = newPtr;
    }
    splitPos = (M - 1) / 2;
    (*upKey) = ptr->keys[splitPos];

    (*newNode) = malloc(sizeof(struct node)); /* Right node after division */
    ptr->n = splitPos; /* Number of keys for the node divided to the left */
    (*newNode)->n = M - 1 - splitPos; /* Number of keys for the node divided to the right */
    for (i = 0; i < (*newNode)->n; i++){
        (*newNode)->p[i] = ptr->p[i + splitPos + 1];
        if (i < (*newNode)->n - 1)
            (*newNode)->keys[i] = ptr->keys[i + splitPos + 1];
        else
            (*newNode)->keys[i] = lastKey;
    }
    (*newNode)->p[(*newNode)->n] = lastPtr;
    return InsertThis;
}

void print(struct node *ptr, int space){
    if (ptr){
        int i;
        for (i = 1; i <= space; i++)
            printf(" ");
        for (i = 0; i < ptr->n; i++)
            printf("%d ", ptr->keys[i]);
        printf("\n");
        for (i = 0; i <= ptr->n; i++)
            print(ptr->p[i], space + 10);
    }
}

void search(int key){
    int pos, i, n;
    struct node *ptr = root;
    
    printf("Search the path:\n");
    
    while (ptr){
        n = ptr->n;
        for (i = 0; i < ptr->n; i++)
            printf(" %d", ptr->keys[i]);
        printf("\n");
        pos = findPos(key, ptr->keys, n);
        if (pos < n && key == ptr->keys[pos]){
            printf("Key %d found at position %d of the last printed node\n", key, i);
            return;
        }
        ptr = ptr->p[pos];
    }
    printf("The key %d is not available\n", key);
}

int findPos(int key, int *keyArray, int n){
    int pos = 0;
    while (pos < n && key > keyArray[pos])
        pos++;
    return pos;
}

void delete_24_tree(int key){
    struct node *uproot;
    enum temp value;

    value = del(root, key);
    
    switch (value){
        case SearchError:
            printf("The key %d is not available\n", key);
            break;
        case SmallerKey :
            uproot = root;
            root = root->p[0];
            free(uproot);
            break;
    }
}

enum temp del(struct node *ptr, int key){
    int pos, i, pivot, n, min;
    int *keyAux;
    enum temp value;
    struct node **p, *lptr, *rptr;

    if (ptr == NULL)
        return SearchError;
    /*Assign values of the node*/
    n = ptr->n;
    keyAux = ptr->keys;
    p = ptr->p;
    min = (M - 1) / 2; /*Minimum number of keys*/

    pos = findPos(key, keyAux, n); //Search for the key to delete
    // p is a leaf
    if (p[0] == NULL){
        if (pos == n || key < keyAux[pos])
            return SearchError;
        /*Swap keys and pointers on the left*/
        for (i = pos + 1; i < n; i++){
            keyAux[i - 1] = keyAux[i];
            p[i] = p[i + 1];
        }
        return --ptr->n >= (ptr == root ? 1 : min) ? Success : SmallerKey;
    }
    
    if (pos < n && key == keyAux[pos]){ //if key is found and p is not a leaf
        struct node *qp = p[pos], *qp1;
        int newKey;
        
        while (1){
            newKey = qp->n;
            qp1 = qp->p[newKey];
            if (qp1 == NULL)
                break;
            qp = qp1;
        }
        keyAux[pos] = qp->keys[newKey - 1];
        qp->keys[newKey - 1] = key;
    }
    
    value = del(p[pos], key);
    
    if (value != SmallerKey)
        return value;

    if (pos > 0 && p[pos - 1]->n > min){
        pivot = pos - 1; /*pivot for left and right nodes*/
        lptr = p[pivot];
        rptr = p[pos];
        /*Assign values for the right node*/
        rptr->p[rptr->n + 1] = rptr->p[rptr->n];
        for (i = rptr->n; i > 0; i--){
            rptr->keys[i] = rptr->keys[i - 1];
            rptr->p[i] = rptr->p[i - 1];
        }
        rptr->n++;
        rptr->keys[0] = keyAux[pivot];
        rptr->p[0] = lptr->p[lptr->n];
        keyAux[pivot] = lptr->keys[--lptr->n];
        return Success;
    }
    
    if (pos < n && p[pos + 1]->n > min){
        pivot = pos; /*pivot for left and right nodes*/
        lptr = p[pivot];
        rptr = p[pivot + 1];
        /*Assign values for the left node*/
        lptr->keys[lptr->n] = keyAux[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
        keyAux[pivot] = rptr->keys[0];
        lptr->n++;
        rptr->n--;
        for (i = 0; i < rptr->n; i++){
            rptr->keys[i] = rptr->keys[i + 1];
            rptr->p[i] = rptr->p[i + 1];
        }
        rptr->p[rptr->n] = rptr->p[rptr->n + 1];
        return Success;
    }

    if (pos == n)
        pivot = pos - 1;
    else
        pivot = pos;

    lptr = p[pivot];
    rptr = p[pivot + 1];
    /*Merge the left node with the right node*/
    lptr->keys[lptr->n] = keyAux[pivot];
    lptr->p[lptr->n + 1] = rptr->p[0];
    for (i = 0; i < rptr->n; i++){
        lptr->keys[lptr->n + 1 + i] = rptr->keys[i];
        lptr->p[lptr->n + 2 + i] = rptr->p[i + 1];
    }
    lptr->n = lptr->n + rptr->n + 1;
    free(rptr); /*remove the right node*/
    for (i = pos + 1; i < n; i++)
    {
        keyAux[i - 1] = keyAux[i];
        p[i] = p[i + 1];
    }
    return --ptr->n >= (ptr == root ? 1 : min) ? Success : SmallerKey;
}

