
 /*****************************************************************************
 *                Copyrights ©2010-2020 Zebra Technologies Corp.
 *                          All rights reserved
 * 
 *  
 *****************************************************************************
 */

/**
 *****************************************************************************
 ** 
 ** @file	rfidapi.h
 ** 
 ** @brief	RFID API3 main header file and defines external interface
 **			for RFID API3 Library
 **
 **
 ****************************************************************************/
#ifndef RFIDCAPI3_H
#define RFIDCAPI3_H

#include "rfidapiTypes.h"
#include "rfidapiConstants.h"
#include "rfidapiStructs.h"
#include "rfidapiErrors.h"


#if defined(__cplusplus)
extern "C"
{
#endif


RFID_STATUS WINAPI RFID_ConnectW(
		RFID_HANDLE32* pReaderHandle, 
		WCHAR* pHostName,  
		UINT32 port,
		UINT32 timeoutMilliseconds,
		LPCONNECTION_INFO lpConnectionInfo);

RFID_STATUS WINAPI RFID_ConnectA(
		RFID_HANDLE32* pReaderHandle, 
		CHAR* pHostName,  
		UINT32 port,
		UINT32 timeoutMilliseconds,
		LPCONNECTION_INFO lpConnectionInfo);

#ifdef UNICODE
#define RFID_Connect RFID_ConnectW
#else
#define RFID_Connect RFID_ConnectA
#endif

RFID_STATUS WINAPI RFID_Reconnect(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_AcceptConnection(RFID_HANDLE32* pReaderHandle,
		SOCKET_HANDLE readerSocket,
		LPSERVER_INFO lpServerInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_Disconnect(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_GetDllVersionInfoW(
		LPVERSION_INFOW lpVersionInfo);

RFID_STATUS WINAPI RFID_GetDllVersionInfoA(
		LPVERSION_INFOA lpVersionInfo);

#ifdef UNICODE
#define RFID_GetDllVersionInfo RFID_GetDllVersionInfoW
#else
#define RFID_GetDllVersionInfo RFID_GetDllVersionInfoA
#endif

RFID_STATUS WINAPI RFID_GetReaderCapsW(
		RFID_HANDLE32 readerHandle, 
		LPREADER_CAPSW lpReaderCaps);

RFID_STATUS WINAPI RFID_GetReaderCapsA(
		RFID_HANDLE32 readerHandle, 
		LPREADER_CAPSA lpReaderCaps);
#ifdef UNICODE
#define RFID_GetReaderCaps RFID_GetReaderCapsW
#else
#define RFID_GetReaderCaps RFID_GetReaderCapsA
#endif

RFID_STATUS WINAPI RFID_GetSledBatteryStatus( 
        RFID_HANDLE32 readerHandle, 
        LPSLED_BATTERY_STATUS lpSledBatteryStatus);

RFID_STATUS WINAPI RFID_GetAntennaConfig(
		RFID_HANDLE32 readerHandle, 
		UINT16 antennaID,
		UINT16* pReceiveSensitivityIndex, 
		UINT16* pTransmitPowerIndex, 
		UINT16* pTransmitFrequencyIndex);

RFID_STATUS WINAPI RFID_SetAntennaConfig(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID, 
		UINT16 receiveSensitivityIndex, 
		UINT16 transmitPowerIndex, 
		UINT16 transmitFrequencyIndex);

RFID_STATUS WINAPI RFID_SetAntennaRfConfig(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID,
		LPANTENNA_RF_CONFIG lpAntennaRfConfig);

RFID_STATUS WINAPI RFID_GetAntennaRfConfig(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID,
		LPANTENNA_RF_CONFIG lpAntennaRfConfig);

RFID_STATUS WINAPI RFID_GetPhysicalAntennaProperties(
		RFID_HANDLE32 readerHandle, 
		UINT16 antennaID, 
		BOOLEAN* pStatus, 
		UINT32* pAntennaGain);

RFID_STATUS WINAPI RFID_ResetConfigToFactoryDefaults(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_SaveLlrpConfig(
		RFID_HANDLE32 readerHandle,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_GetSaveLlrpConfigStatus(
		RFID_HANDLE32 readerHandle,
		BOOLEAN* pSaveStatus);

RFID_STATUS WINAPI RFID_SetGPOState(
		RFID_HANDLE32 readerHandle, 
		UINT32 portNumber, 
		BOOLEAN gpoState);

RFID_STATUS WINAPI RFID_GetGPOState(
		RFID_HANDLE32 readerHandle, 
		UINT32 portNumber, 
		BOOLEAN* pGPOState);
		
RFID_STATUS WINAPI RFID_GetGPIState(
		RFID_HANDLE32 readerHandle, 
		UINT32 portNumber, 
		BOOLEAN* pGPIEnable, 
		GPI_PORT_STATE* pState);

RFID_STATUS WINAPI RFID_EnableGPIPort(
		RFID_HANDLE32 readerHandle, 
		UINT32 portNumber, 
		BOOLEAN gpiEnable);

RFID_STATUS WINAPI RFID_GetSingulationControl(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID, 
		LPSINGULATION_CONTROL lpSingulationControl);

RFID_STATUS WINAPI RFID_SetSingulationControl(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID, 
		LPSINGULATION_CONTROL lpSingulationControl);

RFID_STATUS WINAPI RFID_GetRFMode(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID,  
		UINT32* pRfModeTableIndex,
		UINT32* pTari);

RFID_STATUS WINAPI RFID_SetRFMode(
		RFID_HANDLE32 readerHandle,
		UINT16 antennaID,  
		UINT32 rfModeTableIndex,
		UINT32 tari);

RFID_STATUS WINAPI RFID_GetRadioPowerState(
		RFID_HANDLE32 readerHandle,
		BOOLEAN* pPowerState);

RFID_STATUS WINAPI RFID_SetRadioPowerState(
		RFID_HANDLE32 readerHandle,
		BOOLEAN powerState);

RFID_STATUS WINAPI RFID_GetDutyCycle(
		RFID_HANDLE32 readerHandle,
		UINT16* pDutyCycleIndex,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_SetDutyCycle(
		RFID_HANDLE32 readerHandle,
		UINT16 dutyCycleIndex,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_RegisterEventNotification(
		RFID_HANDLE32 readerHandle, 
		RFID_EVENT_TYPE eventType, 
		EVENT_HANDLE notifyObject);
		
typedef void (WINAPI *RfidEventCallbackFunction)(
		RFID_HANDLE32 readerHandle, 
		RFID_EVENT_TYPE eventType);

RFID_STATUS WINAPI RFID_RegisterEventNotificationCallback(
		RFID_HANDLE32 readerHandle,
		RFID_EVENT_TYPE *pEvents,
		UINT32 numEvents,
		RfidEventCallbackFunction pFnRfidEventCallbackFunction,
		LPVOID rsvd1,
		LPVOID rsvd2		
		);

RFID_STATUS WINAPI RFID_SetEventNotificationParams(
		RFID_HANDLE32 readerHandle, 
		RFID_EVENT_TYPE eventType, 
		STRUCT_HANDLE pEventParams);

RFID_STATUS WINAPI RFID_DeregisterEventNotification(
		RFID_HANDLE32 readerHandle, 
		RFID_EVENT_TYPE eventType);

RFID_STATUS WINAPI RFID_GetEventData(
		RFID_HANDLE32 readerHandle, 
		RFID_EVENT_TYPE eventType, 
		STRUCT_HANDLE pEventData);

RFID_STATUS WINAPI RFID_AddPreFilter(
		RFID_HANDLE32 readerHandle, 
		UINT16 antennaID,
		LPPRE_FILTER lpPreFilter,
		UINT32* pFilterIndex);

RFID_STATUS WINAPI RFID_DeletePreFilter(
		RFID_HANDLE32 readerHandle, 
		UINT16 antennaID,
		UINT32 filterIndex);

RFID_STATUS WINAPI RFID_GetTagStorageSettings(
		RFID_HANDLE32 readerHandle, 
		LPTAG_STORAGE_SETTINGS lpTagStorageSettings);

RFID_STATUS WINAPI RFID_SetTagStorageSettings(
		RFID_HANDLE32 readerHandle, 
		LPTAG_STORAGE_SETTINGS lpTagStorageSettings);

LPTAG_DATA  WINAPI RFID_AllocateTag(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_DeallocateTag(
		RFID_HANDLE32 readerHandle, 
		LPTAG_DATA lpTagData);

RFID_STATUS WINAPI RFID_NXPBrandCheck(
		RFID_HANDLE32 readerHandle,
		LPPOST_FILTER lpPostFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTRIGGER_INFO lpTriggerInfo,
		UINT16 brandID);

RFID_STATUS WINAPI RFID_NXPStopBrandCheck(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_PerformInventory(
		RFID_HANDLE32 readerHandle,
		LPPOST_FILTER lpPostFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTRIGGER_INFO lpTriggerInfo,
		LPSMART_ALGORITHM_PARAMS lpSmartAlgorithmParams);

RFID_STATUS WINAPI RFID_StopInventory(
		RFID_HANDLE32 readerHandle);

LPRFSURVEY_DATA  WINAPI RFID_AllocateRFSurvey(
	RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_DeallocateRFSurvey(
	RFID_HANDLE32 readerHandle,
	LPRFSURVEY_DATA pRFSurveyData);

RFID_STATUS WINAPI RFID_StartRFSurvey(
	RFID_HANDLE32 readerHandle,
	UINT16 antennaID,
	LPRFSURVEY_STOP_TRIGGER lpRfSurveyStopTriggerInfo,
	UINT32 startFrequency,
	UINT32 endFrequency,
	LPVOID rsvd);

RFID_STATUS WINAPI RFID_StopRFSurvey(
	RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_GetRFSurvey(
	RFID_HANDLE32 readerHandle,
	LPRFSURVEY_DATA lpRFSurveyData);

RFID_STATUS WINAPI RFID_PerformTagLocationing(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd,
		LPVOID rsvd1);

RFID_STATUS WINAPI RFID_StopTagLocationing(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_PerformZoneInventory(
		RFID_HANDLE32 readerHandle,
		LPPOST_FILTER lpPostFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTRIGGER_INFO lpTriggerInfo,
		LPZONE_SEQUENCE lpZoneSequence,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_AddZoneW(
		RFID_HANDLE32 readerHandle,
		UINT16 zoneId,
		LPZONE_CONFIGW zoneConfig,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_AddZoneA(
		RFID_HANDLE32 readerHandle,
		UINT16 zoneId,
		LPZONE_CONFIGA zoneConfig,
		LPVOID rsvd);

#ifdef UNICODE
#define RFID_AddZone RFID_AddZoneW
#else
#define RFID_AddZone RFID_AddZoneA
#endif

RFID_STATUS WINAPI RFID_DeleteZone(
		RFID_HANDLE32 readerHandle,
		UINT32 zoneId,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_GetReadTag (
		RFID_HANDLE32 readerHandle, 
		LPTAG_DATA lpTagData);

RFID_STATUS WINAPI RFID_PurgeTags (
		RFID_HANDLE32 readerHandle,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_Read(
		RFID_HANDLE32 readerHandle, 
		UINT8* pTagID, 
		UINT32 tagIDLength,
		LPREAD_ACCESS_PARAMS lpReadAccessParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo, 
		LPTAG_DATA lpTagData,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_Write(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPWRITE_ACCESS_PARAMS lpWriteAccessParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_WriteTagID(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPWRITE_SPECIFIC_FIELD_ACCESS_PARAMS lpWriteSpecificFieldAccessParams,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_WriteKillPassword(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPWRITE_SPECIFIC_FIELD_ACCESS_PARAMS lpWriteSpecificFieldAccessParams,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_WriteAccessPassword(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPWRITE_SPECIFIC_FIELD_ACCESS_PARAMS lpWriteSpecificFieldAccessParams,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);


RFID_STATUS WINAPI RFID_Kill(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagData,
		UINT32 tagDataLength,
		LPKILL_ACCESS_PARAMS lpKillParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_Lock(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagData,
		UINT32 tagDataLength,
		LPLOCK_ACCESS_PARAMS lpLockAccessParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_BlockWrite(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPWRITE_ACCESS_PARAMS lpWriteAccessParams, 
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_BlockErase(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPBLOCK_ERASE_ACCESS_PARAMS lpBlkEraseAccessParams, 
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_Recommission(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPRECOMMISSION_ACCESS_PARAMS lpRecommissionParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_BlockPermalock(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
        UINT32 tagIDLength,
		LPBLOCK_PERMALOCK_ACCESS_PARAMS lpBlkPermalockAccessParams, 
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTAG_DATA lpTagData,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_Authenticate(
	RFID_HANDLE32 readerHandle,
	UINT8* pTagID,
	UINT32 tagIDLength,
	LPAUTHENTICATE_ACCESS_PARAMS lpAuthenticateAccessParams,
	LPACCESS_FILTER lpAccessFilter,
	LPANTENNA_INFO lpAntennaInfo,
	LPTAG_DATA lpTagData,
	LPVOID rsvd);

RFID_STATUS WINAPI RFID_ReadBuffer(
	RFID_HANDLE32 readerHandle,
	UINT8* pTagID,
	UINT32 tagIDLength,
	LPREADBUFFER_ACCESS_PARAMS lpReadBufferAccessParams,
	LPACCESS_FILTER lpAccessFilter,
	LPANTENNA_INFO lpAntennaInfo,
	LPTAG_DATA lpTagData,
	LPVOID rsvd);

RFID_STATUS WINAPI RFID_Untraceable(
	RFID_HANDLE32 readerHandle,
	UINT8* pTagID,
	UINT32 tagIDLength,
	LPUNTRACEABLE_ACCESS_PARAMS lpUntraceableAccessParams,
	LPACCESS_FILTER lpAccessFilter,
	LPANTENNA_INFO lpAntennaInfo,
	LPTAG_DATA lpTagData,
	LPVOID rsvd);

RFID_STATUS WINAPI RFID_Crypto(
	RFID_HANDLE32 readerHandle,
	UINT8* pTagID,
	UINT32 tagIDLength,
	LPCRYPTO_ACCESS_PARAMS lpCryptoAccessParams,
	LPACCESS_FILTER lpAccessFilter,
	LPANTENNA_INFO lpAntennaInfo,
	LPTAG_DATA lpTagData,
	LPVOID rsvd);

RFID_STATUS WINAPI RFID_InitializeAccessSequence(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_AddOperationToAccessSequence(
		RFID_HANDLE32 readerHandle, 
		LPOP_CODE_PARAMS lpOpCodeParams,
		UINT32* pOpCodeIndex);

RFID_STATUS WINAPI RFID_DeleteOperationFromAccessSequence(
		RFID_HANDLE32 readerHandle, 
		UINT32 opCodeIndex);

RFID_STATUS WINAPI RFID_DeinitializeAccessSequence(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_PerformAccessSequence(
		RFID_HANDLE32 readerHandle,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTRIGGER_INFO lpTriggerInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_StopAccessSequence(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_GetLastAccessResult(
		RFID_HANDLE32 readerHandle,
		UINT32* pSuccessCount,
		UINT32* pFailureCount);

#ifdef UNICODE
#define RFID_GetCustomParams RFID_GetCustomParamsW
#else
#define RFID_GetCustomParams RFID_GetCustomParamsA
#endif

RFID_STATUS WINAPI RFID_GetCustomParamsW(
        RFID_HANDLE32 readerHandle, 
        RFID_PARAM_TYPE paramType, 
        WCHAR* pBuffer);

RFID_STATUS WINAPI RFID_GetCustomParamsA(
        RFID_HANDLE32 readerHandle, 
        RFID_PARAM_TYPE paramType, 
        CHAR* pBuffer);

#ifdef UNICODE
#define RFID_SetCustomParams RFID_SetCustomParamsW
#else
#define RFID_SetCustomParams RFID_SetCustomParamsA
#endif

RFID_STATUS WINAPI RFID_SetCustomParamsW(
        RFID_HANDLE32 readerHandle, 
        RFID_PARAM_TYPE paramType, 
        WCHAR* pBuffer);

RFID_STATUS WINAPI RFID_SetCustomParamsA(
        RFID_HANDLE32 readerHandle, 
        RFID_PARAM_TYPE paramType, 
        CHAR* pBuffer);

RFID_STATUS WINAPI RFID_NXPSetEAS( 
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPNXP_SET_EAS_PARAMS lpNXPSetEASParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_NXPReadProtect( 
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPNXP_READ_PROTECT_PARAMS lpNXPReadProtectParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_NXPResetReadProtect(
		RFID_HANDLE32 readerHandle,
		LPNXP_RESET_READ_PROTECT_PARAMS lpNXPResetReadProtectParams,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_PerformNXPEASScan(
		RFID_HANDLE32 readerHandle,
		LPANTENNA_INFO lpAntennaInfo,
		LPTRIGGER_INFO lpTriggerInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_StopNXPEASScan(
		RFID_HANDLE32 readerHandle);

RFID_STATUS WINAPI RFID_FujitsuChangeWordLock(
		RFID_HANDLE32 rfidHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_CHANGE_WORDLOCK_ACCESS_PARAMS lpChangeWordLockParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuChangeBlockLock(  
        RFID_HANDLE32 readerHandle,
 		UINT8* pTagID, 
 		UINT32 tagIDLength, 
		LPFUJITSU_CHANGE_BLOCKLOCK_ACCESS_PARAMS lpChangeBlockLockParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuReadBlockLock(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_READ_BLOCKLOCK_ACCESS_PARAMS lpReadBlockLockParams,
		LPACCESS_FILTER lpAccessFilter, 
		LPANTENNA_INFO lpAntennaInfo,
		LPTAG_DATA lpTagData,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuBurstWrite(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_BURST_WRITE_ACCESS_PARAMS  lpBurstWriteParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTAG_DATA lpTagData,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuBurstErase(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_BURST_ERASE_ACCESS_PARAMS lpBurstEraseParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTAG_DATA lpTagData,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuChangeBlockOrAreaGroupPassword(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_ACCESS_PARAMS lpChangeBlockOrAreaGroupPasswordParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuAreaReadLock(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_AREA_READLOCK_ACCESS_PARAMS lpAreaReadLockParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuAreaWriteLock(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_AREA_WRITELOCK_ACCESS_PARAMS lpAreaWriteLockParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_FujitsuAreaWriteLockWOPassword(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPFUJITSU_AREA_WRITELOCK_WOPASSWORD_ACCESS_PARAMS  lpAreaWriteLockWOPasswordParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_ImpinjQTWrite(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID,
		UINT32 tagIDLength,
		LPIMPINJ_QT_WRITE_PARAMS lpQTWriteParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_ImpinjQTRead(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
		UINT32 tagIDLength,
		LPIMPINJ_QT_READ_PARMS lpQTReadParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTAG_DATA lpTagData, 
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_NXPChangeConfig(
		RFID_HANDLE32 readerHandle,
		UINT8* pTagID, 
		UINT32 tagIDLength,
		LPNXP_CHANGE_CONFIG_PARAMS lpNXPChangeConfigParams,
		LPACCESS_FILTER lpAccessFilter,
		LPANTENNA_INFO lpAntennaInfo,
		LPTAG_DATA lpTagData, 
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_LoginW(
		RFID_HANDLE32* pReaderManagementHandle,
		LPLOGIN_INFOW lpLoginInfo,
		READER_TYPE readerType,
		BOOLEAN secureMode,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_LoginA(
		RFID_HANDLE32* pReaderManagementHandle,
		LPLOGIN_INFOA lpLoginInfo,
		READER_TYPE readerType,
		BOOLEAN secureMode,
		LPVOID rsvd);

#ifdef UNICODE
#define RFID_Login RFID_LoginW
#else
#define RFID_Login RFID_LoginA
#endif

RFID_STATUS WINAPI RFID_Logout(
		RFID_HANDLE32 readerManagementHandle);

RFID_STATUS WINAPI RFID_EnableReadPoint(
		RFID_HANDLE32 readerManagementHandle,
		UINT16 antennaID,
		BOOLEAN status);

RFID_STATUS WINAPI RFID_GetReadPointStatus(
		RFID_HANDLE32 readerManagementHandle,
		UINT16 antennaID,
		BOOLEAN* pStatus);

RFID_STATUS WINAPI RFID_GetReadPointConnectStatus(
	RFID_HANDLE32 readerManagementHandle,
	UINT16 antennaID,
	BOOLEAN* pConnectStatus);

RFID_STATUS WINAPI RFID_SetAntennaMode(
		RFID_HANDLE32 readerManagementHandle,
		ANTENNA_MODE antennaMode);

RFID_STATUS WINAPI RFID_GetAntennaMode(
		RFID_HANDLE32 readerManagementHandle,
		ANTENNA_MODE* pAntennaMode);

RFID_STATUS WINAPI RFID_ExportProfileToReaderW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pProfileName,
		WCHAR* pProfilePath,
		BOOLEAN activation);

RFID_STATUS WINAPI RFID_ExportProfileToReaderA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pProfileName,
		CHAR* pProfilePath,
		BOOLEAN activation);

#ifdef UNICODE
#define RFID_ExportProfileToReader RFID_ExportProfileToReaderW
#else
#define RFID_ExportProfileToReader RFID_ExportProfileToReaderA
#endif

RFID_STATUS WINAPI RFID_TurnOffRadioWhenIdle(
		RFID_HANDLE32 readerManagementHandle,
		INT32 idleTimeoutSeconds);

RFID_STATUS WINAPI RFID_GetRadioIdleTimeout(
		RFID_HANDLE32 readerManagementHandle,
		INT32* pIdleTimeoutSeconds);

RFID_STATUS WINAPI RFID_InstallUserAppW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pPathToPackage);

RFID_STATUS WINAPI RFID_InstallUserAppA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pPathToPackage);

#ifdef UNICODE
#define RFID_InstallUserApp RFID_InstallUserAppW
#else
#define RFID_InstallUserApp RFID_InstallUserAppA
#endif

RFID_STATUS WINAPI RFID_UninstallUserAppW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pAppName);

RFID_STATUS WINAPI RFID_UninstallUserAppA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pAppName);


#ifdef UNICODE
#define RFID_UninstallUserApp RFID_UninstallUserAppW
#else
#define RFID_UninstallUserApp RFID_UninstallUserAppA
#endif
	
RFID_STATUS WINAPI RFID_StartUserAppW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pAppName);

RFID_STATUS WINAPI RFID_StartUserAppA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pAppName);

#ifdef UNICODE
#define RFID_StartUserApp RFID_StartUserAppW
#else
#define RFID_StartUserApp RFID_StartUserAppA
#endif
	
RFID_STATUS WINAPI RFID_StopUserAppW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pAppName);

RFID_STATUS WINAPI RFID_StopUserAppA(
		RFID_HANDLE32 readerHandle,
		CHAR* pAppName);

#ifdef UNICODE
#define RFID_StopUserApp RFID_StopUserAppW
#else
#define RFID_StopUserApp RFID_StopUserAppA
#endif

RFID_STATUS WINAPI RFID_GetUserAppRunStatusW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pAppName,
		BOOLEAN* pRunStatus);

RFID_STATUS WINAPI RFID_GetUserAppRunStatusA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pAppName,
		BOOLEAN* pRunStatus);

#ifdef UNICODE
#define RFID_GetUserAppRunStatus RFID_GetUserAppRunStatusW
#else
#define RFID_GetUserAppRunStatus RFID_GetUserAppRunStatusA
#endif
	
RFID_STATUS WINAPI RFID_ListUserAppsW(
		RFID_HANDLE32 readerManagementHandle,
		LPUSERAPP_LISTW lpAppList);

RFID_STATUS WINAPI RFID_ListUserAppsA(
		RFID_HANDLE32 readerManagementHandle,
		LPUSERAPP_LISTA lpAppList);

#ifdef UNICODE
#define RFID_ListUserApps RFID_ListUserAppsW
#else
#define RFID_ListUserApps RFID_ListUserAppsA
#endif

RFID_STATUS WINAPI RFID_SetUserAppAutoStartW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pAppName,
		BOOLEAN Enable);

RFID_STATUS WINAPI RFID_SetUserAppAutoStartA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pAppName,
		BOOLEAN Enable);

#ifdef UNICODE
#define RFID_SetUserAppAutoStart RFID_SetUserAppAutoStartW
#else
#define RFID_SetUserAppAutoStart RFID_SetUserAppAutoStartA
#endif

RFID_STATUS WINAPI RFID_ImportProfileFromReaderW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pProfileName,
		WCHAR* pProfilePath);

RFID_STATUS WINAPI RFID_ImportProfileFromReaderA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pProfileName,
		CHAR* pProfilePath);

#ifdef UNICODE
#define RFID_ImportProfileFromReader RFID_ImportProfileFromReaderW
#else
#define RFID_ImportProfileFromReader RFID_ImportProfileFromReaderA
#endif

  
RFID_STATUS WINAPI RFID_GetProfileListW(
		RFID_HANDLE32 readerManagementHandle,
		LPPROFILE_LISTW lpProfileList);

RFID_STATUS WINAPI RFID_GetProfileListA(
		RFID_HANDLE32 readerManagementHandle,
		LPPROFILE_LISTA lpProfileList);

#ifdef UNICODE
#define RFID_GetProfileList RFID_GetProfileListW
#else
#define RFID_GetProfileList RFID_GetProfileListA
#endif 

RFID_STATUS WINAPI RFID_SetActiveProfileW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pProfileName);

RFID_STATUS WINAPI RFID_SetActiveProfileA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pProfileName);

#ifdef UNICODE
#define RFID_SetActiveProfile RFID_SetActiveProfileW
#else
#define RFID_SetActiveProfile RFID_SetActiveProfileA
#endif

RFID_STATUS WINAPI RFID_DeleteProfileW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pProfileName);

RFID_STATUS WINAPI RFID_DeleteProfileA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pProfileName);

#ifdef UNICODE
#define RFID_DeleteProfile RFID_DeleteProfileW
#else
#define RFID_DeleteProfile RFID_DeleteProfileA
#endif

RFID_STATUS WINAPI RFID_UpdateSoftwareW(
		RFID_HANDLE32 readerManagementHandle,
		LPFTPSERVER_INFOW pLocationInfo);

RFID_STATUS WINAPI RFID_UpdateSoftwareA(
		RFID_HANDLE32 readerManagementHandle,
		LPFTPSERVER_INFOA pLocationInfo);

#ifdef UNICODE
#define RFID_UpdateSoftware RFID_UpdateSoftwareW
#else
#define RFID_UpdateSoftware RFID_UpdateSoftwareA
#endif

RFID_STATUS WINAPI RFID_UpdateRadioFirmwareW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pFileName);

RFID_STATUS WINAPI RFID_UpdateRadioFirmwareA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pFileName);

#ifdef UNICODE
#define RFID_UpdateRadioFirmware RFID_UpdateRadioFirmwareW
#else
#define RFID_UpdateRadioFirmware RFID_UpdateRadioFirmwareA
#endif
RFID_STATUS WINAPI RFID_AcquireLicenseOnLineW(
	RFID_HANDLE32 readerManagementHandle,
	WCHAR* pLicenseServerURL,
	WCHAR* pActivationID, 
	BOOLEAN installApp,
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_AcquireLicenseOnLineA(
	RFID_HANDLE32 readerManagementHandle,
	CHAR* pLicenseServerURL,
	CHAR* pActivationID,
	BOOLEAN installApp,
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_AcquireLicenseOffLineW(
	RFID_HANDLE32 readerManagementHandle,
	WCHAR* pLicenseBinFileLocation,
	BOOLEAN installApp,
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_AcquireLicenseOffLineA(
	RFID_HANDLE32 readerManagementHandle,
	CHAR* pLicenseBinFileLocation,
	BOOLEAN installApp,
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_ReturnLicenseW(
	RFID_HANDLE32 readerManagementHandle,
	WCHAR* pLicenseServerURL,
	WCHAR* pActivationID,
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_ReturnLicenseA(
	RFID_HANDLE32 readerManagementHandle,
	CHAR* pLicenseServerURL,
	CHAR* pActivationID, 
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_GetAvailableLicensesW(
	RFID_HANDLE32 readerManagementHandle,
	LPLICENSE_LISTW lpLicenseList,
	LPVOID rsvd);
RFID_STATUS WINAPI RFID_GetAvailableLicensesA(
	RFID_HANDLE32 readerManagementHandle,
	LPLICENSE_LISTA lpLicenseList,
	LPVOID rsvd);
#ifdef UNICODE
#define RFID_AcquireLicenseOnLine RFID_AcquireLicenseOnLineW
#define RFID_AcquireLicenseOffLine RFID_AcquireLicenseOffLineW
#define RFID_ReturnLicense	RFID_ReturnLicenseW
#define RFID_GetAvailableLicenses RFID_GetAvailableLicensesW
#else
#define RFID_AcquireLicenseOnLine RFID_AcquireLicenseOnLineA
#define RFID_AcquireLicenseOffLine RFID_AcquireLicenseOffLineA
#define RFID_ReturnLicense RFID_ReturnLicenseA
#define RFID_GetAvailableLicenses RFID_GetAvailableLicensesA
#endif

RFID_STATUS WINAPI RFID_UpdateRadioConfigW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* pFileName);

RFID_STATUS WINAPI RFID_UpdateRadioConfigA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR* pFileName);

#ifdef UNICODE
#define RFID_UpdateRadioConfig RFID_UpdateRadioConfigW
#else
#define RFID_UpdateRadioConfig RFID_UpdateRadioConfigA
#endif

RFID_STATUS WINAPI RFID_GetUpdateStatusW(
		RFID_HANDLE32 readerManagementHandle,
		LPUPDATE_STATUSW lpStatus);

RFID_STATUS WINAPI RFID_GetUpdateStatusA(
		RFID_HANDLE32 readerManagementHandle,
		LPUPDATE_STATUSA lpStatus);

#ifdef UNICODE
#define RFID_GetUpdateStatus RFID_GetUpdateStatusW
#else
#define RFID_GetUpdateStatus RFID_GetUpdateStatusA
#endif

RFID_STATUS WINAPI RFID_Restart(
		RFID_HANDLE32 readerManagementHandle);

RFID_STATUS WINAPI RFID_GetSystemInfoW(
		RFID_HANDLE32 readerManagementHandle,
		LPREADER_SYSTEM_INFOW lpSystemInfo);

RFID_STATUS WINAPI RFID_GetSystemInfoA(
		RFID_HANDLE32 readerManagementHandle,
		LPREADER_SYSTEM_INFOA lpSystemInfo);

RFID_STATUS WINAPI RFID_GetNetworkInterfaceSettingsW(
	RFID_HANDLE32 readerManagementHandle,
	LPREADER_NETWORK_INFOW lpnetworkInfo);

RFID_STATUS WINAPI RFID_GetNetworkInterfaceSettingsA(
	RFID_HANDLE32 readerManagementHandle,
	LPREADER_NETWORK_INFOA lpnetworkInfo);
RFID_STATUS WINAPI RFID_SetNetworkInterfaceSettingsW(
	RFID_HANDLE32 readerManagementHandle,
	LPREADER_NETWORK_INFOW lpnetworkInfo);

RFID_STATUS WINAPI RFID_SetNetworkInterfaceSettingsA(
	RFID_HANDLE32 readerManagementHandle,
	LPREADER_NETWORK_INFOA lpnetworkInfo);
#ifdef UNICODE
#define RFID_GetSystemInfo RFID_GetSystemInfoW
#define RFID_GetNetworkInterfaceSettings RFID_GetNetworkInterfaceSettingsW
#define RFID_SetNetworkInterfaceSettings RFID_SetNetworkInterfaceSettingsW
#else
#define RFID_GetSystemInfo RFID_GetSystemInfoA
#define RFID_GetNetworkInterfaceSettings RFID_GetNetworkInterfaceSettingsA
#define RFID_SetNetworkInterfaceSettings RFID_SetNetworkInterfaceSettingsA
#endif

RFID_STATUS WINAPI RFID_GetHealthStatus(
		RFID_HANDLE32 readerManagementHandle, 
		SERVICE_ID serviceID, 
		HEALTH_STATUS* pHealthStatus, 
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_GetUSBOperationMode(
		RFID_HANDLE32 readerManagementHandle, 
		LPUSB_OPERATION_INFO lpUsbOperationInfo);


RFID_STATUS WINAPI RFID_SetUSBOperationMode(
		RFID_HANDLE32 readerManagementHandle,
		LPUSB_OPERATION_INFO lpUsbOperationInfo);

RFID_STATUS WINAPI RFID_GetGPIDebounceTime(
		RFID_HANDLE32 readerManagementHandle, 
		UINT32 *pDebounceTimeMilliseconds,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_SetGPIDebounceTime(
		RFID_HANDLE32 readerManagementHandle, 
		UINT32 debounceTimeMilliseconds,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_SetUserLED(
		RFID_HANDLE32 readerManagementHandle, 
		LPLED_INFO lpLedInfo);

RFID_STATUS WINAPI RFID_GetLocalTime(
		RFID_HANDLE32 readerManagementHandle, 
		LPSYSTEMTIME lpSystemTime);

RFID_STATUS WINAPI RFID_SetLocalTime(
		RFID_HANDLE32 readerManagementHandle, 
		LPSYSTEMTIME lpSystemTime);

RFID_STATUS WINAPI RFID_GetReaderStats(
		RFID_HANDLE32 readerManagementHandle, 
		UINT16 antennaID,
		LPREADER_STATS lpReaderStats);

RFID_STATUS WINAPI RFID_ClearReaderStats(
		RFID_HANDLE32 readerManagementHandle);

RFID_STATUS WINAPI RFID_SetReaderInfoW(
		RFID_HANDLE32 readerManagementHandle,
		LPREADER_INFOW lpReaderInfo);

RFID_STATUS WINAPI RFID_SetReaderInfoA(
		RFID_HANDLE32 readerManagementHandle,
		LPREADER_INFOA lpReaderInfo);

#ifdef UNICODE
#define RFID_SetReaderInfo RFID_SetReaderInfoW
#else
#define RFID_SetReaderInfo RFID_SetReaderInfoA
#endif

RFID_STATUS WINAPI RFID_GetReaderInfoW(
		RFID_HANDLE32 readerManagementHandle,
		LPREADER_INFOW lpReaderInfo);

RFID_STATUS WINAPI RFID_GetReaderInfoA(
		RFID_HANDLE32 readerManagementHandle,
		LPREADER_INFOA lpReaderInfo);

#ifdef UNICODE
#define RFID_GetReaderInfo RFID_GetReaderInfoW
#else
#define RFID_GetReaderInfo RFID_GetReaderInfoA
#endif

RFID_STATUS WINAPI RFID_GetTimeZoneListW(
		RFID_HANDLE32 readerManagementHandle,
		LPTIME_ZONE_LISTW lpTimeZoneList);

RFID_STATUS WINAPI RFID_GetTimeZoneListA(
		RFID_HANDLE32 readerManagementHandle,
		LPTIME_ZONE_LISTA lpTimeZoneList);

#ifdef UNICODE
#define RFID_GetTimeZoneList RFID_GetTimeZoneListW
#else
#define RFID_GetTimeZoneList RFID_GetTimeZoneListA
#endif

RFID_STATUS WINAPI RFID_SetTimeZone(
		RFID_HANDLE32 readerManagementHandle,
		UINT16 timeZoneIndex);

RFID_STATUS WINAPI RFID_GetLLRPConnectionConfigW(
		RFID_HANDLE32 readerManagementHandle,
		LPLLRP_CONNECTION_CONFIGW lpLLRPConnectionConfig);

RFID_STATUS WINAPI RFID_GetLLRPConnectionConfigA(
		RFID_HANDLE32 readerManagementHandle,
		LPLLRP_CONNECTION_CONFIGA lpLLRPConnectionConfig);

#ifdef UNICODE
#define RFID_GetLLRPConnectionConfig RFID_GetLLRPConnectionConfigW
#else
#define RFID_GetLLRPConnectionConfig RFID_GetLLRPConnectionConfigA
#endif

RFID_STATUS WINAPI RFID_SetLLRPConnectionConfigW(
		RFID_HANDLE32 readerManagementHandle,
		LPLLRP_CONNECTION_CONFIGW lpLLRPConnectionConfig);

RFID_STATUS WINAPI RFID_SetLLRPConnectionConfigA(
		RFID_HANDLE32 readerManagementHandle,
		LPLLRP_CONNECTION_CONFIGA lpLLRPConnectionConfig);

#ifdef UNICODE
#define RFID_SetLLRPConnectionConfig RFID_SetLLRPConnectionConfigW
#else
#define RFID_SetLLRPConnectionConfig RFID_SetLLRPConnectionConfigA
#endif

RFID_STATUS WINAPI RFID_GetLLRPConnectionStatusW(
	RFID_HANDLE32 readerManagementHandle,
	LPLLRP_CONNECTION_STATUSW lpLLRPConnectionStatus);

RFID_STATUS WINAPI RFID_GetLLRPConnectionStatusA(
	RFID_HANDLE32 readerManagementHandle,
	LPLLRP_CONNECTION_STATUSA lpLLRPConnectionStatus);

#ifdef UNICODE
#define RFID_GetLLRPConnectionStatus RFID_GetLLRPConnectionStatusW
#else
#define RFID_GetLLRPConnectionStatus RFID_GetLLRPConnectionStatusA
#endif

RFID_STATUS WINAPI RFID_InitiateLLRPConnectionFromReader(
		RFID_HANDLE32 readerManagementHandle,
		LPVOID rsvd);

RFID_STATUS WINAPI RFID_DisconnectLLRPConnectionFromReader(
		RFID_HANDLE32 readerManagementHandle);

RFID_STATUS WINAPI RFID_SetTraceLevel(
		RFID_HANDLE32 readerHandle,
		UINT32 traceLevel);

WCHAR* WINAPI RFID_GetErrorDescriptionW(
		RFID_STATUS errorCode);

CHAR* WINAPI RFID_GetErrorDescriptionA(
		RFID_STATUS errorCode);

#ifdef UNICODE
#define RFID_GetErrorDescription RFID_GetErrorDescriptionW
#else
#define RFID_GetErrorDescription RFID_GetErrorDescriptionA
#endif

RFID_STATUS WINAPI RFID_GetLastErrorInfoW(
		RFID_HANDLE32 readerHandle,
		LPERROR_INFOW lpErrorInfo);

RFID_STATUS WINAPI RFID_GetLastErrorInfoA(
		RFID_HANDLE32 readerHandle,
		LPERROR_INFOA lpErrorInfo);

#ifdef UNICODE
#define RFID_GetLastErrorInfo RFID_GetLastErrorInfoW
#else
#define RFID_GetLastErrorInfo RFID_GetLastErrorInfoA
#endif

RFID_STATUS WINAPI RFID_SetCableLossCompensation(
		RFID_HANDLE32 readerManagementHandle,
		UINT16 nTotalAntennasToSet,
		LPCABLE_LOSS_COMPENSATION* lppCableLossCompensation);
		
RFID_STATUS WINAPI RFID_GetCableLossCompensation(
		RFID_HANDLE32 readerManagementHandle,
		LPCABLE_LOSS_COMPENSATION lpCableLossCompensation);

RFID_STATUS WINAPI RFID_GetSupportedRegionListW(
		RFID_HANDLE32 readerManagementHandle,
		REGION_LISTW *SupportedRegions);

RFID_STATUS WINAPI RFID_GetSupportedRegionListA(
		RFID_HANDLE32 readerManagementHandle,
		REGION_LISTA *SupportedRegions);

RFID_STATUS WINAPI RFID_GetRegionStandardListW (
		RFID_HANDLE32 readerManagementHandle,
		WCHAR* RegionName,
		STANDARD_INFO_LISTW *RegionStds);

RFID_STATUS WINAPI RFID_GetRegionStandardListA (
		RFID_HANDLE32 readerManagementHandle,
		CHAR* RegionName,
		STANDARD_INFO_LISTA *RegionStds);

RFID_STATUS WINAPI RFID_SetActiveRegionW(
		RFID_HANDLE32 readerManagementHandle,
		WCHAR*  RegionName,
		WCHAR* CommunicationStandardName);

RFID_STATUS WINAPI RFID_SetActiveRegionA(
		RFID_HANDLE32 readerManagementHandle,
		CHAR*  RegionName,
		CHAR* CommunicationStandardName);

RFID_STATUS WINAPI RFID_SetFrequencySetting (
		RFID_HANDLE32 readerManagementHandle,
		BOOLEAN bEnableFrequencyHopping,
		CHANNEL_LIST* lpChannelTable);

RFID_STATUS WINAPI RFID_GetActiveRegionInfoW(
		RFID_HANDLE32 readerManagementHandle,
		ACTIVE_REGION_INFOW* ActiveRegionInfo);

RFID_STATUS WINAPI RFID_GetActiveRegionInfoA(
		RFID_HANDLE32 readerManagementHandle,
		ACTIVE_REGION_INFOA* ActiveRegionInfo);

RFID_STATUS WINAPI RFID_GetActiveRegionStandardInfoW(
		RFID_HANDLE32 readerManagementHandle,
		STANDARD_REGION_INFOW* CurrentActiveRegionStandardInfo);

RFID_STATUS WINAPI RFID_GetActiveRegionStandardInfoA(
		RFID_HANDLE32 readerManagementHandle,
		STANDARD_REGION_INFOA* CurrentActiveRegionStandardInfo);

RFID_STATUS WINAPI RFID_ChangePasswordW(
		RFID_HANDLE32 readerManagementHandle,
		USER_INFOW* lpUserInfo);

RFID_STATUS WINAPI RFID_ChangePasswordA(
		RFID_HANDLE32 readerManagementHandle,
		USER_INFOA* lpUserInfo);

RFID_STATUS WINAPI RFID_GetPowerNegotiationState(
		RFID_HANDLE32 readerManagementHandle,
		BOOLEAN *lpPowerNegotiationInfo);

RFID_STATUS WINAPI RFID_SetPowerNegotiationState(
		RFID_HANDLE32 readerManagementHandle,
		BOOLEAN powerNegotiationInfo);

RFID_STATUS WINAPI RFID_GetPowerNegotiationStatus(
		RFID_HANDLE32 readerManagementHandle,
		POWER_NEGOTIATION_STATUS* lpPowerNegotiationStatus);

RFID_STATUS WINAPI RFID_FactoryReset(
		RFID_HANDLE32 readerManagementHandle);

RFID_STATUS WINAPI RFID_EnterpriseReset(
	RFID_HANDLE32 readerManagementHandle);

RFID_STATUS WINAPI RFID_SetNTPServerW(
		RFID_HANDLE32 readerManagementHandle, 
		LPTIME_SERVER_INFOW lpTimeServerInfo);

RFID_STATUS WINAPI RFID_SetNTPServerA(
		RFID_HANDLE32 readerManagementHandle,
		LPTIME_SERVER_INFOA lpTimeServerInfo);

RFID_STATUS WINAPI RFID_GetNTPServerW(
		RFID_HANDLE32 readerManagementHandle, 
		LPTIME_SERVER_INFOW lpTimeServerInfo);

RFID_STATUS WINAPI RFID_GetNTPServerA(
		RFID_HANDLE32 readerManagementHandle, 
		LPTIME_SERVER_INFOA lpTimeServerInfo);
RFID_STATUS WINAPI RFID_SetWirelessNetworkW(
	RFID_HANDLE32 readerManagementHandle,
	LPWIRELESS_CONFIGURED_PARAMETERSW lpWirelessConfiguredParameters);

RFID_STATUS WINAPI RFID_SetWirelessNetworkA(
	RFID_HANDLE32 readerManagementHandle,
	LPWIRELESS_CONFIGURED_PARAMETERSA lpWirelessConfiguredParameters);
RFID_STATUS WINAPI RFID_SetSysLogServerW(
		RFID_HANDLE32 readerManagementHandle, 
		LPSYSLOG_SERVER_INFOW lpSysLogServerInfo);

RFID_STATUS WINAPI RFID_SetSysLogServerA(
		RFID_HANDLE32 readerManagementHandle, 
		LPSYSLOG_SERVER_INFOA lpSysLogServerInfo);

RFID_STATUS WINAPI RFID_GetSysLogServerW(
		RFID_HANDLE32 readerManagementHandle, 
		LPSYSLOG_SERVER_INFOW lpSysLogServerInfo);

RFID_STATUS WINAPI RFID_GetSysLogServerA(
		RFID_HANDLE32 readerManagementHandle, 
		LPSYSLOG_SERVER_INFOA lpSysLogServerInfo);
RFID_STATUS WINAPI RFID_GetWirelessConfigParametersW(
	RFID_HANDLE32 readerManagementHandle,
	LPWIRELESS_CONFIGURED_PARAMETERSW lpWirelessConfiguredParameters);

RFID_STATUS WINAPI RFID_GetWirelessConfigParametersA(
	RFID_HANDLE32 readerManagementHandle,
	LPWIRELESS_CONFIGURED_PARAMETERSA lpWirelessConfiguredParameters);
	
RFID_STATUS WINAPI RFID_GetTemperatureStatusW(
	RFID_HANDLE32 readerManagementHandle,
	LPTEMPERATURE_STATUSW lpTemperatureStatus);
	
RFID_STATUS WINAPI RFID_GetTemperatureStatusA(
	RFID_HANDLE32 readerManagementHandle,
	LPTEMPERATURE_STATUSA lpTemperatureStatus);

RFID_STATUS WINAPI RFID_GetRadioTransmitDelay(
	RFID_HANDLE32 readerHandle,
	RADIO_TRANSMIT_DELAY_TYPE* pType,
	UINT8* pTime);

RFID_STATUS WINAPI RFID_SetRadioTransmitDelay(
	RFID_HANDLE32 readerHandle,
	RADIO_TRANSMIT_DELAY_TYPE type,
	UINT8 time);

RFID_STATUS WINAPI RFID_GetMLTAlgorithmParameters(
	RFID_HANDLE32 readerHandle,
	MLT_ALGORITHM_PARAMS* pMLTConfiguration);

RFID_STATUS WINAPI RFID_SetMLTAlgorithmParameters(
	RFID_HANDLE32 readerHandle,
	MLT_ALGORITHM_PARAMS MLTConfiguration);
	
RFID_STATUS WINAPI RFID_GetFilesToUpdateW(
	RFID_HANDLE32 readerManagementHandle,
	LPFILE_UPDATE_LISTW lpUpdateFileList);

RFID_STATUS WINAPI RFID_GetFilesToUpdateA(
	RFID_HANDLE32 readerManagementHandle,
	LPFILE_UPDATE_LISTA lpUpdateFileList);

RFID_STATUS WINAPI RFID_UpdatePartition(
	RFID_HANDLE32 readerManagementHandle, 
	UINT16 partitionIndex);

RFID_STATUS WINAPI RFID_GetUpdatePartitionStatusW(
	RFID_HANDLE32 readerManagementHandle,
	LPUPDATE_PARTITION_STATUSW lpStatus);

RFID_STATUS WINAPI RFID_GetUpdatePartitionStatusA(
	RFID_HANDLE32 readerManagementHandle, 
	LPUPDATE_PARTITION_STATUSA lpStatus);

RFID_STATUS WINAPI RFID_UpdateComplete(
	RFID_HANDLE32 readerManagementHandle);

#ifdef UNICODE
#define RFID_GetSupportedRegionList			RFID_GetSupportedRegionListW
#define RFID_GetRegionStandardList			RFID_GetRegionStandardListW
#define RFID_SetActiveRegion				RFID_SetActiveRegionW
#define RFID_GetActiveRegionInfo			RFID_GetActiveRegionInfoW
#define RFID_GetActiveRegionStandardInfo	RFID_GetActiveRegionStandardInfoW
#define RFID_ChangePassword					RFID_ChangePasswordW
#define RFID_SetNTPServer					RFID_SetNTPServerW
#define RFID_GetNTPServer					RFID_GetNTPServerW
#define RFID_SetSysLogServer				RFID_SetSysLogServerW
#define RFID_GetSysLogServer				RFID_GetSysLogServerW
#define	RFID_SetWirelessNetwork				RFID_SetWirelessNetworkW
#define	RFID_GetWirelessConfigParameters	RFID_GetWirelessConfigParametersW
#define RFID_GetTemperatureStatus			RFID_GetTemperatureStatusW
#define RFID_GetFilesToUpdate				RFID_GetFilesToUpdateW
#define RFID_GetUpdatePartitionStatus RFID_GetUpdatePartitionStatusW
#else
#define RFID_GetSupportedRegionList			RFID_GetSupportedRegionListA
#define RFID_GetRegionStandardList			RFID_GetRegionStandardListA
#define RFID_SetActiveRegion				RFID_SetActiveRegionA
#define RFID_GetActiveRegionInfo			RFID_GetActiveRegionInfoA
#define RFID_GetActiveRegionStandardInfo	RFID_GetActiveRegionStandardInfoA
#define RFID_ChangePassword					RFID_ChangePasswordA
#define RFID_SetNTPServer					RFID_SetNTPServerA
#define RFID_GetNTPServer					RFID_GetNTPServerA
#define RFID_SetSysLogServer				RFID_SetSysLogServerA
#define RFID_GetSysLogServer				RFID_GetSysLogServerA
#define	RFID_SetWirelessNetwork				RFID_SetWirelessNetworkA
#define	RFID_GetWirelessConfigParameters	RFID_GetWirelessConfigParametersA
#define RFID_GetTemperatureStatus			RFID_GetTemperatureStatusA
#define RFID_GetFilesToUpdate				RFID_GetFilesToUpdateA
#define RFID_GetUpdatePartitionStatus RFID_GetUpdatePartitionStatusA
#endif

#ifdef __cplusplus
}
#endif

#endif
