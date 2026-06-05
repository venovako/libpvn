#include "pvn.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
  if (1 <= argc) {
    (void)fprintf(stderr, "%s cannot be run on Windows\n", *argv);
    return EXIT_FAILURE;
  }
#else /* !_WIN32 */
  if (3 != argc) {
    (void)fprintf(stderr, "%s cnt bpp\n", *argv);
    return EXIT_FAILURE;
  }
#endif /* ?_WIN32 */
  const unsigned m = 200u;
  const unsigned n = 320u;
  const unsigned sx = 2u;
  const unsigned sy = 2u;
  int c = atoi(argv[1u]);
  if (c <= 0)
    return EXIT_FAILURE;
  const unsigned cnt = (unsigned)c;
  c = atoi(argv[2u]);
  if ((c != 0) && (c != 1) && (c != 4) && (c != 8))
    return EXIT_FAILURE;
  const unsigned bpp = (unsigned)(c ? c : 1);
  const size_t ldA = (size_t)m;
  double *const A = (double*)malloc(ldA * (n * sizeof(double)));
  if (!A)
    return EXIT_FAILURE;
  (void)system("rm -f *.bmp *.dat *.gif *.out *.png");
  (void)fprintf(stderr, "pvn_rvis_start... ");
  pvn_rvis_ctx ctx;
  const char *dat = (const char*)NULL;
  if (c == 0)
    dat = "test_vis_r0.dat";
  else if (c == 1)
    dat = "test_vis_r1.dat";
  else if (c == 4)
    dat = "test_vis_r4.dat";
  else if (c == 8)
    dat = "test_vis_r8.dat";
  else
    return EXIT_FAILURE;
  if (pvn_rvis_start(&ctx, m, n, pvn_rop_id, dat)) {
    (void)fprintf(stderr, "ERROR\n");
    return EXIT_FAILURE;
  }
  (void)fprintf(stderr, "OK\n");
  for (unsigned k = 0u; k < cnt; ++k) {
    for (unsigned j = 0u; j < n; ++j) {
      double *const cA = A + j * ldA;
      for (unsigned i = 0u; i < m; ++i)
        cA[i] = (i + j + k) % 254u;
    }
    (void)fprintf(stderr, "pvn_rvis_frame %u... ", k);
    if (pvn_rvis_frame(&ctx, A, ldA)) {
      (void)fprintf(stderr, "ERROR\n");
      return EXIT_FAILURE;
    }
    (void)fprintf(stderr, "OK\n");
  }
  free(A);
  (void)fprintf(stderr, "pvn_rvis_stop... ");
  int r = 0;
  if (c == 0)
    r = pvn_rvis_stop(&ctx, sx, sy, bpp, "test_vis_r0");
  else if (c == 1)
    r = pvn_rvis_stop(&ctx, sx, sy, bpp, "test_vis_r1");
  else if (c == 4)
    r = pvn_rvis_stop(&ctx, sx, sy, bpp, "test_vis_r4");
  else if (c == 8)
    r = pvn_rvis_stop(&ctx, sx, sy, bpp, "test_vis_r8");
  else
    r = 1;
  (void)fprintf(stderr, "%d\n", r);
  if (r)
    return EXIT_FAILURE;
  (void)fprintf(stderr, "Converting the BMPs to the PNGs with ImageMagick...\n");
  (void)system("for B in *.bmp; do magick $B -quality 90 `basename $B bmp`png; done");
  (void)fprintf(stderr, "Assembling the APNG animation from the PNGs with apngasm...\n");
  if (c == 0)
    (void)system("apngasm -F -o test_vis_r0.png -d 1:6 *-*.png");
  else if (c == 1)
    (void)system("apngasm -F -o test_vis_r1.png -d 1:6 *-*.png");
  else if (c == 4)
    (void)system("apngasm -F -o test_vis_r4.png -d 1:6 *-*.png");
  else if (c == 8)
    (void)system("apngasm -F -o test_vis_r8.png -d 1:6 *-*.png");
  else
    return EXIT_FAILURE;
  (void)fprintf(stderr, "Converting the APNG to the animated GIF with apng2gif...\n");
  if (c == 0)
    (void)system("apng2gif test_vis_r0.png test_vis_r0.gif");
  else if (c == 1)
    (void)system("apng2gif test_vis_r1.png test_vis_r1.gif");
  else if (c == 4)
    (void)system("apng2gif test_vis_r4.png test_vis_r4.gif");
  else if (c == 8)
    (void)system("apng2gif test_vis_r8.png test_vis_r8.gif");
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
