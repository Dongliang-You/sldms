#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#include "../external/gsacak.h"
#include "rankbv.h"

#ifndef UCHAR_SIZE
  #define UCHAR_SIZE 256
#endif

#ifndef M64
	#define M64 0
#endif

#ifndef DEBUG
	#define DEBUG 0 
#endif

#if M64
	typedef int64_t  int_t;
	typedef uint64_t uint_t;
	#define PRIdN	 PRId64
	#define U_MAX 	 UINT64_MAX
	#define I_MAX 	 INT64_MAX
	#define I_MIN 	 INT64_MIN
#else
	typedef int32_t  int_t;
	typedef uint32_t uint_t;
	#define PRIdN	 PRId32
	#define U_MAX 	 UINT32_MAX
	#define I_MAX	 INT32_MAX
	#define I_MIN	 INT32_MIN
#endif

typedef uint32_t int_text;

#ifndef TERMINATOR
  #define TERMINATOR 1
#endif

#ifndef DNA
  #define DNA 0
#endif

#if DNA
  #define terminator '#'
  #if TERMINATOR
    #define separator '$'
  #else
    #define separator '#'
  #endif
#else
  #define terminator 0
  #if TERMINATOR
    #define separator 1
  #else
    #define separator 0
  #endif
#endif

/**********************************************************************/

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

void   time_start(time_t *t_time, clock_t *c_clock);
double time_stop(time_t t_time, clock_t c_clock);

void die(const char* where);
void dies(const char* where, char* format, ...);

int_t print_int(int_t* A, size_t n);
int_t print_char(char* A, int_t n);

unsigned char* cat(unsigned char* s1, unsigned char* s2, size_t *n);
unsigned char* cat_all(unsigned char** R, int k, size_t *n, int verbose, int upper);

int print_array(unsigned char *str, int_da *DA, int_t *SA, int_t *ISA, int_t *LCP,size_t n, size_t m, int last_end,int dnum,FILE *fp,int temptopk);
int print_array_raw(unsigned char *str, int_da *DA, int_t *SA, int_t *ISA, int_t *LCP,size_t n, size_t m, int last_end,int dnum,FILE *fp,int temptopk,int tempFault_tolerance);

/**********************************************************************/

#endif
