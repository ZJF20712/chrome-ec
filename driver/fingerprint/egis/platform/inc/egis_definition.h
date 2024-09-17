/* Copyright 2024 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef __CROS_EC_DRIVER_FINGERPRINT_EGIS_PLATFORM_INC_EGIS_DEFINITION_H_
#define __CROS_EC_DRIVER_FINGERPRINT_EGIS_PLATFORM_INC_EGIS_DEFINITION_H_

//
//	Return code
//
#define OK 0
#define RESULT_MATCHED OK
#define IS_IMAGE OK
#define RESULT_NOT_MATCHED 1
#define RESULT_ABORT 2

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif
#ifdef NULL
#undef NULL
#define NULL 0
#endif

#define HANDLE long

enum { DETECT_NOISE, DETECT_FINGER_ON, DETECT_FINGER_OFF };

enum { NAVI_SINGLE_CLICK, NAVI_UP, NAVI_DOWN, NAVI_LEFT, NAVI_RIGHT };

typedef enum { FOD_ACTIVE_HIGH, FOD_ACTIVE_LOW } INT_POLARITY;

typedef enum { EDGE_TRIGGER = 0, LEVEL_TRIGGER = 4 } INT_TRIGGER;

#define ERROR_FAIL -1
#define ERROR_COMMAND_FAIL -2
#define ERROR_NO_DEVICE -3
#define ERROR_OUT_OF_MEMORY -4
#define ERROR_INVALID_PARAMETER -5
#define ERROR_BUFFER_TOO_SMALL -6
#define ERROR_ERASE_FLASH_FAIL -7
#define ERROR_WRITE_FLASH_FAIL -8
#define ERROR_READ_FLASH_FAIL -9
#define ERROR_VRT_CALIBRATION_FAIL -10
#define ERROR_DTVRT_CALIBRATION_FAIL -11
#define ERROR_LEARNING_FAIL -12
#define ERROR_LEARNING_LIMIT -13
#define ERROR_TEMPLATE_TOO_YOUNG -14
#define ERROR_DETECT_MODE_FAIL -15
#define ERROR_SENSOR_NOT_CLEAR -16
#define ERROR_CALIBRATION_DATA_INCORRECT -17
#define ERROR_SENSOR_STATUS_CHECK_FAIL -20
#define ERROR_TT_CALIBRATION_FAIL -21 // ERROR_PERCENT_LIMIT_CALIBRATION_FAIL
#define ERROR_UNSUPPORT_SENSOR -22
#define ERROR_INVALID_KEYCODE -23
#define ERROR_INVALID_HASHCODE -24
#define ERROR_CHECKSUN_FAIL -25
#define ERROR_OP_ERROR -30
#define ERROR_POLLING_FAIL -31
#define ERROR_MATCHER_FAIL -32

#define VKX_RESULT_INVALID_DATA_VERSION -116
#define VKX_RESULT_INVALID_TEMPLATE_SIZE -117
#define VKX_RESULT_INVALID_KEY_INDEX -118
#define VKX_RESULT_OUT_OF_RESOURCE -104
#define VKX_RESULT_DECRYPT_ERROR -106
#define VKX_NO_FINGER -105
#define VKX_PARTIAL_IMAGE -107
// #define VKX_SINGLE_CLICK          -108
#define VKX_NOT_FINGER -108
#define VKX_BAD_IMAGE -109
#define VKX_TIME_OUT -110
#define VKX_FINER_NOT_REMOVE -111

#define FP_INVALID_FEATURE_LEN -1008
#define FP_INVALID_FORMAT -1010
#define FP_NULL_FEATURE -1019
//
//	Fingerprint status callback
//
#define STATUS_ENROLL_STATUS 1
#define STATUS_EXTRACTING_FEATURE 2
#define STATUS_ENROLL_COUNT 3
#define STATUS_FINGER_REMOVE 4
#define STATUS_WAIT_FINGER_ON 5
#define STATUS_GET_GAIN 6
#define STATUS_EXTI_INTERRUPT 7
#define STATUS_NOT_FULL_FINGER 8
#define STATUS_LOAD_TEMPLATE 9
#define STATUS_HIGH_CLOCK 10
#define STATUS_LOW_CLOCK 11
#define STATUS_SAVE_TEMPLATE 12
#define STATUS_LOAD_LEARNING_TEMPLATE 13
//
//	Fp library return code
//
#define EGIS_OK 0
#define EGIS_INCORRECT_PARAMETER 1
#define EGIS_OUT_OF_MEMORY 2
#define EGIS_COMMAND_FAIL 3
#define EGIS_NO_DEVICE 4
#define EGIS_RELEASE_DEVICE_FAIL 5
#define EGIS_NOT_CALIBRATION 6
#define EGIS_CALIBRATION_DVR_FAIL 7
#define EGIS_CALIBRATION_DTDVR_FAIL 8
#define EGIS_CALIBRATION_FINGER_ON_FAIL 9
#define EGIS_NEED_TO_REPEAT 10
#define EGIS_INCORRECT_STATUS 11
#define EGIS_WAIT_INTERRUPT 12
#define EGIS_WAIT_INTERRUPT_TIMEOUT 13
#define EGIS_FINGER_NOT_TOUCH 14
#define EGIS_FINGER_TOUCH 15
#define EGIS_FINGER_ON_NOT_STABLE 16
#define EGIS_FINGER_NOT_REMOVED 17
#define EGIS_STEAM_ON_SENSOR 18
#define EGIS_WET_FINGER 19
#define EGIS_PARTIAL_FINGER 20
#define EGIS_RESET 21
#define EGIS_RECOVERY 22
#define EGIS_READ_SENSOR_INFO_START 23
#define EGIS_READ_SENSOR_INFO_END 24
#define EGIS_EEPROM_NO_CALIBRATION_DATA 25
#define EGIS_NULL_POINTER 26
#define EGIS_RESET_FAIL 27
#define EGIS_CALCULATE_CHECKSUM_FAIL 28
#define EGIS_CHECKSUM_INVALID_PARAMETER 29
#define EGIS_CHECKSUM_NOT_MATCH 30
#define EGIS_CHECKSUM_CALIBRATION_NOT_MATCH 31
#define EGIS_CONTINUE 32
#define EGIS_EEPROM_ERROR 33

#define EGIS_ESD_NEED_RESET 99
#define EGIS_SPI_FAIL 100
#define EGIS_SET_VBT_FAIL 101
#define EGIS_SET_SENSOR_MODE_FAIL 102
#define EGIS_SET_DETECT_MODE_FAIL 103
#define EGIS_SET_POWER_DOWN_MODE_FAIL 104
#define EGIS_SET_STANDBY_DOWN_MODE_FAIL 105
#define EGIS_CALIBRATION_SENSING_FAIL 106
#define EGIS_CALIBRATION_DETECT_FAIL 107
#define EGIS_GET_OTP_FAIL 108
#define EGIS_SENSOR_NEED_RECOVERY 109 // for emf_check_sensor_status() only
#define EGIS_OCP_DETECT 110
// FP_MERGE_ENROLL_ERROR_CODE
#define FP_MERGE_ENROLL_IAMGE_OK 0
#define FP_MERGE_ENROLL_FINISH 2
#define FP_MERGE_ENROLL_IMAGE_HIGHLY_SIMILARITY 3
#define FP_MERGE_ENROLL_BAD_IMAGE -1
#define FP_MERGE_ENROLL_FEATURE_LOW -2
#define FP_MERGE_ENROLL_FAIL -3
#define FP_MERGE_ENROLL_OUT_OF_MEMORY -4
#define FP_MERGE_ENROLL_UNKNOWN_FAIL -5
#define FP_MERGE_ENROLL_IRREGULAR_CONTEXT -6
#define FP_MERGE_ENROLL_DUPLICATE_IMAGE_REMOVED -7
#define FP_MERGE_ENROLL_REDUNDANT_INPUT -8

//
//	get image variable
//
#define GET_IMAGE_lEVEL_0 0
#define GET_IMAGE_lEVEL_1 1
#define GET_IMAGE_lEVEL_2 2
#define GET_IMAGE_lEVEL_3 3

#endif /* __CROS_EC_DRIVER_FINGERPRINT_EGIS_PLATFORM_INC_EGIS_DEFINITION_H_ */
