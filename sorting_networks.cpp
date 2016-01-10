/*
My experiments with batcher's odd-even sort, creating various permutations of the algorithm with different properties.
Intended for usage in GPU parallel sorting, prototyped on CPU.

Compiled using Visual Studio 2012.
(Dependencies include _BitScanReverse/concurrency::parallel_for/<ppl.h>/__rdtsc/__debugbreak, but can be easily removed)
*/

#include <iostream>
#include <stdio.h>

#include <vector>
#include <ppl.h>


void exchange(int* a, int i, int j) {
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void compare(int* a, int i, int j) {
	//printf("compare %d %d\n", i, j);
    if (a[i] > a[j])
        exchange(a, i, j);
}


/**
 * lo is the starting position and
 * n is the length of the piece to be merged,
 * r is the distance of the elements to be compared
 */
void oddEvenMerge_r1(int* a, int lo, int n, int r) {
	
    int m = r * 2;
    if (m >= n) {
		compare(a, lo, lo + r);
    } 
	else
	{
		oddEvenMerge_r1(a, lo, n, m); // even subsequence
		oddEvenMerge_r1(a, lo + r, n, m); // odd subsequence

		for (int i = lo + r; i + r < lo + n; i += m)
			compare(a, i, i + r);
	}
}

// transformation 1 (non recursive)
void oddEvenMerge_r2(int* a, int s_lo, int n, int s_r) {

	int r = n/2;
	while(r >= 1)
	{
		for(int lo=s_lo; lo < s_lo+r; lo++)
		{		
			int m = r * 2;
			if (m >= n) {
				compare(a, lo, lo + r);
			} 
			else
			{
				for (int i = lo + r; i + r < lo + n; i += m)
					compare(a, i, i + r);
			}	
		}
		r >>= 1;
	}
}

// transformation 2
void oddEvenMerge_r3(int* a, int s_lo, int n, int s_r) {

	// do first depth iteration
	int r = n >> 1;
	for(int lo=s_lo; lo<s_lo + r; lo++)
		compare(a, lo, lo + r);
	
	// go to next iteration
	for(r >>= 1; r >= 1; r >>= 1)
	{
		int m = r << 1;
		for(int lo=s_lo; lo < s_lo+r; lo++)
		{
			for (int i = lo + r; i + r < lo + n; i += m)
				compare(a, i, i + r);
		}
	}
}

// permutation 0: original two step algorithm
void oddEvenMergeSort_Merge(int* a, int n) {

	for(int j=2; j<=n; j <<= 1)
	{
		for(int k=0; k<n; k += j)
		{
			//oddEvenMerge_r1(a, k, j, 1);
			//oddEvenMerge_r2(a, k, j, 1);
			oddEvenMerge_r3(a, k, j, 1);
		}
	}
}

// permutation 1: odd even merge in non colliding passes
// i.e. all comparisons in a pass can be run in parallel.
void oddEvenMergeSort_NCPasses_Branchless(int* a, int n) 
{
	for(int j=2; j<=n; j <<= 1)
	{
		// pass 0
		int r = j >> 1;
		for(int k=0; k<n; k += j)
		{
			for(int lo=k; lo< k+r; lo++)
				compare(a, lo, lo + r);
		}

		// do pass 1-log2(n)
		for(r >>= 1; r >= 1; r >>= 1)
		{
			for(int k=0; k<n; k += j)
			{
				int m = r << 1;
				for(int lo=k; lo < k+r; lo++)
					for (int i = lo + r; i + r < lo + j; i += m)
						compare(a, i, i + r);
			}
		}
	}
}

void oddEvenMergeSort_NCPasses_Branched(int* a, int n) 
{
	for(int j=2; j<=n; j <<= 1)
	{
		for(int r = j >> 1; r >= 1; r >>= 1)
		{
			for(int k=0; k<n; k += j)
			{
				for(int lo=k; lo < k+r; lo++)
				{
					if(r==(j>>1))
						compare(a, lo, lo + r);
					else
					{
						int m = r << 1;
						for (int i = lo + r; i + r < lo + j; i += m)
							compare(a, i, i + r);
					}
				}
			}
		}
	}
}

// alternative algorithm that calculates swap partner and is easily parallelized (but slower due to a lot of wasted iterations)
unsigned int oddEvenMergeSort_GetPartner(int n, int l, int p)
{
	if (p == 1) 
		return n ^ (1 << (l - 1));

	int scale = (1 << (l - p));
	int box = (1 << p);
	int nDivScale = (n/scale);
	int sn = nDivScale - ( nDivScale / box) * box;

	if (sn == 0 || sn == (box - 1) || (sn & 1) == 0) 
		return n;
	else
		return n + scale;
}

void oddEvenMergeSort_Partner(int* a, unsigned int _n)
{
	unsigned long d;
	_BitScanReverse(&d, _n);

	for (int l = 1; l <= d; l++) {
		for (int p = 1; p <= l; p++) {
			for (int n = 0; n < _n; n++) {
				int partner = oddEvenMergeSort_GetPartner(n, l, p);
				if(partner > n)
					compare(a,n, partner);
			}
		}
	}
}

void oddEvenMergeSort_Partner_MT(int* a, unsigned int _n)
{
	unsigned long d;
	_BitScanReverse(&d, _n);

	for (int l = 1; l <= d; l++) {
		for (int p = 1; p <= l; p++) {
			concurrency::parallel_for(0U, _n, 1U, [l,p,d,a](int n) {
				int partner = oddEvenMergeSort_GetPartner(n, l, p);
				if(partner > n)
					compare(a,n, partner);
			});
		}
	}
}

template <class T>
void bitonicSort(T* a, unsigned int n)
{
	int N = n;
	int vi, i, j, k;
	for (k = 2; k <= N; k = 2 * k) 
	{
		for (j = k >> 1; j > 0; j = j >> 1)
		{
			for (vi = 0; vi < N / 2; vi++)
			{
				i = vi + (vi / j)*j;
				int ixj = i + j;

				if ((i&k) == 0)
					compare(a, i, ixj);
				else
					compare(a, ixj, i);
			}
		}
	}
}


const int NUM_TESTS = 1000;

template<class T> bool runTest(int* a, int n, const T& testfunc)
{
	unsigned long long timing=0;
	for(int i=0; i<NUM_TESTS; i++)
	{
		int b[2048];
		memcpy(b, a, n*sizeof(int));

		unsigned long long clk = __rdtsc();
		testfunc(b,n);
		unsigned long long clk1 = __rdtsc();

		timing += (clk1-clk);

		int v=b[0];
		for (int i = 0; i < n; i++)
		{
			if(b[i] < v)
			{
				std::cout << "error detected\n";	
				return false;
			}
			else
				v = b[i];
		}
	}

	std::cout << "execution time: " << timing/NUM_TESTS << " cycles\n";
	return true;
}

int main() {

	int a[2048];
	int n=sizeof(a)/sizeof(a[0]);
	for(int i=0; i<n; i++)
		a[i] = rand()%15;

	// method 1
	runTest(a, n, [](int* a, int n) { std::sort(a, a+n); });
	runTest(a, n, [](int* a, int n) { oddEvenMergeSort_Merge(a, n); });
	runTest(a, n, [](int* a, int n) { oddEvenMergeSort_NCPasses_Branchless(a, n); });
	runTest(a, n, [](int* a, int n) { oddEvenMergeSort_NCPasses_Branched(a, n); });
	runTest(a, n, [](int* a, int n) { oddEvenMergeSort_Partner(a, n); });
	runTest(a, n, [](int* a, int n) { oddEvenMergeSort_Partner_MT(a, n); });
	runTest(a, n, [](int* a, int n) { bitonicSort(a, n); });

	return(0);
}
