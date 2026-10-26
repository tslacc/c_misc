#include "primesieve.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned int *memory;
static unsigned int memoryVals;
static int memoryValidatePrime(const int query){
	unsigned int idx = 0;
	unsigned int searchLimit = sqrt(query)+1;
	while(idx < memoryVals && memory[idx]<searchLimit){
		if(query%memory[idx] == 0 && memory[idx]!=1)
			return 0; //non-prime
		idx++;
	}
	return 1;
}

// Test every integer between the current memory and limit for primality.
// Skip numbers based on the 30k+i wheel factorization rule where i is a member of set {1,7,11,13,17,19,23,29}.
// source: https://en.wikipedia.org/wiki/Primality_test
static void expandMemoryTo(const unsigned int limit){
	const int iVals[] = {1,7,11,13,17,19,23,29};
	unsigned int test = 7;
	int k = 0;
	int i_idx = 0;
	const int I_IDXMAX = 7;
	if(memoryVals>0){
		test = memory[memoryVals-1];
		k = test%30;
		int i = test-30*k;
		for(int idx = 0; idx < I_IDXMAX; idx++){
			if(iVals[idx] == i){
				i_idx = idx;
				break;
			}
		}
	}
	else{
		memory[0] = 2;
		memory[1] = 3;
		memory[2] = 5;
		memoryVals = 3;
		k = 0;
		i_idx = 1;
	}
	while(test < limit){
		if(memoryValidatePrime(test)){
			memory[memoryVals] = test;
			memoryVals++;
		}
		if(i_idx < I_IDXMAX){
			i_idx++;
		} else {
			i_idx=0;
			k++;
		}
		test = 30*k+iVals[i_idx];
	}
	printf("memory now contains %u primes\n", memoryVals);
}
// Find the biggest member in memory that is less than query.
static int custom_bsearch(const unsigned int query){
	size_t l = 0;
	size_t r = memoryVals;
	size_t m = 0;
    while(l <= r){
        m = l + (int)((r - l) / 2.f);
        if(memory[m] < query)
            l = m + 1;
        else if(memory[m] > query)
            r = m - 1;
        else
            return m;
	}
	return m;
}
int countPrimesBelow(const unsigned int query){
	printf("counting primes below %u\n", query);
	if(memory == NULL){
		// TODO Replace magic constant
		memory = malloc(sizeof(int)*80000);
	}
	if(memoryVals>0 && memory[memoryVals-1] >= sqrt(query)+1){ //bsearch here
		return custom_bsearch(query);
	} else {
		expandMemoryTo(query);
		return custom_bsearch(query);
	}
}
