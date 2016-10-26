#ifndef INCLUDE_ST_IMAGE_UTILS_H_
#define INCLUDE_ST_IMAGE_UTILS_H_

#ifdef _MSC_VER
#ifdef __cplusplus
#ifdef ST_IMAGE_UTILS_EXPORTS
#define ST_IMAGE_UTILS_API  extern "C" __declspec(dllexport)
#else
#define ST_IMAGE_UTILS_API extern "C" __declspec(dllimport)
#endif
#else
#ifdef ST_IMAGE_UTILS_EXPORTS
#define ST_IMAGE_UTILS_API __declspec(dllexport)
#else
#define ST_IMAGE_UTILS_API __declspec(dllimport)
#endif
#endif
#else /* _MSC_VER */
#ifdef __cplusplus
#ifdef ST_IMAGE_UTILS_EXPORTS
#define ST_IMAGE_UTILS_API  extern "C" __attribute__((visibility ("default")))
#else
#define ST_IMAGE_UTILS_API extern "C"
#endif
#else
#ifdef ST_IMAGE_UTILS_EXPORTS
#define ST_IMAGE_UTILS_API __attribute__((visibility ("default")))
#else
#define ST_IMAGE_UTILS_API
#endif
#endif
#endif

typedef enum {
	ST_FMT_UNKNOWN = -1,
	ST_FMT_JPEG = 0,
	ST_FMT_PNG,
	ST_FMT_BMP,
	ST_FMT_TIFF,
} st_image_format_t;

/// compatiable with cv_pixel_format
typedef enum {
	// 1 tire, all support, load/save
	ST_PIX_FMT_GRAY8 = 0,
	ST_PIX_FMT_BGRA8 = 4,
	ST_PIX_FMT_BGR8 = 5,

	// 2 tire, transform/conversion
	ST_PIX_FMT_GRAY32F = 20,
	ST_PIX_FMT_ARGB8 = 106,
	ST_PIX_FMT_RGB8 = 107,
} st_pixel_format_t;

typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
} st_color_t;

typedef struct st_image_t st_image_t;

#define ST_IMAGE_OK 0
#define ST_IMAGE_ERR_INVALID_IMAGE -1
#define ST_IMAGE_ERR_INVALID_ARG -2
#define ST_IMAGE_ERR_UNKNOWN_IMAGE_FORMAT -3
#define ST_IMAGE_ERR_UNSUPPORTED_PIXEL_FORMAT -4
#define ST_IMAGE_ERR_OUTOFMEMORY -5

// IO
// format can be GRAY8, BGR8, BGRA8
ST_IMAGE_UTILS_API int st_image_load_from_file(
		const char *path,
		st_pixel_format_t format,
		int max_size_hint,
		int flags,
		st_image_t **out);
ST_IMAGE_UTILS_API int st_image_load_from_blob(
		const unsigned char *data,
		unsigned long size,
		st_pixel_format_t format,
		int max_size_hint,
		int flags,
		st_image_t **out);


ST_IMAGE_UTILS_API int st_image_ping_from_file(
		const char *path,
		int flags,
		st_image_t **out);
ST_IMAGE_UTILS_API int st_image_ping_from_blob(
		const unsigned char *data,
		unsigned long size,
		int flags,
		st_image_t **out);
ST_IMAGE_UTILS_API int st_image_from_bytes(unsigned char *data,
		int w, int h, int stride, st_pixel_format_t pixfmt,
		st_image_t **out);


ST_IMAGE_UTILS_API int st_image_save_to_file(const st_image_t *src, const char *path, int flags);
ST_IMAGE_UTILS_API int st_image_save_to_blob(const st_image_t *src, unsigned char **out, unsigned long *size,
		st_image_format_t fmt, int flags);

ST_IMAGE_UTILS_API void st_image_free_blob(unsigned char *blob);

ST_IMAGE_UTILS_API int st_image_get_width(const st_image_t *img);
ST_IMAGE_UTILS_API int st_image_get_height(const st_image_t *img);

/// Get bits per pixel.
ST_IMAGE_UTILS_API int st_image_get_bpp(const st_image_t *img);
ST_IMAGE_UTILS_API int st_image_get_stride(const st_image_t *img);
ST_IMAGE_UTILS_API st_pixel_format_t st_image_get_pixel_format(const st_image_t *img);
ST_IMAGE_UTILS_API st_image_format_t st_image_get_image_format(const st_image_t *img);
ST_IMAGE_UTILS_API unsigned char *st_image_get_data(st_image_t *img);


/// Get pixels
ST_IMAGE_UTILS_API int st_image_get_pixel(const st_image_t *img, int x, int y, st_color_t *color);

// transform
ST_IMAGE_UTILS_API int st_image_convert(const st_image_t *src,
		st_image_t **dst,
		st_pixel_format_t dst_format);
ST_IMAGE_UTILS_API int st_image_clone(const st_image_t *src,
		st_image_t **dst);

typedef enum {
	ST_CW_0,
	ST_CW_90,
	ST_CW_180,
	ST_CW_270,

	ST_FLIP_UPDOWN,
	ST_FLIP_LEFTRIGHT,

	ST_TRANSPOSE,
} st_tran_type_t;

ST_IMAGE_UTILS_API int st_image_transform(const st_image_t *src,
		st_image_t **dst, st_tran_type_t type);

typedef enum {
	ST_SAMPLE_NEAREST,
	ST_SAMPLE_BILINEAR,
} st_sample_type_t;

ST_IMAGE_UTILS_API int st_image_resize(const st_image_t *src,
		st_image_t **dst, int width, int height, st_sample_type_t type);
ST_IMAGE_UTILS_API int st_image_crop(const st_image_t *src,
		st_image_t **dst, int x, int y, int width, int height);

// rasterization

ST_IMAGE_UTILS_API int st_image_draw_point(st_image_t *dst, int x, int y,
		int size, const st_color_t *color);
ST_IMAGE_UTILS_API int st_image_draw_rect(st_image_t *dst, int x, int y,
		int w, int h, int stroke, const st_color_t *color);
ST_IMAGE_UTILS_API int st_image_fill_rect(st_image_t *dst, int x, int y,
		int w, int h, const st_color_t *color);

/// Free image data (if available) to free memory.
/// NOTE: st_image_free must be called later to free meta data
ST_IMAGE_UTILS_API void st_image_destroy_data(st_image_t *img);

ST_IMAGE_UTILS_API void st_image_free(st_image_t *img);

#endif  // INCLUDE_ST_IMAGE_UTILS_H_

