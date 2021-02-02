/*                Copyrights ©2010-2016 Symbol Technologies LLC
*                           All rights reserved
* 
*  
*****************************************************************************
*/

/**
*****************************************************************************
** 
** @file  rfidapiStructs.h
** 
** @brief <b> Structures </b> 
** 
** 
****************************************************************************/
#ifndef RFIDAPI_STRUCTS_H
#define RFIDAPI_STRUCTS_H
#include "rfidapiErrors.h"

#if defined(__cplusplus)
extern "C"
{
#endif

	
/**
* SEC_CONNECTION holds additional information for establishing secure connection.
*/
typedef struct _SEC_CONNECTION_INFO
{
	BOOLEAN secureMode;	/**< Indicates whether secure connection is required. Remaining parameters take effect only if secureMode is true */
	BOOLEAN validatePeerCert; /**< Validate peer certificate. If true, reader certificate is validated against root certificate */
	UINT32	sizeCertBuff;	/**< Number of bytes in clientCertBuff parameter */
	BYTE*	clientCertBuff;	/**< LLRP client's certificate data. Non-null for client certificate authentication */
	UINT32	sizeKeyBuff;	/**< Number of bytes in keyBuff parameter */
	BYTE*	clientKeyBuff;	/**< Client's private key data. Non-null for client certificate authentication. */
	UINT32	sizePhraseBuff;	/**< Number of bytes in phraseBuff parameter. Zero if key not encrypted */
	BYTE*	phraseBuff;	/**< Private key password */
	UINT32	sizeRootCertBuff;	/**< Number of bytes in rootCertBuff parameter */
	BYTE*	rootCertBuff;	/**< Root certificate data. Non-null for reader certificate validation */
	ULONG	connStatus;		/**< Status of connection. Call OpenSSL ERR_reason_error_string(connStatus) for details */	
	LPVOID lpReserved[4];	/**<  Reserved for future.*/
} SEC_CONNECTION_INFO, *PSEC_CONNECTION_INFO;

/**
* CONNECTION_INFO holds the information related to connection.
*/
typedef struct _CONNECTION_INFO
{
	RFID_VERSION version;			/**<  RFIDAPI version to be used.*/
	PSEC_CONNECTION_INFO lpSecConInfo;	/**< Secure connection parameters. */
	LPVOID lpReserved[3];				/**<  Reserved for future. 1 reserved LPVOID used for PSEC_CONNECTION_INFO **/
}CONNECTION_INFO, *LPCONNECTION_INFO;
/**
SERVER_INFO holds the timeout and version information related to the connection accepted.
*/

typedef struct _SERVER_INFO
{
    UINT32 timeoutMilliseconds;
	RFID_VERSION version;
	PSEC_CONNECTION_INFO lpSecConInfo;	/**< Secure connection parameters. */
	LPVOID lpReserved[3]; /**< Reserved for future. 1 reserved LPVOID used for PSEC_CONNECTION_INFO */

}SERVER_INFO,*LPSERVER_INFO;

/**
* Contains Antenna Information to be used for Inventory or Access Operations
*/
typedef struct _ANTENNA_INFO
{
	UINT16*  pAntennaList;/**<  Array for holding the Antennas to be used for the operation:
								 Range: 1 - N (N = Max. No of Antenna Supported).*/
	UINT32 length;/**<  Number of elements in array pAntennaList .*/
    
	OPERATION_QUALIFIER* pAntennaOpList; /** Array of operations for each antenna ID specified in the pAntennaList.
											 Operation at nth Index of this array is meant for the antenna ID at nth Index in the pAntennaList Array.
											 Should be null for running a Gen2 operation on all the antenna in the pAntennaList
											 It is invalid to set operation as NXP_EAS_SCAN ,if this structure is being used in any Access API  or
											 RFID_PerformAccessSequence Call*/
	LPVOID lpReserved[3];	/**<  Reserved for future.*/
}ANTENNA_INFO, *LPANTENNA_INFO;


/**<
* Contains the result of locating operation being performed.
*/
typedef struct _LOCATION_INFO
{ 
  union
  {
	  INT16 relativeDistance;/**< Locationing result when using LOCATE_TAG mechanism.*/
	  LPVOID lpReserved[16];	/**<  Reserved for place holder for expansion of union.*/
  };	
  LPVOID lpReserved1[4];	/**<  Reserved for future.*/
}LOCATION_INFO,*LPLOCATION_INFO;

/**<
* Contains GPS Location 
*/
typedef struct _GPS_LOCATION
{
	INT32 longitude;/**< Longitude*/
	INT32 latitude; /**< Latitude */
	INT32 altitude; /**< Altitude */
	LPVOID lpReserved[4];	/**<  Reserved for future.*/
}GPS_LOCATION, *LPGPS_LOCATION;

/**<
* Contains MLT algorithm report
*/
typedef struct _MLT_LOCATION_REPORT
{
	UINT32 XCoordEstimate;				  /**<  */
	UINT32 YCoordEstimate;				  /**<  */
	UINT32 ZCoordEstimate;				  /**<  */
	LPVOID lpReserved[4];				  /**<  Reserved for future.*/
}MLT_LOCATION_REPORT,*LPMLT_LOCATION_REPORT;

typedef struct _MLT_TAG_READ_REPORT
{
	UINT32 ReadsDuringTriggerWindow;	  /**<  */
	SYSTEMTIME ReadTimeStamp;				  /**<  */
	UINT32 Confidence;
	LPVOID lpReserved[3];				  /**<  Reserved for future.*/
}MLT_TAG_READ_REPORT, *LPMLT_TAG_READ_REPORT;

typedef struct _MLT_TRANSITION_REPORT
{
	INT32  TransitionDirectionEstimate;  /**<  */
	MLT_TAG_STATE MovingFlag;					  /**<  */
	LPVOID lpReserved[4];				  /**<  Reserved for future.*/
}MLT_TRANSITION_REPORT, *LPMLT_TRANSITION_REPORT;

typedef struct _MLT_READER_PARAMSW
{
	MLT_READER_TYPE readerType;    /**< */
	//WCHAR		readerID[MAX_PATH];		/**< Zone Name of the Tag Events.*/
	LPVOID		lpReserved[4];				/**<  Reserved for future.*/
}MLT_READER_PARAMSW, *LPMLT_READER_PARAMSW;

typedef struct _MLT_READER_PARAMSA
{
	MLT_READER_TYPE readerType;    /**< */
	//CHAR		readerID[MAX_PATH];		/**< Zone Name of the Tag Events.*/
	LPVOID		lpReserved[4];				/**<  Reserved for future.*/
}MLT_READER_PARAMSA, *LPMLT_READER_PARAMSA;

#ifdef UNICODE
typedef MLT_READER_PARAMSW MLT_READER_PARAMS;
typedef LPMLT_READER_PARAMSW LPMLT_READER_PARAMS;
#else
typedef MLT_READER_PARAMSA MLT_READER_PARAMS;
typedef LPMLT_READER_PARAMSA LPMLT_READER_PARAMS;
#endif

typedef struct _MLT_ALGORITHM_REPORT
{
	MLT_LOCATION_REPORT   MLTLocationReport;    /**<  */
	MLT_TAG_READ_REPORT   MLTTagReadReport;	  /**<  */
	MLT_TRANSITION_REPORT MLTTransistionReport; /**<  */
	MLT_READER_PARAMS     MLTReaderParams; /**< */
	LPVOID lpReserved[5];				  /**<  Reserved for future.*/
}MLT_ALGORITHM_REPORT, *LPMLT_ALGORITHM_REPORT;


typedef struct _MLT_TIMEWINDOWSIZE_PARAMS
{
	UINT32		LocationTime;
	UINT32		MotionTime;
	UINT32		TransitionTime;
	UINT32		BucketSize;
	LPVOID		lpReserved[4];
}MLT_TIMEWINDOWSIZE_PARAMS, *LPMLT_TIMEWINDOWSIZE_PARAMS;

typedef struct _MLT_TIMELIMIT_PARAMS
{
	UINT32		MotionDelay;
	UINT32		LostEventDelay;
	UINT32		ReportUpdate;
	LPVOID		lpReserved[4];
}MLT_TIMELIMIT_PARAMS, *LPMLT_TIMELIMIT_PARAMS;

/*typedef struct _MLT_READER_TYPE
{

}MLT_READER_TYPE, *LPMLT_READER_TYPE;*/

typedef struct _MLT_ANTENNA_MAP_TABLE
{
	UINT32 AntennaId;
	INT32 pseudoX;
	INT32 pseudoY;
	INT32 pseudoZ;
	UINT32 GroupId;
	LPVOID		lpReserved[4];
}MLT_ANTENNA_MAP_TABLE, *LPMLT_ANTENNA_MAP_TABLE;

typedef struct _MLT_TRANSITION_SLOPETABLE
{
	INT32		Direction;
	INT32		Slope;
	LPVOID		lpReserved[4];
}MLT_TRANSITION_SLOPETABLE, *LPMLT_TRANSITION_SLOPETABLE;


typedef struct _MLT_ALGORITHM_PARAMS
{
	MLT_TIMEWINDOWSIZE_PARAMS		TimeWindowSizeParams;
	MLT_TIMELIMIT_PARAMS			TimeLimitParams;
	//MLT_READER_TYPE					ReaderType;
	MLT_READER_PARAMS				ReaderType;
	MLT_ANTENNA_MAP_TABLE			AntennaMapTable[8]; 
	MLT_TRANSITION_SLOPETABLE		TransitionSlopeTable[4];
	LPVOID lpReserved[5];				  /**<  Reserved for future.*/
}MLT_ALGORITHM_PARAMS, *LPMLT_ALGORITHM_PARAMS;


/**<
* Contains Impinj QT Data
*/
typedef struct _IMPINJ_QT_DATA
{
	UINT16 QTControlData;				/**<  QT Control bits. Bit 15 controls the Short Range Feature and Bit 14 Controls the Public or Private Memory Map.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}IMPINJ_QT_DATA,*LPIMPINJ_QT_DATA;

/**<
* Contains the Write Parameters for Impinj QT Write
*/
typedef struct _IMPINJ_QT_WRITE_PARAMS
{
	UINT32			accessPassword; /**< Access Password of the Impinj tag*/
	BOOLEAN			QTPersist; /**< Indicates whether the QT control is written to nonvolatile (NVM) or volatile memory*/
	IMPINJ_QT_DATA	qtdata;
	LPVOID lpReserved[4];	/**<  Reserved for future.*/
}IMPINJ_QT_WRITE_PARAMS,*LPIMPINJ_QT_WRITE_PARAMS;

/**<
* Contains the Read Parameters for Impinj QT Read
*/
typedef struct _IMPINJ_QT_READ_PARMS
{
	UINT32			accessPassword; /**< Access Password of the Impinj tag*/
	LPVOID			lpReserved[4];	/**<  Reserved for future.*/
}IMPINJ_QT_READ_PARMS,*LPIMPINJ_QT_READ_PARMS;

/**<
* Contains the NXP Change config parameters
*/
typedef struct _NXP_CHANGE_CONFIG_PARAMS
{
	UINT32			accessPassword;				/**< Access Password of the Impinj tag*/
	UINT16			NXPChangeConfigWord;		/**< Change config word*/
	LPVOID			lpReserved[4];				/**<  Reserved for future.*/
}NXP_CHANGE_CONFIG_PARAMS,*LPNXP_CHANGE_CONFIG_PARAMS;

/**
*  Contains the result of AccessOperation on a particular tag.Applicable only when
   TagData.OpCode is one of ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK,ACCESS_OPERATION_FUJITSU_BURST_WRITE,
   ACCESS_OPERATION_FUJITSU_BURST_ERASE, ACCESS_OPERATION_IMPINJ_QT_READ, ACCESS_OPERATION_NXP_CHANGE_CONFIG
*/

typedef struct _ACCESS_OPERATION_RESULT
{ 
   union
   {    
	    /**
			Result of Fujitsu's ReadBlockLock operation on a 64K Tag
		*/
		struct
		{
		   UINT16 blockLockStatus;   /**<status of the 16 BlockLock flags inside the BlockGroup */
	  	
		 }FujitsuReadBlockLockResult; 
		 
		 
	    /**
			Result of Fujitsu's BurstWrite operation on a 64K Tag
		*/
		 struct
		 {
		   UINT8 numberOfBytesNotWritten;

 		 }FujitsuBurstWriteResult;
		 
		 
	    /**
			Result of Fujitsu's BurstErase operation on a 64K Tag
		*/
		 struct
		 {
	       UINT8 numberOfBytesNotErased;
		   
 		 }FujitsuBurstEraseResult;
		/**
			Result of Impinj QT Data operation 
		*/
		 IMPINJ_QT_DATA QtData;
		/**
			Result of NXPChange config Data operation 
		*/
		 struct
		 {
			UINT16 ChangeConfigWord;		   
 		 }NXPChangeConfigWordResult;

		 /**
		 Result of Authenticate/ReadBuffer
		 */
		 struct
		 {
			 UINT16 receivedBitCount;   /**<received bit count in result of access command Authenticate and ReadBuffer.*/
			 UINT8* pReceivedBitData;	/**<received data in result of access command Authenticate and ReadBuffer.*/
		 }AuthenticateReadBufferResult;

		LPVOID lpReserved1[16];
   };
   LPVOID lpReserved[2];
}ACCESS_OPERATION_RESULT,*LPACCESS_OPERATION_RESULT;

/**
* contains struct info 
*/
typedef struct 
{
	UINT32            memoryAllocated; /**<  Memory allocated by that structure.*/
	UINT32            memoryUsed; /**<  Memory used by that structure.*/
} _STRUCT_INFO;

/** 
* STATE_AWARE_SINGULATION_ACTION contains State Aware Singulation Action parameters.
* It is a member of SINGULATION_CONTROL.
*/
typedef struct _STATE_AWARE_SINGULATION_ACTION
{
	BOOLEAN perform;/**< Indicates whether to perform State Aware Singulation Action or not.
						 Can be TRUE if the capability is supported by reader.*/

	INVENTORY_STATE inventoryState;/**< Indicates the inventory state [A or B] to be matched during the singulation.*/
	SL_FLAG slFlag;/**< Indicates the SL bit [asserted or deasserted] to be matched during the singulation.*/
}STATE_AWARE_SINGULATION_ACTION, *LPSTATE_AWARE_SINGULATION_ACTION;

/** 
* SINGULATION_CONTROL contains Singulation Control parameters.
*/
typedef struct _SINGULATION_CONTROL
{
	SESSION session;/**< Session number to use for the inventory operation.*/
	UINT16 tagPopulation; /**< An estimate of the tag population in view of 
								 the RF field of the antenna.*/
	UINT16 tagTransitTimeMilliseconds; /**< An estimate of the time a tag will typically 
								  remain in the RF field of the antenna specified in milliseconds.*/
	STATE_AWARE_SINGULATION_ACTION stateAwareSingulationAction; /**< Can be ignored if 
														capability is not supported by reader.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}SINGULATION_CONTROL, *LPSINGULATION_CONTROL;

/** 
* PERSISTENCE_CONFIG contains LLRP configs
*/
typedef struct _PERSISTENCE_CONFIG
{
      BOOLEAN saveConfiguration;	/**< If true, configurations made over LLRP (SET_READER_CONFIG) is persisted across shutdown or restart of reader; else configuration is not saved.*/
      BOOLEAN saveTagData; 		/**< If true, tags read by the reader by execution of autonomous mode or ROSpec with custom event reporting are persisted across shutdown or restart of reader; else read tags are lost if reader is shutdown or restarted. */
      BOOLEAN SaveTagEventData; /**< If true, event list as well as the tags read by the reader by execution of autonomous mode or reader operation with custom event reporting is persisted across shutdown or restart of reader; else event list is not saved during a shutdown or reboot.*/
}PERSISTENCE_CONFIG, *LPPERSISTENCE_CONFIG ;

/** 
* RF_MODE_TABLE_ENTRY contains the various parameters of an RF Mode supported by the Reader.
* It is a member of UHF_RF_MODE_TABLE.
*/
typedef struct _RF_MODE_TABLE_ENTRY
{
	UINT32 modeIdentifer;/**< Reader defined identifier as index to this RF_MODES.*/
	DIVIDE_RATIO divideRatio; /**< */
	UINT32 bdrValue; /**< */
	MODULATION modulation;/**< */
	FORWARD_LINK_MODULATION forwardLinkModulationType;/**< */
	UINT32 pieValue;/**< */
	UINT32 minTariValue;/**< */
	UINT32 maxTariValue;/**< */
	UINT32 stepTariValue;/**< */
	SPECTRAL_MASK_INDICATOR spectralMaskIndicator;/**< */
	BOOLEAN epcHAGTCConformance; /**< */
	LPVOID lpReserved[4];				/**<  Reserved for future.*/

} RF_MODE_TABLE_ENTRY, *LPRF_MODE_TABLE_ENTRY;

/** 
* UHF_RF_MODE_TABLE contains RF Mode table parameters for each air protocol.
* It is a member of RF_MODES.
*/
typedef struct _UHF_RF_MODE_TABLE
{
	UINT16					protocolID;/**< Specifies the air protocol in use.*/
	UINT16					numEntries;/**< Number of entries in pTablesEntries.*/
	RF_MODE_TABLE_ENTRY*	pTablesEntries;/**< Pointer to RF MODE Table entries.*/
} UHF_RF_MODE_TABLE, *LPUHF_RF_MODE_TABLE; 

/** 
* RF_MODES gives information on RF Modes supported by the Reader.
* It is a member of READER_CAPS.
*/
typedef struct _RF_MODES
{
	UINT16				numTables;/**< Number of RF Modes supported by the Reader.*/
	UHF_RF_MODE_TABLE*	pUHFTables; /**< Pointer to the RF Mode tables.*/
} RF_MODES, *LPRF_MODES;

/**
* TRANSMIT_POWER_LEVEL_TABLE gives information on the Transmit Power Levels supported by the Reader.
* It is a member of READER_CAPS.
*/
typedef struct _TRANSMIT_POWER_LEVEL_TABLE
{
	UINT16		numValues; /**< Number of elements in the array pPowerValueList.*/
	UINT16*		pPowerValueList;/**<   Array of transmit power expressed in dBm*100 to allow fractional dBm representation.*/
} TRANSMIT_POWER_LEVEL_TABLE, *LPTRANSMIT_POWER_LEVEL_TABLE;

/**
* RECEIVE_SENSITIVITY_TABLE gives information on the Receive Sensitivity values supported by the Reader.
* It is a member of READER_CAPS.
*/
typedef struct _RECEIVE_SENSITIVITY_TABLE
{
	UINT16		numValues; /**< Number of elements in the array pReceiveSensitivityValueList.*/
	UINT32*		pReceiveSensitivityValueList;/**<   Array of Receive Sensitivity in 
											 dB relative to the maximum sensitivity, 
											 Possible Values: 0 to 128.*/
} RECEIVE_SENSITIVITY_TABLE, *LPRECEIVE_SENSITIVITY_TABLE;

/**
* DUTY_CYCLE_TABLE gives information on the Duty Cycle Percentage values supported by the Reader.
* It is a member of READER_CAPS.
*/
typedef struct _DUTY_CYCLE_TABLE
{
	UINT16		numValues; /**< Number of elements in the array pDutyCycleValueList. If 0, it indicates that the 
								reader does not support Duty cycle setting.*/
	UINT16*		pDutyCycleValueList;/**<   Array of Duty Cycle values. For MC devices it is in percentage.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
} DUTY_CYCLE_TABLE, *LPDUTY_CYCLE_TABLE;


/**
*  Frequency Hope Table. Zero or more instance when hopping is true.
* It is a member of FREQ_HOP_INFO.
*/
typedef struct _FREQ_HOP_TABLE
{
	UINT16   hopTableID;
	UINT16   numFreq;
	UINT32*  pFreqList;
} FREQ_HOP_TABLE, *LPFREQ_HOP_TABLE;

/**
*  Frequency Hope Table Entry. 
* It is a member of READER_CAPS.
*/
typedef struct _FREQ_HOP_INFO
{
	UINT16			  numTables;
	FREQ_HOP_TABLE*   pFreqTables;
} FREQ_HOP_INFO, *LPFREQ_HOP_INFO;

/**
* Fixed Frequency Table. At most one instance only when hopping is false
* It is a member of READER_CAPS.
*/
typedef struct _FIXED_FREQ_INFO
{
	UINT16   numFreq;
	UINT32*  pFreqList;
} FIXED_FREQ_INFO, *LPFIXED_FREQ_INFO;

/**
* PerAntennaReceiveSensitivityRange Parameter.
* It is a member of READER_CAPS.
*/
typedef struct _PER_ANTENNA_RECEIVE_SENSITIVITY_RANGE
{
	/*Shall be defined in future when supported.*/
	LPVOID lpReserved[32];				/**<  Reserved for future.*/
} PER_ANTENNA_RECEIVE_SENSITIVITY_RANGE, *LPPER_ANTENNA_RECEIVE_SENSITIVITY_RANGE;


/**
* Reader ID as supported by the reader either MAC for EPC Format.  
* It is a member of READER_CAPS.
*/
typedef struct _READER_IDW
{
	READER_ID_TYPE   type; /**<  Reader shall support representation of ReaderID of type MAC-Address
						   or EPC-ID. This field mentions the type which reader supports and 
						   indicates the Reader ID type of the value field in READER_ID.*/
	WCHAR			 value[MAX_PATH]; /**<  Reader ID as reported by the Reader.*/
} READER_IDW, *LPREADER_IDW;

typedef struct _READER_IDA
{
	READER_ID_TYPE   type; /**<  Reader shall support representation of ReaderID of type MAC-Address
						   or EPC-ID. This field mentions the type which reader supports and 
						   indicates the Reader ID type of the value field in READER_ID.*/
	CHAR			 value[MAX_PATH]; /**<  Reader ID as reported by the Reader.*/
} READER_IDA, *LPREADER_IDA;

#ifdef UNICODE
typedef READER_IDW READER_ID;
typedef LPREADER_IDW LPREADER_ID;
#else
typedef READER_IDA READER_ID;
typedef LPREADER_IDA LPREADER_ID;
#endif

/** 
* VERSIONSW gives name and version of a sp[ecific reader module.
* This is part of READER_MODULE_VERSIONS.
*/
typedef struct _VERSIONW
{
	WCHAR			moduleName[MAX_PATH];/**< Name of module*/
	WCHAR			moduleVersion[MAX_PATH];/**< Version of module*/
}VERSIONW, *LPVERSIONSW;
typedef struct _VERSIONA
{
	CHAR			moduleName[MAX_PATH];
	CHAR			moduleVersion[MAX_PATH];
}VERSIONA, *LPVERSIONA;

/** 
* READER_MODULE_VERSIONS gives versions of reader modules as reported by the Reader.
* This is part of READER_CAPS.
*/
typedef struct _READER_MODULE_VERSIONSW
{
	UINT16 numVersions;/**< Number of versions in pVersionList*/
	VERSIONW *pVersionList;/**< Pointer to an array of VERSIONSW*/
	LPVOID lpReserved[4];				/**<  Reserved  for future.*/
}READER_MODULE_VERSIONSW, *LPREADER_MODULE_VERSIONSW;

typedef struct _READER_MODULE_VERSIONSA
{
	UINT16 numVersions;/**< Number of versions in pVersionList*/
	VERSIONA *pVersionList;/**< Pointer to an array of VERSIONSW*/
	LPVOID lpReserved[4];				/**<  Reserved  for future.*/
}READER_MODULE_VERSIONSA, *LPREADER_MODULE_VERSIONSA;
/** 
* READER_CAPS gives information on Reader Capabilities as reported by the Reader.
* This can be obtained by calling RFID_GetReaderCaps.
*/
typedef struct READER_CAPSW
{
	/* Sub-structure describing memory allocated and used by this     
	structure.*/
	_STRUCT_INFO        structInfo;
	READER_IDW			readerID;/**<  Reader ID in either MAC for EPC Format.*/
	/*  General Capabilities.*/

	WCHAR			firmWareVersion[MAX_PATH];/**< Firmware Version of the reader*/
	WCHAR			modelName[MAX_PATH];/**< Reader Model*/
	UINT16	numAntennas; /**<  Number of Antenna Supported.*/
	//	AIR_PROTOCOL   airProtocolSupported;
	UINT16	numGPIs; /**<  Number of GPI ports.*/
	UINT16	numGPOs; /**<  Number of GPO ports.*/
	BOOLEAN utcClockSupported;
	RECEIVE_SENSITIVITY_TABLE receiveSensitivtyTable; /**<  Receive Sensitivty Table.*/
	PER_ANTENNA_RECEIVE_SENSITIVITY_RANGE perAntennaReceiveSensitivtyRange; /**< Valid if Reader supports multiple antennas 
																									where the antennas can have different receive sensitivity values.
																									Not supported, may be support in future.*/

	/*  Gen2LLRP Capabilities.*/
	BOOLEAN blockEraseSupported; /**<  Indicates whether Block Erase is supported by the Reader.*/
	BOOLEAN blockWriteSupported; /**<  Indicates whether Block Write is supported by the Reader .*/
	BOOLEAN stateAwareSingulationSupported; /**<  Indicates whether Inventory State Aware Singulation is supported by the Reader.*/
	BOOLEAN recommissionSupported; /**<  Indicates whether Recommission is supported by the Reader.*/
	BOOLEAN blockPermalockSupported; /**<  Indicates whether Block Permalock is supported by the Reader.*/
	BOOLEAN writeUMISupported; /**<  Indicates whether UMI (User Memory Indicator) Write is supported by the Reader.*/
	BOOLEAN radioPowerControlSupported; /**<  Indicates whether radio can be powered ON or OFF.*/
	BOOLEAN radioTransmitDelaySupported; /**<  Indicates whether radio can do transmit delay.*/
	UINT16 maxNumOperationsInAccessSequence; /**<  Maximum Number of OpSpecs allowed per access Filter.*/
	UINT16 maxNumPreFilters;  /**<  Maximum Number of Pre-Filters allowed per antenna.*/
	RF_MODES rfModes;/**<  List of RF Mode tables supported by the Reader.*/

	/*   Regulatory Capabilities.*/
	UINT16	countryCode; /**<  Country Code .*/
	COMMUNICATION_STANDARD	communicationStandard; /**<  Communication Standard .*/

	/*  UHF Band capabilities.*/
	TRANSMIT_POWER_LEVEL_TABLE transmitPowerLevelTable; /**< Transmit Power Level Table .*/

	/*  Frequency Information.*/
	BOOLEAN hoppingEnabled; /**<  Is Frequency hopping enabled.*/
	FREQ_HOP_INFO freqHopInfo; /**<  Frequency Hop information.*/
	FIXED_FREQ_INFO fixedFreqInfo; /**<  Fixed Frequency Information .*/
	
	BOOLEAN tagEventReportingSupported;	/**<	Indicates the reader’s ability to report tag visibility state changes.
												If true, the client can choose the tag event state changes to 
												monitor and report. Event state options are New Tag, Tag Invisible, 
												or Tag Visibility Changed.*/
	BOOLEAN rssiFilterSupported;		/**<	Indicates the reader’s ability to report tags based
												on the signal strength of the back-scattered signal 
												from the tag.*/
	BOOLEAN NXPCommandsSupported; /**< Indicates whether the reader supports NXP commands like Change EAS,set Quiet, Reset Quiet,Calibrate*/
	BOOLEAN tagLocationingSupported;		/**<	Indicates the reader’s ability to locate a tag.*/
	LPDUTY_CYCLE_TABLE lpDutyCycleTable;	/**<	List of DutyCycle percentages supported by the reader.*/
	BOOLEAN FujitsuCommandsSupported;       /**< Indicates the reader support for Fujitsu custom commands for 64K and 8K Tags.*/
	BOOLEAN ImpinjCommandSupported;       /**< Indicates the reader support for Impinj QT Commands.*/
	BOOLEAN SledBatteryStatusSupported;				/**< Sled Battery Status support*/
	BOOLEAN TagListFilterSupported;			/**<	Indicates the reader’s ability to report tags based on the tag list filter.*/
	LPREADER_MODULE_VERSIONSW lpVersions; /**< List of versions of reader modules.*/
	BOOLEAN PeriodicTagReportsSupported;	/**< Periodic tag reporting is supported */
	BOOLEAN TagPhaseReportingSupported;		/**< Tag Phase information in tag report */
	BOOLEAN ZoneSuppported;					/**< Zone support*/
	BOOLEAN AntennaRFConfigSupported;		/**< Radio RF capabilities - Stop condition, physical port config & SL_ALL & AB_FLIP support */
	BOOLEAN TagMovingStationarySupported;	/**<	Indicates the reader’s ability to support tag status of moving and stationary.
													If true, the client can choose the tag event state changes to
										monitor and report. Event state options are New Tag, Tag Invisible,
										or Tag Visibility Changed.*/
	BOOLEAN ZebraTriggerSupported; /**< extra triggers support, such as timelapse and GPS distance. */
	BOOLEAN G2V2CommandSupported; /**< support G2V2 commands such as Authenticate, ReadBuffer, Untraceable, and Crypto */
	BOOLEAN GPSReportingSupported; /**< Can Report GPS Location */
	BYTE reserved; /**< Reserved fields for future */
	UINT32  reserved32; /**< Reserved fields for future */
	LPVOID lpReserved[24];				/**<  Reserved  for future.*/
}READER_CAPSW, *LPREADER_CAPSW;

typedef struct READER_CAPSA
{
	/* Sub-structure describing memory allocated and used by this     
	structure.*/
	_STRUCT_INFO        structInfo;
	READER_IDA			readerID;/**<  Reader ID in either MAC for EPC Format.*/
	/*  General Capabilities.*/
	CHAR			firmWareVersion[MAX_PATH];/**< Firmware Version of the reader*/
	CHAR			modelName[MAX_PATH];/**< Reader Model*/

	UINT16	numAntennas; /**<  Number of Antenna Supported.*/
	//	AIR_PROTOCOL   airProtocolSupported;
	UINT16	numGPIs; /**<  Number of GPI ports.*/
	UINT16	numGPOs; /**<  Number of GPO ports.*/
	BOOLEAN utcClockSupported;
	RECEIVE_SENSITIVITY_TABLE receiveSensitivtyTable; /**<  Receive Sensitivty Table.*/
	PER_ANTENNA_RECEIVE_SENSITIVITY_RANGE perAntennaReceiveSensitivtyRange; /**< Valid if Reader supports multiple antennas 
																									where the antennas can have different receive sensitivity values.
																									Not supported, may be support in future.*/

	/*  Gen2LLRP Capabilities.*/
	BOOLEAN blockEraseSupported; /**<  Indicates whether Block Erase is supported by the Reader.*/
	BOOLEAN blockWriteSupported; /**<  Indicates whether Block Write is supported by the Reader .*/
	BOOLEAN stateAwareSingulationSupported; /**<  Indicates whether Inventory State Aware Singulation is supported by the Reader.*/
	BOOLEAN recommissionSupported; /**<  Indicates whether Recommission is supported by the Reader.*/
	BOOLEAN blockPermalockSupported; /**<  Indicates whether Block Permalock is supported by the Reader.*/
	BOOLEAN writeUMISupported; /**<  Indicates whether UMI (User Memory Indicator) Write is supported by the Reader.*/
	BOOLEAN radioPowerControlSupported; /**<  Indicates whether radio can be powered ON or OFF.*/
	BOOLEAN radioTransmitDelaySupported; /**<  Indicates whether radio can do transmit delay.*/
	UINT16 maxNumOperationsInAccessSequence; /**<  Maximum Number of OpSpecs allowed per access Filter.*/
	UINT16 maxNumPreFilters;  /**<  Maximum Number of Pre-Filters allowed per antenna.*/
	RF_MODES rfModes;/**<  List of RF Mode tables supported by the Reader.*/

	/*   Regulatory Capabilities.*/
	UINT16	countryCode; /**<  Country Code .*/
	COMMUNICATION_STANDARD	communicationStandard; /**<  Communication Standard .*/

	/*  UHF Band capabilities.*/
	TRANSMIT_POWER_LEVEL_TABLE transmitPowerLevelTable; /**< Transmit Power Level Table .*/

	/*  Frequency Information.*/
	BOOLEAN hoppingEnabled; /**<  Is Frequency hopping enabled.*/
	FREQ_HOP_INFO freqHopInfo; /**<  Frequency Hop information.*/
	FIXED_FREQ_INFO fixedFreqInfo; /**<  Fixed Frequency Information .*/
	
	BOOLEAN tagEventReportingSupported;	/**<	Indicates the reader’s ability to report tag visibility state changes.
												If true, the client can choose the tag event state changes to 
												monitor and report. Event state options are New Tag, Tag Invisible, 
												or Tag Visibility Changed.*/
	BOOLEAN rssiFilterSupported;		/**<	Indicates the reader’s ability to report tags based
												on the signal strength of the back-scattered signal 
												from the tag.*/
	BOOLEAN NXPCommandsSupported; /**< Indicates whether the NXP Commands - ChangeEAS,EASAlarm,SetQuiet,ResetQuiet are supported by the Reader. */
	BOOLEAN tagLocationingSupported;		/**<	Indicates the reader’s ability to locate a tag.*/
	LPDUTY_CYCLE_TABLE lpDutyCycleTable;	/**<	List of DutyCycle percentages supported by the reader.*/
	BOOLEAN FujitsuCommandsSupported;       /**< Indicates whether the reader supports Fujitsu custom commands for 64K and 8K Tags are supported or not*/
	BOOLEAN ImpinjCommandSupported;       /**< Indicates whether the reader supports Impinj QT Commands*/
	LPREADER_MODULE_VERSIONSA lpVersions; /**< List of versions of reader modules.*/
	BOOLEAN SledBatteryStatusSupported;				/**< Sled Battery Status support*/
	BOOLEAN tagListFilterSupported;			/**<	Indicates the reader’s ability to report tags based on the tag list filter.*/
	BOOLEAN PeriodicTagReportsSupported;	/**< Periodic tag reporting is supported */
	BOOLEAN TagPhaseReportingSupported;		/**< Tag Phase information in tag report */
	BOOLEAN ZoneSuppported;				/**< Zone support*/
	BOOLEAN AntennaRFConfigSupported;		/**< Radio RF capabilities - Stop condition, physical port config & SL_ALL & AB_FLIP support */
	BOOLEAN tagMovingStationarySupported;	/**<	Indicates the reader’s ability to support tag status of moving and stationary.
											If true, the client can choose the tag event state changes to
											monitor and report. Event state options are New Tag, Tag Invisible,
											or Tag Visibility Changed.*/
	BOOLEAN ZebraTriggerSupported; /**< extra triggers support, such as timelapse and GPS distance. */
	BOOLEAN G2V2CommandSupported; /**< support G2V2 commands such as Authenticate, ReadBuffer, Untraceable, and Crypto */
	BOOLEAN GPSReportingSupported; /**< Can Report GPS Location */
	BYTE reserved; /**< Reserved fields for future */
	UINT32  reserved32; /**< Reserved fields for future */

	LPVOID lpReserved[24];				/**<  Reserved  for future.*/
}READER_CAPSA, *LPREADER_CAPSA;
#ifdef UNICODE
typedef READER_CAPSW READER_CAPS;
typedef LPREADER_CAPSW LPREADER_CAPS;
#else
typedef READER_CAPSA READER_CAPS;
typedef LPREADER_CAPSA LPREADER_CAPS;
#endif

/** 
* GPI_TRIGGER contains the GPI-Trigger information.
* GPI based trigger can be specified for Start Trigger as well as Stop Trigger.
*/
typedef struct _GPI_TRIGGER
{
	UINT16 portNumber;/**<  GPI Port number on which the trigger is to be enabled.*/
	BOOLEAN gpiEvent;/**<  The Boolean value that causes a GPI event to trigger.*/
	UINT32 timeoutMilliseconds;/**<  Trigger timeout in milliseconds. If set to zero, it indicates there is no timeout.
				   Ignored for START_TRIGGER_TYPE_GPI; Valid only for STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT.*/
}GPI_TRIGGER, *LPGPI_TRIGGER;

/** 
* HANDHELD_TRIGGER contains the Trigger information for Handheld Gun/Button that can Start/Stop an operation, like Inventory, Access, TagLocationing, etc.
* Handheld Gun/Button based trigger can be specified for Start Trigger as well as Stop Trigger.
*/
typedef struct _HANDHELD_TRIGGER
{
	HANDHELD_TRIGGER_EVENT_TYPE handheldEvent;/**<  The HANDHELD_TRIGGER_EVENT_TYPE that causes the operation to start.*/
	UINT32 timeoutMilliseconds;/**<  Trigger timeout in milliseconds. If set to zero, it indicates there is no timeout.
				   Ignored for START_TRIGGER_TYPE_HANDHELD; Valid only for STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT.*/
}HANDHELD_TRIGGER, *LPHANDHELD_TRIGGER;
/**
* TIMEOFDAY specifies the time of Day
*/
typedef struct _TIMEOFDAY {
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
}TIMEOFDAY, *PTIMEOFDAY, *LPTIMEOFDAY;

/**
* TIMELAPSE_START_TRIGGER defines timelapse start trigger, which includes start point of time of day to trigger periodically.
* It is a member of START_TRIGGER.
*/
typedef struct _TIMELAPSE_START_TRIGGER
{
	TIMEOFDAY* startTime; /**< Specific time of day (9 charactors "12:45:78") to start trigger. It's second unit such as “08:32:14” [0~23]:[0~59]:[0:59]. if string is empty, equivalent to “00:00:00”, timeofday is on the mid-night.*/
	UINT32 period; /**< Certain regular interval (seconds, minutes, hours, days). It's a cycle of inventory and idle. If =0, not periodic. Inventory can’t restart after stop.*/
}TIMELAPSE_START_TRIGGER, *LPTIMELAPSE_START_TRIGGER;

/**
* DISTANCE_TRIGGER specifies when to start the Inventory or AccessSequece operation based in GPS information
* It is a member of START_TRIGGER.
*/
typedef struct _DISTANCE_TRIGGER
{
	UINT32 value; /**< Start operation after (value) km or miles comparing last trigger.*/
}DISTANCE_TRIGGER, *LPDISTANCE_TRIGGER;

/**
* TIMELAPSE_STOP_TRIGGER defines timelapse stop trigger, which includes total running duration with inventory periodic duration.
* It is a member of STOP_TRIGGER.
*/
typedef struct _TIMELAPSE_STOP_TRIGGER
{
	UINT32 totalDuration; /**< Specific total running duration. If = 0, no stop in duration. If work with GPI and GPS start trigger, GPI and GPS start trigger should provide similar timeofday set when trigger enable time.*/
	UINT32 periodicDuration; /**< Define the inventory duration inside perodic window. If =0, no stop in inventory.*/
}TIMELAPSE_STOP_TRIGGER, *LPTIMELAPSE_STOP_TRIGGER;

/** 
* TRIGGER_WITH_TIMEOUT  contains information for triggers with timeout
*/
typedef struct _TRIGGER_WITH_TIMEOUT
{
	UINT16 n;/**<  N attempts or N Tag-Observations.*/
	UINT32 timeoutMilliseconds;/**<  For specifying a fail-safe timeout when this trigger is used as a 
						stop trigger. When the timeout is 0, it indicates that there is no 
						timeout.*/
}TRIGGER_WITH_TIMEOUT, *LPTRIGGER_WITH_TIMEOUT;

/**
* PERIODIC_TRIGGER specifies when to start the Inventory or AccessSequece operation 
* and the time period to run the operation. 
* It is a member of START_TRIGGER.
*/
typedef struct _PERIODIC_TRIGGER
{
	UINT32 periodMilliseconds;/**<  Non-zero value indicating the time period in milliseconds for which inventory will run.*/
	SYSTEMTIME* startTime;	/**< UTC Time to start the operation.*/
}PERIODIC_TRIGGER, *LPPERIODIC_TRIGGER;

/** 
* START_TRIGGER specifies the condition for starting  Inventory or AccessSeqeunce operation.
* It is a member of TRIGGER_INFO.
*/
typedef struct _START_TRIGGER
{
	START_TRIGGER_TYPE	type; /**<  Start trigger of type START_TRIGGER_TYPE.*/
	union
	{
		GPI_TRIGGER gpi; /**< To be specified for START_TRIGGER_TYPE_GPI.*/
		PERIODIC_TRIGGER periodic;/**< To be specified for START_TRIGGER_TYPE_PERIODIC.*/
		HANDHELD_TRIGGER handheld; /**< To be specified for START_TRIGGER_TYPE_HANDHELD.*/
		TIMELAPSE_START_TRIGGER timelapse; /**< To be specified for START_TRIGGER_TYPE_TIMELAPSE.*/
		DISTANCE_TRIGGER distance; /**< To be specified for START_TRIGGER_TYPE_DISTANCE.*/
	}value; /**<  Value need not be specified for START_TRIGGER_TYPE_IMMEDIATE.*/
}START_TRIGGER, *LPSTART_TRIGGER;

/**
* STOP_TRIGGER specifies the condition for stopping Inventory or Access-Sequence operation.
* It is a member of TRIGGER_INFO.
*/
typedef struct _STOP_TRIGGER
{
	STOP_TRIGGER_TYPE	type;/**< Stop trigger of type STOP_TRIGGER_TYPE.*/
	union
	{
		UINT32 duration;/**<  Duration in Milli-seconds if type is STOP_TRIGGER_TYPE_DURATION.*/
		TRIGGER_WITH_TIMEOUT tagObservation; /**<  Upon seeing N tag observations, or timeout 
											 if type is STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT.*/
		TRIGGER_WITH_TIMEOUT numAttempts; /**< N attempts to see all tags in the FOV, or timeout 
										  if type is STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT.*/
		GPI_TRIGGER gpi;/**< GPI with a timeout value if type is STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT.*/
		HANDHELD_TRIGGER handheld;/**< Gun/Button Trigger with a timeout value if type is STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT.*/
		TIMELAPSE_STOP_TRIGGER timelapse; /**< Timelapse stop trigger if type is STOP_TRIGGER_TYPE_TIMELAPSE.*/
	}value; /**<  value corresponding to the stop trigger type.*/
}STOP_TRIGGER, *LPSTOP_TRIGGER;

/**
* EXTRA_TRIGGER_INFO specifies extra trigger pair to support multiple triggers.
* It is a member of TRIGGER_INFO.
*/
typedef struct _EXTRA_TRIGGER_INFO
{
	START_TRIGGER startTrigger[MAX_NUM_EXTRA_TRIGGER]; /**< Extra start triggers.*/
	STOP_TRIGGER stopTrigger[MAX_NUM_EXTRA_TRIGGER]; /**< Extra stop triggers.*/
}EXTRA_TRIGGER_INFO, *LPEXTRA_TRIGGER_INFO;

/**
* STOP_TRIGGER_RFSURVEY specifies the condition for stopping RF Survey operation
* It is a member of TRIGGER_INFO.
*/
typedef struct _STOP_TRIGGER_RFSURVEY
{
	STOP_TRIGGER_TYPE	type;/**< Stop trigger of type STOP_TRIGGER_TYPE.*/
	union
	{
		UINT32 duration;/**<  Duration in Milli-seconds if type is STOP_TRIGGER_TYPE_DURATION.*/
		
		UINT32 numAttempts; /**< N attempts to see all rf survey data in the FOV, or timeout
								  if type is STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT.*/
		
	}value; /**<  value corresponding to the stop trigger type.*/
}RFSURVEY_STOP_TRIGGER, *LPRFSURVEY_STOP_TRIGGER;



/**
* TAG_EVENT_REPORT_INFO specifies the configuration of events for the reader to 
* report tag state changes.
* It is a member of TRIGGER_INFO.
*/
typedef struct _TAG_EVENT_REPORT_INFO
{
	TAG_EVENT_REPORT_TRIGGER	reportNewTagEvent;/**<  Report criteria when a new Tag is visible.*/
	UINT16						newTagEventModeratedTimeoutMilliseconds;
								/**< Timeout in milliseconds for moderating new tag event
									reporting. Use this only when reportNewTagEvent is set to 
									MODERATED.*/
	TAG_EVENT_REPORT_TRIGGER	reportTagInvisibleEvent;/**<  Report criteria when a Tag is invisible.*/
	UINT16						tagInvisibleEventModeratedTimeoutMilliseconds;
								/**< Timeout in milliseconds for moderating tag invisible event
									reporting. Use this only when reportTagInvisibleEvent is set to 
									MODERATED.*/
	TAG_EVENT_REPORT_TRIGGER	reportTagBackToVisibilityEvent;/**<  Report criteria when a Tag is back to visibility.*/
	UINT16						tagBackToVisibilityModeratedTimeoutMilliseconds;
								/**< Timeout in milliseconds for moderating tag back to visibility event
									reporting. Use this only when reportTagBackToVisibilityEvent is set to 
									MODERATED.*/
	TAG_MOVING_EVENT_REPORT 	reportTagMovingEvent;/**<  Report criteria when a Tag is in moving.*/
	UINT16						tagStationaryModeratedTimeoutMilliseconds;
								/**< Timeout in milliseconds for moderating tag stationary status transition.
									Use this only when reportTagMovingEvent is enabled.*/

	LPVOID lpReserved[2];				/**<  Reserved for future.*/
}TAG_EVENT_REPORT_INFO, *LPTAG_EVENT_REPORT_INFO;

/**
* REPORT TRIGGER specifies configuration for different type of reporting trigger mechanism
* Currently supports only “periodic trigger report” 
*/

typedef struct _REPORT_TRIGGERS
{
	UINT32 periodicReportDuration;  /**< Duration is in Seconds, Reports the tags periodically if the tag is continued to be seen after the duration has elapsed.
								0 - Report tags after the end of inventory.
								n - Report tag first time it is reader and then after every 'n' seconds (periodically) time interval if the tag is continued to be read.
								The following restrictions are applicable when using a periodic report trigger on the reader.
								1. Access Operation results will not be sent as part of the periodic report trigger and the reader sends an exception event when access operations are added with reader
								configured for periodic reporting of tags.
								2. Stop trigger of type tagObservation 'Upon_Seeing_N_Tags_Or_Timeout' is not supported with the reader configured for periodic reporting of tags.								
								*/
	LPVOID lpReserved[4];
}REPORT_TRIGGERS,*LPREPORT_TRIGGERS;

/**
* TRIGGER_INFO contains trigger criteria to be used for an Inventory or Access Sequence operation.
*/
typedef struct _TRIGGER_INFO
{
	START_TRIGGER startTrigger;/**<  Condition to be met to start the operation.*/
	STOP_TRIGGER stopTrigger;/**<  Condition to be met to stop the operation.*/
	UINT32 tagReportTrigger; 	/**< 	Signifies when to get triggered  on TAG_READ_EVENT.
										0 - Tag reports will be triggered when the Stop-Criteria 
										is met. In case of periodic trigger, reports will 
										be triggerd after every period.
										n - Report when 'n' unique tags are found
										The default settings is 1, which implies to get Tag reports immediately.
										This will be ignored when lpTagEventReportInfo is not NULL.*/
	LPTAG_EVENT_REPORT_INFO lpTagEventReportInfo; /**<  Parameters for enabling and configuring 
												   events for the reader to report tag state changes.*/
	LPREPORT_TRIGGERS lpReportTriggers; /**<  specifies configuration different type of reporting trigger mechanisms.
										  Currently provides support for periodic report trigger and available only in FX7500.  */
	LPEXTRA_TRIGGER_INFO lpExtraTriggerInfo; /**<  extra trigger information.*/
	LPVOID lpReserved[1];				/**<  Reserved for future.
										1 LPVOID consumed for LPTAG_EVENT_REPORT_INFO
										1 LPVOID consumed for PREPORT_TRIGGERS*/
}TRIGGER_INFO, *LPTRIGGER_INFO;

/** 
* TAG_ZONE_INFO provides information about Zone based Operation.
*/
typedef struct _TAG_ZONE_INFOW
{
	UINT16		zoneID;			/**< Zone Id of the Tag Events.*/	
	WCHAR		zoneName[MAX_PATH];		/**< Zone Name of the Tag Events.*/
	LPVOID		lpReserved[4];				/**<  Reserved for future.*/
}TAG_ZONE_INFOW, *LPTAG_ZONE_INFOW;

typedef struct _TAG_ZONE_INFOA
{
	UINT16		zoneID;			/**< Zone Id of the Tag Events.*/	
	CHAR		zoneName[MAX_PATH];		/**< Zone Name of the Tag Events.*/
	LPVOID		lpReserved[4];				/**<  Reserved for future.*/
}TAG_ZONE_INFOA, *LPTAG_ZONE_INFOA;


#ifdef UNICODE
typedef TAG_ZONE_INFOW TAG_ZONE_INFO;
typedef LPTAG_ZONE_INFOW LPTAG_ZONE_INFO;
#else
typedef TAG_ZONE_INFOA TAG_ZONE_INFO;
typedef LPTAG_ZONE_INFOA LPTAG_ZONE_INFO;
#endif

/**
* TAG_DATA contains Tag related information.
* When Inventory or Read Access Operation is performed, Tags reported by Reader 
* are reported back to application in the format of TAG_DATA. 
*/
typedef struct _TAG_DATA
{
	/* Sub-structure describing memory allocated and used by this     
	structure .*/
	_STRUCT_INFO         structInfo;		

	UINT8*		pTagID; /**< Tag ID, for C1G2 this field refers EPC Data.*/	                        
	UINT32		tagIDLength; /**< Tag ID Length (Number of Bytes).*/	
	UINT32		tagIDAllocated; /**<  Memory allocated for Tag ID  (Number of Bytes) .*/
	UINT16		PC; /**< PC BITS.*/
	UINT32		XPC; /** XPC BITS - XPC_W1 = LOWORD(XPC) and XPC_W2 = HIWORD(XPC) .*/
	UINT16		CRC;  /**< CRC.*/
	UINT16		antennaID;/**< Antenna ID .*/
	union
	{
		struct _UP_TIME
		{
			UINT64		firstSeenTimeStamp;	/**< First Seen Time Stamp, Time of the first observation amongst the tag reports .*/
			UINT64		lastSeenTimeStamp; /**< Last Seen Time Stamp,  Time of the last observation amongst the tag reports .*/
		}upTime;/**< seenTime: Since System UP time .*/
		struct _UTC_TIME
		{
			SYSTEMTIME		firstSeenTimeStamp;	/**< First Seen Time Stamp, Time of the first observation amongst the tag reports .*/
			SYSTEMTIME		lastSeenTimeStamp; /**<  Last Seen Time Stamp,  Time of the last observation amongst the tag reports .*/
		}utcTime;/**< seenTime: As System time .*/

	}seenTime;
	INT8		peakRSSI; /*PeakRSSI, Maximum RSSI value observed amongst the tag reports .*/				
	UINT16		channelIndex; /**< Channel Index, Index of the first channel the tag was seen.*/
	UINT16		tagSeenCount; /**< Tag seen count, Number of tag reports.*/
	ACCESS_OPERATION_CODE opCode;/**< C1G2 Operation as a result of which this Tag is being reported.
								 if opCode is ACCESS_OPERATION_NONE, this Tag is a result of Inventory operation.*/
	ACCESS_OPERATION_STATUS opStatus; /**<Result of C1G2 Access Operation, opCode*/
	MEMORY_BANK		memoryBank;/**< The type descriptor describing the Bank type. This field is to be ignored in 
							       case of opCode specified as ACCESS_OPERATION_BLOCK_PERMALOCK.*/
	UINT8*			pMemoryBankData; /**< The data read from the specified Bank or Block Permalock Mask Data
									 as specified by opCode.*/	
	UINT32			memoryBankDataByteOffset; /**< Byte offset of the data.*/	                       
	UINT32			memoryBankDataLength; /**< The length of the data(Number of Bytes) .*/					
	UINT32			memoryBankDataAllocated; /**<  Memory allocated(Number of Bytes).*/
	TAG_EVENT		tagEvent;
	SYSTEMTIME		tagEventTimeStamp; /**< Time Stamp at which the tagEvent occurred. 
									   This field is not relevant when tagEvent is NONE.*/
	LPLOCATION_INFO lpLocation;	/**< location information. It will be present only when RFID_PerformTagLocationing or RFID_PerformInventory was
									invoked using Operation-qualifier LOCATE_TAG.*/
	LPACCESS_OPERATION_RESULT lpAccessOperationResult; /**<  Contains the result of AccessOperation on a particular tag.
													   Applicable only when TagData.OpCode is one of ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK,ACCESS_OPERATION_FUJITSU_BURST_WRITE,
													   ACCESS_OPERATION_FUJITSU_BURST_ERASE, ACCESS_OPERATION_IMPINJ_QT_READ and ACCESS_OPERATION_NXP_CHANGE_CONFIG*/

	UINT16	transmitPort; /**< Transmit port of the Antenna or Transmit port on which the Tag read.*/
	UINT16	receivePort;  /**< Receive port of the Antenna or Transmit port on which the Tag read. */
	LPTAG_ZONE_INFO lpZoneInfo; /**< Zone information on which the Tag is read.*/
	INT16 phaseInfo; /**< Phase information reported by the reader when this tag was seen.*/
	UINT8 brandValid; /**< Status of NXP Brand ID check status. 0 if Brand ID check failed, 1 if success */
	UINT8 reserved;				/**< for future expansion for byte type */
	LPMLT_ALGORITHM_REPORT lpMLTAlogrithmReport;/**< MLT algorithm output parameters. */
	LPGPS_LOCATION lpGPSLocation;
	LPVOID lpReserved[4]; /**<  Reserved for future.
									1 dword for tagEvent.
									4 dwords for tagEventTimeStamp.
									1 dword for locationInfo
									 */
} TAG_DATA, *LPTAG_DATA;


/**
* RF_SURVEY_DATA contains RF Survey related information.
* When RF Survey Operation is performed, RF Survey data reported by Reader
* are reported back to application in the format of RF_SURVEY_DATA.
*/
typedef struct _RF_SURVEY_DATA
{
	/* Sub-structure describing memory allocated and used by this
	structure .*/
	_STRUCT_INFO         structInfo;

	
	
	union
	{
		struct _UP_TIME_RFSURVEY
		{
			UINT64		firstSeenTimeStamp;	/**< First Seen Time Stamp, Time of the first observation amongst the RF survey reports .*/
			
		}upTime;/**< seenTime: Since System UP time .*/
		struct _UTC_TIME_RFSURVEY
		{
			SYSTEMTIME		firstSeenTimeStamp;	/**< First Seen Time Stamp, Time of the first observation amongst the rf survey reports .*/
			
		}utcTime;/**< seenTime: As System time .*/

	}seenTime;
	UINT32		frequency; 
	UINT32		bandWidth;
	INT8		averagePeakRSSI; /*averagePeakRSSI, Maximum RSSI value observed amongst the rf survey reports .*/
	INT8		peakRSSI; /*PeakRSSI, Maximum RSSI value observed amongst the rf survey reports .*/	
	LPVOID lpReserved[4]; /**<  Reserved for future.
						  */
} RF_SURVEY_DATA, *LPRFSURVEY_DATA;



/**
* TAG_STORAGE_SETTINGS is used to specify the storage settings related to Tags
* using RFID_SetTagStorageSettings. Maximum size of the settings are not retricted and could be 
* adjusted as per requirements/constraints of the machine (device/host) on which the
* application is executed.
*/
typedef struct _TAG_STORAGE_SETTINGS
{
	UINT32 maxTagCount;				/**<  Maximum Tag Count maintained in the API internal Queue. */
	UINT32 maxMemoryBankByteCount;	/**<  Maximum Size of Memory Bank in Bytes (Should be WORD aligned, i.e. double bytes).*/
	UINT32 maxTagIDByteCount ;		/**<  Maximum size of EPC Data in Bytes (Should be WORD aligned, i.e. double bytes).*/
	UINT16 tagFields;				/**< Bit field indicating contents of TAG_DATA. 
					  It inturn specifies whether certain fields are required to be processed by the Reader/Dll.
					  Absence of certain fields can improve performance of the Reader/Dll or result in 
					  specific behavior. For e.g. disabling Antenna Id can result in application 
					  receiving as single unique tag even though there were multiple entries of the 
					  same tag reported from different Antennas*/
	
	BOOLEAN enableAccessReports; 		/**< Indicates whether Dll should enable reporting of 
										TAG_DATA for access operations like Write, Lock, Kill, etc. 
										By default, this will be false, which implies reports will be 
										enabled only for Inventory, Read and Block-permalock Access 
										operations.*/
	BOOLEAN  discardTagsOnInventoryStop;		/**<  Option to discard the tags which are reported by the reader
											when inventory/access-sequence operation has been stopped by a call
											to RFID_StopInventory or RFID_StopAccessSequence. Choosing this option 
											will not purge the tags which are already queued by the Dll.
											This option will be ignored if tagReportTrigger of TRIGGER_INFO 
											is not equal to 1.*/
	BOOLEAN enablePreFilters;			/**< To enable prefilter to be used with inventory, by default it is true*/
	BYTE reserved[3];					/**< for future expansions other than pointers types*/		
	LPVOID lpReserved[2];	/**<  Reserved for future.
							1. One DWORD consumed for enableAccessReports, tagFields, discardTagsOnInventoryStop*/
}TAG_STORAGE_SETTINGS, *LPTAG_STORAGE_SETTINGS;



/**
* RFSURVEY_STORAGE_SETTINGS is used to specify the storage settings related to RF Survey
* using RFID_SetRFSurveyStorageSettings. Maximum size of the settings are not retricted and could be
* adjusted as per requirements/constraints of the machine (device/host) on which the
* application is executed.
*/
typedef struct _RFSURVEY_STORAGE_SETTINGS
{
	UINT32 maxRFSurveyCount;				/**<  Maximum RF Survey Data Count maintained in the API internal Queue. */
	UINT32 maxMemoryBankByteCount;	/**<  Maximum Size of Memory Bank in Bytes (Should be WORD aligned, i.e. double bytes).*/
	UINT32 maxRFSurveyIDByteCount;		/**<  Maximum size of EPC Data in Bytes (Should be WORD aligned, i.e. double bytes).*/
	UINT16 rfsurveyFields;				/**< Bit field indicating contents of RF_SURVEY_DATA.
									It inturn specifies whether certain fields are required to be processed by the Reader/Dll.
									Absence of certain fields can improve performance of the Reader/Dll or result in
									specific behavior. For e.g. disabling Antenna Id can result in application
									receiving as single unique tag even though there were multiple entries of the
									same tag reported from different Antennas*/

	BOOLEAN enableAccessReports; 		/**< Indicates whether Dll should enable reporting of
										RF_SURVEY_DATA for access operations like Write, Lock, Kill, etc.
										By default, this will be false, which implies reports will be
										enabled only for RF Survey, Read and Block-permalock Access
										operations.*/
	UINT32 reserved32;				/**< for future expansions other than pointers types*/
	LPVOID lpReserved[2];	/**<  Reserved for future.
								1. One DWORD consumed for enableAccessReports, tagFields, discardTagsOnInventoryStop*/
}RFSURVEY_STORAGE_SETTINGS, *LPRFSURVEY_STORAGE_SETTINGS;




/**
*  DISCONNECTION_EVENT_DATA carries information on reason for  disconnection.
*  This data corresponds to DISCONNECTION_EVENT.
*/
typedef struct _DISCONNECTION_EVENT_DATA
{
	DISCONNECTION_EVENT_TYPE eventInfo;/**< Reason for disconnection.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}DISCONNECTION_EVENT_DATA, *LPDISCONNECTION_EVENT_DATA;


/**
*  READER_EXCEPTION_EVENT_DATA carries information on reason for  reader exception.
*  This data corresponds to READER_EXCEPTION_EVENT.
*/
typedef struct _READER_EXCEPTION_EVENT_DATAW
{	
	READER_EXCEPTION_EVENT_TYPE	exceptionType; /**<  Event indicating the reason for Reader exception. */
	WCHAR			exceptionInfo[MAX_PATH];/**< Description of the exception as reported by the Reader.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}READER_EXCEPTION_EVENT_DATAW, *LPREADER_EXCEPTION_EVENT_DATAW;

typedef struct _READER_EXCEPTION_EVENT_DATAA
{	
	READER_EXCEPTION_EVENT_TYPE	exceptionType; /**<  Event indicating the reason for Reader exception. */	
	CHAR			exceptionInfo[MAX_PATH];/**< Description of the exception as reported by the Reader.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}READER_EXCEPTION_EVENT_DATAA, *LPREADER_EXCEPTION_EVENT_DATAA;

#ifdef UNICODE
typedef READER_EXCEPTION_EVENT_DATAW READER_EXCEPTION_EVENT_DATA;
typedef LPREADER_EXCEPTION_EVENT_DATAW LPREADER_EXCEPTION_EVENT_DATA;
#else
typedef READER_EXCEPTION_EVENT_DATAA READER_EXCEPTION_EVENT_DATA;
typedef LPREADER_EXCEPTION_EVENT_DATAA LPREADER_EXCEPTION_EVENT_DATA;
#endif


/**
*  ANTENNA_EVENT_DATA carries an event triggered on an Antenna.
*  This data corresponds to ANTENNA_EVENT.
*/
typedef struct _ANTENNA_EVENT_DATA
{
	UINT16 id;/**< antennaID: Antenna on which the event was triggered .*/
	ANTENNA_EVENT_TYPE eventInfo;/**< Antenna Event Information which indicates whether the Antenna 
										  is connected/disconnected.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}ANTENNA_EVENT_DATA, *LPANTENNA_EVENT_DATA;

/**
*  GPI_EVENT_DATA carries an event triggered on a GPI Port.
*  This data corresponds to GPI_EVENT.
*/
typedef struct _GPI_EVENT_DATA
{
	UINT16 port;/**< port: GPI port on which the event was triggered .*/
	BOOLEAN eventInfo;/**< GPI Event Information indicating if the GPI Port State was changed from 
							low to high (true) or high to low (false). For Handheld readers, a true indicated, trigger pressed and false
							indicates trigger released.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}GPI_EVENT_DATA, *LPGPI_EVENT_DATA;

/**
*  HANDHELD_TRIGGER_EVENT_DATA carries the event information related to a trigger of the Handheld Gun/Button.
*  This data corresponds to HANDHELD_TRIGGER_EVENT.
*/
typedef struct _HANDHELD_EVENT_DATA
{
	HANDHELD_TRIGGER_EVENT_TYPE eventInfo;/**< Handheld Gun/Button Trigger Event Information.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}HANDHELD_TRIGGER_EVENT_DATA, *LPHANDHELD_EVENT_DATA;

/**
*   NXP_EAS_ALARM_DATA contains data associated with an NXP_EAS_ALARM_EVENT;
*   It contains the antenna ID which "saw" an NXP tag with it's EAS bit set and
*   also the Alarm code returned by the NXP tag during NXP_EAS_SCAN.
*/

typedef struct _NXP_EAS_ALARM_DATA
{
	UINT64 alarmCode; /**< Alarm code returned by the NXP tag  */
	UINT16 antennaID; /**< Antenna ID which "saw" an NXP tag with it's EAS bit set*/
	LPVOID lpReserved[4]; /**< Reserved for future use */
}NXP_EAS_ALARM_DATA,*LPNXP_EAS_ALARM_DATA;

/**
*   DEBUG_INFO_DATA contains data associated with DEBUG_INFO_EVENT;
*   It contains the Debug information sent from the reader
*/
typedef struct _DEBUG_INFO_DATAW
{
	WCHAR  szDebugInfo[MAX_PATH]; /**< Debug Info returned by the reader */
	LPVOID lpReserved[4]; /**< Reserved for future use */
}DEBUG_INFO_DATAW,*LPDEBUG_INFO_DATAW;

typedef struct _DEBUG_INFO_DATAA
{
	CHAR  szDebugInfo[MAX_PATH]; /**< Debug Info returned by the reader */
	LPVOID lpReserved[4]; /**< Reserved for future use */
}DEBUG_INFO_DATAA,*LPDEBUG_INFO_DATAA;



#ifdef UNICODE
typedef DEBUG_INFO_DATAW DEBUG_INFO_DATA;
typedef LPDEBUG_INFO_DATAW LPDEBUG_INFO_DATA;
#else
typedef DEBUG_INFO_DATAA DEBUG_INFO_DATA;
typedef LPDEBUG_INFO_DATAA LPDEBUG_INFO_DATA;
#endif

/**
*   TEMPERATURE_ALARM_DATA contains data associated with an TEMPERATURE_ALARM_EVENT;
*   It contains the PA/Ambient Temperature Status
*/
typedef struct _TEMPERATURE_ALARM_DATA
{
	TEMPERATURE_SOURCE sourceName; /**< Source name of Temperature Alarm Event (For example whether the alarm is for the PA or ambient temperature) */
	UINT16 currentTemperature;  /**< Current Temperature reported by the reader in Celsius */
	ALARM_LEVEL alarmLevel;  /**< Alarm Level (Low, High or Critical)*/
	LPVOID lpReserved[4];				/**<  Reserved for future*/

}TEMPERATURE_ALARM_DATA, *LPTEMPERATURE_ALARM_DATA;

/**
* STATE_AWARE_ACTION_PARAMS specifies the action to be performed on specific Sessions
* on matching and/or non-matching tags.
* It is a member of PRE_FILTER.
*/
typedef struct _STATE_AWARE_ACTION_PARAMS
{
	TARGET target;/**<  Specifies the session on which the filter should perform the action.*/
	STATE_AWARE_ACTION stateAwareAction;/**<  Specifies the action to be performed on the selected session.*/
}STATE_AWARE_ACTION_PARAMS, *LPSTATE_AWARE_ACTION_PARAMS;

/**
* PRE_FILTER contains the filter criteria for Inventory. Maps to one SELECT command of C1G2.
*/
typedef struct _PRE_FILTER
{
	MEMORY_BANK memoryBank;	/**<  Memory Bank on which the filter is to be applied
							MEMORY_BANK_RESERVED is not allowed for Inventory/Pre-Filters.*/
	UINT8* pTagPattern;		/**<The bit pattern against which to compare for tag-filtering, as UINT8*.
							The application should allocate an UINT8 array and assign to this field.*/
	UINT16 tagPatternBitCount;	/**<  Number of bits in pTagPattern to be taken for comparison.*/
	UINT16 bitOffset;		/**<  Bit offset; The first (msb) bit location of the specified memory bank against 
							which to compare the TagMask.*/
	FILTER_ACTION filterAction;
	union
	{
		STATE_AWARE_ACTION_PARAMS stateAwareParams;/**<  Parameters for StateAware action
												   if filterAction is FILTER_ACTION_STATE_AWARE.*/
		STATE_UNAWARE_ACTION stateUnawareAction;/**<  Parameters for StateAware action
												   if filterAction is FILTER_ACTION_STATE_UNAWARE.*/
	}filterActionParams; /**<  This field is ignored for FilterAction: FILTER_ACTION_DEFAULT.*/
	TRUNCATE_ACTION truncateAction; /**< This field indicates whether only a truncated portion of the tag 
										is to be backscattered by the tag or not. The portion that gets 
										backscattered includes the portion of the tag ID following the mask. 
										This field has to be set only in the last pre-filter.*/
	LPVOID lpReserved[3];				/**<  Reserved for future.
										 * 1 DWORD taken for truncateAction*/
}PRE_FILTER, *LPPRE_FILTER;

/**<
* PRE_FILTER_LIST supports addition of multiple pre filters as part of a specific antenna configuration.
*/
typedef struct _PRE_FILTER_LIST
{
	
	UINT32 preFilterCount;/**<  Number of elements in array pAntennaList .*/
	PRE_FILTER * lpPreFilters;		/**<  filter criteria for Inventory*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}PRE_FILTER_LIST, *LPPRE_FILTER_LIST;
/**<
* Contains Antenna stop trigger configuration 
*/
typedef struct _ANTENNA_STOP_TRIGGER
{
	ANTENNA_STOP_TRIGGER_TYPE stopTriggerType; /**< Antenna stop trigger of type ANTENNA_STOP_TRIGGER_TYPE */
	UINT16 stopTriggerValue;	/**<  Stop Trigger Value */
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}ANTENNA_STOP_TRIGGER,*LPANTENNA_STOP_TRIGGER;
/**<
* Contains Antenna RF configuration 
*/
typedef struct _ANTENNA_RF_CONFIG
{

	UINT16 transmitPowerIndex;/**<The value is the zero-based index into the TransmitPowerTable.*/
	UINT16 receiveSensitivityIndex;/**< The value is the zero-based index into the ReceiveSensitivityTable. */
	UINT16 transmitFrequencyIndex;/**< In frequency-hopping regulatory regions, this is the Frequency hop table ID. In case of Fixed Frequency, this is the one-based channel index in the FixedFrequencyTable.*/
	UINT32 rfModeTableIndex;/**<Index to the RF_MODE_TABLE_ENTRY in UHF_RF_MODE_TABLE present in RF_MODES of READER_CAPS */
	UINT32 tari;/**<Tari value. */
	UINT16 transmitPort;/**<Transmit port of the Antenna. transmitPort field should be used only if the reader supports AntennaRFConfigSupported capability*/
	UINT16 receivePort;/**< Receive port of the Antenna.  receivePort field should be used only only if the reader supports AntennaRFConfigSupported capability*/
	ANTENNA_STOP_TRIGGER antennaStopTrigger;/**< Antenna stop trigger configuration. antennaStopTrigger field should be used only if the reader supports AntennaRFConfigSupported capability*/
	LPVOID lpReserved[32]; /**<  Reserved for future.*/
} ANTENNA_RF_CONFIG, *LPANTENNA_RF_CONFIG;


/**<
* This structure exposes a mechanism to provide the full Antenna configuration including the RF Config, pre filters and singulation control that will be used for the specific antenna.
*/
typedef struct _ANTENNA_CONFIG
{
	/* Sub-structure describing memory allocated and Initilaized used by this structure .*/	
	UINT16 antennaID;/**<Specifies the antenna ID */
	LPANTENNA_RF_CONFIG lpAntennaRfConfig;/**<Pointer to lpAntennaRfConfig */
	LPPRE_FILTER_LIST lpPreFilterList;/**<Pointer to lpPreFilterList. */
	LPSINGULATION_CONTROL lpSingulationControl;/**<Pointer lpSingulationControl  */
	LPVOID lpReserved[32];				/**<  Reserved for future.*/
} ANTENNA_CONFIG, *LPANTENNA_CONFIG;

/**<
* ZONE_CONFIG contains the Configuration of a Zone for Inventory.
*/
typedef struct  _ZONE_CONFIGW
{
	WCHAR ZoneName[MAX_PATH];	/**< Name of the Zone */
	UINT32 antennaCount;/**<  Number of elements in array lpAntennaConfiguartions .*/
	ANTENNA_CONFIG * lpAntennaConfigurations;		/**<  Array of Antenna configuarions of the Antennas in the Zone*/
	LPANTENNA_CONFIG   lpZoneGlobalAntennaConfiguration;   /**<  Pointer to Global Antenna configurations  of all the Antennas in the Zone*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
										 
}ZONE_CONFIGW, *LPZONE_CONFIGW;

typedef struct  _ZONE_CONFIGA
{
	CHAR ZoneName[MAX_PATH];	/**< Name of the Zone */
	UINT32 antennaCount;/**<  Number of elements in array lpAntennaConfiguartions .*/
	ANTENNA_CONFIG * lpAntennaConfigurations;		/**<  Array of Antenna configuarions of the Antennas in the Zone*/
	LPANTENNA_CONFIG   lpZoneGlobalAntennaConfiguration;   /**<  Pointer to Global Antenna configurations  of all the Antennas in the Zone*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
										 
}ZONE_CONFIGA, *LPZONE_CONFIGA;

#ifdef UNICODE
typedef ZONE_CONFIGW ZONE_CONFIG;
typedef LPZONE_CONFIGW LPZONE_CONFIG;
#else
typedef ZONE_CONFIGA ZONE_CONFIG;
typedef LPZONE_CONFIGA LPZONE_CONFIG;
#endif

/**<
* Contains Antenna Information to be used for Zone Inventory or Access Operations
*/
typedef struct _ZONE_INFO
{
	UINT32 zoneID;

	UINT16*  pAntennaList;/**<  Array for holding the Antennas to be used for the operation:
								 Range: 1 - N (N = Max. No of Antenna Supported).*/
	UINT32 antennaListLength;/**<  Number of elements in array pAntennaList .*/
    
	OPERATION_QUALIFIER zoneOperation; /** Array of operations for each antenna ID specified in the pAntennaList.
											 Operation at nth Index of this array is meant for the antenna ID at nth Index in the pAntennaList Array.
											 Should be null for running a Gen2 operation on all the antenna in the pAntennaList
											 It is invalid to set operation as NXP_EAS_SCAN, if this structure is being used in any Access API  or
											 RFID_PerformAccessSequence Call*/
	LPVOID lpReserved[4];	/**<  Reserved for future.*/
}ZONE_INFO, *LPZONE_INFO;

/**
* DEBUG_INFO_PARAMS holds Debug Mask and Length
*/
typedef struct _DEBUG_INFO_PARAMS
{
	UINT16 byteOffset;		/**<  Bit offset. The address of the first (msb) bit against which to apply the 
							Debug Info Mask and compare with the value.*/
	UINT8* pDebugMask;		/**<Bit Mask to be matched 
							The application should allocate an UINT8 array and assign it to this field. 
							The Debuf Info Mask to be used for bit-wise comparison should be filled 
							in the array.*/

	UINT16 debugMaskByteCount;		/**< Number of bits in pDebudMask to be taken for comparison.*/
	LPVOID lpReserved[4];			/**<  Reserved for future.*/
}DEBUG_INFO_PARAMS, *LPDEBUG_INFO_PARAMS;


/**<
* Contains Zone sequence Information to be used for Zone Inventory or Access Operations
*/
typedef struct _ZONE_SEQUENCE
{

	UINT32 zoneListCount;/**<  Number of elements in array pAntennaList .*/

	LPZONE_INFO	 pZoneList;/**<  Array for holding the zone information to be used for the operation:
								 Range: 1 - N (N = Max. No of Antenna Supported).*/

    
	LPVOID lpReserved[4];	/**<  Reserved for future.*/
}ZONE_SEQUENCE, *LPZONE_SEQUENCE;


/**
* TAG_PATTERN holds Tag Pattern criteria  for Filter Operation. 
* If bit i in the mask (pTagMask) is zero, then bit i of the target 
* tag (pTagPattern)  is a don’t care (X); if bit i in the mask is one, 
* then bit i of the target tag is bit i of the tag pattern. 
* For example, “all tags” is specified using a mask length of zero.
*/
typedef struct _TAG_PATTERN
{
	MEMORY_BANK memoryBank;  /**<  Memory Bank to be considered for filtering.*/
	UINT16 bitOffset;		/**<  Bit offset. The address of the first (msb) bit against which to apply the 
							Tag Mask and compare with the value.*/
	UINT8* pTagPattern;		/**<Memory Bank Data which is to be used for comparison.
							The application should allocate an UINT8 array and assign to this field. 
							The Memory Bank Data to be used for comparison should be 
							filled in the array.*/
	UINT16 tagPatternBitCount; /**< Number of bits in pTagPattern to be taken for comparison.*/
	UINT8* pTagMask;		/**<Bit Mask to be matched 
							The application should allocate an UINT8 array and assign it to this field. 
							The Tag Mask to be used for bit-wise comparison should be filled 
							in the array.*/

	UINT16 tagMaskBitCount;		/**< Number of bits in pTagMask to be taken for comparison.*/
	LPVOID lpReserved[4];			/**<  Reserved for future.*/
}TAG_PATTERN, *LPTAG_PATTERN;

/** 
* RSSI_RANGE_FILTER is used to configure filter settings on the reader to filter tags based on the 
* received signal strength of the tag, the time the tag was read, or both.
*/
typedef struct _RSSI_RANGE_FILTER
{
	INT8 peakRSSILowerLimit; /* Peak RSSI Lower Limit */
	INT8 peakRSSIUpperLimit; /* Peak RSSI Upper Limit */
	MATCH_RANGE matchRange; /* Match Range */
	LPVOID lpReserved[4];			/**<  Reserved for future.*/
}RSSI_RANGE_FILTER, *LPRSSI_RANGE_FILTER;

/**
* TAG_LIST_FILTER is used to configure filter settings on the reader to filter tags based on the tag list
*/
typedef struct _TAG_LIST_FILTER
{
	MATCH_TAG_LIST	matchTagList;					/**<	Match Tag List.*/
	UINT32			tagCount;						/**<	Tag count in the list*/
	LPTAG_DATA		tagList[MAX_TAGS_IN_FILTER];	/**<	tag list used in filter */
}TAG_LIST_FILTER, *LPTAG_LIST_FILTER;

/** 
* POST_FILTER holds the Tag Filter criteria  for Inventory Operation. 
* Each Tag will be checked against both pattern A and pattern B, as per the matchPattern condition.
*/
typedef struct _POST_FILTER
{
	/* Sub-structure describing memory allocated and used by this     
	structure .*/
	_STRUCT_INFO         structInfo;

	LPTAG_PATTERN lpTagPatternA;	/**<	Tag Pattern A to be used for comparison. 
											The application should allocate new TAG_PATTERN and assign this field to 
											the allocated pointer.*/
	LPTAG_PATTERN lpTagPatternB;	/**<	Tag Pattern B to be used for comparison. If using only one filter, 
											this pointer should be assigned to NULL.*/
	MATCH_PATTERN  matchPattern;	/**<	Match criteria to be used for filtering using Tag
											patterns A and B.*/ 
	
	LPRSSI_RANGE_FILTER lpRSSIRangeFilter;	/**< Pointer to RSSI Range Filter to be used.*/
	LPTAG_LIST_FILTER lpTagListFilter;		/**< Pointer to Tag List Filter to be used.*/
	LPVOID lpReserved[14];				/**<	Reserved for future.
												1 - Taken for lpRSSIRangeFilter. */
}POST_FILTER, *LPPOST_FILTER;


/** 
* ACCESS_FILTER holds the Tag Filter criteria  for Access Operation. 
* Each Tag will be checked against both pattern A and pattern B, as per the matchPattern condition.
*/
typedef struct _ACCESS_FILTER
{
	/* Sub-structure describing memory allocated and used by this     
	structure .*/
	_STRUCT_INFO         structInfo;

	LPTAG_PATTERN lpTagPatternA; /**< Tag Pattern A to be used for comparison. 
										  The application should allocate new TAG_PATTERN and assign this field to 
										  the allocated pointer.*/
	LPTAG_PATTERN lpTagPatternB;/**< Tag Pattern B to be used for comparison. If using only one filter, this pointer should be assigned to NULL.*/
	MATCH_PATTERN  matchPattern; /**< Match criteria to be used for filtering using Tag patterns A and B.*/
	
	LPRSSI_RANGE_FILTER lpRSSIRangeFilter;	/**< Pointer to RSSI Range Filter to be used.*/
	LPVOID lpReserved[15];				/**<	Reserved for future.
												1 - Taken for lpRSSIRangeFilter. */
}ACCESS_FILTER, *LPACCESS_FILTER;


/**
* READ_ACCESS_PARAMS contains the parameters for Read Access Operation.
*/
typedef struct _READ_ACCESS_PARAMS
{
	MEMORY_BANK			memoryBank;/**< Memory bank from which data is to be read from.*/
	UINT16				byteOffset;/**< Byte offset; The addresss of the first byte to read from the chosen memory bank.
							   Since Access operation is WORD aligned, offset should be in double bytes.*/
	UINT16				byteCount;/**< Number of bytes to read.
							   Since Access operation is WORD aligned, offset be in double bytes.*/
	UINT32				accessPassword;/**< Password to be used for the Access operation.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}READ_ACCESS_PARAMS, *LPREAD_ACCESS_PARAMS;

/**
*  WRITE_ACCESS_PARAMS contains the parameters for Write Access Operation.
*/
typedef struct _WRITE_ACCESS_PARAMS
{
	MEMORY_BANK	memoryBank;/**< Memory bank on which data is to be written to.*/
	UINT16				byteOffset;/**< Byte offset; Addresss of the first byte to be written to the chosen memory bank.
							   Since Access operation is WORD aligned, offset be in double bytes.*/
	UINT8*				pWriteData;/**< Pointer to a UINT8 array which contains the data to be written. 
										  Write operation occurs only on word boundaries, hence it is required to 
										  input this field accordingly. For e.g The word 0x1122 will be UINT8 writeData[2] = {0x11, 0x22}.*/
	UINT16				writeDataLength;/**< Length of the array pWriteData.*/
	UINT32				accessPassword;/**< Password to be used for the Access operation.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}WRITE_ACCESS_PARAMS, *LPWRITE_ACCESS_PARAMS;

/**
*  WRITE_SPECIFIC_FIELD_ACCESS_PARAMS contains the parameters for Write Access Operation.
*/
typedef struct _WRITE_SPECIFIC_FIELD_ACCESS_PARAMS
{
	UINT8*				pWriteData;/**< Pointer to a UINT8 array which contains the data to be written. 
										  Write operation occurs only on word boundaries, hence it is required to 
										  input this field accordingly. For e.g The word 0x1122 will be UINT8 writeData[2] = {0x11, 0x22}.*/
	UINT16				writeDataLength;/**< Length of the array pWriteData.*/
	UINT32				accessPassword;/**< Password to be used for the Access operation.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}WRITE_SPECIFIC_FIELD_ACCESS_PARAMS, *LPWRITE_SPECIFIC_FIELD_ACCESS_PARAMS;
/**
*  KILL_ACCESS_PARAMS contains the parameters for Kill Access Operation, which is Kill-Password.
*/
typedef  struct _KILL_ACCESS_PARAMS
{
	UINT32 killPassword;/**<  Kill Password. If it is zero, LLRP Reader will return zero-password-error.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}KILL_ACCESS_PARAMS, *LPKILL_ACCESS_PARAMS;

/**
*  LOCK_ACCESS_PARAMS contains the parameters for Lock Access Operation.
*/
typedef struct _LOCK_ACCESS_PARAMS
{
	LOCK_PRIVILEGE		privilege[NUM_LOCK_DATA_FIELDS]; /**< Lock privilege for each memory bank. For setting the index use LOCK_DATA_FIELD.
														Assign privilege only for those memory banks whose 
														privilege is to be changed.*/
	UINT32				accessPassword; /**< Lock Password.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}LOCK_ACCESS_PARAMS, *LPLOCK_ACCESS_PARAMS;

/**
*  BLOCK_ERASE_ACCESS_PARAMS contains the parameters for Block-Erase Access Operation.
*/
typedef struct _BLOCK_ERASE_ACCESS_PARAMS
{
	MEMORY_BANK	memoryBank; /**< Memory bank on which access operation is to be performed.*/
	UINT16				byteOffset; /**< Byte offset; The address of first byte to be erased.
							   Since Access operation is WORD aligned, offset be in double bytes.*/
	UINT16				byteCount; /**< Number of Bytes to be erased.
							   Since Access operation is WORD aligned, offset be in double bytes.*/
	UINT32				accessPassword; /**< Password to be used for the Access operation.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}BLOCK_ERASE_ACCESS_PARAMS, *LPBLOCK_ERASE_ACCESS_PARAMS;

/**
*  RECOMMISSION_ACCESS_PARAMS contains the parameters for Recommission Access Operation.
*/
typedef struct _RECOMMISSION_ACCESS_PARAMS
{
	UINT32						killPassword;	/**<  Kill Password. If it is zero, LLRP Reader will return zero-password-error.*/
	RECOMMISSION_OPERATION_CODE	opCode;			/** Recommission operation code.*/
	LPVOID lpReserved[4];							/**<  Reserved for future.*/
}RECOMMISSION_ACCESS_PARAMS, *LPRECOMMISSION_ACCESS_PARAMS;

/**
*  BLOCK_PERMALOCK_ACCESS_PARAMS contains the parameters for BlockPermalock Access Operation.
*/
typedef struct _BLOCK_PERMALOCK_ACCESS_PARAMS
{
	MEMORY_BANK		memoryBank; /**< Memory bank on which access operation is to be performed.*/
	BOOLEAN			readLock;	/**< ReadLock specifies whether a tag backscatters the perlock status
								of, or permalocks, one of more blocks within the memory bank as specified.
								when readLock is true, it performs block perma lock in specified memory bank starting at offset.
								When readLock is false, the Reader get the permalock mask.*/
	UINT16			byteOffset; /**< Byte offset; Address of the first byte for the Mask.*/
	UINT16			byteCount; /**< Specifies the range of Mask, starting from byteOffset and ending (16xBlockRange)-1 blocks later. This is valid only when readLock is false. i.e. for read operation*/
	UINT32			accessPassword;/**< Password to be used for the Access operation.*/
	UINT8*			pMask; /**<Mask specifies which memory blocks a tag permalocks. Masks depends on the
							readLock as follows:
							When readLock is false, the Reader omits permalock mask.								
							when readLock is true, the Reader should permalock as specified by the Mask.
							The application should allocate an UINT8 array and assign it to this field. 
							The Mask to be used for bit-wise comparison should be filled 
							in the array.*/
	UINT16			maskLength; /**< Number of bytes in pMask to be taken for comparison.*/
	LPVOID lpReserved[4];			/**<  Reserved for future.*/
}BLOCK_PERMALOCK_ACCESS_PARAMS, *LPBLOCK_PERMALOCK_ACCESS_PARAMS;

/**
*  AUTHENTICATE_ACCESS_PARAMS contains the parameters for Authenticate Access Operation.
*/
typedef struct _AUTHENTICATE_ACCESS_PARAMS
{
	BOOLEAN			senResp;		/**< SenResp; specifies whether a Tag backscatters its response or stores the response in its ResponseBuffer. FALSE - Store, TRUE - Send*/
	BOOLEAN			incRespLen;		/**< IncRespLen; specifies whether a Tag omits or includes length in its reply. FALSE - Omit, TRUE - Include*/
	UINT8			CSI;			/**< CSI; selects the cryptographic suite that Tag and Interrogator use for the authentication as well as for all subsequent communications.*/
	UINT16			length;			/**< Length; message length in bits.*/
	UINT8*			pMessage;		/**< pMessage; parameters for the authentication.*/
	UINT32			accessPassword;	/**< Password to be used for the Access operation.*/
	LPVOID			lpReserved[4];	/**<  Reserved for future.*/
}AUTHENTICATE_ACCESS_PARAMS, *LPAUTHENTICATE_ACCESS_PARAMS;

/**
*  READBUFFER_ACCESS_PARAMS contains the parameters for Readbuffer Access Operation.
*/
typedef struct _READBUFFER_ACCESS_PARAMS
{
	UINT16			wordPtr;		/**< WordPtr; Word Pointer spicifies the starting address for the read.*/
	UINT16			bitCount;		/**< BitCount; Bit Count specifying the  umber of bits to read.*/
	UINT32			accessPassword;	/**< Password to be used for the Access operation.*/
	LPVOID			lpReserved[4];	/**<  Reserved for future.*/
}READBUFFER_ACCESS_PARAMS, *LPREADBUFFER_ACCESS_PARAMS;

/**
*  UNTRACEABLE_ACCESS_PARAMS contains the parameters for Untraceable Access Operation.
*/
typedef struct _UNTRACEABLE_ACCESS_PARAMS
{
	BOOLEAN				assertU;		/**< U; specifies a value for the U bit in XPC_W1. FALSE - Deassert, TRUE - Assert*/
	BOOLEAN				hideEPC;		/**< EPC; show/hide EPC field. FALSE: SHOW, TRUE: HIDE*/
	UINT8				EPCLength;		/**< EPCLength; Specifies new EPC length field*/
	TID_HIDE_STATE		hideTID;		/**< TID; specifies the TID memory that a Tag untraceably hides.*/
	BOOLEAN				hideUser;		/**< User; specifies whether a Tag untraceably hides User memory. FALSE: SHOW, TRUE: HIDE*/
	TAG_OPERATING_RANGE	operatingRange;	/**< Range; pecifies a Tag’s operating range.*/
	UINT32				accessPassword;		/**< Password to be used for the Access operation.*/
	LPVOID				lpReserved[4];	/**< Reserved for future.*/
}UNTRACEABLE_ACCESS_PARAMS, *LPUNTRACEABLE_ACCESS_PARAMS;

/**
*  CRYPTO_ACCESS_PARAMS contains the parameters for Crypto Access Operation.
*/
typedef struct _CRYPTO_ACCESS_PARAMS
{
	UINT8			keyID;				/**< KeyID; used by the tag crypto algorithm in its response.*/
	UINT32			IChallenge[3];		/**< IChallenge; an array of 3-32 bits words used by crypto.*/
	BOOLEAN			customData;			/**< CustomData; FALSE: indicates no custom data. ONLY tag authentication. TRUE: indicates that data will be included in response.*/
	UINT8			profile;			/**< Profile; 4-bit pointer that selects a memory profile for the addition of custom data. Values above 15 will be return an error.*/
	UINT8			offset;				/**< Offset; specifies a 12-bit offset (in multiple of 64-bit blocks) that needs to be added to the address that is specified by Profile. Values above 4095 will return an error.*/
	UINT8			blockCount;			/**< BlockCount; 4-bit number to define the size of the customer data as a number of 64-bit blocks. Values above 15 will return an error.*/
	UINT8			protMode;			/**< ProtMode; 4-bit value to select the mode of operation that shall be used to process the custom data. Values above 15 will return an error.*/
	UINT32			accessPassword;		/**< Password to be used for the Access operation.*/
	LPVOID			lpReserved[4];		/**< Reserved for future.*/
}CRYPTO_ACCESS_PARAMS, *LPCRYPTO_ACCESS_PARAMS;

/**
*  OP_CODE_PARAMS contains the parameters for Operation Sequence.
*/
typedef struct _OP_CODE_PARAMS
{
	ACCESS_OPERATION_CODE opCode; /**<  C1G2 Access Operation to be performed.*/
	STRUCT_HANDLE opParams; /**<  Pointer to READ_ACCESS_PARAMS or WRITE_ACCESS_PARAMS or 
									KILL_ACCESS_PARAMS or LOCK_ACCESS_PARAMS or BLOCK_ERASE_ACCESS_PARAMS.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}OP_CODE_PARAMS, *LPOP_CODE_PARAMS;


/**
* READER_STATS contains RFID Reader Statistics
*/
typedef struct _READER_STATS
{
	UINT32 identifiedSuccessCount; /**<  The number of successful tags that have been identified across an AntennaReadPoint.*/
	UINT32 identifiedFailureCount; /**<  The number of number of the failed tag identification attempts at the AntennaReadPoint.*/
	UINT32 readSuccessCount; /**<  The number of successful number of tag memory reads at the AntennaReadPoint.*/
	UINT32 readFailureCount; /**<  The number of the failed tag memory reads at the AntennaReadPoint.*/
	UINT32 writeSuccessCount; /**<  The number of successful tag memory writes at the AntennaReadPoint.*/
	UINT32 writeFailureCount; /**<  The number of the failed tag memory writes at the AntennaReadPoint.*/
	UINT32 killSuccessCount; /**<  The number of tags successfully killed at the AntennaReadPoint.*/
	UINT32 killFailureCount; /**<  The number of the failed tag kills at the AntennaReadPoint.*/
	UINT32 lockSuccessCount; /**<  The number of tags successfully locked at the AntennaReadPoint.*/
	UINT32 lockFailureCount; /**<  The number of the failed tag locked at the AntennaReadPoint.*/

	UINT32 blockWriteSuccessCount; /**<  The number successful tag memory block-writes  at the AntennaReadPoint.*/
	UINT32 blockWriteFailureCount; /**<  The number of the failed tag block-writes at the AntennaReadPoint.*/
	UINT32 blockEraseSuccessCount; /**<  The number of tags successfully block-erased at the AntennaReadPoint.*/
	UINT32 blockEraseFailureCount; /**<  The number of the failed tag block-erase at the AntennaReadPoint.*/
	UINT32 blockPermalockSuccessCount; /**<  The number of tags successfully block-permalocked at the AntennaReadPoint.*/
	UINT32 blockPermalockFailureCount; /**<  The number of the failed tag block-permalock at the AntennaReadPoint.*/

	UINT32 nxpChangeEASSuccessCount; /**<  The number successful NXP Tag operation - Change EAS  at the AntennaReadPoint.*/
	UINT32 nxpChangeEASFailureCount; /**<  The number of the failed NXP Tag operation - Change EAS   at the AntennaReadPoint.*/
	UINT32 nxpEASAlarmSuccessCount; /**<  The number successful NXP Tag operation - EAS Alarm  at the AntennaReadPoint.*/
	UINT32 nxpEASAlarmFailureCount; /**<  The number of the failed NXP Tag operation - EAS Alarm   at the AntennaReadPoint.*/
	UINT32 nxpReadProtectSuccessCount; /**<  The number successful NXP Tag operation - Read Protect  at the AntennaReadPoint.*/
	UINT32 nxpReadProtectFailureCount; /**<  The number of the failed NXP Tag operation - Read Protect   at the AntennaReadPoint.*/
	UINT32 nxpResetReadProtectSuccessCount; /**<  The number successful NXP Tag operation - Reset Read Protect  at the AntennaReadPoint.*/
	UINT32 nxpResetReadProtectFailureCount; /**<  The number of the failed NXP Tag operation - Reset Read Protect   at the AntennaReadPoint.*/
	UINT32 nxpCalibrateSuccessCount; /**<  The number successful NXP Tag operation - Calibrate at the AntennaReadPoint.*/
	UINT32 nxpCalibrateFailureCount; /**<  The number of the failed NXP Tag operation - Calibrate at the AntennaReadPoint.*/
	UINT32 nxpchangeConfigSuccessCount; /**<  The number of the successful NXP Tag operation - Change Config at the AntennaReadPoint.*/
	UINT32 nxpchangeConfigFailureCount; /**<  The number of the failed NXP Tag operation - Change Config at the AntennaReadPoint.*/
    
    UINT32 fujitsuChangeWordLockSuccessCount; /**< The number successful Fujitsu 64K Tag operations - ChangeWordLock  at the AntennaReadPoint*/
    UINT32 fujitsuChangeWordLockFailureCount;/**<  The number of the failed Fujitsu 64K Tag operations -  ChangeWordLock  at the AntennaReadPoint*/
    UINT32 fujitsuChangeBlockLockSuccessCount;/**< The number successful Fujitsu 64K Tag operations - ChangeBlockLock at the AntennaReadPoint*/
    UINT32 fujitsuChangeBlockLockFailureCount;/**< The number of the failed Fujitsu 64K Tag operations- ChangeBlockLock at the AntennaReadPoint */
    UINT32 fujitsuReadBlockLockSuccessCount;/**<   The number successful Fujitsu 64K Tag operations - ReadBlockLock   at the AntennaReadPoint*/
    UINT32 fujitsuReadBlockLockFailureCount;/**<   The number of the failed Fujitsu 64K Tag operations-  ReadBlockLock  at the AntennaReadPoint*/
    UINT32 fujitsuChangeBlockOrAreaGroupPasswordSuccessCount;/**<  The number successful Fujitsu 64K Tag operations- ChangeBlockOrAreaGroupPassword  at the AntennaReadPoint */
    UINT32 fujitsuChangeBlockOrAreaGroupPasswordFailureCount;/**< The number of the failed Fujitsu 64K Tag operations - ChangeBlockOrAreaGroupPassword  at the AntennaReadPoint.*/
    UINT32 fujitsuBurstWriteSuccessCount;/**< The number of the successful Fujitsu 64K Tag operation - Burst Write at the AntennaReadPoint*/
    UINT32 fujitsuBurstWriteFailureCount;/**< The number of failed Fujitsu 64K Tag operations   -  Burst Write  at the AntennaReadPoint*/
    UINT32 fujitsuBurstEraseSuccessCount;/**< The number successful Fujitsu 64K Tag operations  -  Burst Erase  at the AntennaReadPoint*/
    UINT32 fujitsuBurstEraseFailureCount;/**< The number of failed Fujitsu 64K Tag operations   -  Burst Erase  at the AntennaReadPoint*/
    UINT32 fujitsuAreaReadLockSuccessCount;/**< The number successful Fujitsu 8K Tag operations - AreaReadLock  at the AntennaReadPoint. */
    UINT32 fujitsuAreaReadLockFailureCount;/**< The number of the failed Fujitsu 8K Tag operation - AreaReadLock  at the AntennaReadPoint.*/
    UINT32 fujitsuAreaWriteLockSuccessCount;/**< The number successful Fujitsu 8K Tag operations -  AreaWriteLock  at the AntennaReadPoint */
    UINT32 fujitsuAreaWriteLockFailureCount;/**< The number of the failed Fujitsu 8K Tag operations - AreaWriteLock  at the AntennaReadPoint.*/
    UINT32 fujitsuAreaWriteLockWOPasswordSuccessCount;/**<  The number of successful Fujitsu 8K Tag operations - AreaWriteLockWOPassword  at the AntennaReadPoint*/
    UINT32 fujitsuAreaWriteLockWOPasswordFailureCount;/**< The number of the failed Fujitsu 8K Tag operations - AreaWriteLockWOPassword at the AntennaReadPoint*/
	UINT32 impinjQTOperationSuccessCount;/**< The number of the success Impinj QT Tag operations - QT Read or Write at the AntennaReadPoint*/
	UINT32 impinjQTOperationFailureCount;/**< The number of the failed Impinj QT Tag operations - QT Read or Write at the AntennaReadPoint*/
	
	UINT32 ambientTemperatureHighAlarmCount;/**< Get the number of the Ambient Temperature High Alarm events */
	UINT32 ambientTemperatureCriticalAlarmCount;/**< Get the number of the Ambient Temperature Critical Alarm events */
	UINT32 paTemperatureHighAlarmCount; /**< Get the number of the PA Temperature High Alarm events */
	UINT32 paTemperatureCriticalAlarmCount;/**< Get the number of the PA Temperature Critical Alarm events */
	UINT32 forwardPowerHighAlarmCount;/**< Get the number of the Forward Power High Alarm events */
	UINT32 forwardPowerLowAlarmCount;/**< Get the number of the Forward Power Low Alarm events */
	UINT32 reversePowerHighAlarmCount;/**< Get the number of the Reverse Power High Alarm events */
	UINT32 echoThresholdAlarmCount;/**< Get the number of the Echo Threshold Alarm events */
	UINT32 databaseWarningCount;/**< Get the number of the database warning events */
	UINT32 databaseErrorCount;/**< Get the number of the database Error events */
	UINT32 gpioInformationCount;/**< Get the number of the GPIO Information events */

	UINT32 reserved32[37];				/**< for future expansions other than pointers types. 2 are used for QT and 2 for changeconfig*/
	LPVOID lpReserved[16];				/**<  Reserved for future.*/
}READER_STATS, *LPREADER_STATS;

/**
* VERSION_INFO contains RFID Version Info
*/
typedef struct _VERSION_INFOW
{
	WCHAR			dllVersion[MAX_PATH];/**<  Specifies the version of RFIDAPI3 C Dll.*/

}VERSION_INFOW, *LPVERSION_INFOW;

typedef struct _VERSION_INFOA
{
	CHAR			dllVersion[MAX_PATH];/**<  Specifies the version of RFIDAPI3 C Dll.*/

}VERSION_INFOA, *LPVERSION_INFOA;

#ifdef UNICODE
typedef VERSION_INFOW VERSION_INFO;
typedef LPVERSION_INFOW LPVERSION_INFO;
#else
typedef VERSION_INFOA VERSION_INFO;
typedef LPVERSION_INFOA LPVERSION_INFO;
#endif

/**
* LOGIN_INFO contains the Login information for logging into the reader.
*/
typedef struct _LOGIN_INFOW
{
	WCHAR hostName[MAX_PATH];/**< Reader IP or Reader Host Name*/
	WCHAR userName[MAX_PATH]; /**< User Name*/
	WCHAR password[MAX_PATH]; /**< Password*/
	RFID_VERSION version;			/**<  RFIDAPI version to be used*/
	BOOLEAN forceLogin;/**<  Force Login this host. This setting is valid only for Merlin version 1.1.0 onwards. 
							To work with FX readers of version lesser than 1.1.0, which doesn't 
							support Forcelogin feature, give this value as false.
					   */
	BYTE reserved1[3];				/**<  3 bytes left after using forceLogin*/
	LPVOID lpReserved[2];				/**<  Reserved for future.
										DWORD 1 - Used for version,
										DWORD 2 - forceLogin*/
}LOGIN_INFOW, *LPLOGIN_INFOW;


/**
* FTP-Server Or Location details for firmware update.
*/
typedef struct _FTPSERVER_INFOW
{
	WCHAR hostName[MAX_PATH];/**< Location of files for Software Update. This also supports update from secure FTP (FTPS) and SCP.
							 E.g. Software available at FTP location: "ftp://157.235.88.198/pub/XR400/TestRelease/Rev_3_3_7" or 
							 Software available locally: "C:/XR400/TestRelease/Rev_3_3_7".*/
	WCHAR userName[MAX_PATH]; /**< If Software available at FTP/SCP location, provide Username for FTP/SCP Server respectively.*/
	WCHAR password[MAX_PATH]; /**< If Software available at FTP/SCP location, provide Password for FTP/SCP Server respectively.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}FTPSERVER_INFOW, *LPFTPSERVER_INFOW;

typedef struct _LOGIN_INFOA
{
	CHAR hostName[MAX_PATH];
	CHAR userName[MAX_PATH];
	CHAR password[MAX_PATH];
	RFID_VERSION version;	
	BOOLEAN forceLogin;
	BYTE reserved1[3];
	LPVOID lpReserved[2];
}LOGIN_INFOA, *LPLOGIN_INFOA;

typedef struct _FTPSERVER_INFOA
{
	CHAR hostName[MAX_PATH];
	CHAR userName[MAX_PATH];
	CHAR password[MAX_PATH];
	LPVOID lpReserved[4];				/**<  Reserved for future.
										1 - Used for version*/
}FTPSERVER_INFOA, *LPFTPSERVER_INFOA;

#ifdef UNICODE
typedef LOGIN_INFOW LOGIN_INFO;
typedef LPLOGIN_INFOW LPLOGIN_INFO;
typedef LPFTPSERVER_INFOW LPFTPSERVER_INFO;
typedef FTPSERVER_INFOW FTPSERVER_INFO;
#else
typedef LOGIN_INFOA LOGIN_INFO;
typedef LPLOGIN_INFOA LPLOGIN_INFO;
typedef LPFTPSERVER_INFOA LPFTPSERVER_INFO;
typedef FTPSERVER_INFOA FTPSERVER_INFO;
#endif
/**
* Gives information on the Firmware/Software update status.
*/
typedef struct _UPDATE_STATUSW
{
	UINT32 percentage; /**< Percentage of update done.*/
	WCHAR  updateInfo[MAX_PATH]; /**< Additional information like 
								 partition on which the update is being done currently and/or 
								 operation currently being performed at reader.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}UPDATE_STATUSW, *LPUPDATE_STATUSW;

typedef struct _UPDATE_STATUSA
{
	UINT32 percentage; /**< Percentage of update done.*/
	CHAR  updateInfo[MAX_PATH]; /**< Partition on which the update is being done currently.*/
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}UPDATE_STATUSA, *LPUPDATE_STATUSA;

#ifdef UNICODE
typedef UPDATE_STATUSW UPDATE_STATUS;
typedef LPUPDATE_STATUSW LPUPDATE_STATUS;
#else
typedef UPDATE_STATUSA UPDATE_STATUS;
typedef LPUPDATE_STATUSA LPUPDATE_STATUS;
#endif

/**
* Gives information on the partition update status.
*/
typedef struct _UPDATE_PARTITION_STATUSW
{
	UINT32 percentage; /**< Percentage of update done.*/
	WCHAR  updateInfo[MAX_PATH]; /**< Additional information like
								 partition on which the update is being done currently and/or
								 operation currently being performed at reader.*/
	BOOLEAN	currentStatus; /* TRUE - In Progress FALSE - Completed */
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}UPDATE_PARTITION_STATUSW, *LPUPDATE_PARTITION_STATUSW;

typedef struct _UPDATE_PARTITION_STATUSA
{
	UINT32 percentage; /**< Percentage of update done.*/
	CHAR  updateInfo[MAX_PATH]; /**< Partition on which the update is being done currently.*/
	BOOLEAN	currentStatus; /* TRUE - In Progress FALSE - Completed */
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}UPDATE_PARTITION_STATUSA, *LPUPDATE_PARTITION_STATUSA;

#ifdef UNICODE
typedef UPDATE_PARTITION_STATUSW UPDATE_PARTITION_STATUS;
typedef LPUPDATE_PARTITION_STATUSW LPUPDATE_PARTITION_STATUS;
#else
typedef UPDATE_PARTITION_STATUSA UPDATE_PARTITION_STATUS;
typedef LPUPDATE_PARTITION_STATUSA LPUPDATE_PARTITION_STATUS;
#endif

/**
* TIME_SERVER_INFO contains the SNTP Host namae
*/

typedef struct _TIME_SERVER_INFOW
{
	WCHAR sntpHostName[MAX_PATH];
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}TIME_SERVER_INFOW, *LPTIME_SERVER_INFOW;

typedef struct _TIME_SERVER_INFOA
{
	CHAR sntpHostName[MAX_PATH];
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}TIME_SERVER_INFOA, *LPTIME_SERVER_INFOA;

#ifdef UNICODE
typedef TIME_SERVER_INFOW TIME_SERVER_INFO;
typedef LPTIME_SERVER_INFOW LPTIME_SERVER_INFO;
#else
typedef TIME_SERVER_INFOA TIME_SERVER_INFO;
typedef LPTIME_SERVER_INFOA LPTIME_SERVER_INFO;
#endif


/**
* SYSLOG_SERVER_INFO contains SysLog Server IP/HostName, port, severity
*/

typedef struct _SYSLOG_SERVER_INFOW
{
	WCHAR remoteServerHostName[MAX_PATH]; /**< Remote Syslog server IP/Host Name */
	UINT32 remoteServerPort; /**< Remote Syslog server port number, by default set to 514 */
	UINT16 minSeverity; /**< Default to Debug */

}SYSLOG_SERVER_INFOW, *LPSYSLOG_SERVER_INFOW;

typedef struct _SYSLOG_SERVER_INFOA
{
	CHAR remoteServerHostName[MAX_PATH]; /**< Remote Syslog server IP/Host Name */
	UINT32 remoteServerPort; /**< Remote Syslog server port number, by default set to 514 */
	UINT16 minSeverity; /**< Default to Debug */

}SYSLOG_SERVER_INFOA, *LPSYSLOG_SERVER_INFOA;

/**
* Get Wrireless Configured Parameters.
*/
typedef struct _WIRELESS_CONFIGURED_PARAMETERSW
{
	WCHAR essid[MAX_PATH];
	WCHAR passKey[MAX_PATH];
	//WCHAR metadata[MAX_PATH];
	BOOLEAN  autoconnect;
}WIRELESS_CONFIGURED_PARAMETERSW, *LPWIRELESS_CONFIGURED_PARAMETERSW;

/**
* Get Wrireless Configured Parameters.
*/
typedef struct _WIRELESS_CONFIGURED_PARAMETERSA
{
	CHAR essid[MAX_PATH];
	CHAR passKey[MAX_PATH];
	//CHAR metadata[MAX_PATH];
	BOOLEAN autoconnect;
}WIRELESS_CONFIGURED_PARAMETERSA, *LPWIRELESS_CONFIGURED_PARAMETERSA;
#ifdef UNICODE
typedef SYSLOG_SERVER_INFOW SYSLOG_SERVER_INFO;
typedef LPSYSLOG_SERVER_INFOW LPSYSLOG_SERVER_INFO;
typedef WIRELESS_CONFIGURED_PARAMETERSW WIRELESS_CONFIGURED_PARAMETERS;
typedef LPWIRELESS_CONFIGURED_PARAMETERSW LPWIRELESS_CONFIGURED_PARAMETERS;
#else
typedef SYSLOG_SERVER_INFOA SYSLOG_SERVER_INFO;
typedef LPSYSLOG_SERVER_INFOA LPSYSLOG_SERVER_INFO;
typedef WIRELESS_CONFIGURED_PARAMETERSA WIRELESS_CONFIGURED_PARAMETERS;
typedef LPWIRELESS_CONFIGURED_PARAMETERSA LPWIRELESS_CONFIGURED_PARAMETERS;
#endif

/**
* READER_INFO gives information on the Reader's name, location, description and contact.
*/
typedef struct _READER_INFOW
{
	WCHAR name[MAX_PATH];
	WCHAR description[MAX_PATH];
	WCHAR location[MAX_PATH];
	WCHAR contact[MAX_PATH];
	BOOLEAN antennaCheck;
	BOOLEAN reserved[3];
	LPTIME_SERVER_INFOW lpTimeServerInfo;
	LPVOID lpReserved[2];				/**<  Reserved for future.*/
}READER_INFOW, *LPREADER_INFOW;
typedef struct _READER_INFOA
{
	CHAR name[MAX_PATH];
	CHAR description[MAX_PATH];
	CHAR location[MAX_PATH];
	CHAR contact[MAX_PATH];
	BOOLEAN antennaCheck;
	BOOLEAN reserved[3];
	LPTIME_SERVER_INFOA lpTimeServerInfo;

	LPVOID lpReserved[2];				/**<  Reserved for future.*/
}READER_INFOA, *LPREADER_INFOA;

#ifdef UNICODE
typedef READER_INFOW READER_INFO;
typedef LPREADER_INFOW LPREADER_INFO;
#else
typedef READER_INFOA READER_INFO;
typedef LPREADER_INFOA LPREADER_INFO;
#endif

/**
* READER_SYSTEM_INFO gives information on the Reader's current state.
*/
typedef struct _READER_NETWORK_INFOW
{
	WCHAR IPAddress[MAX_PATH];
	WCHAR MACAddress[MAX_PATH];
	WCHAR SubnetMask[MAX_PATH];
	WCHAR Gateway[MAX_PATH];
	WCHAR DNSServer[MAX_PATH];
	WCHAR IPVersion[MAX_PATH];
	WCHAR IPV6Address[MAX_PATH];
	WCHAR IPV6Suffix[MAX_PATH];
	WCHAR IPV6DNS[MAX_PATH];
	WCHAR IPV6GateWay[MAX_PATH];
	WCHAR Interface[MAX_PATH];
	BOOLEAN isDHCPEnabled;
	BOOLEAN isCoreConfig;
	BOOLEAN isDHCPv6Enabled;
	BOOLEAN EnableRAPAckets;
	LPVOID lpReserved[4];					/**<  Reserved for future. */

}READER_NETWORK_INFOW, *LPREADER_NETWORK_INFOW;


/*
* Device Info - HostName, Manufacturer, Model, HW Version
*/

typedef struct _READER_DEVICE_INFOW
{
	WCHAR hostName[MAX_PATH];/**<  Host Name */
	WCHAR manufacturer[MAX_PATH];		/**<  Manufacturer*/
	WCHAR model[MAX_PATH]; /**< Model */
	WCHAR hardWareVersion[MAX_PATH]; /**< Hardware version -- RF Board version */
	WCHAR bootLoaderVersion[MAX_PATH]; /**< Boot Loader version */
	UINT32 NumPhysicalAntennas; /**< Number of Physical Antennas */
	LPVOID lpReserved[4];					/**<  Reserved for future. */

}READER_DEVICE_INFOW, *LPREADER_DEVICE_INFOW;

typedef struct _READER_SYSTEM_INFOW
{
	/* Sub-structure describing memory allocated and used by this     
	structure .*/
	_STRUCT_INFO         structInfo;
	WCHAR radioFirmwareVersion[MAX_PATH];/**<  Firmware version of the Radio.*/
	WCHAR fPGAVersion[MAX_PATH];		/**<  FPGA version.*/
	WCHAR upTime[MAX_PATH];				/**<  Reader's up-time.*/
	WCHAR readerName[MAX_PATH];			/**<  Reader name.*/
	WCHAR readerLocation[MAX_PATH];		/**<  Location of the Reader.*/
	UINT32 ramAvailable;				/**<  RAM memory available.*/
	UINT32 flashAvailable;				/**<  Flash memory available.*/
	UINT32	ramUsed;						/**<  Ram Used.*/
	UINT32	ramTotal;						/**<  Total RAM.*/
	INT8	cpuUsageForUserProcesses;			/**<  CPU utilization in % for user processes.*/
	INT8	cpuUsageForSystemProcesses;		/**<  CPU utilization in % for system processes.*/
	UINT16	reserved1;						/**<  Reservation for 16 bits remaining after use of two INT8s.*/
	UINT32 reserved32[4];				/**< for future expansions other than pointers types*/
	WCHAR *serialNumber;				/**<  Serial number of the Reader.*/
	POWER_SOURCE_TYPE powerSource;		/*Power Source type POE, POE+ or DC Supply*/
	LPREADER_NETWORK_INFOW networkInfo;
	LPREADER_DEVICE_INFOW deviceInfo; /* Contains HostName, Manufacturer, Model, HW Version*/	
	LPVOID lpReserved[2];					/**<  Reserved for future. 
											1 - ramUsed, 
											1 - ramTotal,
											1 - cpuUsedUser+cpuUsedSystem+reserved1*/
}READER_SYSTEM_INFOW, *LPREADER_SYSTEM_INFOW;


/**
* READER_SYSTEM_INFO gives information on the Reader's current state.
*/

typedef struct _READER_NETWORK_INFOA
{
	CHAR IPAddress[MAX_PATH];
	CHAR MACAddress[MAX_PATH];
	CHAR SubnetMask[MAX_PATH];
	CHAR Gateway[MAX_PATH];
	CHAR DNSServer[MAX_PATH];
	CHAR IPVersion[MAX_PATH];
	CHAR IPV6Address[MAX_PATH];
	CHAR IPV6Suffix[MAX_PATH];
	CHAR IPV6DNS[MAX_PATH];
	CHAR IPV6GateWay[MAX_PATH];
	CHAR Interface[MAX_PATH];
	BOOLEAN isDHCPEnabled;
	BOOLEAN isCoreConfig;
	BOOLEAN isDHCPv6Enabled;
	BOOLEAN EnableRAPAckets;
	LPVOID lpReserved[4];					/**<  Reserved for future. */

}READER_NETWORK_INFOA, *LPREADER_NETWORK_INFOA;

/*
* Device Info - HostName, Manufacturer, Model, HW Version
*/

typedef struct _READER_DEVICE_INFOA
{
	CHAR hostName[MAX_PATH];/**<  Host Name */
	CHAR manufacturer[MAX_PATH];		/**<  Manufacturer*/
	CHAR model[MAX_PATH]; /**< Model */
	CHAR hardWareVersion[MAX_PATH]; /**< Hardware version -- RF Board version */
	CHAR bootLoaderVersion[MAX_PATH]; /**< Boot Loader version */
	UINT32 numPhysicalAntennas; /**< Number of Physical Antennas */
	LPVOID lpReserved[4];					/**<  Reserved for future. */

}READER_DEVICE_INFOA, *LPREADER_DEVICE_INFOA;

typedef struct _READER_SYSTEM_INFOA
{
	/* Sub-structure describing memory allocated and used by this     
	structure .*/
	_STRUCT_INFO         structInfo;
	CHAR	radioFirmwareVersion[MAX_PATH];	/**<  Firmware version of the Radio.*/
	CHAR	fPGAVersion[MAX_PATH];			/**<  FPGA version.*/
	CHAR	upTime[MAX_PATH];				/**<  Reader's up-time.*/
	CHAR	readerName[MAX_PATH];			/**<  Reader name.*/
	CHAR	readerLocation[MAX_PATH];		/**<  Location of the Reader.*/
	UINT32	ramAvailable;					/**<  RAM available for use.*/
	UINT32	flashAvailable;					/**<  Flash memory available.*/
	UINT32	ramUsed;						/**<  Ram Used.*/
	UINT32	ramTotal;						/**<  Total RAM.*/
	INT8	cpuUsageForUserProcesses;		/**<  CPU utilization in % for user processes.*/
	INT8	cpuUsageForSystemProcesses;		/**<  CPU utilization in % for system processes.*/
	UINT16	reserved1;						/**<  Reservation for 16 bits remaining after use of two INT8s.*/
	UINT32 reserved32[4];				/**< for future expansions other than pointers types*/
	CHAR *serialNumber;				/**<  Serial number of the Reader.*/
	POWER_SOURCE_TYPE powerSource;
	LPREADER_NETWORK_INFOA networkInfo;
	LPREADER_DEVICE_INFOA deviceInfo; /* Contains HostName, Manufacturer, Model, HW Version*/	
	LPVOID lpReserved[2];					/**<  Reserved for future. 
											1 - ramUsed, 
											1 - ramTotal,
											1 - cpuUsedUser+cpuUsedSystem+reserved1*/
}READER_SYSTEM_INFOA, *LPREADER_SYSTEM_INFOA;

#ifdef UNICODE
typedef READER_SYSTEM_INFOW READER_SYSTEM_INFO;
typedef LPREADER_SYSTEM_INFOW LPREADER_SYSTEM_INFO;
typedef READER_NETWORK_INFOW READER_NETWORK_INFO;
typedef LPREADER_NETWORK_INFOW LPREADER_NETWORK_INFO;
#else
typedef READER_SYSTEM_INFOA READER_SYSTEM_INFO;
typedef LPREADER_SYSTEM_INFOA LPREADER_SYSTEM_INFO;
typedef READER_NETWORK_INFOA READER_NETWORK_INFO;
typedef LPREADER_NETWORK_INFOA LPREADER_NETWORK_INFO;
#endif
/**
* PROFILE_LIST gives the list of profiles available in the Reader.
*/
typedef struct _PROFILE_LISTW
{
	UINT16	numProfiles;	/**<  Number of profiles.*/
	WCHAR	**pProfileName;	/**<  List of profiles.*/
	UINT16	activeProfileIndex;/**< zero based index specifies which profile is active*/
	LPVOID lpReserved[16]; /**<  Reserved for future.*/
} PROFILE_LISTW, *LPPROFILE_LISTW;

typedef struct _PROFILE_LISTA
{
	UINT16	numProfiles;	/**<  Number of profiles.*/
	CHAR	**pProfileName;	/**<  List of profiles.*/
	UINT16	activeProfileIndex;/**< zero based index specifies which profile is active*/
	LPVOID lpReserved[16]; /**<  Reserved for future.*/
} PROFILE_LISTA, *LPPROFILE_LISTA;

#ifdef UNICODE
typedef PROFILE_LISTW PROFILE_LIST;
typedef LPPROFILE_LISTW LPPROFILE_LIST;
#else
typedef PROFILE_LISTA PROFILE_LIST;
typedef LPPROFILE_LISTA LPPROFILE_LIST;
#endif

/**
* LICENSE_INFO contains the details of license
*/
typedef struct _LICENSE_INFOW
{
	UINT16	index;
	WCHAR	szName[260];
	WCHAR	szVersion[260];
	WCHAR	szExpiryDate[260];
	UINT16	count;
	WCHAR	szHostID[260];
}LICENSE_INFOW, *LPLICENSE_INFOW;

typedef struct _LICENSE_INFOA
{
	UINT16	index;
	CHAR	szName[260];
	CHAR	szVersion[260];
	CHAR	szExpiryDate[260];
	UINT16	count;
	CHAR	szHostID[260];
}LICENSE_INFOA, *LPLICENSE_INFOA;

/**
* LICENSE_LIST gives the list of active License available
*/
typedef struct _LICENSE_LISTW
{
	UINT32	numLicenses;	/**<  Number of Licenses.*/
	LICENSE_INFOW *pLicenseList;	/**<  List of License Info.*/
	LPVOID lpReserved[4]; /**<  Reserved for future.*/
}LICENSE_LISTW, *LPLICENSE_LISTW;

typedef struct _LICENSE_LISTA
{
	UINT32	numLicenses;	/**<  Number of Licenses.*/
	LICENSE_INFOA *pLicenseList;	/**<  List of License Info..*/
	LPVOID lpReserved[4]; /**<  Reserved for future.*/
}LICENSE_LISTA, *LPLICENSE_LISTA;

#ifdef UNICODE
typedef LICENSE_LISTW LICENSE_LIST;
typedef LPLICENSE_LISTW LPLICENSE_LIST;
#else
typedef LICENSE_LISTA LICENSE_LIST;
typedef LPLICENSE_LISTA LPLICENSE_LIST;
#endif


/**
* TIME_ZONE_LIST gives the list of Time zones available in the Reader.
*/
typedef struct _TIME_ZONE_LISTW
{
	UINT16	numTimeZones;	/**<  Number of Time Zones.*/
	WCHAR	**pTimeZone;	/**<  List of Time Zones.*/
	UINT16	activeTimeZoneIndex;/**< zero based index specifies which Time Zone is active*/
	LPVOID lpReserved[4]; /**<  Reserved for future.*/
} TIME_ZONE_LISTW, *LPTIME_ZONE_LISTW;

typedef struct _TIME_ZONE_LISTA
{	
	UINT16	numTimeZones;
	CHAR	**pTimeZone;
	UINT16	activeTimeZoneIndex;
	LPVOID lpReserved[4]; /**<  Reserved for future.*/
} TIME_ZONE_LISTA, *LPTIME_ZONE_LISTA;

#ifdef UNICODE
typedef TIME_ZONE_LISTW TIME_ZONE_LIST;
typedef LPTIME_ZONE_LISTW LPTIME_ZONE_LIST;
#else
typedef TIME_ZONE_LISTA TIME_ZONE_LIST;
typedef LPTIME_ZONE_LISTA LPTIME_ZONE_LIST;
#endif

/**
* FILE_UPDATE_LIST gives the list of changed files to update
*/
typedef struct _FILE_UPDATE_LISTW
{
	UINT16	numFiles;	/**<  Number of Files changed.*/
	WCHAR	**pFileName;	/**<  List of File Names*/
} FILE_UPDATE_LISTW, *LPFILE_UPDATE_LISTW;

typedef struct _FILE_UPDATE_LISTA
{
	UINT16	numFiles;	/**<  Number of Files changed.*/
	CHAR	**pFileName;	/**<  List of File Names*/
} FILE_UPDATE_LISTA, *LPFILE_UPDATE_LISTA;

#ifdef UNICODE
typedef FILE_UPDATE_LISTW FILE_UPDATE_LIST;
typedef LPFILE_UPDATE_LISTW LPFILE_UPDATE_LIST;
#else
typedef FILE_UPDATE_LISTA FILE_UPDATE_LIST;
typedef LPFILE_UPDATE_LISTA LPFILE_UPDATE_LIST;
#endif

/** 
* SECURE_LLRP_CONFIG configures parameters for secure mode LLRP operation
*/
typedef struct _SEC_LLRP_CONFIG 
{
		BOOLEAN secureMode;			/**< Enable TLS mode of operation for LLRP*/
		BOOLEAN validatePeerCert;	/**< Enable validation of peer by verifying its certificate */
		LPVOID lpReserved[4];		/**<  Reserved for future.*/
}SEC_LLRP_CONFIG, *LPSEC_LLRP_CONFIG;

/**
* LLRP_CONNECTION_STATUS get the current LLRP Connection status
*/
typedef struct _LLRP_CONNECTION_STATUSW
{
	BOOLEAN isConnected; /**< LLRP Connection status*/
	WCHAR  clientIPAddress[MAX_PATH];	/**< IP Address of the Client connected to LLRP. To be ignored if  LLRPConnectStatus is false*/
	LPVOID lpReserved[4];		/**<  Reserved for future.*/
}LLRP_CONNECTION_STATUSW, *LPLLRP_CONNECTION_STATUSW;

/**
* LLRP_CONNECTION_STATUS get the current LLRP Connection status
*/
typedef struct _LLRP_CONNECTION_STATUSA
{
	BOOLEAN isConnected; /**< LLRP Connection status*/
	CHAR  clientIPAddress[MAX_PATH];	/**< IP Address of the Client connected to LLRP. To be ignored if  LLRPConnectStatus is false*/
	LPVOID lpReserved[4];		/**<  Reserved for future.*/
}LLRP_CONNECTION_STATUSA, *LPLLRP_CONNECTION_STATUSA;


/**
* LLRP_CONNECTION_CONFIG configures the parameters for LLRP Server/Client connectivity.
*/
typedef struct _LLRP_CONNECTION_CONFIGW
{
	BOOLEAN isClient; /**< Configure LLRP as a client.*/
	UINT32 port;	/**<  LLRP Server's port number.*/
	WCHAR  hostServerIP[MAX_PATH];	/**<  IP address of the LLRP server the reader needs to connect to while in client mode.*/
	LPSEC_LLRP_CONFIG	pSecureModeConfig; /**<  Configurations for LLRP secure mode operation. */
	LPVOID lpReserved[5]; /**<  Reserved for future.1 used for pSecureModeConfig*/
} LLRP_CONNECTION_CONFIGW, *LPLLRP_CONNECTION_CONFIGW;

typedef struct _LLRP_CONNECTION_CONFIGA
{	
	BOOLEAN isClient;
	UINT32 port;
	CHAR	hostServerIP[MAX_PATH];
	LPSEC_LLRP_CONFIG	pSecureModeConfig; /**<  Configurations for LLRP secure mode operation. */
	LPVOID lpReserved[5]; /**<  Reserved for future.1 used for pSecureModeConfig*/
} LLRP_CONNECTION_CONFIGA, *LPLLRP_CONNECTION_CONFIGA;

#ifdef UNICODE
typedef LLRP_CONNECTION_CONFIGW LLRP_CONNECTION_CONFIG;
typedef LPLLRP_CONNECTION_CONFIGW LPLLRP_CONNECTION_CONFIG;
typedef LLRP_CONNECTION_STATUSW LLRP_CONNECTION_STATUS;
typedef LPLLRP_CONNECTION_STATUSW LPLLRP_CONNECTION_STATUS;

#else
typedef LLRP_CONNECTION_CONFIGA LLRP_CONNECTION_CONFIG;
typedef LPLLRP_CONNECTION_CONFIGA LPLLRP_CONNECTION_CONFIG;
typedef LLRP_CONNECTION_STATUSA LLRP_CONNECTION_STATUS;
typedef LPLLRP_CONNECTION_STATUSA LPLLRP_CONNECTION_STATUS;
#endif


/** 
* USB_OPERATION_INFO provides information of the reader's USB operation mode.
*/
typedef struct _USB_OPERATION_INFO
{	
	USB_OPERATION_MODE mode;/**< Specifies the USB operation mode*/
	BOOLEAN allowLLRPConnectionOverride;/**< When mode is NETWORK, it is possible for a different client connecting via the USB interface
										to override an LLRP client is connected over the primary interface.*/
	LPVOID lpReserved[4]; /**<  Reserved for future.*/
} USB_OPERATION_INFO, *LPUSB_OPERATION_INFO;



/** 
* LED_INFO contains information of the User LED settings.
*/
typedef struct _LED_INFO
{	
	LED_COLOR ledColor; /**< Specifies LED Color*/
	UINT32 durationSeconds;   /**< Duration in Seconds. If zero, the LED blinks continuously*/
	BOOLEAN blink;	/**< Specifies LED to be blinked or not */
	LPVOID lpReserved[4]; /**<  Reserved for future.*/
}LED_INFO, *LPLED_INFO;



/** 
* ERROR_INFO provides additional information on the last API that failed.
*/
typedef struct _ERROR_INFOW
{
	SYSTEMTIME		errorTimeStamp; /**< UTC Timestamp at which the error occurred.*/
	RFID_STATUS		rfidStatusCode;/**< Error code of the operation that failed.*/
	WCHAR			statusDesc[MAX_PATH];/**< Error description pertaining to rfidStatusCode.*/
	WCHAR			vendorMessage[MAX_PATH];/**< Vendor specific error message for the error that occurred; E.g. LLRP message or Reader Management Message.*/
}ERROR_INFOW, *LPERROR_INFOW;

typedef struct _ERROR_INFOA
{
	SYSTEMTIME		errorTimeStamp; /**< UTC Timestamp at which the error occurred.*/
	RFID_STATUS		rfidStatusCode;/**< Error code of the operation that failed.*/
	CHAR			statusDesc[MAX_PATH];/**< Error description pertaining to rfidStatusCode.*/
	CHAR			vendorMessage[MAX_PATH];/**< Vendor specific error message for the error that occurred; E.g. LLRP message or Reader Management Message.*/
}ERROR_INFOA, *LPERROR_INFOA;

#ifdef UNICODE
typedef ERROR_INFOW ERROR_INFO;
typedef LPERROR_INFOW LPERROR_INFO;
#else
typedef ERROR_INFOA ERROR_INFO;
typedef LPERROR_INFOA LPERROR_INFO;
#endif
/*Parameters of APIs encapsulating  NXP  commands */

/**
*  NXP_SET_EAS_PARAMS contains parameters for NXP's EAS (Electronic Article Surveillance) operation 
*/
typedef struct _NXP_SET_EAS_PARAMS
{
	UINT32   accessPassword; /**< Access Password of the NXP tag*/
	BOOLEAN  EASState;       /**< Desired state of the EAS bit*/
	LPVOID lpReserved[4];       /**< Reserved for future use */
}NXP_SET_EAS_PARAMS,*LPNXP_SET_EAS_PARAMS;


/**
*  NXP_READ_PROTECT_PARAMS contains parameters for NXP's Read-Protect(SetQuiet) operation
*/
typedef struct _NXP_READ_PROTECT_PARAMS
{
	UINT32 accessPassword; /**< Access Password of the NXP tag*/
	LPVOID lpReserved[4];	   /**< Reserved for future use */
 }NXP_READ_PROTECT_PARAMS,*LPNXP_READ_PROTECT_PARAMS;


/**
* NXP_RESET_READ_PROTECT_PARAMS contains parameters for NXP's Reset Read-Protect(Reset Quiet) operation
*/
typedef struct _NXP_RESET_READ_PROTECT_PARAMS
{
	UINT32 accessPassword; /**<  Access Password of the NXP tag*/
	LPVOID lpReserved[4];     /**< Reserved for future use */
 }NXP_RESET_READ_PROTECT_PARAMS,*LPNXP_RESET_READ_PROTECT_PARAMS;
 
/**
* FUJITSU_CHANGEWORDLOCK_ACCESS_PARAMS contains the parameters for Fujitsu's ChangeWordLock operation.Only 2 LSBs of payloadAction and PayloadMask
* are considered for ChangeWordLock's payload.
*/

typedef struct _FUJITSU_CHANGE_WORDLOCK_ACCESS_PARAMS
{
	MEMORY_BANK memoryBankName; 
    UINT16 byteOffset;            /**< Byte-Offset of the starting word*/
    UINT8  payloadAction;         /**< 2LSBs specify the WordLock-Action for words 0 and 1 (in that order)*/
	UINT8  payloadMask;			  /**< 2LSBs specify  the Mask for the action bits in payloadAction*/				
    UINT32 blockGroupPassword;    /**< Password of the containing BlockGroup*/
    LPVOID lpReserved[2];
}FUJITSU_CHANGE_WORDLOCK_ACCESS_PARAMS,*LPFUJITSU_CHANGE_WORDLOCK_ACCESS_PARAMS;
typedef struct _FUJITSU_CHANGE_BLOCKLOCK_ACCESS_PARAMS
{ 
	UINT8 blockGroupOffset;      /**< Index of the Block ranging from 0-31*/
	UINT16 payloadAction;		 /**< A 16 bit pattern where each bit specifies status of BlockLock flags for its respective Block*/
	UINT16 payloadMask;			 /**< Mask for payload Action where each bit specifies whether to apply or ignore the corresponding 'Action' bit*/
	UINT32 blockGroupPassword;   /**< Password of the containing BlockGroup*/
	LPVOID lpReserved[2];

}FUJITSU_CHANGE_BLOCKLOCK_ACCESS_PARAMS,*LPFUJITSU_CHANGE_BLOCKLOCK_ACCESS_PARAMS;
/**
* Parameters for Fujitsu's ReadBlockOperation for 64K Tags.Specifies the BlockGroup whose BlockLock flags will be read.
*/
typedef struct _FUJITSU_READ_BLOCKLOCK_ACCESS_PARAMS
{ 
	UINT8 blockGroupOffset;  /**< Index of the Block ranging from 0-31*/
	LPVOID lpReserved[2];

}FUJITSU_READ_BLOCKLOCK_ACCESS_PARAMS,*LPFUJITSU_READ_BLOCKLOCK_ACCESS_PARAMS;
/**
*  Parameters for the BurstWrite Operation.Specifies BurstWrite Data , the memory location where it should be written and the access password of the tag(s).  
*/
typedef struct _FUJITSU_BURST_WRITE_ACCESS_PARAMS
{ 
	MEMORY_BANK memoryBank;       /**< Memory Bank Name */
	UINT16  byteOffset;			  /**< Byte-Offset in the Memory Bank,must be a multiple of 4 */	
	UINT8*  pBurstWriteData;	  /**< Pointer to Byte-Array containg Data for BurstWrite */
	UINT16  burstWriteDataLength;  /**< Byte-Length of the BurstWrite Data,must be a multiple of 4*/
	UINT32  accessPassword;		  /**< Access password of the participant tags*/
	LPVOID lpReserved[2];		
 
}FUJITSU_BURST_WRITE_ACCESS_PARAMS,*LPFUJITSU_BURST_WRITE_ACCESS_PARAMS;
/**
*  Parameters for the BurstErase Operation.Specifies the memory location of Data to be Burst-erased,its byte-length and the access password of the tag(s).  
*/
typedef struct _FUJITSU_BURSTERASE_ACCESS_PARAMS
{ 
	MEMORY_BANK memoryBank;       /**< Memory Bank Name */
	UINT16  byteOffset;			  /**< Byte-Offset in the Memory Bank,must be a multiple of 4 */	
	UINT16  burstEraseLength;     /**< Byte-Length of the Data to be BurstErased,must be a multiple of 4*/
	UINT32  accessPassword;		  /**< Access password of the participant tags*/
	LPVOID lpReserved[2];		
 
}FUJITSU_BURST_ERASE_ACCESS_PARAMS,*LPFUJITSU_BURST_ERASE_ACCESS_PARAMS;
/**
* Parameters for Fujitsu's AreaReadLock operation for 8K Tags.Specifies
*/

typedef struct _FUJITSU_AREA_READLOCK_ACCESS_PARAMS
{ 
	UINT8  areaGroupOffset;      /**< Index of the Block ranging from 0-15*/
	UINT16 payloadAction;		 /**< A 16 bit pattern where each bit specifies the desired status for ReadLock flag of its respective Area*/
	UINT16 payloadMask;			 /**< Mask for payload Action where each bit specifies whether to apply or ignore the corresponding 'Action' bit*/
	UINT32 areaGroupPassword;    /**< Password of the containing AreaGroup*/
	LPVOID lpReserved[2];

}FUJITSU_AREA_READLOCK_ACCESS_PARAMS,*LPFUJITSU_AREA_READLOCK_ACCESS_PARAMS;
/**
* Parameters for Fujitsu's AreaWriteLock operation for 8K Tags.Specifies
*/

typedef struct _FUJITSU_AREA_WRITELOCK_ACCESS_PARAMS
{ 
	UINT8  areaGroupOffset;      /**< Index of the Block ranging from 0-15*/
	UINT16 payloadAction;		 /**< A 16 bit pattern where each bit specifies the desired status for the WriteLock flag of its respective Area*/
	UINT16 payloadMask;			 /**< Mask for payload Action where each bit specifies whether to apply or ignore the corresponding 'Action' bit*/
	UINT32 areaGroupPassword;    /**< Password of the containing AreaGroup*/
	LPVOID lpReserved[2];

}FUJITSU_AREA_WRITELOCK_ACCESS_PARAMS,*LPFUJITSU_AREA_WRITELOCK_ACCESS_PARAMS;
/**
* Parameters for Fujitsu's AreaWriteLockWoPassword operation for 8K Tags.Specifies the AreaGroup and the PayloadAction.
  The term "WOPASSWORD" meants Without-AreaGroupPassword,as can be noted from the structure definition.
*/

typedef struct _FUJITSU_AREA_WRITELOCK_WOPASSWORD_ACCESS_PARAMS
{ 
	UINT8  areaGroupOffset;      /**< Index of the Block ranging from 0-15*/
	UINT16 payloadAction;		 /**< A 16 bit pattern where each bit specifies whether to set/ignore the status of WriteLock flag of its corresponding Area*/
	LPVOID lpReserved[2];

}FUJITSU_AREA_WRITELOCK_WOPASSWORD_ACCESS_PARAMS,*LPFUJITSU_AREA_WRITELOCK_WOPASSWORD_ACCESS_PARAMS;
/**
* Parameters for Fujitsu's ChangeBlockOrAreaGroupPassword Operation for 64K Tags.Specifies the Block or Area(according to whether the target is 64K or 8K),
  the new-password and the current password.
*/
typedef struct _FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_ACCESS_PARAMS
{ 
	UINT8 blockOrAreaGroupOffset;  /**< Index of the Block (ranging from 0-31) Or AreaGroup(ranging from 0-15)*/
	UINT32 newPassword;            /**< New BlockGroup or AreaGroup password*/
	UINT32 currentPassword;		   /**< Current BlockGroup or AreaGroup password*/
	LPVOID lpReserved[2];

}FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_ACCESS_PARAMS,*LPFUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_ACCESS_PARAMS;

/**
* Return value of ListUserApps operation - UNICODE. Contains application related information.
*/
typedef struct _USERAPP_DATAW
{ 
	WCHAR *pAppName;			/**< Name of the app*/
	BOOLEAN runStatus;			/**< Current run status of the app*/
	BOOLEAN autoStart;			/**< Whether the app will run on startup*/
	WCHAR *pMetaData;			/**< Application metadata ex. version*/	
}USERAPP_DATAW, *LPUSERAPP_DATAW;

typedef struct _USERAPP_LISTW
{ 
	UINT32 numMaxApps;
	USERAPP_DATAW *pUserAppData;
}USERAPP_LISTW, *LPUSERAPP_LISTW;

/**
* Return value of ListUserApps operation - ASCII. Contains application related information.
*/

typedef struct _USERAPP_DATAA
{ 
	CHAR *pAppName;				/**< Name of the app*/
	BOOLEAN runStatus;			/**< Current run status of the app*/
	BOOLEAN autoStart;			/**< Whether the app will run on startup*/
	CHAR *pMetaData;			/**< Application metadata. ex:version*/	
}USERAPP_DATAA, *LPUSERAPP_DATAA;

typedef struct _USERAPP_LISTA
{ 
	UINT32 numMaxApps;
	USERAPP_DATAA *pUserAppData;
}USERAPP_LISTA, *LPUSERAPP_LISTA;


#ifdef UNICODE
typedef USERAPP_LISTW USERAPP_LIST;
typedef LPUSERAPP_LISTW LPUSERAPP_LIST;
#else
typedef LPUSERAPP_LISTA LPUSERAPP_LIST;
typedef USERAPP_LISTA USERAPP_LIST;
#endif
/**
* CABLE_LOSS_COMPENSATION gives the way to set/get the cable loss compensation factor.
*/
typedef struct _CABLE_LOSS_COMPENSATION_
{ 
	UINT16 antennaID; 				/**< antenna ID*/
	FLOAT cableLengthInFt;			/**< cable length in ft*/
	FLOAT cableLossPer100Ft; 		/**< cable loss per 100 ft*/

}CABLE_LOSS_COMPENSATION, *LPCABLE_LOSS_COMPENSATION;

   
/** 
 * SLED_BATTERY_STATUS contains Sled Battery related information, battery level in percantage 
 * and status (charging\discharging or critical) 
 */ 
typedef struct _SLED_BATTERY_STATUS 
{ 
    UINT32 batteryLevel;        /**< Battery level in percentage (%) */ 
    BATTERY_STATUS status;      /**< status (charging\discharging or critical) */ 
    LPVOID lpReserved[2]; 
}SLED_BATTERY_STATUS,*LPSLED_BATTERY_STATUS;

/** 
 *  STANDARD_REGION_INFO contains given region regulatory
 *		related standard information
 */ 
typedef struct _STANDARD_REGION_INFOW
{
	WCHAR strRegionName[MAX_PATH];			/**< Region Name e.g. European Union*/
	WCHAR strStandardName[MAX_PATH];		/**< Region Standard name e.g. EU 302.208*/
	BOOLEAN bIsHoppingConfigurable;			/**< whether hopping configuration is allowed*/
	UINT32 iNumChannels;					/**< Number of frequency channels supported by region*/
	UINT32 *piChannelInfo;					/**< List of channels supported by region*/
	UINT32 *piChannelValueInfo;				/**< Value list of channels supported by region*/
}STANDARD_REGION_INFOW,*LPSTANDARD_REGION_INFOW;

/** 
 *  STANDARD_REGION_INFO contains given region regulatory
 *		related standard information
 */
typedef struct _STANDARD_REGION_INFOA
{
	CHAR strRegionName[MAX_PATH];			/**< Region Name e.g. European Union*/
	CHAR strStandardName[MAX_PATH];			/**< Region Standard name e.g. EU 302.208*/
	BOOLEAN bIsHoppingConfigurable;			/**< whether hopping configuration is allowed*/
	UINT32 iNumChannels;					/**< Number of frequency channels supported by region*/
	UINT32 *piChannelInfo;					/**< List of channels supported by region*/
	UINT32 *piChannelValueInfo;				/**< Value list of channels supported by region*/
}STANDARD_REGION_INFOA,*LPSTANDARD_REGION_INFOA;

/** 
 *  USER_INFO contains given user name, old and new password
 */
typedef struct _USER_INFOW
{
	WCHAR userName[MAX_PATH];
	WCHAR oldPassword[MAX_PATH];
	WCHAR newPassword[MAX_PATH];
	LPVOID lpReserved[4];
}USER_INFOW,*LPUSER_INFOW;

/** 
 *  USER_INFO contains given user name, old and new password
 */
typedef struct _USER_INFOA
{
	CHAR userName[MAX_PATH];
	CHAR oldPassword[MAX_PATH];
	CHAR newPassword[MAX_PATH];
	LPVOID lpReserved[4];
}USER_INFOA,*LPUSER_INFOA;

/** 
 *  ACTIVE_REGION_INFO contains currently active region regulatory
 *		related information
 */ 
typedef struct _ACTIVE_REGION_INFOW
{
	WCHAR strRegionName[MAX_PATH];			/**< Region Name e.g. European Union*/
	WCHAR strStandardName[MAX_PATH];		/**< Region Standard name e.g. EU 302.208*/
    BOOLEAN bIsHoppingOn;					/**< whether hopping is ON or OFF*/
	UINT32 iNumChannels;					/**< Number of frequency channels enabled in active region*/
	UINT32 *piChannelInfo;					/**< List of channels that are currently enabled in active region*/
}ACTIVE_REGION_INFOW,*LPACTIVE_REGION_INFOW;

/** 
 *  ACTIVE_REGION_INFO contains currently active region regulatory
 *		related information
 */ 
typedef struct _ACTIVE_REGION_INFOA
{
	CHAR strRegionName[MAX_PATH];			/**< Region Name e.g. European Union*/
	CHAR strStandardName[MAX_PATH];			/**< Region Standard name e.g. EU 302.208*/
    BOOLEAN bIsHoppingOn;					/**< whether hopping is ON or OFF*/
	UINT32 iNumChannels;					/**< Number of frequency channels enabled in active region*/
	UINT32 *piChannelInfo;					/**< List of channels that are currently enabled in active region*/
}ACTIVE_REGION_INFOA,*LPACTIVE_REGION_INFOA;

/** 
 *  REGION_LIST contains list of supported regions on reader
 * 
 */ 
typedef struct _REGION_LISTW
{
	UINT32 numRegions;			/**< Total number of region */
	WCHAR** RegionNamesList;	/**< Region names list*/
}REGION_LISTW,*LPREGION_LISTW;

/** 
 *  REGION_LIST contains list of supported regions on reader
 * 
 */ 
typedef struct _REGION_LISTA
{
	UINT32 numRegions;			/**< Total number of region */
	CHAR** RegionNamesList;		/**< Region names list*/
}REGION_LISTA,*LPREGION_LISTA;

/** 
 *  STANDARD_INFO_LIST contains list of standard supported by
 *		particular region e.g. for region Hong Kong, 1. Hong Kong A 2.Hong Kong B
 */ 
typedef struct _STANDARD_INFO_LISTW
{
	UINT32 numStds;						/**< Total number of standrad supported by region */
	STANDARD_REGION_INFOW *StdsList;	/**< List standrad supported by region */
}STANDARD_INFO_LISTW,*LPSTANDARD_INFO_LISTW;

/** 
 *  STANDARD_INFO_LIST contains list of standard supported by
 *		particular region e.g. for region Hong Kong, 1. Hong Kong A 2.Hong Kong B
 */
typedef struct _STANDARD_INFO_LISTA
{
	UINT32 numStds;						/**< Total number of standrad supported by region */
	STANDARD_REGION_INFOA *StdsList;	/**< List standrad supported by region */
}STANDARD_INFO_LISTA,*LPSTANDARD_INFO_LISTA;

/** 
 *  CHANNEL_LIST contains list of channels to be configured 
 *		for particular region
 */
typedef struct _CHANNEL_LIST
{ 
	UINT32 iNumChannels;	/**< Total number of channels */
	UINT32* ChannelList;	/**< List of channels to be used by reader */
}CHANNEL_LIST,*LPCHANNEL_LIST;

/**
* TEMPERATURE_STATUS gives the current temperature and PA temprature.
*/
typedef struct _TEMPERATURE_STATUS_W
{
	FLOAT paTemp; 		/**< PA Temperature*/
	FLOAT ambientTemp;	/**< Current temperature*/
}TEMPERATURE_STATUSW, *LPTEMPERATURE_STATUSW;

/**
* TEMPERATURE_STATUS gives the current temperature and PA temprature.
*/
typedef struct _TEMPERATURE_STATUS_A
{
	FLOAT paTemp; 		/**< PA Temperature*/
	FLOAT ambientTemp;	/**< Current temperature*/
}TEMPERATURE_STATUSA, *LPTEMPERATURE_STATUSA;
#ifdef UNICODE
typedef STANDARD_REGION_INFOW			STANDARD_REGION_INFO;
typedef LPSTANDARD_REGION_INFOW			LPSTANDARD_REGION_INFO;
typedef ACTIVE_REGION_INFOW				ACTIVE_REGION_INFO;
typedef LPACTIVE_REGION_INFOW			LPACTIVE_REGION_INFO;
typedef REGION_LISTW					REGION_LIST;
typedef LPREGION_LISTW					LPREGION_LIST;
typedef STANDARD_INFO_LISTW				STANDARD_INFO_LIST;
typedef LPSTANDARD_INFO_LISTW			LPSTANDARD_INFO_LIST;
typedef USER_INFOW						USER_INFO;
typedef LPTEMPERATURE_STATUSW			LPTEMPERATURE_STATUS;
#else
typedef STANDARD_REGION_INFOA			STANDARD_REGION_INFO;
typedef LPSTANDARD_REGION_INFOA			LPSTANDARD_REGION_INFO;
typedef ACTIVE_REGION_INFOA				ACTIVE_REGION_INFO;
typedef LPACTIVE_REGION_INFOA			LPACTIVE_REGION_INFO;
typedef REGION_LISTA					REGION_LIST;
typedef LPREGION_LISTA					LPREGION_LIST;
typedef STANDARD_INFO_LISTA				STANDARD_INFO_LIST;
typedef LPSTANDARD_INFO_LISTA			LPSTANDARD_INFO_LIST;
typedef USER_INFOA						USER_INFO;
typedef LPTEMPERATURE_STATUSA			LPTEMPERATURE_STATUS;
#endif

/**
* _IOT_CLOUD_INFOW contains the Endpoint address and protocol in IOT Cloud Configuration
*/

typedef struct _IOT_CLOUD_INFOW
{
	WCHAR iotCloudEndpointAddr[MAX_PATH];
	WCHAR iotCloudDeviceToken[MAX_PATH];
	WCHAR iotCloudProtocol[MAX_PATH];
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}IOT_CLOUD_INFOW, *LPIOT_CLOUD_INFOW;

typedef struct _IOT_CLOUD_INFOA
{
	CHAR iotCloudEndpointAddr[MAX_PATH];
	CHAR iotCloudDeviceToken[MAX_PATH];
	CHAR iotCloudProtocol[MAX_PATH];
	LPVOID lpReserved[4];				/**<  Reserved for future.*/
}IOT_CLOUD_INFOA, *LPIOT_CLOUD_INFOA;

#ifdef UNICODE
typedef IOT_CLOUD_INFOW IOT_CLOUD_INFO;
typedef LPIOT_CLOUD_INFOW LPIOT_CLOUD_INFO;
#else
typedef IOT_CLOUD_INFOA IOT_CLOUD_INFO;
typedef LPIOT_CLOUD_INFOA LPIOT_CLOUD_INFO;
#endif


/**
* SMARTLens Parameters
*/
typedef struct _SMART_ALGORITHM_PARAMS
{
	SMART_ALGORITHM_SELECTOR SMARTAlgorithmSelector;
	LPVOID reserved[2];

}SMART_ALGORITHM_PARAMS,*LPSMART_ALGORITHM_PARAMS;

//RFIDAPI_STRUCT_H
#ifdef __cplusplus
}
#endif
#endif //RFIDAPI_STRUCT_H
