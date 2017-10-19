#include <stdio.h>

int main()
{
    // remove números pares: cat test04.in | grep .*[13579]$
    FILE* input = popen("grep .*[13579]$", "r");
    char buffer[1024];
    while(fgets(buffer, 1024, input))
        printf("%s\n", buffer);
    return 0;
}
