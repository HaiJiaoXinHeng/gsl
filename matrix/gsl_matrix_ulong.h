#ifndef GSL_matrix_ulong_H
#define GSL_matrix_ulong_H

#include <stdlib.h>
#include <gsl_errno.h>
#include <gsl_config.h>

typedef struct
{
  size_t size1;
  size_t size2;
  unsigned long * data;
  
} gsl_matrix_ulong ;

gsl_matrix_ulong * gsl_matrix_ulong_alloc (size_t n1, size_t n2);
gsl_matrix_ulong * gsl_matrix_ulong_calloc (size_t n1, size_t n2);
void gsl_matrix_ulong_free (gsl_matrix_ulong * m);

unsigned long * gsl_matrix_ulong_ptr(const gsl_matrix_ulong * m, size_t i, size_t j);
unsigned long  gsl_matrix_ulong_get(const gsl_matrix_ulong * m, size_t i, size_t j);
void  gsl_matrix_ulong_set(gsl_matrix_ulong * m, size_t i,  size_t j, unsigned long x);

int gsl_matrix_ulong_fread (FILE * stream, gsl_matrix_ulong * m) ;
int gsl_matrix_ulong_fwrite (FILE * stream, const gsl_matrix_ulong * m) ;
int gsl_matrix_ulong_fscanf (FILE * stream, gsl_matrix_ulong * m);
int gsl_matrix_ulong_fprintf (FILE * stream, const gsl_matrix_ulong * m, const char * format);

extern int gsl_check_range ;


#ifdef HAVE_INLINE

extern inline 
unsigned long
gsl_matrix_ulong_get(const gsl_matrix_ulong * m, const size_t i, const size_t j)
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
gsl_matrix_ulong_set(gsl_matrix_ulong * m, 
		     const size_t i, const size_t j, 
		     const unsigned long x)
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

#endif /* !GSL_MATRIX_ULONG_H */
