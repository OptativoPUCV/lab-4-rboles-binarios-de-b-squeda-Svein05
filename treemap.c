#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree->root == NULL)
    {
        tree->root = createTreeNode(key, value);
        return;
    }

    TreeNode* aux = tree->root;
    TreeNode* padre = NULL;

    while(aux != NULL)
    {
        padre = aux;

        if (is_equal(tree, key, aux->pair->key))
            return;

        if (tree->lower_than(key, aux->pair->key))
            aux = aux->left;
        else
            aux = aux->right;
    }

    TreeNode* newNodo = createTreeNode(key, value);
    if (newNodo == NULL) return;

    newNodo->parent = padre;
    if (tree->lower_than(key, padre->pair->key))
        padre->left = newNodo;
    else
        padre->right = newNodo;

    tree->current = newNodo;
    
    return;
}

TreeNode * minimum(TreeNode * x)
{
    if (x == NULL) return NULL;

    TreeNode* aux = x;
    while (aux->left != NULL) aux = aux->left;

    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    if (tree == NULL || tree->root == NULL) return;

    // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL)
    {
        if (node == tree->root) {
            tree->root = NULL; // Si es la raíz, actualizamos el árbol
        } else if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    // Caso 2: Nodo con un solo hijo (derecha)
    if (node->left == NULL)
    {
        if (node == tree->root) {
            tree->root = node->right;
        } else if (node->parent->left == node) {
            node->parent->left = node->right;
        } else {
            node->parent->right = node->right;
        }
        if (node->right != NULL) {
            node->right->parent = node->parent;
        }
        free(node->pair);
        free(node);
        return;
    }

    // Caso 2: Nodo con un solo hijo (izquierda)
    if (node->right == NULL)
    {
        if (node == tree->root) {
            tree->root = node->left;
        } else if (node->parent->left == node) {
            node->parent->left = node->left;
        } else {
            node->parent->right = node->left;
        }
        if (node->left != NULL) {
            node->left->parent = node->parent;
        }
        free(node->pair);
        free(node);
        return;
    }

    TreeNode* minimo = minimum(node->right);

    node->pair->key = minimo->pair->key;
    node->pair->value = minimo->pair->value;

    removeNode(tree, minimo);

    return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    TreeNode* aux = tree->root;
    while(aux != NULL)
    {
        if (is_equal(tree, key, aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        
        if (tree->lower_than(key, aux->pair->key)) 
            aux = aux->left;
        else 
            aux = aux->right;
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode* minimo = minimum(tree->root);
    if (minimo == NULL) return NULL;

    tree->current = minimo;
    return minimo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    
    if (tree == NULL || tree->root == NULL || tree->current == NULL) return NULL;

    TreeNode* actual = tree->current;

    if (actual->right != NULL)
    {
        TreeNode* minimo = minimum(actual->right);
        tree->current = minimo;
        return minimo->pair;
    }

    TreeNode* padre = actual->parent;
    while (padre != NULL && actual == padre->right)
    {
        actual = padre;
        padre = padre->parent;
    }

    tree->current = padre;
    return padre->pair;
}
