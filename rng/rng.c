#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <gsl_errno.h>
#include <gsl_rng.h>

gsl_rng *
gsl_rng_alloc (const gsl_rng_type * T)
{

  gsl_rng *r = (gsl_rng *) malloc (sizeof (gsl_rng));

  if (r == 0)
    {
      GSL_ERROR_RETURN ("failed to allocate space for rng struct",
			GSL_ENOMEM, 0);
    };

  r->state = malloc (T->size);

  if (r->state == 0)
    {
      free (r);		/* exception in constructor, avoid memory leak */

      GSL_ERROR_RETURN ("failed to allocate space for rng state",
			GSL_ENOMEM, 0);
    };

  r->name = T->name;
  r->max = T->max;
  r->size = T->size;
  r->set = T->set;
  r->get = T->get;

  gsl_rng_set (r, gsl_rng_default_seed);	/* seed the generator */

  return r;
}


gsl_rng *
gsl_rng_cpy (gsl_rng * dest, const gsl_rng * src)
{
  if (dest->size != src->size)
    {
      dest->state = realloc (dest->state, src->size);

      if (dest->state == 0)
	{
	  GSL_ERROR_RETURN ("failed to reallocate space for rng state",
			    GSL_ENOMEM, 0);
	}
    }

  dest->name = src->name;
  dest->max = src->max;
  dest->size = src->size;
  dest->set = src->set;
  dest->get = src->get;

  memcpy (dest->state, src->state, src->size);

  return dest;
}


gsl_rng *
gsl_rng_clone (const gsl_rng * q)
{
  gsl_rng *r = (gsl_rng *) malloc (sizeof (gsl_rng));

  if (r == 0)
    {
      GSL_ERROR_RETURN ("failed to allocate space for rng struct",
			GSL_ENOMEM, 0);
    };

  r->state = malloc (q->size);

  if (r->state == 0)
    {
      free (r);		/* exception in constructor, avoid memory leak */

      GSL_ERROR_RETURN ("failed to allocate space for rng state",
			GSL_ENOMEM, 0);
    };

  r->name = q->name;
  r->max = q->max;
  r->size = q->size;
  r->set = q->set;
  r->get = q->get;

  memcpy (r->state, q->state, q->size);

  return r;
}

void
gsl_rng_set (const gsl_rng * r, unsigned long int seed)
{
  (r->set) (r->state, seed);
}

unsigned long int
gsl_rng_get (const gsl_rng * r)
{
  return (r->get) (r->state);
}

double
gsl_rng_uniform (const gsl_rng * r)
{
  unsigned long int k = (r->get) (r->state);
  unsigned long int max = r->max;

  return k / ((double) max);
}

double
gsl_rng_uniform_pos (const gsl_rng * r)
{
  unsigned long int max = r->max;
  unsigned long int k;

  do 
    {
      k = (r->get) (r->state);
    }
  while (k == 0) ;

  return k / ((double) max);
}

double
gsl_rng_uniform_gt0_lt1 (const gsl_rng * r)
{
  unsigned long int max = r->max;
  unsigned long int k;
  volatile double x; /* store x in memory, we need to test exact equality */

  do 
    {
      k = (r->get) (r->state);
      x = k / ((double) max)
    }
  while (x == 0 || x == 1) ;

  return x;
}

unsigned long int
gsl_rng_max (const gsl_rng * r)
{
  return r->max;
}

unsigned long int
gsl_rng_min (const gsl_rng * r)
{
  return r->min;
}

const char *
gsl_rng_name (const gsl_rng * r)
{
  return r->name;
}

void
gsl_rng_print_state (const gsl_rng * r)
{
  size_t i;
  unsigned char *p = (unsigned char *) (r->state);
  const size_t n = r->size;

  for (i = 0; i < n; i++)
    {
      printf ("%.2x", *(p + i));	/* FIXME: we assumed that a char is 8 bits */
    }

}

void
gsl_rng_free (gsl_rng * r)
{
  free (r->state);
  free (r);
}
