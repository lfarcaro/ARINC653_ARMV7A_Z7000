// General includes
#include "algorithm_fibcall1.h"

// Algorithm initialization method
void ALGORITHM_FIBCALL1_INITIALIZE(void) {
}

// Algorithm execution method
void ALGORITHM_FIBCALL1_EXECUTE(void) {
    algorithm_fibcall1_Fnew = 1;
    algorithm_fibcall1_Fold = 0;
    for (algorithm_fibcall1_i = 2; algorithm_fibcall1_i <= FIBCALL1_N; algorithm_fibcall1_i++) {
        algorithm_fibcall1_temp = algorithm_fibcall1_Fnew;
        algorithm_fibcall1_Fnew = algorithm_fibcall1_Fnew + algorithm_fibcall1_Fold;
        algorithm_fibcall1_Fold = algorithm_fibcall1_temp;
    }
    algorithm_fibcall1_ans = algorithm_fibcall1_Fnew;
}
