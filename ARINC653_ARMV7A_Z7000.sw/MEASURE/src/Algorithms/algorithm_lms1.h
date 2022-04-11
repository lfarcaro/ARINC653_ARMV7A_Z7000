#ifndef ALGORITHM_LMS1_H_
#define ALGORITHM_LMS1_H_

// Algorithm definitions
#define ALGORITHM_LMS1_RAND_MAX 32768
#define ALGORITHM_LMS1_PI 3.14159265358979323846
#define ALGORITHM_LMS1_N 201
#define ALGORITHM_LMS1_L 20 /* filter order, (length L+1) */

// Algorithm variables
extern float algorithm_lms1_mu;

// Algorithm initialization method
void ALGORITHM_LMS1_INITIALIZE(void);

// Algorithm execution method
void ALGORITHM_LMS1_EXECUTE(void);

#endif
