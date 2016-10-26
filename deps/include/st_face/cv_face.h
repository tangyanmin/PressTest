
#ifndef INCLUDE_CVFACE_API_CV_FACE_H_
#define INCLUDE_CVFACE_API_CV_FACE_H_

#include "cv_common.h"

/// @defgroup cvface_common cvface common
/// @brief Common definitions for cvface
/// @{

/// face struct definition
typedef struct cv_face_t {
	cv_rect_t rect;			///< 代表面部的矩形区域
	float score;			///< 置信度，用于筛除负例，与人脸照片质量无关
	cv_pointf_t points_array[21];	///< 人脸关键点的数组，如果没有计算关键点，则为NULL
	int points_count;		///< 人脸关键点数组的长度，如果没有计算关键点，则为0
	int yaw;			///< 水平转角，真实度量的左负右正
	int pitch;			///< 俯仰角，真实度量的上负下正
	int roll;			///< 旋转角，真实度量的左负右正
	int eye_dist;			///< 两眼间距
	int ID;				///< faceID，用于表示在实时人脸跟踪中的相同人脸在不同帧多次出现，在人脸检测的结果中无实际意义
} cv_face_t;

/// common config flags, 0x------00 ~ 0x------FF
#define CV_FACE_DEFAULT_CONFIG		0x00000000  ///< 默认选项，不设置任何开关
#define CV_FACE_SKIP_BELOW_THRESHOLD	0x00000001  ///< 检测阈值开关，开启时会将置信度低于阈值的人脸过滤掉，相应误检少，漏检多；关闭则会输出所有检测到的脸，误检多，漏检少，需要自行过滤；用户需根据不同模型的具体阈值决定是否开启，默认为开启
#define CV_FACE_RESIZE_IMG_320W		0x00000002  ///< resize图像为长边320的图像
#define CV_FACE_RESIZE_IMG_640W		0x00000004  ///< resize图像为长边640的图像
#define CV_FACE_RESIZE_IMG_1280W	0x00000008  ///< resize图像为长边1280的图像

/// face orientation definition
typedef enum {
	CV_FACE_UP = 0,		///< 人脸向上，即人脸朝向正常
	CV_FACE_LEFT = 1,	///< 人脸向左，即人脸被逆时针旋转了90度
	CV_FACE_DOWN = 2,	///< 人脸向下，即人脸被逆时针旋转了180度
	CV_FACE_RIGHT = 3	///< 人脸向右，即人脸被逆时针旋转了270度
} cv_face_orientation;

/// @}

//>> CONFIG_ST_FACE_API_DETECTOR

/// @defgroup cvface_detect cvface detect
/// @brief face position detection interfaces
///
/// This set of interfaces processing face position & 21 points landmark detection routines
///
/// @{

/// detect config flags, 0x----00-- ~ 0x----FF--
#define CV_DETECT_ENABLE_ALIGN		0x00000100  ///< 面部关键点选项，是否使用关键点算法，建议使用

/// @brief 创建人脸检测句柄
/// @param model_path 模型路径，若不指定模型可为NULL
/// @param config 配置选项，推荐使用 CV_FACE_SKIP_BELOW_THRESHOLD|CV_DETECT_ENABLE_ALIGN
CV_SDK_API
cv_handle_t
cv_face_create_detector(
	const char *model_path,
	unsigned int config
);

/// @brief 销毁已初始化的人脸检测句柄
/// @param detector_handle 已初始化的人脸检测句柄
CV_SDK_API
void cv_face_destroy_detector(
	cv_handle_t detector_handle
);

/// @brief 人脸检测
/// @param detector_handle 已初始化的人脸检测句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param orientation 图片中人脸的方向
/// @param p_faces_array 检测到的人脸信息数组，api负责分配内存，需要调用cv_face_release_detector_result函数释放
/// @param p_faces_count 检测到的人脸数量，api负责分配内存，需要调用cv_face_release_detector_result函数释放
CV_SDK_API
cv_result_t
cv_face_detect(
	cv_handle_t detector_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_face_orientation orientation,
	cv_face_t **p_faces_array,
	int *p_faces_count
);

/// @brief 释放人脸检测返回结果时分配的空间
/// @param faces_array 检测到的人脸信息数组
/// @param faces_count 检测到的人脸数量
CV_SDK_API
void
cv_face_release_detector_result(
	cv_face_t *faces_array,
	int faces_count
);


#ifdef CONFIG_ST_FACE_API_PRIVATE
CV_SDK_API
cv_result_t
cv_face_rect_align21p(
	cv_handle_t detector_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_rect_t rect,
	cv_pointf_t *points_array
);

CV_SDK_API
cv_result_t
cv_face_get_face_pose(
	const cv_pointf_t *lm,
	int lm_count,
	int *yaw, int *pitch, int *roll,
	int *eye_dist
);

#endif  // CONFIG_ST_FACE_API_PRIVATE
/// @}

//>> CONFIG_ST_FACE_API_TRACK

/// @defgroup cvface_track cvface track
/// @brief face tracking interfaces
///
/// This set of interfaces processing face tracking routines
///
/// @{

/// track config flags, 0x--00---- ~ 0x--FF----
#define CV_TRACK_MULTI_TRACKING	0x00010000  ///< 跟踪一张人脸或多张人脸选项，若开启则跟踪所有检测到的人脸，关闭则只跟踪检测到的人脸中最大的一张脸，默认关闭

/// @brief 创建实时人脸跟踪句柄
/// @param model_path 模型路径，若不指定模型可为NULL
/// @param config 配置选项，推荐使用 CV_FACE_SKIP_BELOW_THRESHOLD
CV_SDK_API
cv_handle_t
cv_face_create_tracker(
	const char *model_path,
	unsigned int config
);

/// @brief 销毁已初始化的实时人脸跟踪句柄
/// @param tracker_handle 已初始化的实时人脸跟踪句柄
CV_SDK_API
void
cv_face_destroy_tracker(
	cv_handle_t tracker_handle
);

/// @brief 对连续视频帧进行实时快速人脸跟踪
/// @param tracker_handle 已初始化的实时人脸跟踪句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param orientation 视频中人脸的方向
/// @param p_faces_array 检测到的人脸信息数组，api负责分配内存，需要调用cv_facesdk_release_tracker_result函数释放
/// @param p_faces_count 检测到的人脸数量
CV_SDK_API
cv_result_t
cv_face_track(
	cv_handle_t tracker_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_face_orientation orientation,
	cv_face_t **p_faces_array,
	int *p_faces_count
);

/// @brief 释放实时人脸跟踪返回结果时分配的空间
/// @param faces_array 检测到的人脸信息数组
/// @param faces_count 检测到的人脸数量
CV_SDK_API
void
cv_face_release_tracker_result(
	cv_face_t *faces_array,
	int faces_count
);

/// @}

//>> CONFIG_ST_FACE_API_ALIGN

/// @defgroup cvface_align cvface align
/// @brief face landmark alignment interfaces
///
/// This set of interfaces detect face 106 points landmark for refined face description
///
/// @{

/// @brief 创建人脸关键点检测句柄
/// @param model_path 模型路径，若不指定模型可为NULL
/// @param facial_points_count 面部关键点数量，只支持106
CV_SDK_API
cv_handle_t
cv_face_create_alignment(
	const char *model_path,
	int facial_points_count
);

#ifdef CONFIG_ST_FACE_API_PRIVATE
/// @brief 对已提取出21点的图像提取106点
/// @param alignment_handle 已完成初始化的用于align的实例句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param points_array21 输入的21点坐标
/// @param points_array 指向检测到的106点坐标
CV_SDK_API
cv_result_t
cv_face_align_21p_106p(
	cv_handle_t alignment_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	cv_pointf_t *points_array21,
	cv_pointf_t *points_array106
);
#endif  // CONFIG_ST_FACE_API_PRIVATE

/// @brief 销毁已初始化的人脸关键点检测句柄
/// @param alignment_handle 已初始化的人脸关键点检测句柄
CV_SDK_API
void cv_face_destroy_alignment(
	cv_handle_t alignment_handle
);

/// @brief 调用该函数进行面部关键点检测
/// @param alignment_handle 已初始化的人脸关键点检测句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param face 输入待处理的人脸信息，需要包括关键点信息，即face.points_array 不为NULL
/// @param facial_points_array 检测到的关键点数组（由用户分配和释放，长度需为facial_points_count）
CV_SDK_API
cv_result_t
cv_face_align(
	cv_handle_t alignment_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	const cv_face_t *face,
	cv_pointf_t *facial_points_array
);

/// @}

//>> CONFIG_ST_FACE_API_ATTRIBUTE

/// @defgroup cvface_attribute cvface attribute
/// @brief face attribute detection interfaces
///
/// This set of interfaces detect face attribute like emotion, accessories etc.
///
/// @{

typedef enum {
	CV_ATTR_FEATURE = 0,
	CV_ATTR_EMOTION = 1,
} cv_attribute_type;

typedef enum {
	CV_FEATURE_AGE = 0,		///< Real age 0 - 100
	CV_FEATURE_GENDER_MALE = 1,	///< Female <= 50, Male > 50
	CV_FEATURE_ATTRACTIVE = 2,	///< 0 - 100
	CV_FEATURE_EYEGLASS = 3,	///< false <= 50, true > 50
	CV_FEATURE_SUNGLASS = 4,	///< false <= 50, true > 50
	CV_FEATURE_SMILE = 5,		///< false <= 50, true > 50
	CV_FEATURE_LENGTH = 6		///< attribute feature length
} cv_attribute_feature_type;

typedef enum {
	CV_EMOTION_ANGRY = 0,
	CV_EMOTION_CALM = 1,
	CV_EMOTION_CONFUSED = 2,
	CV_EMOTION_DISGUST = 3,
	CV_EMOTION_HAPPY = 4,
	CV_EMOTION_SAD = 5,
	CV_EMOTION_SCARED = 6,
	CV_EMOTION_SURPRISED = 7,
	CV_EMOTION_LENGTH = 8
} cv_attribute_emotion_type;

/// @brief 创建人脸属性检测句柄
/// @param model_path 模型库所在文件夹的绝对路径或相对路径(例如"data/attribute.tar")
/// @param type 属性的类型
CV_SDK_API
cv_handle_t
cv_face_create_attribute_detector(
	const char *model_path,
	cv_attribute_type type
);

/// @brief 销毁已初始化的人脸属性检测句柄
/// @param attribute_handle 已初始化的人脸属性检测句柄
CV_SDK_API
void
cv_face_destroy_attribute_detector(
	cv_handle_t attribute_handle
);


/// @brief 人脸属性检测
/// @param attribute_handle 已初始化的人脸属性检测句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式, 现仅支持CV_PIX_FMT_BGRA8888
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param face 输入待处理的人脸信息，需要包括关键点信息，即 face.points_array 不为NULL
/// @param results_array 检测到的人脸属性数组（由用户分配和释放，长度需为对应属性的LENGTH定义）
CV_SDK_API
cv_result_t
cv_face_attribute_detect(
	cv_handle_t attribute_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	const cv_face_t *face,
	int *results_array
);

/// @}

//>> CONFIG_ST_FACE_API_LIVENESS

/// @defgroup cvface_liveness cvface liveness
/// @brief face liveness detection interfaces
///
/// This set of interfaces detect face is liveness or not.
///
/// @{

/// liveness config flags, 0x00------ ~ 0xFF------
#define CV_LIVENESS_NORESET		0x01000000  ///<  关闭自动RESET选项
#define CV_LIVENESS_INPUT_BY_FRAME	0x02000000  ///<  针对视频连续帧输入选项，如不使用则为针对摄像头实时获取帧输入
#define CV_LIVENESS_ENABLE_BLINK	0x10000000  ///<  开启眨眼检测
#define CV_LIVENESS_ENABLE_MOUTH	0x20000000  ///<  开启嘴部动作检测
#define CV_LIVENESS_ENABLE_HEADYAW	0x40000000  ///<  开启头部动作检测
#define CV_LIVENESS_ENABLE_HEADNOD	0x80000000  ///<  开启头部动作检测
#define CV_LIVENESS_ENABLE_ALL_DETECTOR	(CV_LIVENESS_ENABLE_BLINK | CV_LIVENESS_ENABLE_MOUTH | CV_LIVENESS_ENABLE_HEADYAW | CV_LIVENESS_ENABLE_HEADNOD)  ///<  开启所有检测功能
#define CV_LIVENESS_DEFAULT_CONFIG	CV_LIVENESS_ENABLE_ALL_DETECTOR

/// @brief 创建活体检测句柄
/// @param model_path 模型路径，若不指定模型可为NULL
/// @param config 配置选项，若没有特殊要求，推荐使用CV_LIVENESS_DEFAULT_CONFIG
CV_SDK_API
cv_handle_t
cv_face_create_liveness_detector(
	const char *model_path,
	unsigned int config
);

/// @brief 销毁已初始化的活体检测句柄
/// @param liveness_handle 已初始化的活体检测句柄
CV_SDK_API
void cv_face_destroy_liveness_detector(
	cv_handle_t liveness_handle
);

#define CV_LIVENESS_STATE_EYESOPEN	0x01
#define CV_LIVENESS_STATE_MOUTHOPEN	0x02
#define CV_LIVENESS_STATE_YAW		0x04
#define CV_LIVENESS_STATE_PITCH		0x08

#define CV_LIVENESS_IS_EYESOPEN(state)	(((state) & CV_LIVENESS_STATE_EYESOPEN) == CV_LIVENESS_STATE_EYESOPEN)
#define CV_LIVENESS_IS_MOUTHOPEN(state)	(((state) & CV_LIVENESS_STATE_MOUTHOPEN) == CV_LIVENESS_STATE_MOUTHOPEN)
#define CV_LIVENESS_IS_YAW(state)	(((state) & CV_LIVENESS_STATE_YAW) == CV_LIVENESS_STATE_YAW)
#define CV_LIVENESS_IS_PITCH(state)	(((state) & CV_LIVENESS_STATE_PITCH) == CV_LIVENESS_STATE_PITCH)

/// @brief 对连续输入的视频帧进行活体检测
/// @param liveness_handle 已初始化的活体检测句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param face 输入待处理的人脸信息，需要包括关键点信息，即face.points_array 不为NULL
/// @param score 活体检测得分
/// @param state 活体检测器各项状态结果，是否眨眼：CV_LIVENESS_IS_EYESOPEN(state)，是否张嘴：CV_LIVENESS_IS_MOUTHOPEN(state)，是否摇头：CV_LIVENESS_IS_YAW(state)，是否点头：CV_LIVENESS_IS_PITCH(state)
CV_SDK_API
cv_result_t
cv_face_liveness_detect(
	cv_handle_t liveness_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	const cv_face_t *face,
	float *score,
	int *state
);

#ifdef CONFIG_ST_FACE_API_PRIVATE

/// @brief 为待输入活体检测的视频帧设置时间戳
/// @param liveness_handle 已初始化的活体检测句柄
/// @param pts 需要设置的时间戳，单位为毫秒
/// @return 成功返回CV_OK，否则返回错误类型
CV_SDK_API
cv_result_t
cv_face_liveness_set_pts(
	cv_handle_t liveness_handle,
	int pts
);
#endif  // CONFIG_ST_FACE_API_PRIVATE

/// @brief 重置活体检测器，当人脸消失或发生变化时，需调用该函数进行重置。若初始化时设置CV_LIVENESS_NORESET选项，则不需要调用该函数
/// @param liveness_handle 已初始化的活体检测句柄
CV_SDK_API
void cv_face_liveness_detector_reset(
	cv_handle_t liveness_handle
);

/// @}

//>> CONFIG_ST_FACE_API_VERIFY

/// @defgroup cvface_verify cvface verify
/// @brief face verification interfaces
///
/// This set of interfaces processing face verification routines.
///
/// @{

/// @brief 创建人脸验证句柄
/// @param model_path 模型库所在文件夹的绝对路径或相对路径(例如"data/verify.tar")
CV_SDK_API
cv_handle_t
cv_verify_create_handle(
	const char *model_path
);

/// @brief 创建人脸验证句柄的副本，与旧的句柄共享内存，可用于多线程调用
/// @param old_handle 旧的人脸验证句柄，新句柄释放后才能释放旧句柄
CV_SDK_API
cv_handle_t
cv_verify_duplicate_handle(
	cv_handle_t old_handle
);

/// @brief 销毁已初始化的人脸验证句柄
/// @param verify_handle 已初始化的人脸验证句柄
CV_SDK_API
void
cv_verify_destroy_handle(
	cv_handle_t verify_handle
);

CV_SDK_API
int
cv_verify_get_version(
	cv_handle_t verify_handle
);

/// @brief 提取人脸特征，返回特征数据长度，可以把返回数组编码成字符串后存储起来以便以后使用
/// @param verify_handle 已初始化的人脸验证句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式, 现仅支持CV_PIX_FMT_BGRA8888
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param image_stride 用于检测的图像中每一行的跨度（以字节计算）
/// @param face 输入待处理的人脸信息，需要包括关键点信息，即 face.points_array 不为NULL
/// @param p_feature 人脸特征数据，需要调用cv_verify_release_feature函数释放
/// @param feature_blob_size 可以是NULL；如果不为NULL,返回p_feature指向空间的大小，与cv_feature_header_t中的len相同
CV_SDK_API
cv_result_t
cv_verify_get_feature(
	cv_handle_t verify_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	unsigned int image_width,
	unsigned int image_height,
	unsigned int image_stride,
	const cv_face_t *face,
	cv_feature_t **p_feature,
	unsigned int *feature_blob_size
);

/// @brief 特征信息编码成字符串，编码后的字符串用于保存
/// @param feature 输入的特征信息
/// @param feature_str 输出的编码后的字符串,由用户分配和释放
CV_SDK_API
cv_result_t
cv_verify_serialize_feature(
	const cv_feature_t *feature,
	char *feature_str
);


/// @brief 解码字符串成特征信息
/// @param feature_str 输入的待解码的字符串，api负责分配内存，需要调用cv_verify_release_feature释放
CV_SDK_API
cv_feature_t *
cv_verify_deserialize_feature(
	const char *feature_str
);

/// @brief 人脸验证
/// @param verify_handle 已初始化的人脸验证句柄
/// @param feature1 第一张人脸特征信息
/// @param feature2 第二张人脸特征信息
/// @param score 人脸验证相似度得分，范围0-1，得分越接近1越相似
CV_SDK_API
cv_result_t
cv_verify_compare_feature(
	cv_handle_t verify_handle,
	const cv_feature_t *feature1,
	const cv_feature_t *feature2,
	float *score
);

/// @brief 释放提取人脸特征时分配的空间
/// @param feature 提取到的人脸特征信息
CV_SDK_API
void
cv_verify_release_feature(
	cv_feature_t *feature
);

/// @}

//>> CONFIG_ST_FACE_API_FACE_SEARCH

/// @defgroup cvface_search cvface search
/// @brief face searching interfaces
///
/// This set of interfaces processing face search routines.
///
/// @{

/// @defgroup cvface_database cvface database for search
/// @ingroup cvface_search
/// @brief face database CURD interfaces for face searching
/// @{

/// @brief 创建人脸数据库句柄
CV_SDK_API
cv_handle_t
cv_verify_create_db();

/// @brief 销毁已初始化的人脸数据库句柄
/// @param db_handle 已初始化的人脸数据库句柄
CV_SDK_API
void
cv_verify_destroy_db(
	cv_handle_t db_handle
);

/// @brief 创建人脸数据库
/// @param db_handle 已初始化的人脸数据库句柄
/// @param items 用于创建数据库的人脸特征信息数组
/// @param item_count 特征信息数量
CV_SDK_API
cv_result_t
cv_verify_build_db(
	cv_handle_t db_handle,
	cv_feature_t* const *items,
	unsigned int item_count
);

/// @brief 人脸数据库增加数据
/// @param db_handle 已初始化的人脸数据库句柄
/// @param item 待加入的人脸特征信息
/// @param idx 加入后的人脸数据库索引值
CV_SDK_API
cv_result_t
cv_verify_add_face(
	cv_handle_t db_handle,
	const cv_feature_t *item,
	int *idx
);

/// @brief 人脸数据库删除数据
/// @param db_handle 已初始化的人脸数据库句柄
/// @param idx 待删除的人脸数据库索引值（与加入时一致）
CV_SDK_API
cv_result_t
cv_verify_delete_face(
	cv_handle_t db_handle,
	int idx
);

/// @brief 将人脸数据库信息保存为数据库文件
/// @param db_handle 已初始化的人脸数据库句柄
/// @param db_path 人脸数据库保存文件的路径
CV_SDK_API
cv_result_t
cv_verify_save_db(
	cv_handle_t db_handle,
	const char *db_path
);

/// @brief 加载人脸数据库文件
/// @param db_handle 已初始化的人脸数据库句柄
/// @param db_path 人脸数据库保存文件的路径
CV_SDK_API
cv_result_t
cv_verify_load_db(
	cv_handle_t db_handle,
	const char *db_path
);

/// @}

/// @brief 搜索人脸数据库
/// @param verify_handle 已初始化的人脸验证句柄
/// @param db_handle 已初始化的人脸数据库句柄
/// @param query 待搜索的人脸特征信息
/// @param top_k 最大的人脸搜索数量
/// @param top_idxs 搜索到的人脸数据库索引值数组（由用户分配和释放）
/// @param top_scores 搜索到的人脸相似度得分数组（由用户分配和释放），范围0-1，得分越接近1越相似
/// @param result_length 实际搜索到的人脸数量
CV_SDK_API
cv_result_t
cv_verify_search_face(
	cv_handle_t verify_handle,
	cv_handle_t db_handle,
	const cv_feature_t *query,
	unsigned int top_k,
	int *top_idxs,
	float *top_scores,
	unsigned int *result_length
);

/// @brief 从一组特征数组中搜索人脸
/// @param verify_handle 已初始化的人脸验证句柄
/// @param list_feature 人脸特征信息数组
/// @param list_count 人脸特征信息数量
/// @param query 待搜索的人脸特征信息
/// @param top_k 最大的人脸搜索数量
/// @param top_idxs 搜索到的人脸数据库索引值数组（由用户分配和释放）
/// @param top_scores 搜索到的人脸相似度得分数组（由用户分配和释放），范围0-1，得分越接近1越相似
/// @param result_length 实际搜索到的人脸数量
CV_SDK_API
cv_result_t
cv_verify_search_face_from_list(
	cv_handle_t verify_handle,
	cv_feature_t* const *list_feature,
	int list_count,
	const cv_feature_t *query,
	unsigned int top_k,
	int *top_idxs,
	float *top_scores,
	unsigned int *result_length
);

/// @}

//>> CONFIG_ST_FACE_API_FACE_GROUP

/// @defgroup cvface_group cvface group
/// @brief face grouping interfaces
///
/// This set of interfaces processing face grouping routines.
///
/// @{

typedef struct cv_face_group_t {
	unsigned int count;		///< 人脸数量
	int *idxs;			///< 人脸索引数组
	unsigned int group_flags;	///< 保留参数
} cv_face_group_t;

/// @brief 批量人脸分组
/// @param verify_handle 已初始化的人脸验证句柄
/// @param features 人脸特征信息数组
/// @param feature_count 人脸特征信息数量
/// @param p_groups_array 分组结果，api负责分配内存，需要调用cv_verify_release_grouping_result释放
/// @param group_count 分组的组数，需用户初始化为0
CV_SDK_API
cv_result_t
cv_verify_grouping(
	cv_handle_t verify_handle,
	const cv_feature_t * const *features,
	unsigned int feature_count,
	cv_face_group_t **p_groups_array,
	unsigned int *groups_count
);

/// @brief 释放批量人脸分组结果返回时分配的空间
/// @param p_groups_array 保存分组结果的数组
/// @param groups_count 分组的数量
CV_SDK_API
void
cv_verify_release_grouping_result(
	cv_face_group_t *p_groups_array,
	unsigned int groups_count
);

/// @}


//>> CONFIG_ST_FACE_API_DEWATERMARK

/// @defgroup cvface_dewatermark cvface dewatermark
/// @brief face image dewatermark
///
/// This set of interfaces processing face image dewatermark routines.
///
/// @{

/// @brief 创建去水印句柄
CV_SDK_API
cv_handle_t
cv_face_create_dewatermark(const char* model_path);

/// @brief 销毁已初始化的去水印句柄
/// @param dewatermark_handle 已初始化的去水印句柄
CV_SDK_API
void
cv_face_destroy_dewatermark(
	cv_handle_t dewatermark_handle
);

/// @brief 去水印
/// @param dewatermark_handle 已初始化的去水印句柄
/// @param image 用于检测的图像数据
/// @param piexl_format 用于检测的图像数据的像素格式, 现仅支持CV_PIX_FMT_BGRA8888
/// @param out_color_image 用于输出保存的4通道彩色图像
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param strategy 去水印方式，0 自动判断，1 固定水印，2 随机水印
CV_SDK_API
cv_result_t
cv_face_dewatermark(
	cv_handle_t dewatermark_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	unsigned char *output_image,
	unsigned int image_width,
	unsigned int image_height,
	int strategy
);

/// @}

//>> CONFIG_ST_FACE_API_FACESEG

/// @defgroup cvface_faceseg cvface segment
/// @brief face part segment
///
/// This set of interfaces processing face segment dewatermark routines.
///
/// @{
CV_SDK_API
cv_handle_t
cv_face_create_faceseg(const char* model_path);

CV_SDK_API
void
cv_face_destroy_faceseg(
	cv_handle_t faceseg_handle
);

CV_SDK_API
cv_result_t
cv_face_faceseg(
	cv_handle_t faceseg_handle,
	const unsigned char *image,
	cv_pixel_format pixel_format,
	unsigned int width,
	unsigned int height,
	unsigned int stride,
	const cv_face_t *face,
	unsigned char *output_image
);

/// @}

//>> CONFIG_API_END__

#endif  // INCLUDE_CVFACE_API_CV_FACE_H_
