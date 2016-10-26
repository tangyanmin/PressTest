
#ifndef INCLUDE_CVFACE_API_CV_FACE_INTERNAL_H_
#define INCLUDE_CVFACE_API_CV_FACE_INTERNAL_H_

#include "cv_common.h"
#include <model/ModelLoader.hpp>

#if CONFIG_ST_FACE_INTERNAL_API

#ifdef _MSC_VER
#ifdef __cplusplus
#ifdef SDK_EXPORTS
#define CV_SDK_INTERNAL  extern "C" __declspec(dllexport)
#else
#define CV_SDK_INTERNAL extern "C" __declspec(dllimport)
#endif
#else
#ifdef SDK_EXPORTS
#define CV_SDK_INTERNAL __declspec(dllexport)
#else
#define CV_SDK_INTERNAL __declspec(dllimport)
#endif
#endif
#else /* _MSC_VER */
#ifdef __cplusplus
#ifdef SDK_EXPORTS
#define CV_SDK_INTERNAL  extern "C" __attribute__((visibility ("default")))
#else
#define CV_SDK_INTERNAL extern "C"
#endif
#else
#ifdef SDK_EXPORTS
#define CV_SDK_INTERNAL __attribute__((visibility ("default")))
#else
#define CV_SDK_INTERNAL
#endif
#endif
#endif

#else
#define CV_SDK_INTERNAL
#endif  // CONFIG_ST_FACE_INTERNAL_API

/// @defgroup cvface_internal cvface internal API
/// @brief face internal interfaces
///
/// This set of internal interfaces
///
/// @{

// ====================== spider detect ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_spider_detector(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_spider_detect(
	cv_handle_t spider_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_rect_t **p_faces_array,
	float **p_scores,
	int *p_faces_count
);

CV_SDK_INTERNAL
float cv_face_spider_detector_get_threshold(
	cv_handle_t spider_handle
);

CV_SDK_INTERNAL
void
cv_face_release_spider_detector_result(
	cv_rect_t *faces_array,
	float *faces_scores,
	int faces_count
);

CV_SDK_INTERNAL
void cv_face_destroy_spider_detector(
	cv_handle_t spider_handle
);

// ====================== craft detect ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_craft_detector(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_craft_detect(
	cv_handle_t craft_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_rect_t **p_faces_array,
	float **p_scores,
	int *p_faces_count
);

CV_SDK_INTERNAL
float cv_face_craft_detector_get_threshold(
	cv_handle_t craft_handle
);

CV_SDK_INTERNAL
void
cv_face_release_craft_detector_result(
	cv_rect_t *faces_array,
	float *scores,
	int faces_count
);

CV_SDK_INTERNAL
void cv_face_destroy_craft_detector(
	cv_handle_t craft_handle
);

// ====================== hunter detect ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_hunter_detector(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_hunter_detect(
	cv_handle_t hunter_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_rect_t **p_faces_array,
	float **p_scores,
	int *p_faces_count
);

CV_SDK_INTERNAL
float cv_face_hunter_detector_get_threshold(
	cv_handle_t hunter_handle
);

CV_SDK_INTERNAL
void
cv_face_release_hunter_detector_result(
	cv_rect_t *faces_array,
	float *scores,
	int faces_count
);

CV_SDK_INTERNAL
void cv_face_destroy_hunter_detector(
	cv_handle_t hunter_handle
);

// ====================== simple align ========================
/// @brief 创建人脸关键点检测句柄
/// @param model_path 模型路径，若不指定模型可为NULL
/// @param facial_points_count 面部关键点数量，只支持106
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_simple_alignment(
	imgbase::model::ModelLoader &loader,
	int facial_points_count
);

/// @brief 对已提取出21点的图像提取106点
/// @param alignment_handle 已完成初始化的用于align的实例句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param points_array21 输入的21点坐标
/// @param points_array 指向检测到的106点坐标
CV_SDK_INTERNAL
cv_result_t
cv_face_simple_align(
	cv_handle_t alignment_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	const cv_pointf_t *points_array21,
	cv_pointf_t *points_array106
);

/// @brief 销毁已初始化的人脸关键点检测句柄
/// @param alignment_handle 已初始化的人脸关键点检测句柄
CV_SDK_INTERNAL
void cv_face_destroy_simple_alignment(
	cv_handle_t alignment_handle
);

// ====================== deep align ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_deep_align(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_deep_align(
	cv_handle_t da_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_rect_t rect,
	cv_pointf_t **points_array,
	int *points_count
);

CV_SDK_INTERNAL
void
cv_face_deep_align_release_result(
	cv_pointf_t *points_array,
	int points_count
);

CV_SDK_INTERNAL
void cv_face_destroy_deep_align(
	cv_handle_t da_handle
);

// ====================== utilities ========================
CV_SDK_INTERNAL
void
cv_face_5points_to_rect(
	const cv_pointf_t *lm_5p,
	cv_rect_t &r
);

// ==================== random watermark ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_rand_dewatermark(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_rand_dewatermark(
	cv_handle_t dewatermark_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	unsigned char *output_image,
	unsigned int image_width,
	unsigned int image_height
);

CV_SDK_INTERNAL
void
cv_face_destroy_rand_dewatermark(
	cv_handle_t dewatermark_handle
);

// ==================== pattern watermark ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_pattern_dewatermark(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_pattern_dewatermark(
	cv_handle_t dewatermark_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	unsigned char *output_image,
	unsigned int image_width,
	unsigned int image_height
);

CV_SDK_INTERNAL
void
cv_face_destroy_pattern_dewatermark(
	cv_handle_t dewatermark_handle
);

// ==================== helen segment ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_helen_segment(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_helen_segment(
	cv_handle_t faceseg_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	unsigned int width,
	unsigned int height,
	unsigned int stride,
	unsigned int points_count,
	const cv_pointf_t *points_array,
	unsigned char *output_image
);

CV_SDK_INTERNAL
void
cv_face_destroy_helen_segment(
	cv_handle_t segment_handle
);

// ==================== mouth live ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_mouth_live(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_mouth_live(
	cv_handle_t live_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	std::vector<imgbase::Point2f> pose,
	int ms
);

CV_SDK_INTERNAL
void
cv_face_destroy_mouth_live(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
void
cv_face_mouth_live_reset(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
float
cv_face_mouth_live_get_score(
	cv_handle_t live_handle
);

// ==================== blink live ========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_blink_live(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_blink_live(
	cv_handle_t live_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	std::vector<imgbase::Point2f> pose,
	int ms
);

CV_SDK_INTERNAL
void
cv_face_destroy_blink_live(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
void
cv_face_blink_live_reset(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
float
cv_face_blink_live_get_score(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
float
cv_face_blink_live_get_image_quality(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
float
cv_face_live_get_align_stability_score(
	std::vector<imgbase::Point2f> pose
);

// ==================== nod live ==========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_nod_live(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_nod_live(
	cv_handle_t live_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	std::vector<imgbase::Point2f> pose,
	int ms
);

CV_SDK_INTERNAL
void
cv_face_destroy_nod_live(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
void
cv_face_nod_live_reset(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
float
cv_face_nod_live_get_score(
	cv_handle_t live_handle
);

// ==================== yaw live ==========================
CV_SDK_INTERNAL
cv_handle_t
cv_face_create_yaw_live(
	imgbase::model::ModelLoader &model
);

CV_SDK_INTERNAL
cv_result_t
cv_face_yaw_live(
	cv_handle_t live_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	std::vector<imgbase::Point2f> pose,
	int ms
);

CV_SDK_INTERNAL
void
cv_face_destroy_yaw_live(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
void
cv_face_yaw_live_reset(
	cv_handle_t live_handle
);

CV_SDK_INTERNAL
float
cv_face_yaw_live_get_score(
	cv_handle_t live_handle
);

/// @}

#endif  // INCLUDE_CVFACE_API_CV_FACE_INTERNAL_H_
