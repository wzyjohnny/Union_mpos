/*
***********************************************************************************************
版权说明：
版本号：
生成日期：
作者：
内容：
功能：
与其它文件的关系：
修改日志：
***********************************************************************************************
*/
#ifndef _MPOS_CMDAPI_H_
#define _MPOS_CMDAPI_H_
#include "NDK.h"
#include "mpos_api_frame.h"

#define WEAK_DEFAULT __attribute__ ((weak, alias("ME_DefaultCmd")))
/************************************************************************************************
* LCD
************************************************************************************************/
extern int CommandParse_ClearScreen(unsigned char* pbuf,  int buf_len, pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetDispMode(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetRefreshMode(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern  int CommandParse_SetLcdXY(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetLcdXY(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetLcdAttr(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_DispImage(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_DrawLine(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_DrawRectangle(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Refresh(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetBacklight(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetFontSize(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetFontColor(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Dispstr(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_LoadMenu(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* KB
************************************************************************************************/
extern int CommandParse_KbHit(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_KbGetCode(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_KbGetString(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* 文件系统
************************************************************************************************/
extern int CommandParse_OpenRecords(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_GetRecordNum(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_WriteRecord(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_GetRecord(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_ModifyRecord(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
/************************************************************************************************
* 磁卡
************************************************************************************************/
extern int CommandParse_OpenCard(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_CloseCard(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetMagCardAuthen(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetMagCard_ENC(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* IC
************************************************************************************************/
extern int CommandParse_IccSetType(unsigned char *pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_IccDetect(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_IccPowerUp(unsigned char* pbuf,  int buf_len, pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_IccPowerDown(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Iccrw(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* 非接
************************************************************************************************/
extern int CommandParse_RFIDPowerup(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_RFIDPowerDown(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_RFIDAPDU(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1StoreKey(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1LoadKey(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1Auth(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1AuthKey(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1ReadBlock(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1WriteBlock(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1Increment(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_M1Decrement(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* 安全认证
************************************************************************************************/
extern int CommandParse_ReadDeviceInformation(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_GetRandomNumber(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_WriteEquipmentInformation(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_DeviceAuth(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_MutualAuth1(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_MutualAuth2(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_MutualAuth3(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
extern int CommandParse_SetGeoPosition(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;

extern int CommandParse_UnionPayCall(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;
//extern int UnionPay_Recv(unsigned char* pbuf,  int buf_len, const pSysParam_t pTParam) WEAK_DEFAULT;


/************************************************************************************************
* 密码键盘
************************************************************************************************/
extern int CommandParse_PasswordInput(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_LoadMKey(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_DataEncryptionOrDecryption(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_DataMac(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_LoadWKey(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_LoadDukpt(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* 打印
************************************************************************************************/
extern int CommandParse_PrnInit(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetPrnStatus(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_PaperSkip(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetFontType(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetLineSpace(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetGreyScale(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetFont(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Setting(unsigned char * pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_PrintNormal(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* pboc
************************************************************************************************/
extern int CommandParse_PbocCapk_Set(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_PbocAid_Set(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_TranPro_Set(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Pboc_GetData(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Pboc_Standard(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Pboc_SencAuth(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Pboc_End(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_QPboc_Pboc(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_Pboc_GetLastWaterinfo(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* 终端管理类
************************************************************************************************/
extern int CommandParse_Buzzer(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_BlinkingLights(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_ScanStringInstruction(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetDateTime(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetDateTime(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_SetTerminalParam(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetTerminalParam(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_CancelReset(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_LoadApp(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_CommTest(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_ShutDown(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_DownloadMode(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
//extern int CommandParse_Bat_per(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* ME30
************************************************************************************************/
extern int CommandParse_OpenCard_ME30(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_GetMagCard_ME30(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_PasswordInput_ME30(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_LoadKey_ME30(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
/************************************************************************************************
* 生产
************************************************************************************************/
extern int CommandParse_CommMode_ME30(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_BTDisconnect(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_CheckData(puchar pbuf,  int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
extern int CommandParse_ReadDeviceInfoPro(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;
//extern int CommandParse_SetSN(unsigned char* pbuf, int buf_len, const pSysParam_t pTParam)WEAK_DEFAULT;


#endif
