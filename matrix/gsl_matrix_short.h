#ifndef GSL_MATRIX_SHORT_H
#define GSL_MATRIX_SHORT_H

#include <stdlib.h>
#include <gsl_errno.h>
#include <gsl_config.h>

typedef struct
{
  size_t size1;
  size_t size2;
  short * data;
} gsl_matrix_short ;

gsl_matrix_short * gsl_matrix_short_alloc (size_t n1, size_t n2);
gsl_matrix_short * gsl_matrix_short_calloc (size_t n1, size_t n2);
void gsl_matrix_short_free (gsl_matrix_short * m);

short * gsl_matrix_short_ptr(const gsl_matrix_short * m, size_t i, size_t j);
short   gsl_matrix_short_get(const gsl_matrix_short * m, size_t i, size_t j);
void  gsl_matrix_short_set(gsl_matrix_short * m, size_t i,  size_t j, short x);

int gsl_matrix_short_fread (FILE * stream, gsl_matrix_short * m) ;
int gsl_matrix_short_fwrite (FILE * stream, const gsl_matrix_short * m) ;
int gsl_matrix_short_fscanf (FILE * stream, gsl_matrix_short * m);
int gsl_matrix_short_fprintf (FILE * stream, const gsl_matrix_short * m, const char * format);

extern int gsl_check_range ;


#ifdef HAVE_INLINE

extern inline 
short
gsl_matrix_short_get(const gsl_matrix_short * m, const size_t i, const size_t j)
{
#ifndef GSL_RANGE_CHECK_OFF
  if (i >= m->size1)  /* size_t is unsigned, can't be negative */
    {
      GSL_ERROR_RETURN("first index out of range", GSL_EINVAL, 0) ;
    }
  else if (j >= m->size2) /* size_t is unsigned, can't be negative */
    {
      GSL_ERROR_RETURN("second index out of range", GSL_EINVAL, 0) ;
    }
#endif
  return m->data[i * m->size2 + j] ;
} 

extern inline 
void
gsl_matrix_short_set(gsl_matrix_short * m, 
		   const size_t i, const size_t j, 
		   const short x)
{
#ifndef GSL_RANGE_CHECK_OFF
  if (i >= m->size1) /* size_t is unsigned, can't be negative */
    {
      GSL_ERROR_RETURN_NOTHING("first index out of range", GSL_EINVAL) ;
    }
  else if (j >= m->size2) /* size_t is unsigned, can't be negative */
    {
      GSL_ERROR_RETURN_NOTHING("second index out of range", GSL_EINVAL) ;
    }
#endif
  m->data[i * m->size2 + j] = x ;
}
#endif /* HAVE_INLINE */

#endif /* !GSL_MATRIX_SHORT_H */
