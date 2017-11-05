#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <search.h>

#define n_threads 8
#define n_numbers 20000
#define stack_size 2000000 // 2000000*sizeof(int) = 4MB

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
int main_stack[stack_size];

void* verifica_numero(void* val)
{
    int i,j=0, nao_primos[n_numbers+1];
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
       for(--j;j>0;j--)
          main_stack[main_stack_counter++]=nao_primos[j]; 
    pthread_mutex_unlock(&chave);
    return NULL;
}

int cmp(const void* a, const void* b)
{
    return (* (int*) a-*(int*) (b));
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
                if(numbers[i][j]==-1){
                    stop_all=1;
                    goto work;
                }
            }
        }
    work:
        for(j=i>8? i-1:i;j>=0;j--)
            pthread_create(&threads[j],NULL,verifica_numero,&numbers[j]);
        for(j= i>8?i-1:i;j>=0;j--)
            pthread_join(threads[j],NULL);
        if(stop_all)
            break;
    }
    printf("YOLOLO: %ld", main_stack_counter);
    qsort(main_stack,main_stack_counter-1,sizeof(int),cmp);
    for(main_stack_counter--;main_stack_counter>0;main_stack_counter--)
        printf("%d\n",main_stack[main_stack_counter]);
    return 0;
}
