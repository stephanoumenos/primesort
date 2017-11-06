#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n_threads 16
#define n_numbers 5000
#define initial_stack_size 1000 // 1000*sizeof(int) = 2KB
/* Obs: It's alright to use such a small array. It's very quick to make new ones and this makes the code much more portable. */

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

pthread_mutex_t chave = PTHREAD_MUTEX_INITIALIZER;

long int main_stack_counter = 0;
int* main_stack=NULL; // Temos que alocar o inicial no main porque o C é meio chato com isso
long int stack_size = initial_stack_size; // To keep track of current stack size

void* verifica_numero(void* val)
{
    int i,j=0, nao_primos[n_numbers];
    int* numbers = (int*) val;
    for(i=0;i<n_numbers;i++){
        if(numbers[i]==-1)
            break;
        if(!verifica_primos(numbers[i]))
            nao_primos[j++]=numbers[i];
    }
    if(!j)
        return NULL;
    pthread_mutex_lock(&chave);
    for(--j;j>=0;j--){
        if(main_stack_counter<stack_size)
            main_stack[main_stack_counter++]=nao_primos[j]; 
        else{ // Oh jeez, we need a bigger array!
            int* new_stack = malloc(2*stack_size*sizeof(int));
            for(long int i=0;i<main_stack_counter;i++)
                new_stack[i]=main_stack[i]; // Copies old stack
            stack_size*=2;
            free(main_stack);
            main_stack=new_stack;
            main_stack[main_stack_counter++]=nao_primos[j];
        }
    }
    pthread_mutex_unlock(&chave);
    return NULL;
}

int cmp(const void* a, const void* b)
{
    return (* (int*) a-*(int*) (b));
}

int main()
{
    main_stack = malloc(initial_stack_size*sizeof(int));
    int i,j,stop_all=0;
    int numbers[n_threads][n_numbers];
    pthread_t threads[n_threads];
    while(1){
        for(i=0;i<n_threads;i++)
            for(j=0;j<n_numbers;j++){
                scanf("%d", &numbers[i][j]);
                if(numbers[i][j]==-1){
                    stop_all=1;
                    goto work;
                }
            }
    work:
        for(j=0;j<=(i>n_threads-1?n_threads-1:i);j++)
            pthread_create(&threads[j],NULL,verifica_numero,&numbers[j]);
        for(j=0;j<=(i>n_threads-1?n_threads-1:i);j++)
            pthread_join(threads[j],NULL);
        if(stop_all)
            break;
    }
    qsort(main_stack,main_stack_counter,sizeof(int),cmp);
    for(main_stack_counter--;main_stack_counter>=0;main_stack_counter--)
        printf("%d\n",main_stack[main_stack_counter]);
    free(main_stack);
    return 0;
}
