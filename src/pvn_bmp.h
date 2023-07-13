#ifndef PVN_BMP_H
#define PVN_BMP_H

#ifndef PVN_H
#error pvn_bmp.h not intended for direct inclusion
#endif /* !PVN_H */

#ifndef PVN_RGB_24
#define PVN_RGB_24(r, g, b) (((((r) << 8u) | (g)) << 8u) | (b))
#else /* PVN_RGB_24 */
#error PVN_RGB_24 already defined
#endif /* ?PVN_RGB_24 */

typedef struct
{
  uint32_t f_size;
  uint16_t reserved[2]; /* { 0u, 0u } */
  uint32_t i_offset;
  uint32_t d_size; /* 40u = BMPINFOHEADER */
  uint32_t width;
  int32_t height;
  uint16_t planes; /* 1u */
  uint16_t bpp;
  uint32_t compression; /* 0u */
  uint32_t i_size;
  int32_t h_ppm; /* 0 */
  int32_t v_ppm; /* 0 */
  uint32_t c_palette; /* may be 0u if implied */
  uint32_t i_colors; /* 0u */
} pvn_bmp_header_t;

typedef union
{
  uint32_t c;
  struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
  } chan;
} pvn_bmp_palette_entry_t;

typedef struct
{
  pvn_bmp_palette_entry_t *palette;
  uint8_t *image;
  uint32_t i_ldaB;
  pvn_bmp_header_t header;
} *pvn_bmp_t;

typedef void (*pvn_bmp_pixel_setter_t)(const pvn_bmp_t bmp, const uint32_t x, const uint32_t y, const uint32_t c);
typedef uint32_t (*pvn_bmp_pixel_getter_t)(const pvn_bmp_t bmp, const uint32_t x, const uint32_t y);

PVN_EXTERN_C int pvn_bmp_create(pvn_bmp_t *const bmp, const uint32_t width, const int32_t height, const uint16_t bpp);
PVN_EXTERN_C void pvn_bmp_destroy(const pvn_bmp_t bmp);

PVN_EXTERN_C int pvn_bmp_set_palette_color(const pvn_bmp_t bmp, const uint32_t ix, const uint32_t c);
PVN_EXTERN_C int pvn_bmp_get_palette_color(const pvn_bmp_t bmp, const uint32_t ix, uint32_t *const c);

PVN_EXTERN_C pvn_bmp_pixel_setter_t pvn_bmp_get_pixel_setter(const pvn_bmp_t bmp);
PVN_EXTERN_C pvn_bmp_pixel_getter_t pvn_bmp_get_pixel_getter(const pvn_bmp_t bmp);

PVN_EXTERN_C int pvn_bmp_fwrite(const pvn_bmp_t bmp, const char *const fn);
PVN_EXTERN_C int pvn_bmp_read_cmap(const pvn_bmp_t bmp, const char *const fn);

#endif /* !PVN_BMP_H */
