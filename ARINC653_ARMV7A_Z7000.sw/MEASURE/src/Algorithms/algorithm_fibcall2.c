// General includes
#include "algorithm_fibcall2.h"

// Algorithm initialization method
void ALGORITHM_FIBCALL2_INITIALIZE(void) {
}

// Algorithm execution method
void ALGORITHM_FIBCALL2_EXECUTE(void) {
    algorithm_fibcall2_Fnew = 1;
    algorithm_fibcall2_Fold = 0;
    for (algorithm_fibcall2_i = 2; algorithm_fibcall2_i <= FIBCALL1_N; algorithm_fibcall2_i++) {
        algorithm_fibcall2_temp = algorithm_fibcall2_Fnew;
        algorithm_fibcall2_Fnew = algorithm_fibcall2_Fnew + algorithm_fibcall2_Fold;
        algorithm_fibcall2_Fold = algorithm_fibcall2_temp;
    }
    algorithm_fibcall2_ans = algorithm_fibcall2_Fnew;
}
