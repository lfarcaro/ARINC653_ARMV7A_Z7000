#ifndef ALGORITHM_LMS2_H_
#define ALGORITHM_LMS2_H_

// Algorithm definitions
#define ALGORITHM_LMS2_RAND_MAX 32768
#define ALGORITHM_LMS2_PI 3.14159265358979323846
#define ALGORITHM_LMS2_N 201
#define ALGORITHM_LMS2_L 20 /* filter order, (length L+1) */

// Algorithm variables
extern float algorithm_lms2_mu;

// Algorithm initialization method
void ALGORITHM_LMS2_INITIALIZE(void);

// Algorithm execution method
void ALGORITHM_LMS2_EXECUTE(void);

#endif
