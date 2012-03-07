//#define USE_SSE4

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#ifdef USE_SSE4
	#include <smmintrin.h>
#else
	#include <pmmintrin.h>
#endif

#define VL 4

inline void dp4f_asm(float *, float *, float *);
inline void dp4f(float *, float *, float *);
inline void dp4(__m128, __m128, float *);
static unsigned long long rdtsctime();
void rand_vec(float* vec, int len);
void print_vec(float* vec, int len);

int main() {

#ifdef _SMMINTRIN_H_INCLUDED
	printf("SSE4 enabled\n");
#endif

#ifdef _PMMINTRIN_H_INCLUDED
	printf("SSE3 enabled\n");
#endif

	unsigned long long flop, start, stop, cycles;
	flop = 2*VL;
	
	float dpd __attribute__((aligned(16)));
	float *a = _mm_malloc(VL*sizeof(float), 16);
	float *b = _mm_malloc(VL*sizeof(float), 16);

	rand_vec(a, VL);
	rand_vec(b, VL);

	printf("Vektor A:");
	print_vec(a, VL);
	printf("\n");


	printf("Vektor B:");
	print_vec(b, VL);
	printf("\n");

	start = rdtsctime();
	
	dp4f(a, b, &dpd);
	
	stop = rdtsctime();
	cycles = stop - start;	

	printf("cycles: %llu flops: %f \n", cycles, flop/cycles);
	printf("dot product is %f\n", dpd);

	return 0;
}

/**
 *  Generate random vector of length len
 *  with elements floats in range 0 to 1
 */

void rand_vec(float* vec, int len) {
	srand(getpid() * time(NULL));
	
	for(int i = 0; i < len; i++) {
		vec[i] = rand()*1.0f/RAND_MAX;
	}
}

/**
 * Print vector of length len
 */
void print_vec(float* vec, int len) {

	for(int i = 0; i < len; i++) {
		printf("%f ", vec[i]);
	}
}

/**
 * Routine for calculating dot product of length 4
 * vector of floats (single precision)
 * written in inline asm using SSE4 instructions
 */

inline void dp4f_asm(float *v1, float *v2, float *dp) {
	__asm__(
		"movaps %1, %%xmm1 #jow\n\t"
		"movaps %2, %%xmm2 \n\t"
		"dpps $0xF1, %%xmm2, %%xmm1 \n\t"
		"movss %%xmm1, %0 \n\t" 
		:"=m"(dp[0])
		:"m"(v1[0]), "m"(v2[0])
		:"xmm1", "xmm2");
}

/**
 * Routine for calculating dot product of length 4
 * vector of floats (single precision)
 * written using intrisics. Choose between SSE3 and SSE4 
 * at compiletime using flag USE_SSE4
 */

inline void dp4f(float *v1, float *v2, float *dp) {
	__m128 a = _mm_load_ps(v1);
	__m128 b = _mm_load_ps(v2);
	dp4(a, b, dp);
}

#ifdef USE_SSE4
inline void dp4(__m128 v1, __m128 v2, float *dp) {
	__m128 tmp = _mm_dp_ps(v1, v2);
	_mm_store_ss(dp, tmp);	
}
#else
inline void dp4(__m128 v1, __m128 v2, float *dp) {
	__m128 prod = _mm_mul_ps(v1, v2);
	__m128 sum = _mm_hadd_ps(prod, prod);
	_mm_store_ss(dp, sum);	
}
#endif

/**
 * Timing method returnig number of cycles
 */

static unsigned long long rdtsctime() {
	unsigned int eax, edx;
	unsigned long long val;
	__asm__ __volatile__("rdtsc":"=a"(eax), "=d"(edx));
	val = edx;
	val = val << 32;
	val += eax;
	return val;
}
