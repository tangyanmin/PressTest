
#ifndef INCLUDE_CVFACE_API_CV_FACE_BATCH_H_
#define INCLUDE_CVFACE_API_CV_FACE_BATCH_H_

#include "cv_common.h"
#include "cv_face.h"

/// image 结构体定义
typedef struct cv_image_t {
	unsigned char *data; 			// 像素值存放的空间
	cv_pixel_format pixel_format;   // 像素类型
	int image_width;				// 图像宽度
	int image_height;				// 图像高度
	int image_stride;				// 图像行的字节跨度，等于宽度*通道数*量化字节数			
} cv_image_t;

/// @brief 设定该进程使用的GPU编号
/// @param device_id 该进程将要使用的GPU编号
CV_SDK_API
cv_result_t
cv_set_device(
	int device_id
);

/// @brief 创建批量人脸检测句柄
/// @param model_path 模型路径，若不指定模型可为NULL
/// @param config 配置选项，推荐使用 CV_FACE_SKIP_BELOW_THRESHOLD, 但 CV_DETECT_ENABLE_ALIGN 将被忽略, 输出结果一定不含关键点信息
/// @param batch_size 每批次处理的图像数量 (目前，batch_detect通过CPU并行实现，batch_size实际指定线程数)
CV_SDK_API
cv_handle_t
cv_face_create_detector_batch(
	const char *model_path,
	unsigned int config, 
	int batch_size
);

/// @brief 销毁已初始化的批量人脸检测句柄
/// @param detector_handle 已初始化的人脸检测句柄
CV_SDK_API
void cv_face_destroy_detector_batch(
	cv_handle_t detector_handle
);

/// @brief 批量人脸检测
/// @param detector_handle 已初始化的人脸检测句柄
/// @param image 用于检测的图像数据数组
/// @param count 图像数量
/// @param orientation 图片中人脸的方向
/// @param p_faces_array 每张图象检测到的人脸信息数组，用户负责分配指针的内存, 长度等于图像数量, API负责分配每个图像的结果的内存, 需要调用cv_face_release_detector_result_batch函数释放
/// @param p_faces_count 每张图像检测到的人脸数量，用户负责分配内存, 长度等于图像数量
CV_SDK_API
cv_result_t
cv_face_detect_batch(
	cv_handle_t detector_handle,
	const cv_image_t *images, 
	int count, 
	cv_face_orientation orientation,
	cv_face_t **p_faces_array,
	int *p_faces_count
);

/// @brief 释放批量人脸检测返回结果时分配的空间
/// @param count 图像数量
/// @param p_faces_array 每张图像检测到的人脸信息数组
/// @param p_faces_count 每张图像检测到的人脸数量
CV_SDK_API
void
cv_face_release_detector_result_batch(
	int count, 
	cv_face_t **p_faces_array
);

/// @brief 批量特征提取的属性功能配置
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_DISABLE          0
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_AGE          (1<<0)
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_GENDER_MALE  (1<<1)
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_ATTRACTIVE   (1<<2)
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_EYEGLASS     (1<<3)
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_SUNGLASS     (1<<4)
#define VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_SMILE        (1<<5)

/// @brief 创建批量人脸对齐、验证特征提取、属性提取句柄
/// @param model_path_align     人脸对齐模型库所在文件夹的绝对路径或相对路径, 可为NULL
/// @param model_path_verify    人脸验证模型库所在文件夹的绝对路径或相对路径(例如"data/verify.tar") 
/// @param model_path_attribute 人脸属性模型库所在文件夹的绝对路径或相对路径(例如"data/verify.tar"), 若不配置属性功能则可为NULL
/// @param config 功能配置, 如 VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_AGE 等
/// @param batch_size 每批次处理的人脸图像数量 (batch_size过大，可能导致显存不足而崩溃; batch_size并不限制一次送入的人脸数量，api内部会自行分批处理)
CV_SDK_API
cv_handle_t
cv_verify_create_handle_batch(
	const char *model_path_align, 
	const char *model_path_verify, 
	const char *model_path_attribute, 
	unsigned int config, 
	int batch_size
);

/// @brief 销毁已初始化的批量人脸对齐、验证特征提取、属性提取句柄
/// @param verify_handle 已初始化的批量人脸对齐、验证特征提取、属性提取句柄
CV_SDK_API
void
cv_verify_destroy_handle_batch(
	cv_handle_t verify_handle
);

CV_SDK_API
int
cv_verify_get_version_batch(
	cv_handle_t verify_handle
);

CV_SDK_API
int
cv_verify_get_feature_blob_size_batch(
	cv_handle_t verify_handle
);

/// @brief 批量人脸对齐、提取验证特征、提取属性
/// @param verify_handle 已初始化的批量人脸对齐、验证特征提取、属性提取句柄
/// @param image 用于检测的图像数据数组
/// @param count 图像数量
/// @param p_faces_array 每张图象检测到的人脸信息数组, 和图像数量相等，即图像和人脸一一对应
/// @param attribute_enable 属性提取功能配置, 如 VERIFY_BATCH_CONFIG_ENABLE_ATRRIBUTE_AGE 等
/// @param p_feature_array 提取出的人脸特征数组, 第一维为图像顺序, 此一维需要用户分配内存, 结果需要调用 cv_verify_release_feature_batch 释放
/// @param p_attribute_result_array 提取出的人脸属性数组, 第一维为图像顺序, 第二维为各个属性的值, 需要用户分配内存
/// @param feature_blob_size 可以是NULL；如果不为NULL,返回p_feature指向空间的大小，与cv_feature_header_t中的len相同, (仅一个，并非长度为count的数组)
CV_SDK_API
cv_result_t
cv_verify_get_feature_batch(
	cv_handle_t verify_handle, 
	const cv_image_t *images, 
	int count, 
	cv_face_t *p_faces_array, 
	unsigned int attribute_enable, 
	cv_feature_t **p_feature_array,
	int **p_attribute_result_array,
	unsigned int *feature_blob_size
);

/// @brief 释放批量人脸对齐、提取验证特征、提取属性时分配的空间
/// @param count 图像数量
/// @param p_faces_count 每张图像检测到的人脸数量
/// @param p_feature_array 提取出的人脸特征数组
CV_SDK_API
void
cv_verify_release_feature_batch(
	int count, 
	cv_feature_t **p_feature_array
);

#endif  // INCLUDE_CVFACE_API_CV_FACE_BATCH_H_
