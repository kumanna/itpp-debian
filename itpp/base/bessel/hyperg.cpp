/*!
 * \file
 * \brief Implementation of confluent hypergeometric function
 * \author Tony Ottosson
 *
 * -------------------------------------------------------------------------
 *
 * IT++ - C++ library of mathematical, signal processing, speech processing,
 *        and communications classes and functions
 *
 * Copyright (C) 1995-2009  (see AUTHORS file for a list of contributors)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * -------------------------------------------------------------------------
 *
 * This is slightly modified routine from the Cephes library:
 * http://www.netlib.org/cephes/
 */

#include <itpp/base/bessel/bessel_internal.h>
#include <itpp/base/itassert.h>


/*
 * Confluent hypergeometric function
 *
 * double a, b, x, y, hyperg();
 *
 * y = hyperg( a, b, x );
 *
 * DESCRIPTION:
 *
 * Computes the confluent hypergeometric function
 *
 *                          1           2
 *                       a x    a(a+1) x
 *   F ( a,b;x )  =  1 + ---- + --------- + ...
 *  1 1                  b 1!   b(b+1) 2!
 *
 * Many higher transcendental functions are special cases of
 * this power series.
 *
 * As is evident from the formula, b must not be a negative
 * integer or zero unless a is an integer with 0 >= a > b.
 *
 * The routine attempts both a direct summation of the series
 * and an asymptotic expansion.  In each case error due to
 * roundoff, cancellation, and nonconvergence is estimated.
 * The result with smaller estimated error is returned.
 *
 * ACCURACY:
 *
 * Tested at random points (a, b, x), all three variables
 * ranging from 0 to 30.
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       0,30         2000       1.2e-15     1.3e-16
 qtst1:
 21800   max =  1.4200E-14   rms =  1.0841E-15  ave = -5.3640E-17
 ltstd:
 25500   max = 1.2759e-14   rms = 3.7155e-16  ave = 1.5384e-18
 *    IEEE      0,30        30000       1.8e-14     1.1e-15
 *
 * Larger errors can be observed when b is near a negative
 * integer or zero.  Certain combinations of arguments yield
 * serious cancellation error in the power series summation
 * and also are not in the region of near convergence of the
 * asymptotic series.  An error message is printed if the
 * self-estimated relative error is greater than 1.0e-12.
 */

/*
  Cephes Math Library Release 2.8:  June, 2000
  Copyright 1984, 1987, 1988, 2000 by Stephen L. Moshier
*/


double hyp2f0(double, double, double, int, double *);
static double hy1f1p(double, double, double, double *);
static double hy1f1a(double, double, double, double *);

#define MAXNUM 1.79769313486231570815E308    /* 2**1024*(1-MACHEP) */
#define MACHEP 1.11022302462515654042E-16   /* 2**-53 */

double hyperg(double a, double b, double x)
{
  double asum, psum, acanc, pcanc = 0, temp;

  /* See if a Kummer transformation will help */
  temp = b - a;
  if (fabs(temp) < 0.001 * fabs(a))
    return(exp(x) * hyperg(temp, b, -x));


  psum = hy1f1p(a, b, x, &pcanc);
  if (pcanc < 1.0e-15)
    goto done;


  /* try asymptotic series */

  asum = hy1f1a(a, b, x, &acanc);


  /* Pick the result with less estimated error */

  if (acanc < pcanc) {
    pcanc = acanc;
    psum = asum;
  }

done:
  if (pcanc > 1.0e-12) {
    it_warning("hyperg(): partial loss of precision");
  }

  return(psum);
}


/* Power series summation for confluent hypergeometric function */
static double hy1f1p(double a, double b, double x, double *err)
{
  double n, a0, sum, t, u, temp;
  double an, bn, maxt, pcanc;


  /* set up for power series summation */
  an = a;
  bn = b;
  a0 = 1.0;
  sum = 1.0;
  n = 1.0;
  t = 1.0;
  maxt = 0.0;


  while (t > MACHEP) {
    if (bn == 0) {  /* check bn first since if both */
      it_warning("hy1f1p(): function singularity");
      return(MAXNUM);   /* an and bn are zero it is */
    }
    if (an == 0)    /* a singularity  */
      return(sum);
    if (n > 200)
      goto pdone;
    u = x * (an / (bn * n));

    /* check for blowup */
    temp = fabs(u);
    if ((temp > 1.0) && (maxt > (MAXNUM / temp))) {
      pcanc = 1.0; /* estimate 100% error */
      goto blowup;
    }

    a0 *= u;
    sum += a0;
    t = fabs(a0);
    if (t > maxt)
      maxt = t;
    /*
    if( (maxt/fabs(sum)) > 1.0e17 )
    {
    pcanc = 1.0;
    goto blowup;
    }
    */
    an += 1.0;
    bn += 1.0;
    n += 1.0;
  }

pdone:

  /* estimate error due to roundoff and cancellation */
  if (sum != 0.0)
    maxt /= fabs(sum);
  maxt *= MACHEP;  /* this way avoids multiply overflow */
  pcanc = fabs(MACHEP * n  +  maxt);

blowup:

  *err = pcanc;

  return(sum);
}


/*       hy1f1a() */
/* asymptotic formula for hypergeometric function:
 *
 *        (    -a
 *  --    ( |z|
 * |  (b) ( -------- 2f0( a, 1+a-b, -1/x )
 *        (  --
 *        ( |  (b-a)
 *
 *
 *                                x    a-b                     )
 *                               e  |x|                        )
 *                             + -------- 2f0( b-a, 1-a, 1/x ) )
 *                                --                           )
 *                               |  (a)                        )
 */

static double hy1f1a(double a, double b, double x, double *err)
{
  double h1, h2, t, u, temp, acanc, asum, err1, err2;

  if (x == 0) {
    acanc = 1.0;
    asum = MAXNUM;
    goto adone;
  }
  temp = log(fabs(x));
  t = x + temp * (a - b);
  u = -temp * a;

  if (b > 0) {
    temp = lgam(b);
    t += temp;
    u += temp;
  }

  h1 = hyp2f0(a, a - b + 1, -1.0 / x, 1, &err1);

  temp = exp(u) / gam(b - a);
  h1 *= temp;
  err1 *= temp;

  h2 = hyp2f0(b - a, 1.0 - a, 1.0 / x, 2, &err2);

  if (a < 0)
    temp = exp(t) / gam(a);
  else
    temp = exp(t - lgam(a));

  h2 *= temp;
  err2 *= temp;

  if (x < 0.0)
    asum = h1;
  else
    asum = h2;

  acanc = fabs(err1) + fabs(err2);


  if (b < 0) {
    temp = gam(b);
    asum *= temp;
    acanc *= fabs(temp);
  }


  if (asum != 0.0)
    acanc /= fabs(asum);

  acanc *= 30.0; /* fudge factor, since error of asymptotic formula
    * often seems this much larger than advertised */

adone:


  *err = acanc;
  return(asum);
}


double hyp2f0(double a, double b, double x, int type, double *err)
{
  double a0, alast, t, tlast, maxt;
  double n, an, bn, u, sum, temp;

  an = a;
  bn = b;
  a0 = 1.0e0;
  alast = 1.0e0;
  sum = 0.0;
  n = 1.0e0;
  t = 1.0e0;
  tlast = 1.0e9;
  maxt = 0.0;

  do {
    if (an == 0)
      goto pdone;
    if (bn == 0)
      goto pdone;

    u = an * (bn * x / n);

    /* check for blowup */
    temp = fabs(u);
    if ((temp > 1.0) && (maxt > (MAXNUM / temp)))
      goto error;

    a0 *= u;
    t = fabs(a0);

    /* terminating condition for asymptotic series */
    if (t > tlast)
      goto ndone;

    tlast = t;
    sum += alast; /* the sum is one term behind */
    alast = a0;

    if (n > 200)
      goto ndone;

    an += 1.0e0;
    bn += 1.0e0;
    n += 1.0e0;
    if (t > maxt)
      maxt = t;
  }
  while (t > MACHEP);


pdone: /* series converged! */

  /* estimate error due to roundoff and cancellation */
  *err = fabs(MACHEP * (n + maxt));

  alast = a0;
  goto done;

ndone: /* series did not converge */

  /* The following "Converging factors" are supposed to improve accuracy,
   * but do not actually seem to accomplish very much. */

  n -= 1.0;
  x = 1.0 / x;

  switch (type) { /* "type" given as subroutine argument */
  case 1:
    alast *= (0.5 + (0.125 + 0.25 * b - 0.5 * a + 0.25 * x - 0.25 * n) / x);
    break;

  case 2:
    alast *= 2.0 / 3.0 - b + 2.0 * a + x - n;
    break;

  default:
    ;
  }

  /* estimate error due to roundoff, cancellation, and nonconvergence */
  *err = MACHEP * (n + maxt)  +  fabs(a0);


done:
  sum += alast;
  return(sum);

  /* series blew up: */
error:
  *err = MAXNUM;
  it_warning("hy1f1a(): total loss of precision");
  return(sum);
}
