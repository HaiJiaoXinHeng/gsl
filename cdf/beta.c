/* cdf/cdf_beta.c
 * 
 * Copyright (C) 2003 Brian Gough.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 */

#include <config.h>
#include <math.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_math.h>

#include "beta_inc.c"

double
gsl_cdf_beta_P (const double x, const double a, const double b)
{
  double P;

  if (x <= 0.0 || x > 1.0)
    {
      return 0.0;
    }

  P = beta_inc_AXPY (1.0, 0.0, a, b, x);

  return P;
}

double
gsl_cdf_beta_Q (const double x, const double a, const double b)
{
  double P;

  if (x < 0.0 || x >= 1.0)
    {
      return 0.0;
    }

  P = beta_inc_AXPY (-1.0, 1.0, a, b, x);

  return P;
}
