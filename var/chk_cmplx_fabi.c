#if (defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER))
#include <mathimf.h>
#else /* !Intel */
#ifdef __cplusplus
#include <complex>
#include <cmath>
#else /* !__cplusplus */
#include <complex.h>
#include <math.h>
#endif /* ?__cplusplus */
#endif /* ?Intel */

#if (defined(__GNUC__) && !defined(__clang__) && !defined(__NVCOMPILER))
float complex
#ifdef _WIN32
CHK_CMPLX
#else /* !_WIN32 */
chk_cmplx_
#endif /* ?_WIN32 */
(const float complex *const a, const float complex *const b)
{
  return *a + *b;
}
#else /* !gfortran */
void
#ifdef _WIN32
CHK_CMPLX
#else /* !_WIN32 */
chk_cmplx_
#endif /* ?_WIN32 */
(float complex *const c, const float complex *const a, const float complex *const b)
{
  *c = *a + *b;
}
#endif /* ?gfortran */
