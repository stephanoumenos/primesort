#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <search.h>

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

void *thread_job(int* number)
{
    if(!verifica_primos(*number)){
        pthread_mutex_lock(&chave);
        root=inserir(root,*number);
        pthread_mutex_unlock(&chave);
    }
    return NULL;
}

int main()
{
    int numbers[8],i,j, stop_all=0;
    pthread_t t1,t2,t3,t4,t5,t6,t7,t8;
    pthread_t threads[8] = {t1,t2,t3,t4,t5,t6,t7,t8};
    while(1){
        for(i=0;i<8;++i){
            scanf("%d", &numbers[i]);
            if(numbers[i]==-1){
                break;
                stop_all=1;
            }
        }
        for(j=i-1;j>0;j--)
            pthread_create(&threads[i],NULL,thread_job,&numbers[i]);
        for(i=i-1;i>0;i--)
            pthread_join(threads[i],NULL);
        if(stop_all)
            break;
    }
    print_tree(root);
    return 0;
}
