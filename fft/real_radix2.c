#include <math.h>
#include <gsl_errno.h>
#include <gsl_fft_real.h>

int
gsl_fft_real_radix2 (double data[],
		     const unsigned int n)
{
  int result ;
  unsigned int p, p_1, q;
  unsigned int i; 
  unsigned int logn = 0;
  int status;

  if (n == 1) /* identity operation */
    {
      return 0 ;
    }

  /* make sure that n is a power of 2 */

  result = gsl_fft_binary_logn(n) ;

  if (result == -1) {
    GSL_ERROR ("n is not a power of 2", GSL_EINVAL);
    return -1;
  } else {
    logn = result ;
  }

  /* bit reverse the ordering of input data for decimation in time algorithm */
  
  status = gsl_fft_real_bitreverse_order(data, n, logn) ;

  /* apply fft recursion */

  p = 1; q = n ;

  for (i = 1; i <= logn; i++)
    {
      unsigned int a, b;

      p_1 = p ;
      p = 2 * p ;
      q = q / 2 ;

#ifdef DEBUG
#define DISPLAY for(k=0; k<n ;k++) {printf("%d: %e\n",k,data[k]); } ;
      printf("at beginning of loop i=%d, p=%d, p_1=%d, q=%d\n",i,p,p_1,q) ;
      DISPLAY ;
#endif

      /* a = 0 */

      for (b = 0; b < q; b++)
	{
	  double t0_real = data[b*p] + data[b*p + p_1] ;
	  double t1_real = data[b*p] - data[b*p + p_1] ;
	  
	  data[b*p] = t0_real ;
	  data[b*p + p_1] = t1_real ;
	}

#ifdef DEBUG
      printf("after doing a=0\n",i) ;
      DISPLAY ;
#endif

      /* a = 1 ... p_{i-1}/2 - 1 */

      {
	double w_real = 1.0;
	double w_imag = 0.0;

	const double theta = - 2.0 * M_PI / p;
	
	const double s = sin (theta);
	const double t = sin (theta / 2.0);
	const double s2 = 2.0 * t * t;
	
	for (a = 1; a < (p_1)/2; a++)
	  {
	    /* trignometric recurrence for w-> exp(i theta) w */
	    
	    {
	      const double tmp_real = w_real - s * w_imag - s2 * w_real;
	      const double tmp_imag = w_imag + s * w_real - s2 * w_imag;
	      w_real = tmp_real;
	      w_imag = tmp_imag;
	    }
	    
	    for (b = 0; b < q; b++)
	      {
		double z0_real = data[b*p + a] ;
		double z0_imag = data[b*p + p_1 - a] ;
		double z1_real = data[b*p + p_1 + a] ;
		double z1_imag = data[b*p + p - a] ;
		
		/* t0 = z0 + w * z1 */
		
		double t0_real = z0_real + w_real * z1_real - w_imag * z1_imag;
		double t0_imag = z0_imag + w_real * z1_imag + w_imag * z1_real;
		
		/* t1 = z0 - w * z1 */
		
		double t1_real = z0_real - w_real * z1_real + w_imag * z1_imag;
		double t1_imag = z0_imag - w_real * z1_imag - w_imag * z1_real;
		
		data[b*p + a] = t0_real ;
		data[b*p + p - a] = t0_imag ;
		
		data[b*p + p_1 - a] = t1_real ;
		data[b*p + p_1 + a] = -t1_imag ;
	      }
	  }
      }

#ifdef DEBUG      
      if ((p_1)/2 > 1) {
	printf("after doing a=1 ... p_{i-1}/2-1 (q=%d)\n",q) ;
	DISPLAY ;
      }
#endif

      if (p_1 >  1) {
	
	for (b = 0; b < q; b++) {
	  /* a = p_{i-1}/2 */

	  data[b*p + p - p_1/2] *= -1 ;
#ifdef DEBUG
	  printf("after doing a=p_{i-1}/2 (q=%d)\n",q) ;
	  DISPLAY ;
#endif
	}
      }

    }
  return 0;
  
}
