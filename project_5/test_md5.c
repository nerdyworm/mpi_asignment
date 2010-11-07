#include "md5.h"
#include "omp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * md5(char *str)
{
  md5_byte_t digest[16];
  md5_state_t state;
  md5_init  (&state);
	md5_append(&state, str, strlen(str));
	md5_finish(&state, digest);
  
  char hex[16 * 2 + 1]; int i;
  for (i = 0; i < 16; ++i)
    sprintf(hex + i * 2, "%02x", digest[i]);
  
  return strdup(hex);
}

int main()
{
  char *hex =  md5("a#*0Y");
    
  char str[5];
  #pragma omp parallel private(str)
  {
    int a, b, c, d, e;
    
    #pragma omp for
    for(a = 33; a <= 126; a++)
    for(b = 33; b <= 126; b++)
    for(c = 33; c <= 126; c++)
    for(d = 33; d <= 126; d++)
    for(e = 33; e <= 126; e++)
    {
      //sprintf(str, "%c%c%c%c%c", a, b, c, d, e);
      str[0] = a;
      str[1] = b;
      str[2] = c;
      str[3] = d;
      str[4] = e;

      //printf("%s\n", str);

      char *sum = md5(str);
      int result = strcmp(sum, hex);
      free(sum);
      
      if(result == 0) {
        printf("cracked: %s\n", str);
        exit(0);
      }
    }
  }
}
