#include <tommath.h>
#ifdef BN_MP_CLEAR_C
/* LibTomMath, multiple-precision integer library -- Tom St Denis
 *
 * LibTomMath is a library that provides multiple-precision
 * integer arithmetic as well as number theoretic functionality.
 *
 * The library was designed directly after the MPI library by
 * Michael Fromberger but has been written from scratch with
 * additional optimizations in place.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://math.libtomcrypt.com
 */

/* clear one (frees)  */
void
mp_clear (mp_int * a)
{
  volatile mp_digit *p;
  int len;

  /* only do anything if a hasn't been freed previously */
  if (a->dp != NULL) {
    /* first zero the digits */
	len = a->alloc;
	p = a->dp;
	while (len--) {
		*p++ = 0;
	}

    /* free ram */
    XFREE(a->dp);

    /* reset members to make debugging easier */
    a->dp    = NULL;
    a->alloc = a->used = 0;
    a->sign  = MP_ZPOS;
  }
}
#endif

/* $Source: /cvs/libtom/libtommath/bn_mp_clear.c,v $ */
/* $Revision: #1 $ */
/* $Date: 2016/10/14 $ */
