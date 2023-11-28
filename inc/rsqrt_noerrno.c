/* Correctly-rounded reciprocal square root of binary64 value.

Copyright (c) 2022 Alexei Sibidanov.

This file is part of the CORE-MATH project
(https://core-math.gitlabpages.inria.fr/).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* modified by venovako */

#include <fenv.h>
#ifdef __x86_64__
#include <x86intrin.h>
#endif

// Warning: clang also defines __GNUC__
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#pragma STDC FENV_ACCESS ON

static inline int get_rounding_mode (void)
{
#ifdef __x86_64__
  const unsigned flagp = _mm_getcsr ();
  return (flagp&(3<<13))>>3;
#else
  return fegetround ();
#endif
}

typedef unsigned __int128 u128;
typedef unsigned long u64;
typedef long i64;
typedef union {double f; unsigned long u;} b64u64_u;

static double __attribute__((noinline)) as_rsqrt_refine(double rf, double a){
  b64u64_u ir = {.f = rf}, ia = {.f = a};
  if(ia.u < 1l<<52){
    i64 nz = __builtin_clzll(ia.u);
    ia.u <<= nz - 11;
    ia.u &= ~0ul>>12;
    i64 e = nz - 12;
    ia.u |= e<<52;
  }
  if(ia.u<<11 == 1ul<<63){
  } else {
    unsigned mode = get_rounding_mode ();
    int e = (ia.u>>52)&1;
    u64 rm, am;
    rm = (ir.u<<11|1l<<63)>>11;
    am = ((ia.u&(~0ul>>12))|1l<<52)<<(5-e);
    u128 rt = (u128)rm*am;
    u64 rth = rt>>64, rtl = rt;
    u128 rrt = (u128)rtl*rm;
    u64 t0 = rrt, t1 = (rrt>>64) + rth*rm;
    rrt = (u128)t1<<64|t0;
    i64 s = rrt>>127, dd = 1 - 2*s;
    u128 rts = ((rt<<1)^(-s)) + s;
    u128 prrt;
    u64 am2 = am<<1, am20 = -am;
    do {
      ir.u -= dd;
      prrt = rrt;
      am20 += am2;
      u128 tt = rts - am20;
      rrt -= tt;
    } while(__builtin_expect(!((prrt^rrt)>>127), 0));
    ir.u += (rrt>>127)?0:dd;
    rrt = (rrt>>127)?rrt:prrt;
    if(__builtin_expect(mode==FE_TONEAREST, 1)){
      rm = (ir.u<<11|1l<<63)>>11;
      rt = (u128)rm*am;
      rrt += am>>2;
      rrt += rt;
      u64 inc = rrt>>127;
      ir.u += inc;
    } else {
      ir.u += mode==FE_UPWARD;
    }
    rf = ir.f;
  }
  return rf;
}

double cr_rsqrt(double x){
  b64u64_u ix = {.f = x};
  double r;
  if(__builtin_expect(ix.u < 1l<<52, 0)){
    if(__builtin_expect(ix.u, 1)){
      r = __builtin_sqrt(x)/x;
    } else {
      return __builtin_inf();
    }
  } else if(__builtin_expect(ix.u >= 0x7fful<<52, 0)){
    if(!(ix.u<<1)) return -__builtin_inf(); // x=-0
    if(ix.u > 0xfff0000000000000ul) return x;
    if(ix.u >> 63){
      feraiseexcept (FE_INVALID);
      return __builtin_nan("<0");
    }
    if(!(ix.u<<12)) return 0.0;
    return x;
  } else {
    r = (1/x)*__builtin_sqrt(x);
  }
  double rx = r*x, drx = __builtin_fma(r, x, -rx);
  double h = __builtin_fma(r,rx,-1.0) + r*drx, dr = (r*0.5)*h;
  double rf = r - dr;
  dr -= r - rf;
  b64u64_u idr = {.f = dr}, ir = {.f = rf};
  u64 aidr = (idr.u&(~0ul>>1)) - (ir.u & (0x7ffl<<52)) + (0x3fel<<52), mid = (aidr - 0x3c90000000000000 + 16)>>5;
  if(__builtin_expect( mid==0 || aidr<0x39b0000000000000l || aidr>0x3c9fffffffffff80l, 0))
    rf = as_rsqrt_refine(rf, x);
  return rf;
}

/* rsqrt function is not in glibc so define it here just to compile tests */
double rsqrt(double x){
  return cr_rsqrt(x);
}
