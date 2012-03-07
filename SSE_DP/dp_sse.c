#define USE_SSE4
//#undef USE_SSE4

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#ifdef USE_SSE4
	#include <smmintrin.h>
#else
	#include <pmmintrin.h>
#endif

#define VL 32


void rand_vec(float* vec, int len);
void print_vec(float* vec, int len);
void print_vec(float* vec, int len);
void dp4f_naive(float *v1, float *v2, float *dp);
inline void dp4f_asm(float *v1, float *v2, float *dp);
inline void dp4f(float *v1, float *v2, float *dp);
inline void dp4f_sse(float *v1, float *v2, float *dp);
#ifdef USE_SSE4
inline void dp4f_sse4(float *v1, float *v2, float *dp);
inline void dp4_sse4(__m128 v1, __m128 v2, float *dp);
void dp_sse4(float *a, float *b, float *dp, int len);
#endif
inline void dp4_sse(__m128 v1, __m128 v2, float *dp);
static unsigned long long rdtsctime();
void dp_naive(float *a, float *b, float *dp, int len);
void dp_sse(float *a, float *b, float *dp, int len);
void dp_asm(float *a, float *b, float *dp, int len);


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
		printf("%4.3f ", vec[i]);
	}
}

/**
 * Naive implementation of dotproduct using loop
 */

void dp4f_naive(float *v1, float *v2, float *dp) {
	for(int i = 0; i < 4; i++) {
		*dp += v1[i] * v2[i];
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
#ifdef USE_SSE4
	dp4f_sse4(v1, v2, dp);
#else
	dp4f_sse(v1, v2, dp);
#endif
}

inline void dp4f_sse(float *v1, float *v2, float *dp) {
	__m128 a = _mm_load_ps(v1);
	__m128 b = _mm_load_ps(v2);
	dp4_sse(a, b, dp);
}

inline void dp4_sse(__m128 v1, __m128 v2, float *dp) {
	__m128 prod = _mm_mul_ps(v1, v2);
	__m128 sum = _mm_hadd_ps(prod, prod);
	_mm_store_ss(dp, sum);	
}

#ifdef USE_SSE4
inline void dp4f_sse4(float *v1, float *v2, float *dp) {
	__m128 a = _mm_load_ps(v1);
	__m128 b = _mm_load_ps(v2);
	dp4_sse4(a, b, dp);
}

inline void dp4_sse4(__m128 v1, __m128 v2, float *dp) {
	const int mask = 0xF1;
	__m128 tmp = _mm_dp_ps(v1, v2, mask);
	_mm_store_ss(dp, tmp);	
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


/**
 * Naive implementation of dot product for vectors of
 * length divisible by 4
 */

void dp_naive(float *a, float *b, float *dp, int len) {
	for(int i = 0; i < len; i+=4) {
		dp4f_naive(&a[i], &b[i], dp);	
	}
}


/**
 * SSE implementation of dot product for vectors of
 * length divisible by 4
 */

void dp_sse(float *a, float *b, float *dp, int len) {
	for(int i = 0; i < len; i+=4) {
		dp4f_sse(&a[i], &b[i], dp);	
	}
}

/**
 * SSE4 implementation of dot product for vectors of
 * length divisible by 4
 */
#ifdef USE_SSE4
void dp_sse4(float *a, float *b, float *dp, int len) {
	for(int i = 0; i < len; i+=4) {
		dp4f_sse4(&a[i], &b[i], dp);	
	}
}
#endif

/**
 * ASM-SSE4 implementation of dot product for vectors of
 * length divisible by 4
 */

void dp_asm(float *a, float *b, float *dp, int len) {
	for(int i = 0; i < len; i+=4) {
		dp4f_asm(&a[i], &b[i], dp);	
	}
}


int main() {

#ifdef USE_SSE4
	printf("SSE4 enabled\n");
#endif

	unsigned long long flop, start, stop, cycles;
	flop = 2*VL;
	
//	float test1[8] __attribute__((aligned(16))) = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
//	float test2[8] __attribute__((aligned(16))) = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float dpd __attribute__((aligned(16))) = 0.0f;
	float *a = _mm_malloc(VL*sizeof(float), 16);
	float *b = _mm_malloc(VL*sizeof(float), 16);

	// get random vectors of length VL
	rand_vec(a, VL);
	rand_vec(b, VL);

	// print vectors
	printf("Vektor A: ");
	print_vec(a, VL);
	printf("\n");

	printf("Vektor B: ");
	print_vec(b, VL);
	printf("\n");

	/* naive first */
	dpd = 0.0f;
	printf("\n=== Running naive dot product ===\n");
	start = rdtsctime();
	dp_naive(a, b, &dpd, VL);
	stop = rdtsctime();
	cycles = stop - start;	
	printf("cycles: %llu flops per cyc: %f \n", cycles, (double)flop/cycles);
	printf("dot product is %f\n", dpd);


	/* then SSE intrinsics */
	dpd = 0.0f;
	printf("\n=== Running SSE dot product ===\n");
	start = rdtsctime();
//	dp_sse(a, b, &dpd, VL);
	stop = rdtsctime();
	cycles = stop - start;	
	printf("cycles: %llu flops per cyc: %f \n", cycles, (double)flop/cycles);
	printf("dot product is %f\n", dpd);


	/* then SSE asm */
	dpd = 0.0f;
	printf("\n=== Running ASM-SSE dot product ===\n");
	start = rdtsctime();
	dp_asm(a, b, &dpd, VL);
	stop = rdtsctime();
	cycles = stop - start;	
	printf("cycles: %llu flops per cyc: %f \n", cycles, (double)flop/cycles);
	printf("dot product is %f\n", dpd);

	return 0;
}

