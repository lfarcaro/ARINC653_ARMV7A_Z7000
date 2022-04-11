// General includes
#include "algorithm_bsort2.h"

// Algorithm initialization method
void ALGORITHM_BSORT2_INITIALIZE(void) {
	unsigned int i;
	for (i = 0; i < ALGORITHM_BSORT2_SIZE; i++) {
		algorithm_bsort2_sa[i] = ALGORITHM_BSORT2_SIZE - i;
	}
}

// Algorithm execution method
void ALGORITHM_BSORT2_EXECUTE(void) {
	for (algorithm_bsort2_el = 0; algorithm_bsort2_el < ALGORITHM_BSORT2_SIZE; algorithm_bsort2_el++) {
		algorithm_bsort2_b = 0;
		for (algorithm_bsort2_il = 0; algorithm_bsort2_il < (ALGORITHM_BSORT2_SIZE-1); algorithm_bsort2_il++) {
			if (algorithm_bsort2_sa[algorithm_bsort2_il] > algorithm_bsort2_sa[algorithm_bsort2_il + 1]) {
				algorithm_bsort2_t = algorithm_bsort2_sa[algorithm_bsort2_il];
				algorithm_bsort2_sa[algorithm_bsort2_il] = algorithm_bsort2_sa[algorithm_bsort2_il + 1];
				algorithm_bsort2_sa[algorithm_bsort2_il + 1] = algorithm_bsort2_t;
				algorithm_bsort2_b = 1;
			}
		}
		if (algorithm_bsort2_b == 0) {
			break;
		}
	}
}
