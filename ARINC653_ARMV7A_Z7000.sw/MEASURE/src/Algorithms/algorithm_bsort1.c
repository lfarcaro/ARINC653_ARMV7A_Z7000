// General includes
#include "algorithm_bsort1.h"

// Algorithm initialization method
void ALGORITHM_BSORT1_INITIALIZE(void) {
	unsigned int i;
	for (i = 0; i < ALGORITHM_BSORT1_SIZE; i++) {
		algorithm_bsort1_sa[i] = ALGORITHM_BSORT1_SIZE - i;
	}
}

// Algorithm execution method
void ALGORITHM_BSORT1_EXECUTE(void) {
	for (algorithm_bsort1_el = 0; algorithm_bsort1_el < ALGORITHM_BSORT1_SIZE; algorithm_bsort1_el++) {
		algorithm_bsort1_b = 0;
		for (algorithm_bsort1_il = 0; algorithm_bsort1_il < (ALGORITHM_BSORT1_SIZE-1); algorithm_bsort1_il++) {
			if (algorithm_bsort1_sa[algorithm_bsort1_il] > algorithm_bsort1_sa[algorithm_bsort1_il + 1]) {
				algorithm_bsort1_t = algorithm_bsort1_sa[algorithm_bsort1_il];
				algorithm_bsort1_sa[algorithm_bsort1_il] = algorithm_bsort1_sa[algorithm_bsort1_il + 1];
				algorithm_bsort1_sa[algorithm_bsort1_il + 1] = algorithm_bsort1_t;
				algorithm_bsort1_b = 1;
			}
		}
		if (algorithm_bsort1_b == 0) {
			break;
		}
	}
}
