#include <stdio.h>

#define buffer_size 5120

void verifica_primos(int buffer[], unsigned int buffer_size)
{
   /* Rotina que decide se um número é primo ou não */ 


}

int main()
{
    // remove números pares: cat test04.in | grep .*[13579]$
    int number=0;
    int buffer[buffer_size];
    unsigned int i = 0;
    while(number!=-1){
        scanf("%d", number);
        if(number%2) // Se o número é impar, é candidato
            buffer[i++] = number;
        if(i==buffer_size+1){ // Estourou o buffer
            i = 0; // Resseta flag        
            verifica_primos(buffer,buffer_size);
        }
    }
    return 0;
}
