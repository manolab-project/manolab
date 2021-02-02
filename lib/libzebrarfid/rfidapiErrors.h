/*                Copyrights ©2010-2020 Zebra Technologies Corp.
 *                          All rights reserved
 * 
 *  
 *****************************************************************************
 */

/**
 *****************************************************************************
 ** 
 ** @file  rfidapiErrors.h
 ** 
 ** @brief <b>  Error codes </b> 
 ** 
 ** 
 ****************************************************************************/
#ifndef RFIDAPI_ERRORS_H
#define RFIDAPI_ERRORS_H
#include "rfidapiTypes.h"

#if defined(__cplusplus)
extern "C"
{
#endif
typedef enum _RFID_STATUS
{
	RFID_API_SUCCESS					=	0,		/*!< Function succeeded*/
	RFID_API_COMMAND_TIMEOUT			=	1,		/*!< Command timeout*/
	RFID_API_PARAM_ERROR 				=	2,		/*!< Invalid parameter given*/
	RFID_API_PARAM_OUT_OF_RANGE			=	3,		/*!< Parameter out of range*/
	RFID_API_CANNOT_ALLOC_MEM 			=	4,		/*!< Cannot allocate memory*/
	RFID_API_UNKNOWN_ERROR 				=	5,		/*!< Unable to determine error*/
	RFID_API_INVALID_HANDLE				=	6,		/*!< Invalid Handle is provided*/
	RFID_API_BUFFER_TOO_SMALL			=	7,		/*!< Assigned memory buffer is small*/
	RFID_READER_FUNCTION_UNSUPPORTED	=	8,		/*!< Functionality Not Supported by Reader*/
	RFID_RECONNECT_FAILED				=	9,		/*!< Unable to Reconnect to reader. The Reader state 
													might have changed. Attempt Connect API instead of Reconnect API.*/
	RFID_API_DATA_NOT_INITIALISED		=	10,		/*!< DATA should be initalised before using the structure*/
	RFID_API_ZONE_ID_ALREADY_EXITS		=	11,		/*!< Zone ID already exists*/
	RFID_API_ZONE_ID_NOT_FOUND			=	12,		/*!< Zone ID not found*/
	
	RFID_COMM_OPEN_ERROR				=	100,	/*!< Unable to open connection to the reader*/
	RFID_COMM_CONNECTION_ALREADY_EXISTS	=	101,	/*!< Connection Already exists*/
	RFID_COMM_RESOLVE_ERROR				=	102,	/*!< Unable to resolve IP or Host Name*/
	RFID_COMM_SEND_ERROR				=	103,	/*!< Error writing to transport interface*/
	RFID_COMM_RECV_ERROR				=	104,	/*!< Error reading from transport interface*/
	RFID_COMM_NO_CONNECTION				=	105,	/*!< No Connection exists to the Host*/
	RFID_INVALID_SOCKET					=   106,    /*!< Invalid Socket Handle passed*/
	RFID_READER_REGION_NOT_CONFIGURED   =   107,    /*!< Region of the Reader is not configured*/
	RFID_READER_REINITIALIZING			=   108,    /*!< Reader is reinitializing after a Firmware/OEM update*/
	RFID_SECURE_CONNECTION_ERROR		=	109,	/*!< Secure connection error. Process connStatus in SEC_CONNECTION_INFO for additional info */
	RFID_ROOT_SECURITY_CERTIFICATE_ERROR	=	110,	/*!< Root CA Security certificate missing or invalid*/
	RFID_HOST_SECURITY_CERTIFICATE_ERROR	=	111,	/*!< Host Security certificate missing or invalid*/
	RFID_HOST_SECURITY_KEY_ERROR			=	112,	/*!< Host private key missing or invalid*/
	
	RFID_CONFIG_GET_FAILED				=	200,	/*!< Unable to get the configuration*/
	RFID_CONFIG_SET_FAILED				=	201,	/*!< Unable to set the configuration*/
	RFID_CONFIG_NOT_SUPPORTED           =   202,    /*!< Configuration not supported> */
	RFID_CAP_NOT_SUPPORTED				=	300,	/*!< The specified capability is not supported*/
	RFID_CAP_GET_FAILED					=	301,	/*!< Unable to get the capability*/
	
	RFID_FILTER_NO_FILTER				=	400,	/*!< No Filter criteria specified*/
	RFID_FILTER_INVALID_INDEX			=	401,	/*!< Invalid index specified*/
	RFID_FILTER_MAX_FILTERS_EXCEEDED	=	402,	/*!< Reached limit for maximum number of filters that can be added*/ 
	RFID_NO_READ_TAGS					=	403,	/*!< No read Tags */
	RFID_NO_REPORTED_EVENTS				=	404,	/*!< No Events have been reported from the Reader */
	RFID_INVENTORY_MAX_TAGS_EXCEEDED	=	405,	/*!< Tag List is full, tag cannot be stored*/
	RFID_INVENTORY_IN_PROGRESS			=	406,	/*!< Inventory in progress, cannot perform
														requested operation*/ 
	RFID_NO_INVENTORY_IN_PROGRESS		=	407,	/*!< There is no Inventory in progress*/ 

	RFID_TAG_LOCATING_IN_PROGRESS		=	420,	/*!< Tag Locating in progress, cannot perform
														requested operation*/ 
	RFID_NO_TAG_LOCATING_IN_PROGRESS	=	421,	/*!< There is no Tag locating operation in progress*/ 

	RFID_NXP_EAS_SCAN_IN_PROGRESS		=	422,	/*!< NXP EAS SCAN Operation in progress, cannot perform
														requested operation*/ 
	RFID_NO_NXP_EAS_SCAN_IN_PROGRESS	=	423,	/*!< There is no NXP EAS SCAN operation in progress*/ 

	RFID_ACCESS_IN_PROGRESS				=	500,	/*!< Access Operation in progress, cannot perform
														requested operation*/ 
	RFID_NO_ACCESS_IN_PROGRESS			=	501,	/*!< There is no Access operation in progress*/ 
	RFID_ACCESS_TAG_READ_FAILED			=	502,	/*!< Tag read failed*/
	RFID_ACCESS_TAG_WRITE_FAILED		=	503,	/*!< Tag write failed*/
	RFID_ACCESS_TAG_LOCK_FAILED			=	504,	/*!< Tag Lock failed*/
	RFID_ACCESS_TAG_KILL_FAILED			=	505,	/*!< Tag Kill failed*/
	RFID_ACCESS_TAG_BLOCK_ERASE_FAILED	=	506,	/*!< Tag Block Erase failed*/
	RFID_ACCESS_TAG_BLOCK_WRITE_FAILED	=	507,	/*!< Tag Block Write failed*/
	RFID_ACCESS_TAG_NOT_FOUND			=	508,	/*!< Tag(s) not found in the field which matches the set Filter(s)*/
	RFID_ACCESS_SEQUENCE_NOT_INITIALIZED	=	510,	/*!< Access Sequence is not initialized*/ 
	RFID_ACCESS_SEQUENCE_EMPTY				=	511,	/*!< Access Sequence is Empty*/ 
	RFID_ACCESS_SEQUENCE_IN_USE				=	512,	/*!< Access Sequence is already in use*/ 
	RFID_ACCESS_SEQUENCE_MAX_OP_EXCEEDED	=	513,	/*!< Reached limit for maximum number of operations that can be added into Access-Sequence*/ 
	RFID_ACCESS_TAG_RECOMMISSION_FAILED		=	514,	/*!< Tag Recommission failed*/
	RFID_ACCESS_TAG_BLOCK_PERMALOCK_FAILED	=	515,	/*!< Tag Block Permalock failed*/
    RFID_ACCESS_NXP_TAG_SET_EAS_FAILED       =  516,     /*!<Tag Set EAS failed*/
	RFID_ACCESS_NXP_TAG_READ_PROTECT_FAILED  =  517,     /*!<Tag ReadProtect failed*/
	RFID_ACCESS_FUJITSU_CHANGE_WORDLOCK_FAILED = 518,    /*!< Tag Change WordLock failed*/
	RFID_ACCESS_FUJITSU_CHANGE_BLOCKLOCK_FAILED = 519,	 /*!< Tag Change BlockLock failed*/
	RFID_ACCESS_FUJITSU_READ_BLOCKLOCK_FAILED   = 520,	 /*!< Tag Read BlockLock failed*/
	RFID_ACCESS_FUJITSU_BURST_WRITE_FAILED      = 521,	 /*!< Tag Burst Write failed*/
	RFID_ACCESS_FUJITSU_BURST_ERASE_FAILED	    = 522,   /*!< Tag Burst Erase failed*/		
	RFID_ACCESS_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_FAILED = 523,  /*!< Tag Change BlockOrAreaGroup Passowrd failed*/
    RFID_ACCESS_FUJITSU_AREA_READLOCK_FAILED    = 524,   /*!< Tag Area ReadLock failed*/	
	RFID_ACCESS_FUJITSU_AREA_WRITELOCK_FAILED   = 525,   /*!< Tag Area WriteLock failed*/
	RFID_ACCESS_FUJITSU_AREA_WRITELOCK_WOPASSWORD_FAILED = 526,	/*!< Tag Area WriteLockWOPassword failed*/
	RFID_ACCESS_NXP_CHANGE_CONFIG_FAILED		= 527,			/*!< NXP Change Config failed*/
	RFID_ACCESS_IMPINJ_QT_READ_FAILED			= 528,			/*!< Impinj QT Read failed*/
	RFID_ACCESS_IMPINJ_QT_WRITE_FAILED			= 529,			/*!< Impinj QT Write failed*/
	RFID_ACCESS_G2V2_AUTHENTICATE_FAILED		= 530,			/*!< G2V2 Authenticate failed*/
	RFID_ACCESS_G2V2_READBUFFER_FAILED			= 531,			/*!< G2V2 ReadBuffer failed*/
	RFID_ACCESS_G2V2_UNTRACEABLE_FAILED			= 532,			/*!< G2V2 Untraceable failed*/
	RFID_ACCESS_G2V2_CRYPTO_FAILED				= 533,			/*!< G2V2 Crypto failed*/
	RFID_RM_INVALID_USERNAME_PASSWORD	=	601,	/*!< Invalid Username or password*/
	RFID_RM_NO_UPDATION_IN_PROGRESS		=	602,	/*!< No Updation going on at present*/
	RFID_RM_UPDATION_IN_PROGRESS		=	603,	/*!< Updation already in progress*/
	RFID_RM_COMMAND_FAILED				=	604,	/*!< RM Command processing failed*/
	RFID_NXP_BRANDID_CHECK_IN_PROGRESS	=	605,	/*!< BrandID Checking Inprogress*/
	RFID_NO_RF_SURVEY_OPERATION_IN_PROGRESS = 606,	/*!< There is no RF Survey in progress*/ 
	RFID_RFSURVEY_IN_PROGRESS				=607,   /*!< There is RF Survey in progress*/
	RFID_INVALID_ERROR_CODE				=	700		/*!< Invalid Error-Code*/
}RFID_STATUS;


typedef enum _ACCESS_OPERATION_STATUS
{	
	ACCESS_SUCCESS							=	0,		/*!< Tag Operation succeeded*/
	ACCESS_TAG_NON_SPECIFIC_ERROR			=	1,		/*!< Non-Specific Tag Error*/
	ACCESS_READER_NON_SPECIFIC_ERROR		=	2, 		/*!< Non-Specific Reader Error*/
	ACCESS_NO_RESPONSE_FROM_TAG				=	3,		/*!< No response from Tag*/
	ACCESS_INSUFFIFICENT_POWER				=	4,		/*!< Operation Failed due to insufficient Power*/
	ACCESS_INSUFFICENT_POWER				=	4,		/*!< Operation Failed due to insufficient Power*/ /*typo corrected*/
	ACCESS_TAG_MEMORY_LOCKED_ERROR			=	5,		/*!< Operation Failed as Tag memory was locked*/
	ACCESS_TAG_MEMORY_OVERRUN_ERROR			=	6,		/*!< Operation Failed due to memory overrun*/
	ACCESS_ZERO_KILL_PASSWORD_ERROR			=	7,		/*!< Operation Failed as Kill password provided was zero*/
	ACCESS_TAG_IN_PROCESS_STILL_WORKING					=	8,		/*!< Operation in Process: Still working*/
	ACCESS_TAG_SUCCESS_STORED_RESPONSE_WITHOUT_LENGTH	=	9,		/*!< Operation Success: Stored response without length*/
	ACCESS_TAG_SUCCESS_STORED_RESPONSE_WITH_LENGTH		=	10,		/*!< Operation Success: Stored response with length*/
	ACCESS_TAG_SUCCESS_SEND_RESPONSE_WITHOUT_LENGTH		=	11,		/*!< Operation Success: Send response without length*/
	ACCESS_TAG_SUCCESS_SEND_RESPONSE_WITH_LENGTH		=	12,		/*!< Operation Success: Send response with length*/
	ACCESS_TAG_ERROR_STORED_RESPONSE_WITHOUT_LENGTH		=	13,		/*!< Operation Error: Stored response without length*/
	ACCESS_TAG_ERROR_STORED_RESPONSE_WITH_LENGTH		=	14,		/*!< Operation Error: Stored response with length*/
	ACCESS_TAG_ERROR_SEND_RESPONSE_WITHOUT_LENGTH		=	15,		/*!< Operation Error: Send response without length*/
	ACCESS_TAG_ERROR_SEND_RESPONSE_WITH_LENGTH			=	16,		/*!< Operation Error: Send response with length*/
}ACCESS_OPERATION_STATUS;

#ifdef __cplusplus
}
#endif

#endif //RFIDAPI_ERRORS_H
