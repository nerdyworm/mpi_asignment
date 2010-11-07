project_5/md5.h                                                                                     0000644 0001750 0001750 00000006500 11465625437 013754  0                                                                                                    ustar   benjamin                        benjamin                                                                                                                                                                                                               /*
  Copyright (C) 1999, 2002 Aladdin Enterprises.  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  L. Peter Deutsch
  ghost@aladdin.com

 */
/* $Id: md5.h,v 1.4 2002/04/13 19:20:28 lpd Exp $ */
/*
  Independent implementation of MD5 (RFC 1321).

  This code implements the MD5 Algorithm defined in RFC 1321, whose
  text is available at
	http://www.ietf.org/rfc/rfc1321.txt
  The code is derived from the text of the RFC, including the test suite
  (section A.5) but excluding the rest of Appendix A.  It does not include
  any code or documentation that is identified in the RFC as being
  copyrighted.

  The original and principal author of md5.h is L. Peter Deutsch
  <ghost@aladdin.com>.  Other authors are noted in the change history
  that follows (in reverse chronological order):

  2002-04-13 lpd Removed support for non-ANSI compilers; removed
	references to Ghostscript; clarified derivation from RFC 1321;
	now handles byte order either statically or dynamically.
  1999-11-04 lpd Edited comments slightly for automatic TOC extraction.
  1999-10-18 lpd Fixed typo in header comment (ansi2knr rather than md5);
	added conditionalization for C++ compilation from Martin
	Purschke <purschke@bnl.gov>.
  1999-05-03 lpd Original version.
 */

#ifndef md5_INCLUDED
#  define md5_INCLUDED

/*
 * This package supports both compile-time and run-time determination of CPU
 * byte order.  If ARCH_IS_BIG_ENDIAN is defined as 0, the code will be
 * compiled to run only on little-endian CPUs; if ARCH_IS_BIG_ENDIAN is
 * defined as non-zero, the code will be compiled to run only on big-endian
 * CPUs; if ARCH_IS_BIG_ENDIAN is not defined, the code will be compiled to
 * run on either big- or little-endian CPUs, but will run slightly less
 * efficiently on either one than if ARCH_IS_BIG_ENDIAN is defined.
 */

typedef unsigned char md5_byte_t; /* 8-bit byte */
typedef unsigned int md5_word_t; /* 32-bit word */

/* Define the state of the MD5 Algorithm. */
typedef struct md5_state_s {
    md5_word_t count[2];	/* message length in bits, lsw first */
    md5_word_t abcd[4];		/* digest buffer */
    md5_byte_t buf[64];		/* accumulate block */
} md5_state_t;

#ifdef __cplusplus
extern "C" 
{
#endif

/* Initialize the algorithm. */
void md5_init(md5_state_t *pms);

/* Append a string to the message. */
void md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);

/* Finish the message and return the digest. */
void md5_finish(md5_state_t *pms, md5_byte_t digest[16]);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif /* md5_INCLUDED */
                                                                                                                                                                                                project_5/sample.c                                                                                  0000600 0001750 0001750 00000002541 11465625300 014521  0                                                                                                    ustar   benjamin                        benjamin                                                                                                                                                                                                               #include <stdio.h> 
#include <string.h> 
#include <stddef.h> 
#include <stdlib.h> 
#include "mpi.h" 
#define CHUNKSIZE   10 
#define N       100 

void openmp_code(){ 
	int nthreads, tid, i, chunk; 
	float a[N], b[N], c[N]; 
	for (i=0; i < N; i++) 
		a[i] = b[i] = i * 1.0;             // initialize arrays 
	chunk = CHUNKSIZE; 
	#pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid) 
	{ 
		tid = omp_get_thread_num(); 
		if (tid == 0){ 
			nthreads = omp_get_num_threads(); 
			printf("Number of threads = %d\n", nthreads); 
		} 
		// printf("Thread %d starting...\n",tid); 
		#pragma omp for schedule(dynamic,chunk) 
		for (i=0; i<N; i++){ 
			c[i] = a[i] + b[i]; 
			// printf("Thread %d: c[%d]= %f\n",tid,i,c[i]); 
		} 
	}  /* end of parallel section */ 
} 

main(int argc, char **argv ) { 
	char message[20]; 
	int i,rank, size, type=99; 
	MPI_Status status; 
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	if(rank == 0) { 
		strcpy(message, "Hello, world"); 
		for (i=1; i<size; i++) 
			MPI_Send(message, 13, MPI_CHAR, i, type, MPI_COMM_WORLD); 
	} 
	else 
		MPI_Recv(message, 20, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status); 
	if( 1){ //* if this machine has multiple cores 
		openmp_code(); //run the mpi code 
	} 
	printf( "Message from process =%d : %.13s\n", rank,message); 
	MPI_Finalize(); 
}
                                                                                                                                                               project_5/seq.c                                                                                     0000644 0001750 0001750 00000000605 11465620612 014040  0                                                                                                    ustar   benjamin                        benjamin                                                                                                                                                                                                               #include <omp.h>
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
                                                                                                                           project_5/test_md5.c                                                                                0000644 0001750 0001750 00000002065 11465625421 015001  0                                                                                                    ustar   benjamin                        benjamin                                                                                                                                                                                                               #include "md5.h"
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           