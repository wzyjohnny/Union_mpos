#include <stdlib.h>
#include <string.h>
#include "mpos_api_frame.h"
#include "mpos_cmd_api.h"
#include "mpos_command.h"
#include "wdebug.h"
/*
***********************************************************************************************
* 宏定义
***********************************************************************************************
*/




//#define PDEBUG_DATA(info, data, len) 	{int i;\     
//											NDK_ScrPrintf("[%s %d]%s: len=%d data=", __FILE__, __LINE__, (info), (len));\
//											for(i=0; i<(len); i++){\
//												NDK_ScrPrintf("%02x ", *(data+i));\
//											}\
//											NDK_ScrPrintf("\n\r");\
//										}




/*绑定命令码和命令处理函数*/
typedef struct
{
	unsigned char command_code[2];
	int (*func)(unsigned char *, int, const pSysParam_t);
	pSysParam_t m_pTParamList;		/*参数表*/	
}Command_Code_t;


#define MPOS_STX				0x02 									/* 帧头 */
#define MPOS_ETX				0x03									/* 帧尾 */


#define MPOS_STX_OFFSET		0 										/* 帧头 */
#define MPOS_STX_SIZE			0x01									/* 帧头 */

#define MPOS_LEN_OFFSET		(MPOS_STX_OFFSET+MPOS_STX_SIZE)		/*数据长度偏移量*/
#define MPOS_LEN_SIZE			0x02									/*数据长度偏移量*/


#define MPOS_COMMAND_OFFSET		(MPOS_LEN_OFFSET+MPOS_LEN_SIZE)		/*命令码偏移量*/
#define MPOS_COMMAND_SIZE			0x02									/*命令码偏移量*/

#define MPOS_SEP_OFFSET		(MPOS_COMMAND_OFFSET+MPOS_COMMAND_SIZE)		/*分割符号*/
#define MPOS_SEP_SIZE			0x01									/*分割符号偏移量*/

#define MPOS_SN_OFFSET			(MPOS_SEP_OFFSET+MPOS_SEP_SIZE)		/*序列号偏移量*/
#define MPOS_SN_SIZE			0x01									/*序列号偏移量*/


/*
***********************************************************************************************
* 变量定义
***********************************************************************************************
*/
#ifndef NOAUDIO
static ME_COM_T ME_Com_Para;
#else
ME_COM_T ME_Com_Para;
//extern int ME_GetBtPort();
extern int Audio_Protocol_Tx(unsigned char* pindata, int *data_len, int ComID);
extern void Comb_Packet_from_data(unsigned char *pdataBuf,int datalen, unsigned char *pPacketBuf,int* Packetlen, int nType);
extern int get_data_from_recv_packet(unsigned char *pPacketBuf,int Packetlen, unsigned char *pdataBuf,int* datalen, int *pRecvDataLen);
#endif

/*系统参数*/
SysParam_t c_TParamTLV ={PARAM_NEED_TLV, 0x01};
 SysParam_t c_TParamPrint ={PARAM_SPECIAL_PRINT, 0x01};

static const Command_Code_t Command_Code_Manage[] = 
{
/* LCD Object */
//	{{0xA1, 0x01}, CommandParse_ClearScreen, NULL},
//	{{0xA1, 0x02}, CommandParse_SetDispMode, NULL},
//	{{0xA1, 0x03}, CommandParse_SetRefreshMode, NULL},
//	{{0xA1, 0x04}, CommandParse_SetLcdXY, NULL},
//	{{0xA1, 0x05}, CommandParse_GetLcdXY, NULL},
//	{{0xA1, 0x06}, CommandParse_GetLcdAttr, NULL},
//	{{0xA1, 0x07}, CommandParse_DispImage, NULL},
//	{{0xA1, 0x08}, CommandParse_DrawLine, NULL},
//	{{0xA1, 0x09}, CommandParse_DrawRectangle, NULL},
//	{{0xA1, 0x0A}, CommandParse_Refresh, NULL},
//	{{0xA1, 0x0B}, CommandParse_SetBacklight, NULL},
//	{{0xA1, 0x0C}, CommandParse_GetFontSize, NULL},
//	{{0xA1, 0x0D}, CommandParse_SetFontColor, NULL},
//	{{0xA1, 0x0E}, CommandParse_Dispstr, NULL},
//	{{0xA1, 0x0F}, CommandParse_LoadMenu, NULL},

/* KB Object */
//	{{0xB1, 0x01}, CommandParse_KbHit, NULL},
//	{{0xB1, 0x02}, CommandParse_KbGetCode, NULL},
//	{{0xB1, 0x03}, CommandParse_KbGetString, NULL},
//	{{0xB1, 0x04}, CommandParse_KbGetString, &c_TParamTLV},

/* FILE Object */           //暂不使用
//	{{0xC1, 0x01}, CommandParse_OpenRecords, NULL},
//	{{0xC1, 0x02}, CommandParse_GetRecordNum, NULL},
//	{{0xC1, 0x03}, CommandParse_WriteRecord,NULL},
//	{{0xC1, 0x04}, CommandParse_ModifyRecord,NULL},
//	{{0xC1, 0x05}, CommandParse_GetRecord, NULL},
//	
/* mag Object */
//	{{0xD1, 0x01}, CommandParse_OpenCard, NULL},
//	{{0xD1, 0x02}, CommandParse_CloseCard, NULL},
//	{{0xD1, 0x04}, CommandParse_GetMagCardAuthen, NULL},
//	{{0xD1, 0x05}, CommandParse_GetMagCard_ENC, NULL},
//	{{0xD1, 0x06}, CommandParse_GetMagCard_ENC, &c_TParamTLV},
//	
/* IC Object */
//	{{0xE1, 0x01}, CommandParse_IccDetect, NULL},
//	{{0xE1, 0x03}, CommandParse_IccPowerUp, NULL},
//	{{0xE1, 0x04}, CommandParse_IccPowerDown, NULL},
//	{{0xE1, 0x05}, CommandParse_Iccrw, NULL},

/* RFID Object */
//	{{0xE2, 0x01}, CommandParse_RFIDPowerup, NULL},
//	{{0xE2, 0x02}, CommandParse_RFIDPowerDown, NULL},
//	{{0xE2, 0x03}, CommandParse_RFIDAPDU, NULL},
//	{{0xE2, 0x05}, CommandParse_M1StoreKey, NULL},
//	{{0xE2, 0x06}, CommandParse_M1LoadKey, NULL},
//	{{0xE2, 0x07}, CommandParse_M1Auth, NULL},
//	{{0xE2, 0x08}, CommandParse_M1AuthKey, NULL},
//	{{0xE2, 0x09}, CommandParse_M1ReadBlock, NULL},
//	{{0xE2, 0x0A}, CommandParse_M1WriteBlock, NULL},
//	{{0xE2, 0x0B}, CommandParse_M1Increment, NULL},
//	{{0xE2, 0x0C}, CommandParse_M1Decrement, NULL},	

/*AUTH Object*/
//	{{0xF1, 0x01}, CommandParse_ReadDeviceInformation, NULL},
//	{{0xF1, 0x02}, CommandParse_GetRandomNumber, NULL},	
//	{{0xF1, 0x03}, CommandParse_DeviceAuth, NULL},		
//	{{0xF1, 0x06}, CommandParse_WriteEquipmentInformation, NULL},
	{{0xF1, 0xF1}, CommandParse_MutualAuth1, NULL},
	{{0xF1, 0xF2}, CommandParse_MutualAuth2, NULL},
	{{0xF1, 0xF3}, CommandParse_MutualAuth3, NULL},
	{{0xF1, 0xF4}, CommandParse_SetGeoPosition, NULL},
	

/* transmission Object*/

	{{0x1E, 0x01}, CommandParse_UnionPayCall, NULL},
	
/*PINPAD Object*/
//	{{0x1A, 0x01}, CommandParse_PasswordInput, NULL},
//	{{0x1A, 0x02}, CommandParse_LoadMKey, NULL},
//	{{0x1A, 0x03}, CommandParse_DataEncryptionOrDecryption, NULL},	
//	{{0x1A, 0x04}, CommandParse_DataMac, NULL},
//	{{0x1A, 0x05}, CommandParse_LoadWKey, NULL},
//	{{0x1A, 0x08}, CommandParse_DataMac, &c_TParamTLV},
//	{{0x1A, 0x17}, CommandParse_LoadDukpt, NULL},
/* prn Object */
//	{{0x1B, 0x49}, CommandParse_PrnInit, NULL},
//	{{0x1B, 0x47}, CommandParse_GetPrnStatus, NULL},
//	{{0x1B, 0x46}, CommandParse_PaperSkip, NULL},
//	{{0x1B, 0x53}, CommandParse_Setting, NULL},
//	{{0x1B, 0x50}, CommandParse_PrintNormal, NULL},
//	{{0x1B, 0x5B}, CommandParse_PrintNormal, &c_TParamPrint},

/*PBOC Object*/
//	{{0x1C, 0x01}, CommandParse_PbocCapk_Set, NULL},
//	{{0x1C, 0x02}, CommandParse_PbocAid_Set, NULL},	
//	{{0x1C, 0x03}, CommandParse_TranPro_Set, NULL},
//	{{0x1C, 0x04}, CommandParse_Pboc_GetData, NULL},
//	{{0x1C, 0x05}, CommandParse_Pboc_Standard, NULL},
//	{{0x1C, 0x06}, CommandParse_Pboc_SencAuth, NULL},	
//	{{0x1C, 0x07}, CommandParse_Pboc_End, NULL},
//	{{0x1C, 0x08}, CommandParse_QPboc_Pboc, NULL},
//	{{0x1C, 0x09}, CommandParse_Pboc_GetLastWaterinfo, NULL},

/* TERMINAL MANAGEMENT Object */
//	{{0x1D, 0x01}, CommandParse_Buzzer, NULL},
//	{{0x1D, 0x02}, CommandParse_BlinkingLights, NULL},
//	{{0x1D, 0x03}, CommandParse_ScanStringInstruction, NULL},
//	{{0x1D, 0x04}, CommandParse_SetDateTime, NULL},
//	{{0x1D, 0x05}, CommandParse_GetDateTime, NULL},
//	{{0x1D, 0x06}, CommandParse_SetTerminalParam, NULL},
//	{{0x1D, 0x07}, CommandParse_GetTerminalParam, NULL},
//	{{0x1D, 0x08}, CommandParse_CancelReset, NULL},
//	{{0x1D, 0x09}, CommandParse_LoadApp, NULL},
//	{{0x1D, 0x0a}, CommandParse_CommTest, NULL},
//	{{0x1D, 0x0b}, CommandParse_ShutDown, NULL},
//	//{{0x1D, 0x0c}, CommandParse_Bat_per, NULL},
//#ifdef NOAUDIO
//    {{0x1D, 0x0d}, CommandParse_DownloadMode, NULL},
//#endif     

/*ME30 */
//	{{0xD1, 0x21}, CommandParse_OpenCard_ME30, NULL},
//	{{0xD1, 0x22}, CommandParse_GetMagCard_ME30, NULL},
//	{{0x1A, 0x21}, CommandParse_PasswordInput_ME30, NULL},	
//	{{0x1A, 0x22}, CommandParse_LoadKey_ME30, NULL},	

/*生产 */
//	{{0x1D, 0x20}, CommandParse_CommMode_ME30, NULL},	
//	{{0x1D, 0x21}, CommandParse_BTDisconnect, NULL},	
//	{{0x1D, 0x22}, CommandParse_CheckData, NULL},	
//	{{0xFF, 0x01}, CommandParse_ReadDeviceInfoPro, NULL},	
	//{{0xFF, 0x02}, CommandParse_SetSN, NULL},	
};


int ME_DefaultCmd(void)
{
    return NDK_ERR_NOT_SUPPORT;
}

/*
***********************************************************************************************
* 函数原型：
* 功能描述：
* 输入参数：
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
int ME_Printf( const char *format, ... ) {
	int len;
	va_list arg;
	char str[512];
	int ret = 0;

	if(g_METPEDCtl.cIsDebug)
	{
	    va_start( arg, format );
	    if( (len = vsprintf( str, format, arg )) < 0 ){
	        return -1;
	    }
	    NDK_PortWrite(PORT_NUM_USB, len, str);
	    va_end( arg );
    }
    return ret;
}

/*
***********************************************************************************************
* 函数原型：ME_Endian_Swap
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
ushort ME_Endian_Swab16(ushort n)     
{ 	
	if(nlMpos_Command.EndianMode == BIG_ENDIAN)
		return Endian_Swab16(n);
	else
		return n;
}
/*
***********************************************************************************************
* 函数原型：ME_Endian_Swap
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_Endian_Swab32(uint n)     
{ 
	if(nlMpos_Command.EndianMode == BIG_ENDIAN)
		return Endian_Swab32(n);
	else
		return n;
}

/*
***********************************************************************************************
* 函数原型：ME_Bcd2Int
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_Bcd2Int(void *pindata, int size)
{
	uint ret =0,tmp = 0;
	void *paddr= (void*)pindata;
	
	ret = _VAR_READ(paddr, size);
	if(size == _VAR_BIT8) {
		tmp = _BCD2INT(ret);
	}
	else if(size == _VAR_BIT16) {
		tmp = _BCD2INT(ret)*100;
		ret = (ret >> 8);
		tmp += _BCD2INT(ret);
	}
	else if(size == _VAR_BIT32) {
		tmp = 0;
	}	
	return tmp;
}
/*
***********************************************************************************************
* 函数原型：ME_Int2Bcd
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_Int2Bcd(void *poutdata, uint n, int size)
{
	uint tmp = 0;
	
	if(size == _VAR_BIT8) {
		tmp = _INT2BCD(n);
		_VAR_WRITE(poutdata, tmp, size);
	}
	else if(size == _VAR_BIT16) {
		((uchar*)&tmp)[1] = _INT2BCD(n%100);
		((uchar*)&tmp)[0] = _INT2BCD(n/100);
		_VAR_WRITE(poutdata, tmp, size);
	}
	else if(size == _VAR_BIT32) {
		tmp = 0;
	}	
	return tmp;
}
/*
***********************************************************************************************
* 函数原型：ME_ReadLen
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_ReadLen(void *pindata, int size)
{
	uint ret = 0;
	
	if(nlMpos_Command.LengthMode == BCD_MODE)
	{
		ret = nlMpos_Command.mpos_bcd2int(pindata, size);
	}
	else if(nlMpos_Command.LengthMode == ENDIAN_MODE)
	{
		ret = nlMpos_Command.mpos_getvar(pindata, size);
		ret = nlMpos_Command.mpos_endian_swab16(ret);
	}
	return (ret);
}
/*
***********************************************************************************************
* 函数原型：ME_WriteLen
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_WriteLen(void *poutdata, uint n, int size)
{
	uint ret = 0;
	
	if(nlMpos_Command.LengthMode == BCD_MODE)
	{
		ret = nlMpos_Command.mpos_int2bcd(poutdata, n, size);
	}
	else if(nlMpos_Command.LengthMode == ENDIAN_MODE)
	{
		ret = nlMpos_Command.mpos_endian_swab16((ushort)n);
		ret = nlMpos_Command.mpos_setvar(poutdata, ret, size);
	}	
	return (ret);
}
/*
***********************************************************************************************
* 函数原型：ME_GetVarLen
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_GetVar(void* poutdata, int size)
{
	uint ret =0;
	void *paddr= (void*)poutdata;
	
	if(size == _VAR_BIT8)
		ret = _VAR_READ(paddr, size);
	else if(size == _VAR_BIT16) {
		((uchar*)&ret)[0] = _VAR_READ((paddr+0), _VAR_BIT8);
		((uchar*)&ret)[1] = _VAR_READ((paddr+1), _VAR_BIT8);		
	}
	else if(size == _VAR_BIT24) {
		((uchar*)&ret)[0] = _VAR_READ((paddr+0), _VAR_BIT8);
		((uchar*)&ret)[1] = _VAR_READ((paddr+1), _VAR_BIT8);
		((uchar*)&ret)[2] = _VAR_READ((paddr+2), _VAR_BIT8);
	}
	else if(size == _VAR_BIT32) {
		((uchar*)&ret)[0] = _VAR_READ((paddr+0), _VAR_BIT8);
		((uchar*)&ret)[1] = _VAR_READ((paddr+1), _VAR_BIT8);
		((uchar*)&ret)[2] = _VAR_READ((paddr+2), _VAR_BIT8);
		((uchar*)&ret)[3] = _VAR_READ((paddr+3), _VAR_BIT8);		
	}
	return ret;
}
/*
***********************************************************************************************
* 函数原型：ME_Read_FrameLen
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
uint ME_SetVar(void* pindata, uint indata, int size)
{
	void *paddr= (void*)pindata;
	
	if(size == _VAR_BIT8)
		_VAR_WRITE(paddr, indata, size);
	else if(size == _VAR_BIT16) {
		_VAR_WRITE((paddr+0), (indata), _VAR_BIT8);
		_VAR_WRITE((paddr+1), (indata>>8), _VAR_BIT8);		
	}
	else if(size == _VAR_BIT24) {
		_VAR_WRITE((paddr+0), (indata), _VAR_BIT8);
		_VAR_WRITE((paddr+1), (indata>>8), _VAR_BIT8);	
		_VAR_WRITE((paddr+2), (indata>>16), _VAR_BIT8);
	}
	else if(size == _VAR_BIT32) {
		_VAR_WRITE((paddr+0), (indata), _VAR_BIT8);
		_VAR_WRITE((paddr+1), (indata>>8), _VAR_BIT8);	
		_VAR_WRITE((paddr+2), (indata>>16), _VAR_BIT8);
		_VAR_WRITE((paddr+3), (indata>>24), _VAR_BIT8);	
	}
	return 0;
}



uint ME_GetCmdOffset()
{
	return MPOS_COMMAND_OFFSET;
}

uint ME_GetVarOffset()
{
	return (MPOS_SN_OFFSET+MPOS_SN_SIZE);
}

uint ME_GetVarLen(void *pindata)
{
	return nlMpos_Command.mpos_readlen(pindata+MPOS_LEN_OFFSET, _VAR_BIT16);
}

/*
***********************************************************************************************
* 函数原型：ME_EDC
* 功能描述：edc 校验
* 输入参数：paddr 
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
static uchar ME_EDC(uchar *pindata)     
{
	uchar Edc = 0; 
	uint i, len;

	len = nlMpos_Command.mpos_readlen(pindata+MPOS_LEN_OFFSET, _VAR_BIT16);
	PDEBUG("len :%d",len);
	for(i=0; i<MPOS_LEN_SIZE+len+1; i++) /* 数据长度 */
	{
		Edc = (uchar)(Edc^pindata[MPOS_LEN_OFFSET+i]);  
	}
	PDEBUG_DATA("edc data", &pindata[MPOS_LEN_OFFSET], MPOS_LEN_SIZE+len+1);
	PDEBUG("edc %02x", Edc);
	return Edc;
}
/*
***********************************************************************************************
* 函数原型：ME_EDC_Check
* 功能描述：edc
* 输入参数：
* 输出参数：
* 返回值：	
* 调用关系：
***********************************************************************************************
*/
static int ME_EDC_Check(unsigned char *pindata, int indata_len)   
{

    PDEBUG("==================recv==============");
	PDEBUG_DATA("pindata", pindata, indata_len);
	uchar edc = ME_EDC(pindata);
	uint len ;

	
	len= nlMpos_Command.mpos_readlen(pindata+MPOS_LEN_OFFSET, _VAR_BIT16);
	if( edc != pindata[MPOS_LEN_OFFSET+2+ len + 1]) /*VAR+ETX+LRC*/
	{
		return NDK_ERR;
	}

	return NDK_OK;
}

/*
***********************************************************************************************
* 函数原型：s4 ME_API_COMInit(unsigned char ucCommMode)
* 功能描述：串口初始化
* 输入参数：
* 输出参数：
* 调用关系：
***********************************************************************************************
*/
int ME_API_COMInit(unsigned char ucCommMode)
{
    switch(ucCommMode)
    {
		case COMMODE_USB:
			ME_Com_Para.u1COMID = PORT_NUM_USB;
			break;
		case COMMODE_AUDIO:
			ME_Com_Para.u1COMID = PORT_NUM_AUDIO;
			break;
		case COMMODE_BT:
		default:
			ME_Com_Para.u1COMID = PORT_NUM_USB;
			break;
    }	
    ME_Com_Para.u4TimeOut = 50;
    NDK_PortClose(ME_Com_Para.u1COMID);
    NDK_PortOpen(ME_Com_Para.u1COMID , "115200,8,N,1");
    return 0;	
}

/*
***********************************************************************************************
* 函数原型：s4 ME_API_COMRead(PCOM pCOMStruct)
* 功能描述：读串口
* 输入参数：
* 输出参数：
* 返回值：-1：参数错、读取超时
		  >0：实际读取的字节数
* 调用关系：
***********************************************************************************************
*/
static int ME_API_COMRead(PME_COM_T pCOMStruct)
{
	int nRet;
	int nReadlen = 0;
	
	if((pCOMStruct==NULL)|| (pCOMStruct->pu1DataBuf==NULL))
	{
		return NDK_ERR;
	}
	nRet = NDK_PortRead( pCOMStruct->u1COMID,\
						pCOMStruct->u4DataLen,\
						(char*)pCOMStruct->pu1DataBuf,\
						pCOMStruct->u4TimeOut, &nReadlen);
	//ME31_DEBUG("\r\nret %02x %02x",nRet,nReadlen);
	if(nRet  != NDK_OK
		)
	{
		return NDK_ERR;
	}
	return nReadlen;
}
/*
***********************************************************************************************
* 函数原型：s4 ME_API_COMReadLen(PCOM pCOMStruct)
* 功能描述：读串口数据长度
* 输入参数：
* 输出参数：
* 返回值：-1：参数错、读取超时
		  >0：实际读取的字节数
* 调用关系：
***********************************************************************************************
*/
static int ME_API_COMReadLen(PME_COM_T pCOMStruct)
{
	int nRet;
	int readlen = 0;
	
	if((pCOMStruct==NULL)|| (pCOMStruct->pu1DataBuf==NULL))
	{
		return NDK_ERR;
	}
	nRet = NDK_PortReadLen( pCOMStruct->u1COMID, &readlen) ;
	if(readlen!=0)
		ME31_DEBUG("\r\nRET %02x,com %02x,readlen %02x",nRet,pCOMStruct->u1COMID, readlen);
	if( nRet!= NDK_OK)
	{
		return NDK_ERR;
	}
	return readlen;
}
/*
***********************************************************************************************
* 函数原型：s4 ME_API_COMWrite(PCOM pCOMStruct)
* 功能描述：写串口
* 输入参数：
* 输出参数：
* 返回值：	0：成功	－1：失败
* 调用关系：
***********************************************************************************************
*/
static int ME_API_COMWrite(PME_COM_T pCOMStruct)
{
	if((pCOMStruct==NULL)|| (pCOMStruct->pu1DataBuf==NULL))
	{
		return NDK_ERR;
	}
	if( NDK_PortWrite( pCOMStruct->u1COMID,\
						pCOMStruct->u4DataLen,\
						(const char*)pCOMStruct->pu1DataBuf) != NDK_OK)
	{
		return NDK_ERR;
	}
	return  NDK_OK;
}


/*
***********************************************************************************************
* 函数原型：s4 ME_API_COMClose(PCOM pCOMStruct)
* 功能描述：串口初始化
* 输入参数：
* 输出参数：
* 调用关系：
***********************************************************************************************
*/
int ME_API_COMClose(void)
{
    return NDK_PortClose(ME_Com_Para.u1COMID);
}

/*
***********************************************************************************************
* 函数原型：ME_Receive_Command_From_Host
* 功能描述：读COMMAND
* 输入参数：
* 输出参数：
* 返回值：	MPOS_RIGHT：成功	MPOS_ERROR：失败
* 调用关系：
***********************************************************************************************
*/
static int ME_Receive_Command_From_Host(unsigned char *pindata, int indata_len, int msec)  
{
	int len;
	int nReadLen;	

	ME_Com_Para.u4TimeOut = msec;
	ME_Com_Para.pu1DataBuf= pindata;


	nReadLen = 0;
	NDK_PortReadLen(ME_Com_Para.u1COMID,&nReadLen);
	if(nReadLen>=1){		
		memset(pindata, 0, indata_len); 			/*清空接收缓冲避免影响下次接收*/
		ME_Com_Para.u4DataLen=1;
		if(ME_API_COMRead(&ME_Com_Para)==ME_Com_Para.u4DataLen)
		{
			if(MPOS_STX == ME_Com_Para.pu1DataBuf[0]) // 收到 帧头
			{
				ME_Com_Para.pu1DataBuf += ME_Com_Para.u4DataLen; // 偏移
			}
			else
				return NDK_ERR;
		}
		else
			return NDK_ERR;
	}
	else
		return NDK_ERR;

	
	ME_Com_Para.u4DataLen = 2;
	ME_Com_Para.u4TimeOut = msec * ME_Com_Para.u4DataLen;
	if(ME_API_COMRead(&ME_Com_Para) == ME_Com_Para.u4DataLen)  // 数据长度
	{
		len = nlMpos_Command.mpos_readlen(ME_Com_Para.pu1DataBuf, _VAR_BIT16);	
		if((1+2+len+2)>indata_len)
			return NDK_ERR;
		ME_Com_Para.pu1DataBuf += ME_Com_Para.u4DataLen; // 偏移
	}
	else
	{
	    PDEBUG("here");
		return	NDK_ERR;
	}
	
	ME_Com_Para.u4DataLen = len;
	ME_Com_Para.u4TimeOut = msec * ME_Com_Para.u4DataLen;
	if(ME_Com_Para.u4TimeOut > 1000) {
		ME_Com_Para.u4TimeOut = 1000; // 最大1s
	}
	if(ME_API_COMRead(&ME_Com_Para) == ME_Com_Para.u4DataLen)  // 指令号+分割符+序列号+VAR
	{
		ME_Com_Para.pu1DataBuf += ME_Com_Para.u4DataLen; // 偏移
	}
	else
	{
        PDEBUG("here");
		return	NDK_ERR;
	}
	ME_Com_Para.u4DataLen = 1;
	ME_Com_Para.u4TimeOut = msec;
	if(ME_API_COMRead(&ME_Com_Para) == ME_Com_Para.u4DataLen)  // ETX
	{
		if(MPOS_ETX != ME_Com_Para.pu1DataBuf[0]) // 收到 帧头
		{
		    PDEBUG("here");
			return NDK_ERR;
		}
		ME_Com_Para.pu1DataBuf += ME_Com_Para.u4DataLen; // 偏移
	}
	ME_Com_Para.u4DataLen = 1;
	ME_Com_Para.u4TimeOut = msec;
	if(ME_API_COMRead(&ME_Com_Para) == ME_Com_Para.u4DataLen)  // lrc
	{
		ME_Com_Para.pu1DataBuf += ME_Com_Para.u4DataLen; // 偏移
	}
	else
	{
	    PDEBUG("here");
		return	NDK_ERR;
	}
	len = ME_Com_Para.pu1DataBuf - pindata;
	return len;
}


/*
***********************************************************************************************
* 函数原型：ME_Send_Responsion_To_Host
* 功能描述：发送应答
* 输入参数：
* 输出参数：
* 返回值：	
* 调用关系：
***********************************************************************************************
*/
int ME_Send_Responsion_To_Host(unsigned char* pindata,int indata_len,char* perrcode)
{
	uint i;	
	uchar *ptem;
	uchar temp[12];
	uchar Edc; 

	if(pindata == NULL)
	{
		indata_len = 0;
	}
	memset(temp,0x00,sizeof(temp));

	PDEBUG_DATA("send to host", pindata, indata_len);
    //发送前先清一次接收缓存
    NDK_PortClrBuf(PORT_NUM_USB);

	/*数据长度 ==  指令号(2)  + 分割符(1)  + 序列号(1) + 2Byte应答码+可变数据*/
	nlMpos_Command.mpos_writelen(&temp[MPOS_LEN_OFFSET], indata_len+6, _VAR_BIT16);
	for(i=0; i<MPOS_COMMAND_SIZE; i++) /* 命令码 */
	{
		temp[MPOS_COMMAND_OFFSET+i] = g_METPEDCtl.u1MposCmd[i];  
	}

	if(g_METPEDCtl.u1MposSn == 255) /* 当前序列号 */
		temp[MPOS_SN_OFFSET] = 1;
	else 
		temp[MPOS_SN_OFFSET] = g_METPEDCtl.u1MposSn+1;
	memcpy(&temp[MPOS_VARIABLE_OFFSET], perrcode, 2);

	temp[0] = MPOS_STX;
	temp[MPOS_SEP_OFFSET] = 0x2F;/*分割符*/
	ME_Com_Para.u4DataLen = 9;	//10

	ME_Com_Para.pu1DataBuf = temp;


	ME_API_COMWrite(&ME_Com_Para);
	Edc=0;
	for(i=0; i<8; i++) /* 数据长度 */
	{
		Edc = (uchar)(Edc^temp[MPOS_LEN_OFFSET+i]);  
	}

	ME_Com_Para.u4DataLen = indata_len;	
	ME_Com_Para.pu1DataBuf = pindata;

	ME_API_COMWrite(&ME_Com_Para);
	for(i=0; i<indata_len; i++) /* 数据长度 */
	{
		Edc = (uchar)(Edc^pindata[i]);  
	}

	temp[10] = MPOS_ETX;
	Edc ^= MPOS_ETX;
	temp[11] = Edc;					/* LRC */
	ME_Com_Para.u4DataLen= 2;
	ME_Com_Para.pu1DataBuf= &temp[10];
	
	ME_API_COMWrite(&ME_Com_Para);

#if 0
	ME31_DEBUG("\n回发数据\n");
	ptem = &temp[0];
	ME31_DEBUG_BUF(ptem, 9);
	ME31_DEBUG_BUF(pindata, indata_len);	
	ptem = &temp[10];
	ME31_DEBUG_BUF(ptem, 2);

#endif
	return 0;
}


int ME_GetMessageType(unsigned char *type)
{
    memcpy(type, g_METPEDCtl.u1MposCmd, 2);
    return 0;
}

/*
***********************************************************************************************
函数原型：CommandParse
功能描述：解析函数
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
static int ME_CommandParse(unsigned char* pindata, int indata_len)
{
	int i,size;
	int iRet;
	size = sizeof(Command_Code_Manage)/sizeof(Command_Code_Manage[0]);
	g_METPEDCtl.u1MposSn = pindata[MPOS_SN_OFFSET]; // 当前的SN
	memcpy(g_METPEDCtl.u1MposCmd, pindata+MPOS_COMMAND_OFFSET, MPOS_COMMAND_SIZE); // 当前命令码
	ME31_DEBUG_BUF(g_METPEDCtl.u1MposCmd, 2);
	if(nlMpos_Command.mpos_edc_check(pindata, indata_len) != NDK_OK)
	{
		ME31_DEBUG("\r\nedc err");
		PDEBUG("\nhere dec err![%s][%d]\n",__FUNCTION__,__LINE__);
		nlMpos_Command.mpos_sdata_to_host(NULL, 0, CMD_ERR_LRC);
		return NDK_ERR;
	}
	
	for(i=0; i<size; i++)
	{
		if(	(Command_Code_Manage[i].command_code[0] == pindata[MPOS_COMMAND_OFFSET])
				&&(Command_Code_Manage[i].command_code[1] == pindata[MPOS_COMMAND_OFFSET+1])
			) /* 命令码 相同 */
		{
			iRet =  (*Command_Code_Manage[i].func)(pindata, indata_len, Command_Code_Manage[i].m_pTParamList);
			if(iRet==NDK_ERR_NOT_SUPPORT)
			    break;
                     else
			    return 0;
		}
	}
	
	nlMpos_Command.mpos_sdata_to_host(NULL, 0, CMD_ERR_UNSUPPORT); // 命令码解析错误
	return NDK_ERR;
}


const nlmpos_command_t nlMpos_Command = {
    .mpos_com_init	= ME_API_COMInit,
    .mpos_com_read  = ME_API_COMRead,
    .mpos_com_write = ME_API_COMWrite,
    .mpos_com_readlen = ME_API_COMReadLen,
    .mpos_edc_check = ME_EDC_Check,
    .mpos_endian_swab16 = ME_Endian_Swab16,
    .mpos_endian_swab32 = ME_Endian_Swab32,  
    .mpos_getvar = ME_GetVar,
    .mpos_setvar = ME_SetVar,
    
    .mpos_int2bcd = ME_Int2Bcd,
    .mpos_bcd2int = ME_Bcd2Int,
    .mpos_readlen = ME_ReadLen,
    .mpos_writelen = ME_WriteLen,
	
    .mpos_sdata_to_host = ME_Send_Responsion_To_Host,
    .mpos_rdata_from_host = ME_Receive_Command_From_Host,
    
    .mpos_command_parse = ME_CommandParse,
    .Lcd_StartY = 16,
    .EndianMode = BIG_ENDIAN,
    .LengthMode = BCD_MODE,
};


