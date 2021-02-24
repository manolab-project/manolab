/*                Copyrights ©2010-2020 Zebra Technologies Corp.
 *                          All rights reserved
 * 
 *  
 *****************************************************************************
 */

/**
 *****************************************************************************
 ** 
 ** @file  rfidapiConstants.h
 ** 
 ** @brief <b>  Enumerated Types </b> 
 **
 ** 
 ****************************************************************************/
#ifndef RFIDAPI_CONSTANTS_H
#define RFIDAPI_CONSTANTS_H

#if defined(__cplusplus)
extern "C"
{
#endif

	
/**
* RFID_VERSION is a member of CONNECTION_INFO.
*/
typedef enum _RFID_VERSION
{
	RFID_API3_5_0 = 0, /**< Basic version of RFIDAPI3 - Dll version 5.0.xx*/

	RFID_API3_5_1 = 1, /**<  RFID API3  - Dll version 5.1.xx. Supports features related to TagEventReporting, RSSI-Filtering, 
											NXP Tag Support, RFID_WriteXXX, APIs, Push-Model of Firmware Update in fixed Readers, 
											TagLocationing, DutyCycle.*/
	RFID_API3_5_5 = 2, /**<  RFID API3  - Dll version 5.5.xx. Supports features related to Periodic Reporting of Tags, SL_All, Phase,
											AB_Flip, Antenna Configuration and Secure Connection.*/
	RFID_API3_5_6 = 3, /**<  RFID API3  - Dll version 5.5.xx. Supports features related to Select,MLT algorithms for SNAP device and 
											Force Software update feature.*/
	RFID_API3_5_7 = 4, /**<  RFID API3  - Dll version 5.5.xx. Supports features related to Gen2 V2 Features, License Management, 
					   Start Trigger extended with Time Lapse & Distance, Stop Trigger extended with Time Lapse, Extra Triggers*/
}RFID_VERSION;

/**
* RFID_EVENT_TYPE is a parameter for RFID_RegisterEventNotification.
*/
typedef enum _RFID_EVENT_TYPE
{
	GPI_EVENT = 0, 				/**<  A GPI event (state change from high to low, or low to high) has occurred on a GPI port.
								When this event is signaled, RFID_GetEventData can be called to know 
								the GPI port and the event that has occurred. 
								The Dll can store a maximum of
								2000 GPI_EVENT_DATA, which if not retrieved using RFID_GetEventData,
								results in dropping of events in FIFO manner.
								*/
	TAG_DATA_EVENT = 1,			
	TAG_READ_EVENT = 1,			/**<  Applications can register for this event to get Tag Reports from the 
								Reader.
								When the event corresponding to this is signaled, it implies that 
								Tag(s) are available for the Application to read and the application can call
								RFID_GetReadTag till all Tags are fetched from the Dll's Queue.
								The Dll can store a maximum of
								4096 TAG_DATA by default, which if not retrieved using RFID_GetEventData,
								results in dropping of further events. The maximum Tag Storage count can be altered
								using the function RFID_SetTagStorageSettings. Inorder to get Tag reports indicating 
								result of Access-operations, applications can set the value in 
								RFID_SetTagStorageSettings.
								*/
	BUFFER_FULL_WARNING_EVENT = 2,/**<  When the internal buffers are 90% full, this event will be signaled.*/

	ANTENNA_EVENT = 3,			/**<  A particular Antenna has been Connected/Disconnected.
								When this event is signaled, RFID_GetEventData can be called to know 
								the Antenna and the Connection Status that has occurred.
								The Dll can store a maximum of
								2000 ANTENNA_EVENT_DATA, which if not retrieved using RFID_GetEventData,
								results in dropping of events in FIFO manner.
								*/
	INVENTORY_START_EVENT = 4,	/**<  Inventory Operation has started. In case of periodic trigger
									this event will be triggered for each period.*/
	INVENTORY_STOP_EVENT = 5,	/**<  Inventory Operation has stopped. In case of periodic trigger
									this event will be triggered for each period.*/
	ACCESS_START_EVENT = 6,		/**<  Access Operation has started.*/
	ACCESS_STOP_EVENT = 7,		/**<  Access Operation has stopped.*/
	DISCONNECTION_EVENT = 8,	/**< Event notifying disconnection from the Reader. 
								When this event is signaled, RFID_GetEventData can be called to know 
								the reason for the disconnection. The Application can call RFID_Reconnect 
								periodically to attempt reconnection or call RFID_Disconnect to cleanup and exit.*/
	BUFFER_FULL_EVENT = 9,		/**<  When the internal buffers are 100% full, this event will be signaled and 
								  tags are discarded in FIFO manner.*/
    NXP_EAS_ALARM_EVENT = 10,		/**<This Event is generated when Reader finds a(NXP)tag with it's EAS System bit	
									still set to true.>*/

	READER_EXCEPTION_EVENT = 11, /**< Event notifying that an exception has occurred in the Reader. 
								When this event is signaled, RFID_GetEventData can be called to know 
								the reason for the exception. The Application can continue to use the connection if
								the reader renders is usable.*/		
	HANDHELD_TRIGGER_EVENT = 12, 	/**<  A Handheld Gun/Button event Pull/Release has occurred.
								When this event is signaled, RFID_GetEventData can be called to know 
								which event that has occurred. 
								The Dll can store a maximum of
								2000 HANDHELD_TRIGGER_EVENT_DATA, which if not retrieved using RFID_GetEventData,
								results in dropping of events in FIFO manner.
								*/						  
    DEBUG_INFO_EVENT = 13,		/**<This Event is generated when Reader sends a debug information>*/

	TEMPERATURE_ALARM_EVENT = 14, /** <When the reader’s operating temperature reaches Threshold level, 
								  this event will be generated. RFID_GetEventData can be called to get the event 
								  details like source name (PA/Ambient), current Temperature 
								  and alarm Level (Low, High or Critical).>*/
	RF_SURVEY_DATA_READ_EVENT = 15, /**<  Applications can register for this event to get RF Survey Reports from the 
								Reader.
								When the event corresponding to this is signaled, it implies that 
								RF Survey data are available for the Application to read and the application can call
								RFID_GetRFSurvey or rfid_GetRfSurveyData till all RF Survey data are fetched from the Dll's Queue.
								The Dll can store a maximum of
								4096 RF_SURVEY_DATA by default, which if not retrieved using RFID_GetEventData,
								results in dropping of further events. The maximum RF SURVEY Storage count can be altered
								using the function RFID_SetRFSurveyStorageSettings. Inorder to get RFSurvey reports indicating 
								result of Access-operations, applications can set the value in 
								RFID_SetRFSurveyStorageSettings.
								*/

	RF_SURVEY_START_EVENT = 16,/**<  RF Survey Operation has started. In case of periodic trigger
									this event will be triggered for each period.*/

	RF_SURVEY_STOP_EVENT = 17,/**<  RF Survey Operation has stopped. In case of periodic trigger
									this event will be triggered for each period.*/

	AUTONOMOUS_EVENT = 18,/**<  Autonomous Events, RFID_GetEventData() to ask LLRP reader to dump unreported event.
									if enable moving tag, RFID_GetEventData() to ask LLRP reader to dump stationary tag */
}RFID_EVENT_TYPE;

/**
* READER_EXCEPTION_EVENT_TYPE specifies the event indicating the reason for Reader exception.
*/
typedef enum _READER_EXCEPTION_EVENT_TYPE
{
	UNKNOWN_EXCEPTION = 0, /**< Unknown exception. More data of the exception may be available in exceptionInfo of READER_EXCEPTION_EVENT_DATA.*/
}READER_EXCEPTION_EVENT_TYPE;

/**
* DISCONNECTION_EVENT_TYPE specifies the event indicating the reason for Disconnection.
*/
typedef enum _DISCONNECTION_EVENT_TYPE
{
	READER_INITIATED_DISCONNECTION = 0, /**< Disconnection notified from Reader 
								 (For e.g. Disconnect initiated from the web-console).*/
	READER_EXCEPTION = 1, /**< Reader has reported an exception. */
	CONNECTION_LOST = 2, /**<  Connection to Reader lost, say because of a network
								problem. If the connection to the reader was inactive (no keep-alive
								message from the Reader) for a time greater 
								than 10 times the timeout as specified in RFID_Connect, this event
								will be triggered to the application.*/
}DISCONNECTION_EVENT_TYPE;

/**
* ANTENNA_EVENT_TYPE specified the event indicating whether an antenna is connected or disconnected.
*/
typedef enum _ANTENNA_EVENT_TYPE
{
    ANTENNA_CONNECTED = 1,	/**<  Antenna is connected*/
    ANTENNA_DISCONNECTED = 0,	/**<  Antenna is disconnected*/
}ANTENNA_EVENT_TYPE;

/**
* GPI_PORT_STATE specifies the state of a GPI port.
*/
typedef enum _GPI_PORT_STATE
{
    GPI_PORT_STATE_LOW = 0,			/**<  GPI state is low.*/
    GPI_PORT_STATE_HIGH = 1,		/**<  GPI state is high.*/
    GPI_PORT_STATE_UNKNOWN = 2,		/**<  GPI state is unknown.*/
}GPI_PORT_STATE;

/**
* HANDHELD_TRIGGER_EVENT_TYPE specifies the type trigger that occurred.
*/
typedef enum _HANDHELD_TRIGGER_EVENT_TYPE
{
    HANDHELD_TRIGGER_RELEASED = 0,		/**<  The Gun/Button on the Handheld device was released.*/
    HANDHELD_TRIGGER_PRESSED = 1,		/**<  The Gun/Button on the Handheld device was pressed.*/
}HANDHELD_TRIGGER_EVENT_TYPE;

/**
*  READER_ID_TYPE indicates whether the Reader specifies the ReaderID as MAC Address or EPC-Id.
*/
typedef enum _READER_ID_TYPE
{
    MAC_ADDRESS = 0,/**<  Reader ID: MAC Address.*/
    EPC_ID = 1		/**<  Reader ID: EPC-ID.*/
}READER_ID_TYPE;

/**
* FORWARD_LINK_MODULATION indicates the type of forward link modulation. 
* It is a member of RF_MODE_TABLE_ENTRY.
*/
typedef enum _FORWARD_LINK_MODULATION
{
    FORWARD_LINK_MODULATION_PR_ASK = 0,
    FORWARD_LINK_MODULATION_SSB_ASK = 1,
    FORWARD_LINK_MODULATION_DSB_ASK = 2,
}FORWARD_LINK_MODULATION;

/**
* MODULATION indicates the type of modulation.
* It is a member of RF_MODE_TABLE_ENTRY.
*/
typedef enum _MODULATION
{
    MV_FM0 = 0,
    MV_2 = 1,
    MV_4 = 2,
	MV_8 = 3,
}MODULATION;

/**
* DIVIDE_RATIO indicates the Divide Ratio.
* It is a member of RF_MODE_TABLE_ENTRY.
*/
typedef enum _DIVIDE_RATIO
{
    DR_8 = 0,
    DR_64_3 = 1,
}DIVIDE_RATIO;

/**
* SPECTRAL_MASK_INDICATOR indicates the Spectral mask characteristics of the mode.
* It is a member of RF_MODE_TABLE_ENTRY.
*/
typedef enum _SPECTRAL_MASK_INDICATOR
{
    SMI_UNKNOWN = 0,
    SMI_SI = 1,
	SMI_MI = 2,
	SMI_DI = 3

}SPECTRAL_MASK_INDICATOR;

/**
* COMMUNICATION_STANDARD indicates the communication standard currently set for the Reader.
* It is a member of READER_CAPS.
*/
typedef enum _COMMUNICATION_STANDARD
{
	UNSPECIFIED=0,
	US_FCC_PART_15,
	ETSI_302_208,
	ETSI_300_220,
	AUSTRALIA_LIPD_1W,
	AUSTRALIA_LIPD_4W,
	JAPAN_ARIB_STD_T89,
	HONGKONG_OFTA_1049,
	TAIWAN_DGT_LP0002,
	KOREA_MIC_ARTICLE_5_2
}COMMUNICATION_STANDARD;

/**
* MEMORY_BANK specifies the four memory banks as supported by Class 1 Gen 2 Specification.
*/
typedef enum _MEMORY_BANK
{
    MEMORY_BANK_RESERVED = 0,	/**<  Reserved Memory: contain the kill and access passwords*/
    MEMORY_BANK_EPC,		/**<  EPC Memory: Contains CRC, Protocol-Control Bits, EPC Code*/
    MEMORY_BANK_TID,		/**<  TID Memory: Contains 8-bit class identifier and identifying information*/
    MEMORY_BANK_USER		/**<  User Memory: Contains user-specific data storage.*/
}MEMORY_BANK;

/**
* START_TRIGGER_TYPE indicates type of start trigger.
*/
typedef enum _START_TRIGGER_TYPE
{
	START_TRIGGER_TYPE_IMMEDIATE = 0, /**<  Start immediately when RFID_PerformInventory or RFID_PerformAccessSequence is called*/
	START_TRIGGER_TYPE_PERIODIC, /**<  Start periodically as per START_TRIGGER_PERIODIC*/
	START_TRIGGER_TYPE_GPI, /**<  Start based on GPI Trigger set by GPI_TRIGGER*/
	START_TRIGGER_TYPE_HANDHELD, /**<  Start based on Handheld Gun/Button Trigger set by HANDHELD_TRIGGER*/
	START_TRIGGER_TYPE_TIMELAPSE, /**<  Start based on timelapse Trigger set by TIMELAPSE_TRIGGER*/
	START_TRIGGER_TYPE_DISTANCE, /**<  Start based on GPS distance Trigger set by DISTANCE_TRIGGER*/
	START_TRIGGER_TYPE_NONE, /**<  Start type range check*/
}START_TRIGGER_TYPE;

/**
* STOP_TRIGGER_TYPE indicates the type of stop trigger.
*/
typedef enum _STOP_TRIGGER_TYPE
{
	STOP_TRIGGER_TYPE_IMMEDIATE = 0, /**<  Stop immediately when RFID_StopInventory or RFID_StopAccessSequence is called*/
	STOP_TRIGGER_TYPE_DURATION, /**<  Stop after a specified duration*/
	STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT, /**<  Stop after a specified GPI Timeout*/
	STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT, /**<  Stop after a specified number of Tags are read*/
	STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT, /**<  Stop after a specified number of attempts*/
	STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT,/**<  Stop after a specified Handheld Trigger Gun/Button Timeout*/
	STOP_TRIGGER_TYPE_TIMELAPSE, /**<  Stop after a timelapse gets triggered*/
	STOP_TRIGGER_TYPE_NONE, /**<  Stop type range check*/
}STOP_TRIGGER_TYPE;

/**
* TRUNCATE_ACTION indicates whether only a truncated portion of the tag is
* to be backscattered by the tag or not.
*/
typedef enum _TRUNCATE_ACTION
{
	TRUNCATE_ACTION_UNSPECIFIED = 0,/**<  The Reader decides what truncate action to take.*/
	TRUNCATE_ACTION_DO_NOT_TRUNCATE,/**<  Do not truncate*/
	TRUNCATE_ACTION_TRUNCATE,/**<  Truncate*/
}TRUNCATE_ACTION;

/**
* SESSION specifies the 4 sessions as supported by Class 1 Gen 2 Specification.
*/
typedef enum _SESSION
{
	SESSION_S0 = 0,/**<  Indicates to perform inventory based on Session S0 */
	SESSION_S1 = 1,/**<  Indicates to perform inventory based on Session S1 */
	SESSION_S2 = 2,/**<  Indicates to perform inventory based on Session S2 */
	SESSION_S3 = 3,/**<  Indicates to perform inventory based on Session S3 */
}SESSION;

/**
* INVENTORY_STATE specifies the two inventory-States as supported by Class 1 Gen 2 Specification.
*/

typedef enum _INVENTORY_STATE
{
	INVENTORY_STATE_A = 0, /**<  Indicates to select tags in State A for the session specified in SINGULATION_CONTROL.*/
	INVENTORY_STATE_B = 1, /**<  Indicates to select tags in State B for the session specified in SINGULATION_CONTROL.*/
	INVENTORY_STATE_AB_FLIP = 2 /**<  Indicates to select tags in State A or B and then flip them for the session specified in SINGULATION_CONTROL.*/
}INVENTORY_STATE;

/**
* FILTER_ACTION specifies the filter actions for the PreFilter: State Aware, Unaware or Default.
* 
*/
typedef enum _FILTER_ACTION
{
	FILTER_ACTION_DEFAULT = 0, /**<  Target and Action will be based on Reader-Settings*/
	FILTER_ACTION_STATE_AWARE = 1,/**<  State Aware */ 	
	FILTER_ACTION_STATE_UNAWARE = 2, /**<  State Unaware */
}FILTER_ACTION;



/**
* SL_FLAG specifies the Select-Flag as supported by Class 1 Gen 2 Specification.
*/
typedef enum _SL_FLAG
{
	SL_FLAG_ASSERTED = 0, /**<  Indicates to select tags with SL TRUE*/
	SL_FLAG_DEASSERTED = 1,/**<  Indicates to select tags with SL FALSE*/ 	
	SL_ALL  = 2
}SL_FLAG;

/**
* TARGET  specifies the Target Parameter for PreFilter (C1G2 Select) as supported by Class 1 Gen 2 Specification.
*/
typedef enum _TARGET
{
    TARGET_SL = 0,					/**<  Inventoried state for SL*/
    TARGET_INVENTORIED_STATE_S0 = 1,/**<  Inventoried state for session S0*/
    TARGET_INVENTORIED_STATE_S1 = 2,/**<  Inventoried state for session S1*/
    TARGET_INVENTORIED_STATE_S2 = 3,/**<  Inventoried state for session S2*/
    TARGET_INVENTORIED_STATE_S3 = 4,/**<  Inventoried state for session S3*/
}TARGET;

/**
* STATE_UNAWARE_ACTION specifies the state unware Action Parameter for PreFilter (C1G2 Select).
*/
typedef enum _STATE_UNAWARE_ACTION
{
	STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT = 0x00,	/**<   Action 0: 
	Matching tags: SELECT , Non-matching tags: UNSELECT*/
	STATE_UNAWARE_ACTION_SELECT = 0x01,	/**<   Action 1: 
	Matching tags: SELECT, Non-matching tags: do nothing*/
	STATE_UNAWARE_ACTION_NOT_UNSELECT = 0x02,	/**<   Action 2: 
	Matching tags: do nothing, Non-matching tags: UNSELECT*/
	STATE_UNAWARE_ACTION_UNSELECT = 0x03,	/**<   Action 3: 
	Matching tags: UNSELECT , Non-matching tags: do nothing*/
	STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT = 0x04,	/**<   Action 4: 
	Matching tags: UNSELECT , Non-matching tags: SELECT*/	
	STATE_UNAWARE_ACTION_NOT_SELECT = 0x05,	/**<   Action 5: 
	Matching tags: do nothing, Non-matching tags: SELECT*/
}STATE_UNAWARE_ACTION;

/**
* STATE_AWARE_ACTION specifies the state aware action Parameter for PreFilter (C1G2 Select).
*/
typedef enum _STATE_AWARE_ACTION 
{
	STATE_AWARE_ACTION_INV_A_NOT_INV_B			= 0x00,	/**<   Action 0:  
	Matching tags: inventoried state to A, Non-matching tags: inventoried state to B*/
	STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL		= 0x00,	/**<   Action 0:
	Matching tags: assert SL , Non-matching tags: deassert SL*/
	STATE_AWARE_ACTION_INV_A					= 0x01,/**<   Action 1: 
	Matching tags: inventoried state to A, Non-matching tags: do nothing*/
	STATE_AWARE_ACTION_ASRT_SL	= 0x01,/**<   Action 1: 
	Matching tags: assert SL , Non-matching tags: do nothing*/
	STATE_AWARE_ACTION_NOT_INV_B = 0x02,/**<   Action 2:
	Matching tags: do nothing, Non-matching tags: inventoried state to B*/
	STATE_AWARE_ACTION_NOT_DSRT_SL	= 0x02,/**<   Action 2: 
	Matching tags: do nothing, Non-matching tags: deassert SL*/
	STATE_AWARE_ACTION_INV_A2BB2A = 0x03,/**<   Action 3:
	Matching tags: (A to B, B to A), Non-matching tags:  do nothing*/
	STATE_AWARE_ACTION_INV_A2BB2A_NOT_INV_A = 0x03,/**<   Action 3:
	[Obsolete("Do not use this enumeration. Use STATE_AWARE_ACTION_INV_A2BB2A")]												
	Matching tags: (A to B, B to A), Non-matching tags:  do nothing*/
	STATE_AWARE_ACTION_NEG_SL = 0x03,/**<   Action 3:
	Matching tags: negate SL , Non-matching tags:  do nothing*/
	STATE_AWARE_ACTION_NEG_SL_NOT_ASRT_SL		= 0x03,/**<   Action 3:
	[Obsolete("Do not use this enumeration. Use STATE_AWARE_ACTION_NEG_SL")]
	Matching tags: negate SL , Non-matching tags:  do nothing*/
	STATE_AWARE_ACTION_INV_B_NOT_INV_A			= 0x04,/**<   Action 4: 
	Matching tags: inventoried state to B, Non-matching tags:   inventoried state to A*/
	STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL		= 0x04,/**<   Action 4: 
	Matching tags: deassert SL, Non-matching tags:  assert SL*/
	STATE_AWARE_ACTION_INV_B = 0x05,/**<   Action 5: 
	Matching tags: inventoried state to B, Non-matching tags:  do nothing*/
	STATE_AWARE_ACTION_DSRT_SL	= 0x05,/**<   Action 5: 
	Matching tags: deassert SL  , Non-matching tags:  do nothing*/
	STATE_AWARE_ACTION_NOT_INV_A	= 0x06,/**<   Action 6: 
	Matching tags: do nothing, Non-matching tags:  inventoried state to A*/
	STATE_AWARE_ACTION_NOT_ASRT_SL	= 0x06,/**<   Action 6: 
	Matching tags: do nothing, Non-matching tags:  assert SL*/
	STATE_AWARE_ACTION_NOT_INV_A2BB2A	= 0x07,/**<   Action 7:
	Matching tags: do nothing, Non-matching tags:  (A to B, B to A)*/
	STATE_AWARE_ACTION_NOT_NEG_SL	= 0x07/**<   Action 7:
	Matching tags: do nothing, Non-matching tags:  negate SL */
}STATE_AWARE_ACTION;


/**
*  LOCK_PRIVILEGE specifies the privilege Parameter for RFID_Lock.
*/
typedef enum _LOCK_PRIVILEGE
{
	 LOCK_PRIVILEGE_NONE = 0,				/**<   This is the default Value for privilege field in LOCK_ACCESS_PARAMS. 
															 This value indicates that when user is calling RFID_Lock, he  
															 doesnt want to change the lock privilege of the particular memory bank*/
    LOCK_PRIVILEGE_READ_WRITE = 1,		/**<   Lock Privilege: Read-Write*/
    LOCK_PRIVILEGE_PERMA_LOCK = 2,		/**<   Lock Privilege: Permanent Lock*/
    LOCK_PRIVILEGE_PERMA_UNLOCK = 3,	/**<   Lock Privilege: Permanent unlock*/
    LOCK_PRIVILEGE_UNLOCK = 4,			/**<   Lock Privilege: Unlock*/
}LOCK_PRIVILEGE;
#define NUM_LOCK_DATA_FIELDS 5 /**< This indicates the number of data fields that can be used for 
										Lock operation. The fields are as in LOCK_DATA_FIELD*/

/**
*  LOCK_DATA_FIELD specifies the data field on which lock operation is to performed using RFID_Lock.*/
typedef enum _LOCK_DATA_FIELD
{
    LOCK_KILL_PASSWORD = 0,	/**<   Lock the data field: Kill Password.*/
    LOCK_ACCESS_PASSWORD= 1,	/**<   Lock the data field: Access Password.*/
    LOCK_EPC_MEMORY = 2,		/**<   Lock the data field: EPC Memory.*/
    LOCK_TID_MEMORY = 3,		/**<   Lock the data field: TID Memory.*/
    LOCK_USER_MEMORY = 4,		/**<   Lock the data field: User Memory.*/
}LOCK_DATA_FIELD;

/**
*  RECOMMISSION_OPERATION_CODE specifies the operation Parameter for RFID_Recommission.
* Note: Options 3 and 7 are subject to change as functionality is not finalized.
*/
typedef enum _RECOMMISSION_OPERATION_CODE
{
	RECOMMISSION_DISABLE_PERMALOCK = 1,				/**<   The Block PermaLocking disabled, and any blocks of user memory that has been
															previously blocked permalocked are no longer block permalocked.*/
    RECOMMISSION_DISABLE_USER_MEMORY = 2,			/**<   The User Memory has been rendered inaccessible. */
    RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION = 3,/**<   Same as RECOMMISSION_DISABLE_USER_MEMORY above. */
	RECOMMISSION_DISABLE_PASSWORD = 4,				/**<   Unlock EPC, TID, and User memory banks has been unlocked and Render the kill 
															and access passwords permanently unreadable.*/
    RECOMMISSION_DISABLE_PERMALOCK_PASSWORD = 5,	/**<   Combines DISABLE_PERMALOCK and DISABLE_PASSWORD.*/
    RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD = 6,	/**<   Combine DISABLE_USER_MEMORY and DISABLE_PASSWORD.*/
	RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION = 7 /**< Same as RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD above.*/
}RECOMMISSION_OPERATION_CODE;

/**
* UNTRACEABLE: TID_HIDE_STATE specifies the two TID hide States as supported by Class 1 Gen 2 Specification.
*/
typedef enum _TID_HIDE_STATE
{
	TID_HIDE_NONE = 0, /**<  Hide none.*/
	TID_HIDE_SOME = 1, /**<  Hide some, If Tag's allocation class id E0h, then  the Tag untraceably hides TID memory above 10h, inclusive, if the Tag’s allocation class identifier is E2h then the Tag untraceably hides TID memory above 20h, inclusive */
	TID_HIDE_ALL  = 2  /**<  Hide all*/
}TID_HIDE_STATE;

/**
* UNTRACEABLE: TAG_OPERATING_RANGE specifies the Tag operating range as supported by Class 1 Gen 2 Specification.
*/
typedef enum _TAG_OPERATING_RANGE
{
	RANGE_NORMAL  = 0, /**<  Normal Range */
	RANGE_TOGGLE  = 1, /**<  Toggle temporarily */
	RANGE_REDUCED = 2  /**<  Reduced */
}TAG_OPERATING_RANGE;

/**
*  ANTENNA_MODE specifies the Antenna mode setting for RFID_SetAntennaMode.
*/
typedef enum _ANTENNA_MODE
{
	 ANTENNA_MODE_BISTATIC	= 0, /**< Bi-static  Antenna.*/
	 ANTENNA_MODE_MONOSTATIC= 1 /**<  Mono-static Antenna.*/
}ANTENNA_MODE;


/**
*  ACCESS_OPERATION_CODE specifies the Operation Code for Operation Sequence.
*/
typedef enum _ACCESS_OPERATION_CODE
{
	ACCESS_OPERATION_READ						= 0, /**<   Read*/
	ACCESS_OPERATION_WRITE						= 1, /**<   Write*/
	ACCESS_OPERATION_LOCK						= 2, /**<   Lock*/
	ACCESS_OPERATION_KILL						= 3, /**<   Kill*/
	ACCESS_OPERATION_BLOCK_WRITE				= 4, /**<   Block Write*/
	ACCESS_OPERATION_BLOCK_ERASE				= 5, /**<   Block Erase*/
	ACCESS_OPERATION_RECOMMISSION				= 6, /**<  Recommission*/
	ACCESS_OPERATION_BLOCK_PERMALOCK			= 7, /**<  Block Permalock*/
	ACCESS_OPERATION_NXP_SET_EAS				= 8, /**< Change EAS state of NXP tag(s)*/
	ACCESS_OPERATION_NXP_READ_PROTECT			= 9, /**<Protect NXP tags from Inventory and access */
	ACCESS_OPERATION_NXP_RESET_READ_PROTECT     = 10,/**< Remove Read Protection for NXP tags*/
	ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK    = 11,/**< Changes WordLock flags for two words in Fujitsu's 64K tags*/
	ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK	= 12,/**< Changes BlockLock flags for the Blocks inside a BlockGroup of Fujitsu's 64K Tags*/
	ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK		= 13,/**< Reads status of BlockLock flags inside a BlockGroup of Fujitsu's 64K Tags*/
	ACCESS_OPERATION_FUJITSU_BURST_WRITE        = 14,/**< Does a BurstWrite of Data,in multiples of 4 bytes,into Fujitsu 64K Tags and returns with number of bytes not successfully written.*/  
	ACCESS_OPERATION_FUJITSU_BURST_ERASE		= 15,/**< Does a BurstErase of Data,in multiples of 4 bytes,in the 64K Fujitsu Tags and returns with number of bytes not successfully erased.*/
	ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD = 16,/**<Changes a BlockGroup's password or an AreaGroup's password depending on whether target tag is 64K or 8K Fujitsu Tag*/
	ACCESS_OPERATION_FUJITSU_AREA_READLOCK		= 17,/**< Changes ReadLock flags for the Areas inside an AreaGroup of Fujitsu's 8K Tags*/
	ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK		= 18,/**< Changes WriteLock flags for the Areas inside an AreaGroup of Fujitsu's 8K Tags*/
	ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD = 19, /**<Sets WriteLock flags for the Areas inside an AreaGroup of Fujitsu's 8K Tags without requiring the AreaGroup's password*/
	ACCESS_OPERATION_IMPINJ_QT_WRITE			= 20, /**<  Sets the Read/Write and Persistence flags*/
	ACCESS_OPERATION_IMPINJ_QT_READ				= 21, /**<  Gets the Read/Write and Persistence flags*/
	ACCESS_OPERATION_NXP_CHANGE_CONFIG			= 22, /**<  NXP change config access operation*/
	ACCESS_OPERATION_AUTHENTICATE				= 23, /**<  G2V2 command authenticate*/	
	ACCESS_OPERATION_READBUFFER					= 24, /**<  G2V2 command ReadBuffer*/
	ACCESS_OPERATION_UNTRACEABLE				= 25, /**<  G2V2 command Untraceable*/
	ACCESS_OPERATION_CRYPTO						= 26, /**<  G2V2 command Crypto*/
	ACCESS_OPERATION_NONE = 0xFF, /**<  Not to be used for RFID_AddOperationToAccessSequence*/
}ACCESS_OPERATION_CODE;


/**
*  TAG_FIELD specifies the fields to be enabled in TAG_DATA.
*/
typedef enum _TAG_FIELD
{
	ANTENNA_ID					= 1,	/**<   Fetch AntennaID on which Tag was found from Reader */
	FIRST_SEEN_TIME_STAMP		= 2,	/**<   Fetch Tag's First Seen Time Stamp from Reader */
	LAST_SEEN_TIME_STAMP		= 4,	/**<   Fetch Tag's Last Seen Time Stamp from Reader */
	PEAK_RSSI					= 8,	/**<   Fetch RSSI of the tag from Reader */
	TAG_SEEN_COUNT				= 16,	/**<   Fetch Tag Seen Count of the tag from Reader */
	PC							= 32,	/**<   Fetch Tag's PC field from Reader */
	XPC							= 64,	/**<   Fetch Tag's XPC field from Reader */
	CRC							= 128,	/**<   Fetch Tag's CRC field from Reader */
	CHANNEL_INDEX				= 256,	/**<   Fetch the Channel Index on which the Tag was found*/
	PHYSICAL_PORT				= 512,	/**<   Fetch the Transmit and receive port of the Antenna on which the Tag was found*/
	ZONE_ID						= 1024, /**<   Fetch the Zone ID of the zone where the tag was found */
	ZONE_NAME					= 2048, /**<   Fetch the Zone Name where the tag was found */
	PHASE_INFO					= 4096, /**<   Fetch the Phase information reported by the reader when this tag was seen */
	BRAND_CHECK_STATUS			= 8192, /**<   Fetch the brand ID check status field */
	GPS_COORDINATES				= 16384, /**<  Fetch the GPS Location of the tag*/
	ALL_TAG_FIELDS				= 0x7FFF /**<	Fetch all fields*/
}TAG_FIELD;



/**
*  RFSURVEY_FIELD specifies the fields to be enabled in RF_SURVEY_DATA
*/
typedef enum _RFSURVEY_FIELD
{
	RFSURVEY_ANTENNA_ID = 1,	/**<   Fetch AntennaID on which RF Survey data was found from Reader */
	TIME_STAMP = 2,	/**<   Fetch RF Survey's data First Seen Time Stamp from Reader */
	AVERAGE_RSSI = 4,	/**<   Fetch RSSI of the tag from Reader */
	RFSURVEY_PEAK_RSSI = 8,	/**<   Fetch RSSI of the tag from Reader */
	BANDWIDTH = 16,	/**<   Fetch bandwidth */
	FREQUENCY = 32,	/**<   Fetch freuuncy*/
	ALL_RFSURVEY_FIELDS = 64
}RFSURVEY_FIELD;



/**
*  TAG_EVENT specifies the event pertaining to a Tag reported via TAG_DATA.
*/
typedef enum _TAG_EVENT
{	
	UNKNOWN_STATE = 0,			/**<   This implies that the Tag is a result of autonomous mode operation and
									but the state of the tag is not known.*/
	NEW_TAG_VISIBLE = 1,		/**<   This implies that the Tag is a result of autonomous mode operation and
									the tag is visible for the first time.*/
	TAG_NOT_VISIBLE = 2,		/**<   This implies that the Tag is a result of autonomous mode operation and
									the tag is not visible.*/
	TAG_BACK_TO_VISIBILITY = 3,	/**<   This implies that the Tag is a result of autonomous mode operation and
									the tag is back to visibility.*/
	TAG_MOVING = 4,				/**<   This implies that the Tag is moving generated by moving/stationary check */
	TAG_STATIONARY = 5,			/**<   This implies that the Tag is stationary generated by moving/stationary check */
	NONE = 6,				/**<   This implies that the Tag is not a result of autonomous mode operation.*/

}TAG_EVENT;

/**
*  Report Trigger Types for Tag reporting when Reader is operating in Autonomous mode.
*/
typedef enum _TAG_EVENT_REPORT_TRIGGER
{
	NEVER = 0,				/**< Disable new event reporting.*/
	IMMEDIATE = 1,			/**< Report immediately when the event occurs. */
	MODERATED = 2,			/**< Report subject to a moderation time.*/

}TAG_EVENT_REPORT_TRIGGER;
/**
*  Types to disable/enable moving event derived from Autonomous mode.
*/
typedef enum _TAG_MOVING_EVENT_REPORT
{
	TAG_MOVING_EVENT_DISABLE = 0,			/**< Disable moving event reporting. */
	TAG_MOVING_EVENT_ENABLE  = 1,			/**< Enable moving event reporting. */
}TAG_MOVING_EVENT_REPORT;
/**
*  READER_TYPE specifies the Reader Type and is a parameter for RFID_Login.
*/
typedef enum _READER_TYPE
{
	XR = 0, /**<   All Readers of XR Series*/
	FX = 1, /**<   All Readers of FX Series*/
	MC = 2, /**<   All (LLRP compliant) Readers of MC Series*/
}READER_TYPE;

/**
* MATCH_PATTERN specifies whether the criteria of filtering tags matching patterns A and B.
* It is a member of POST_FILTER and ACCESS_FILTER.
*/
typedef enum _MATCH_PATTERN
{
	A_AND_B=0,		/**<   Filter tags that match both the pattern A and pattern B.*/
	NOTA_AND_B,		/**<   Filter tags that do not match pattern A where as matches pattern B.*/
	NOTA_AND_NOTB,	/**<   Filter tags that do not match pattern A and also do not match pattern B.*/
	A_AND_NOTB		/**<   Filter Tags which match pattern A but do not match Pattern B. */
}MATCH_PATTERN;

/**
* MATCH_RANGE specifies the Selection for match range.
* It is a member of RSSI_RANGE_FILTER and TIME_RANGE_FILTER.
*/
typedef enum _MATCH_RANGE
{
	WITHIN_RANGE = 0,				/**<	Within range; between lower and upper time limits, 
											lower and upper limit inclusive. */
	OUTSIDE_RANGE = 1,				/**<	Outside range; outside lower and upper time limits, 
											lower and upper limit inclusive. */
	GREATER_THAN_LOWER_LIMIT = 2,	/**<	Greater than lower limit; Greater than lower time limit, 
											lower limit inclusive, upper limit ignored. */
	LOWER_THAN_UPPER_LIMIT = 3,	/**<	Lower than upper limit; Lower than upper time limit,
											upper limit inclusive, lower limit ignored.*/
}MATCH_RANGE;

/**
* MATCH_TAG_LIST specifies the Selection for match tag list.
* It is a member of RSSI_RANGE_FILTER and TIME_RANGE_FILTER.
*/
typedef enum _MATCH_TAG_LIST
{
	INCLUSIVE_TAG_LIST = 0,			/**<	included by tag list. */
	EXCLUSIVE_TAG_LIST = 1,			/**<	excluded by tag list. */
}MATCH_TAG_LIST;

/**
*  TRACE_LEVEL specifies the trace-level setting for RFID_SetTraceLevel.
*/
typedef enum _TRACE_LEVEL
{
	TRACE_LEVEL_OFF = 0,		/**<   Disable all traces.*/
	TRACE_LEVEL_FATAL	= 1,	/**<   Set trace-level to see Fatal Errors.*/
	TRACE_LEVEL_ERROR = 2,		/**<   Set trace-level to see Errors.*/
	TRACE_LEVEL_WARNING = 4,	/**<   Set trace-level to see Warnings (Unexpected events).*/
	TRACE_LEVEL_INFO = 8,		/**<   Set trace-level to see info (Tag reports, Events, function entry/exit of APIs, etc)*/
	TRACE_LEVEL_VERBOSE = 16,	/**<   Set trace-level to see verbose (function entry/exit of internal functions).*/
	TRACE_LEVEL_ALL = 31		/**<   Enable all traces.*/
}TRACE_LEVEL;

/**
*  READER_ID_LENGTH specifies the length of ReaderID.
*/
typedef enum _READER_ID_LENGTH
{
	MAC_LENGTH = 8,/**<   Length of MAC Address*/
	EPC_LENGTH = 12/**<   Length of EPC-Id*/
}READER_ID_LENGTH;

/**
*  SERVICE ID specifies the Application/ Component for RFID_GetHealthStatus
*/
typedef enum _SERVICE_ID
{
	RM   = 0, /**<   RM Server*/
	LLRP_SERVER = 1, /**<   LLRP Server*/
}SERVICE_ID;

/**
*  HEALTH status specifies whether it is up/down
*/
typedef enum _HEALTH_STATUS
{
	DOWN = 0,/**<   Health status is DOWN*/
	UP   = 1 /**<   Health status is UP*/

}HEALTH_STATUS;

/**
*  USB_OPERATION_MODE specifies the operation mode of USB setting on the Reader
*/
typedef enum _USB_OPERATION_MODE
{
	ACTIVE_SYNC = 0,/**<   Indicates that activeSync will be enabled over USB connection */
	NETWORK = 1,/**<   Indicates that Virtual Network over USB will be enabled*/
}USB_OPERATION_MODE;

typedef enum _POWER_SOURCE_TYPE
{
	FULL = 0,/**<   Indicates that Power source type is FULL 24v */
	POE_PLUS = 1,/**<   Indicates that Power source type is POE+*/
    POE = 2,/**<   Indicates that Power source type is POE  */
}POWER_SOURCE_TYPE;

typedef enum _POWER_NEGOTIATION_STATUS
{
	DISABLED_STATE = 0,/**<   Indicates that Power Negotiation status is Disabled */
	ONGOING = 1,/**<   Indicates that Power Negotiation status is Disabled */
    SUCCEEDED = 2,/**<   Indicates that Power Negotiation status is Succeeded  */
	FAILURE = 3,/**<   Indicates that Power Negotiation status is Failure  */
	NOT_APPLICABLE = 4,/**<   Indicates that Power Negotiation status is NotApplicable  */
}POWER_NEGOTIATION_STATUS;

typedef enum _LED_COLOR
{
	LED_OFF =0,
	LED_RED,
	LED_GREEN,
	LED_YELLOW
}LED_COLOR;

/**
*   OPERATION_QUALIFIER indicates the operation to be performed on the corresponding antenna
*/
typedef enum _OPERATION_QUALIFIER
{
  C1G2_OPERATION = 0,  /**< Inventory or Access operation(Default) */
  NXP_EAS_SCAN =1,	   /**< Electronic Article Surveillance */
  LOCATE_TAG = 2,				/**< Tag locating using default algorithm. This algorithm can be performed only on Single antenna.*/
}OPERATION_QUALIFIER;

/**
* Source of the Temperature Alarm Event
*/
typedef enum _TEMPERATURE_SOURCE
{
	 AMBIENT = 0, /**< Ambient Temperature */
	 PA, /**< PA Temperature */
}TEMPERATURE_SOURCE;

/**
* Specifies Alarm Level
*/
typedef enum _ALARM_LEVEL
{
	 LOW = 0, /**< Low */
	 HIGH, /**< High */
	 CRITICAL /**< Critical */
}ALARM_LEVEL;


typedef enum _ANTENNA_STOP_TRIGGER_TYPE
{
  ANTENNA_STOP_TRIGGER_TYPE_N_ATTEMPTS=1, /**<  Stop after a specified number of attempts*/  
  ANTENNA_STOP_TRIGGER_TYPE_DURATION_MILLISECS=2, /**<  Stop after a specified Millsecond duration*/
  ANTENNA_STOP_TRIGGER_TYPE_DURATION_SECS=3, /**<  Stop after a specified Second duration*/
}ANTENNA_STOP_TRIGGER_TYPE;

/** 
 *   BATTERY_STATUS ENUM 
 */
typedef enum _BATTERY_STATUS 
{ 
  BATTERY_CHARGING = 0,         /**<  battery charging */ 
  BATTERY_DISCHARGING = 1,      /**<  battery discharging */ 
  BATTERY_LEVEL_CRITICAL = 2,   /**<  battery level critical*/ 
  BATTERY_STATUS_UNKNOWN = -1           /**<   status is unknown */
}BATTERY_STATUS; 

/**
*   RADIO_TRANSMIT_DELAY_TYPE ENUM
*/
typedef enum _RADIO_TRANSMIT_DELAY_TYPE
{
	RADIO_TRANSMIT_DELAY_OFF = 0,				/**<  disable radio transmit delay*/
	RADIO_TRANSMIT_DELAY_ON_NO_TAG = 1,			/**<  enable radio transmit delay when no tag in FOV*/
	RADIO_TRANSMIT_DELAY_ON_NO_UNIQUE_TAG = 2,	/**<  enable radio transmit delay when no tag in FOV*/
}RADIO_TRANSMIT_DELAY_TYPE;


/**
*  RFID_PARAM_TYPE is used to get/set parameter of what type
*/
typedef enum _RFID_PARAM_TYPE
{
    ALGORITHM_PARAMS = 0,/**<  ALGORITHM_PARAMS: Set/Get Algorithm related parameters.*/
	PERFORM_VSWR_TEST = 1, /**< PERFORM_VSWR_TEST : Start VSWR Test */
	STOP_VSWR_TEST = 2, /**< STOP_VSWR_TEST : Stop VSWR Test */
	READER_DIAGNOSTICS_CONFIGURATION = 3, /**< READER_DIAGNOSTICS_CONFIGURATION : Set/Get reader diagnostics configuration */
	/***********************************************/
	/* Please keep add your param type before this */
	/***********************************************/
	RFID_PARAM_MAX
}RFID_PARAM_TYPE;

typedef enum _SMART_ALGORITHM_SELECTOR
{
   SMART_NONE = 0,
   SMART_SELECT,
   SMART_MLT,
   SMART_SELECT_MLT
}SMART_ALGORITHM_SELECTOR;

typedef enum _MLT_TAG_STATE
{
    MLT_TAG_MOVING = 0,
    MLT_TAG_STATIC,
    MLT_TAG_LOST
}MLT_TAG_STATE;

typedef enum _MLT_READER_TYPE
{
    MLT_IOR_READER = 0,
    MLT_TRANSITION_READER,
    MLT_STAR_READER,
    MLT_CONTACT_READER

}MLT_READER_TYPE;

#ifdef __cplusplus
}
#endif
#endif //RFIDAPI_CONSTANTS_H
