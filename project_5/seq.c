#include <omp.h>
#include <stdio.h>

main()
{
 char str[10];

 #pragma omp parallel
 {
    int a, b, c, d, e, f, g, h, i, j, k;
    #pragma omp for
    for(a = 33; a <= 126; a++)
    for(b = 33; b <= 126; b++)
    for(c = 33; c <= 126; c++)
    for(d = 33; d <= 126; d++)
    for(e = 33; e <= 126; e++)
    {
      
          
         // printf("%c%c%c%c%c\n", a, b, c, d, e);
    }
 }
}
