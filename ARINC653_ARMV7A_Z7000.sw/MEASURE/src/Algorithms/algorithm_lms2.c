#include "algorithm_lms2.h"

float algorithm_lms2_mu = 0.01;

int algorithm_lms2_rand(){
  static unsigned long next = 1;

  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

static float algorithm_lms2_log(float r){
  return 4.5;
}

static float algorithm_lms2_fabs(float n)
{
  float f;

  if (n >= 0) f = n;
  else f = -n;
  return f;
}

static float algorithm_lms2_sqrt(float val){
  float x = val/10;

  float dx;

  double diff;
  double min_tol = 0.00001;

  int i, flag;

  flag = 0;
  if (val == 0 ) x = 0;
  else {
    for (i=1;i<20;i++)
      {
    if (!flag) {
      dx = (val - (x*x)) / (2.0 * x);
      x = x + dx;
      diff = val - (x*x);
      if (algorithm_lms2_fabs(diff) <= min_tol) flag = 1;
    }
    else { }         /* JG */
/*    x =x; */
      }
  }
  return (x);
}

static float algorithm_lms2_gaussian()
{
    static int ready = 0;       /* flag to indicated stored value */
    static float gstore;        /* place to store other value */
    static float rconst1 = (float)(2.0/ALGORITHM_LMS2_RAND_MAX);
    static float rconst2 = (float)(ALGORITHM_LMS2_RAND_MAX/2.0);
    float v1,v2,r,fac;
    float gaus;

/* make two numbers if none stored */
    if(ready == 0) {
            v1 = (float)algorithm_lms2_rand() - rconst2;
            v2 = (float)algorithm_lms2_rand() - rconst2;
            v1 *= rconst1;
            v2 *= rconst1;
            r = v1*v1 + v2*v2;
        while (r > 1.0f) {
            v1 = (float)algorithm_lms2_rand() - rconst2;
            v2 = (float)algorithm_lms2_rand() - rconst2;
            v1 *= rconst1;
            v2 *= rconst1;
            r = v1*v1 + v2*v2;
        }       /* make radius less than 1 */

/* remap v1 and v2 to two Gaussian numbers */
        fac = algorithm_lms2_sqrt(-2.0f*algorithm_lms2_log(r)/r);
        gstore = v1*fac;        /* store one */
        gaus = v2*fac;          /* return one */
        ready = 1;              /* set ready flag */
    }

    else {
        ready = 0;      /* reset ready flag for next pair */
        gaus = gstore;  /* return the stored one */
    }

    return(gaus);
}


static float algorithm_lms2_sin( float rad){
  float app;

  float diff;
  int inc = 1;

  while (rad > 2*ALGORITHM_LMS2_PI)
    rad -= 2*ALGORITHM_LMS2_PI;
  while (rad < -2*ALGORITHM_LMS2_PI)
    rad += 2*ALGORITHM_LMS2_PI;
  app = diff = rad;
   diff = (diff * (-(rad*rad))) /
      ((2.0 * inc) * (2.0 * inc + 1.0));
    app = app + diff;
    inc++;
  while(algorithm_lms2_fabs(diff) >= 0.00001) {
    diff = (diff * (-(rad*rad))) /
      ((2.0 * inc) * (2.0 * inc + 1.0));
    app = app + diff;
    inc++;
  }

  return(app);
}

/*
      function lms(x,d,b,l,mu,alpha)

Implements NLMS Algorithm b(k+1)=b(k)+2*mu*e*x(k)/((l+1)*sig)

x      = input data
d      = desired signal
b[0:l] = Adaptive coefficients of lth order fir filter
l      = order of filter (> 1)
mu     = Convergence parameter (0.0 to 1.0)
alpha  = Forgetting factor   sig(k)=alpha*(x(k)**2)+(1-alpha)*sig(k-1)
         (>= 0.0 and < 1.0)

returns the filter output
*/

float algorithm_lms2(float x,float d,float *b,int l,
                  float mu,float alpha)
{
    int ll;
    float e,mu_e,/*lms_const,*/y; /* JG */
    static float px[51];      /* max L = 50 */
    static float sigma = 2.0; /* start at 2 and update internally */

    px[0]=x;

/* calculate filter output */
    y=b[0]*px[0];
    for(ll = 1 ; ll <= l ; ll++)
        y=y+b[ll]*px[ll];

/* error signal */
    e=d-y;

/* update sigma */
    sigma=alpha*(px[0]*px[0])+(1-alpha)*sigma;
    mu_e=mu*e/sigma;

/* update coefficients */
    for(ll = 0 ; ll <= l ; ll++)
        b[ll]=b[ll]+mu_e*px[ll];
/* update history */
    for(ll = l ; ll >= 1 ; ll--)
        px[ll]=px[ll-1];

    return(y);
}

void ALGORITHM_LMS2_INITIALIZE(void) {
    algorithm_lms2_mu = 0.01;
}

void  ALGORITHM_LMS2_EXECUTE(void)
{
    static float d[ALGORITHM_LMS2_N],b[21];
    float signal_amp,noise_amp,arg,x/*,y*/; /* JG */
    int k;

/* create signal plus noise */
    signal_amp = algorithm_lms2_sqrt(2.0);
    noise_amp = 0.2*algorithm_lms2_sqrt(12.0);
    arg = 2.0*ALGORITHM_LMS2_PI/20.0;
    for(k = 0 ; k < ALGORITHM_LMS2_N ; k++) {
        d[k] = signal_amp*algorithm_lms2_sin(arg*k) + noise_amp*algorithm_lms2_gaussian();
    }

/* scale based on L */
    algorithm_lms2_mu = 2.0*algorithm_lms2_mu/(ALGORITHM_LMS2_L+1);

    x = 0.0;
    for(k = 0 ; k < ALGORITHM_LMS2_N ; k++) {
        algorithm_lms2(x,d[k],b,ALGORITHM_LMS2_L,algorithm_lms2_mu,0.01);
/* delay x one sample */
        x = d[k];
    }
    return ;
}
