#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <search.h>

#define n_threads 8
#define n_numbers 10000

/* Binary search tree */

struct _node
{
    int val;
    struct _node* esq;
    struct _node* dir;
};

typedef struct _node node;

node* inserir(node* elem, int num)
{
    if(!elem){
        node* new = malloc(sizeof(node));
        new->esq = NULL;
        new->dir = NULL;
        new->val = num;
        return new;
    }
    if(num<elem->val)
        elem->esq=inserir(elem->esq,num);
    else
        elem->dir=inserir(elem->dir,num);
    return elem; // Nao mudou
}

void print_tree(node* raiz)
{
    if(raiz){
        int i;
        print_tree(raiz->dir);
        printf("%d\n",raiz->val);
        print_tree(raiz->esq);
    }
}

int search_tree(node* raiz, int chave)
{
    if(raiz){
        if(raiz->val>chave)
            return search_tree(raiz->esq,chave);
        else if(raiz->val<chave)
            return search_tree(raiz->dir,chave);
        else{
            return 1; // Achou
        }
    }
    return 0; // Nao achou
}

/* ------------------------------------------- */

int verifica_primos(int num)
{
    /* Rotina que decide se um número é primo ou não */ 
    int cont;
    if(num==1)
        return 0;
    else if(num%2==0 && num>2)
        return 0;
    for(cont=3;cont<num/2;cont+=2)
        if(num%cont==0)
            return 0;
    return 1;
}


node* root = NULL; // Global root

pthread_mutex_t chave = PTHREAD_MUTEX_INITIALIZER;

void* verifica_numero(void* val)
{
    int* number = (int*) val;
    if(!verifica_primos(*number)){
        pthread_mutex_lock(&chave);
        root=inserir(root,*number);
        pthread_mutex_unlock(&chave);
    }
    return NULL;
}

int main()
{
    int i,j,stop_all=0;
    int numbers[n_threads][n_numbers];
    pthread_t threads[n_threads];
    while(1){
        for(i=0;i<n_threads;i++){
            for(j=0;j<n_numbers;j++){
                scanf("%d", &numbers[i][j]);
                if(numbers[i]==-1){
                    stop_all=1;
                    break;
                }
            if(stop_all)
                break
            }
        }
        for(j=i-1;j>=0;j--)
            pthread_create(&threads[j],NULL,verifica_numero,&numbers[j]);
        for(j=i-1;j>=0;j--)
            pthread_join(threads[j],NULL);
        if(stop_all)
            break;
    }
    print_tree(root);
    return 0;
}
