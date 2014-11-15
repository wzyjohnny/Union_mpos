/*
***********************************************************************************************
版权说明：通用版本
版本号：
生成日期：
作者：
模块目的/功能：
主要函数及其功能：
修改日志：
***********************************************************************************************
*/
#include <stdlib.h>
#include <string.h>
#include "NDK.h"
//#include "mpos_api_kb.h"
#include "mpos_api_lcd.h"
#include "mpos_api_alg.h"
#include "mpos_command.h"

extern ME31_CONFIG_T g_me31conf;

const char *pszAppVer = "1.4";          //通用版本,版本号
const char *pszHello = "欢迎使用 等待操作";         //通用欢迎使用界面

const ME_KbItem_t stMainItem[]= {
	{K_F1,"", ME_FuncMenu},
};


const ME_KbItem_t stPasswordItem[]= {
	{0,"150349", ME_MKSetup},
	{0,"123456", ME_Sysmanage},		
	{0,"2580", ME_DebugSwitch},			//测试使用,生产中去除
	{0,"186111", ME_ScrSetContrast},	
	{0,"186624", ME_SetCommMode},
//	{0,"31415926", ME_FsDel},			//测试使用,生产中去除
	//{0,"147896", Mpos_PbocInfo},
};


const ME_GuiItem_t stFuncItem[]= {
	{K_ZERO,"0.版本信息", {0, 16}, ME_DeviceInfo},
	{K_TWO, "2.系统设置", {0, 63}, ME_PasswordMenu},	
};

const ME_GuiItem_t stSysItem[]= {
	{K_ONE,"1.时间设置", {0, 16}, ME_SetDateTime},
	//{K_TWO,"2.电源管理", {0, 28}, ME_SetSleepTime},	
};

const ME_GuiItem_t stCommItem[]= {
	{K_ONE,"1.USB", {0, 16}, ME_UsbParaSet},
	{K_THREE,"3.音频", {0, 40}, ME_AudioParaSet},
};




/*
***********************************************************************************************
函数原型：int ME_BtFormat(char *namebuf)
功能描述：获取蓝牙出厂默认命名名称,C-%S-%S
输入参数：
输出参数：namebuf
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_BtFormat(char *namebuf)
{
    unsigned int unLen;
    char szPosSN[32];
    char szPosType[32];

    memset(szPosSN,0x00,sizeof(szPosSN));
    memset(szPosType,0x00,sizeof(szPosType));
    NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TYPE,&unLen,szPosType);
	if(memcmp(szPosType,"ME31HW_FULL_",12)==0)
		szPosType[4] = 0; 
    if((NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &unLen, szPosSN) == NDK_OK)&&(unLen>=6))
    {
        szPosSN[unLen] = 0;
        sprintf(namebuf, "C-%s-%s",szPosType,&szPosSN[unLen-6]);
        return NDK_OK;
    }	
    else
        return NDK_ERR;
}







/*
***********************************************************************************************
函数原型：int ME_SysGetPosInfo(EM_SYS_HWINFO_ME emFlag,uint * punLen,char * psBuf)
功能描述：通用，设置CSN等
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
/*
int ME_SysSetPosInfo(EM_SYS_HWINFO_ME emFlag,char * psBuf)
{
	int nRet;
	if(emFlag==SYS_HWINFO_GET_CSN){
		nRet = ME_FsWrite(SN_FILE,(const unsigned char *)psBuf,0,strlen(psBuf));
	}
	else
		nRet = NDK_SysSetPosInfo(emFlag,psBuf);
	return nRet;
}


/*
***********************************************************************************************
函数原型：int ME_SysGetPosInfo(EM_SYS_HWINFO_ME emFlag,uint * punLen,char * psBuf)
功能描述：银商，获取CSN   KSN 等
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_SysGetPosInfo(EM_SYS_HWINFO_ME emFlag,uint * punLen,char * psBuf)
{
	int nRet;
	char szCsn[24];	
	unsigned int unLen;
	ST_SEC_KCV_INFO stKcvInfoIn;

	if(emFlag==SYS_HWINFO_GET_CSN){
		nRet = NDK_FsFileSize(SN_FILE,&unLen);
		if(nRet!=0)
			return nRet;
		nRet = ME_FsRead(SN_FILE,(unsigned char *)szCsn,0,unLen);
		if(nRet==0)
			PubAscToHex((uchar *)szCsn, unLen, 0, (uchar *)psBuf);		
		*punLen = (unLen+1)/2;
	}
	else if(emFlag==SYS_HWINFO_GET_KCV){
		*punLen = 1;
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		stKcvInfoIn.nCheckMode = SEC_KCV_NONE;
		stKcvInfoIn.nLen = 0;
		nRet =  NDK_SecGetKcv(SEC_KEY_TYPE_TMK, 1, &stKcvInfoIn);
	}
	else
		nRet = NDK_SysGetPosInfo(emFlag,punLen,psBuf);
	return nRet;
}


/*
***********************************************************************************************
函数原型：ME_DeviceInfo
功能描述：
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_DeviceInfo()
{
    int nKey;
    int nRet;
    int nLen;
    int nTimeCount;
    int nPageFlag;
    char szBuf[32];
    
    nPageFlag =0;
    nTimeCount = 0;
    while(1)
    {
        if(nPageFlag==0)
        {
			me_ScrClrs();
			nRet = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, (uint *)&nLen, szBuf);
			if((nRet==NDK_OK)&&(nLen>0))
			{
				NDK_ScrPrintf("SN:%s\n",szBuf);
			}
			else 
			NDK_ScrPrintf("SN:000000000000\n");
			nRet = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_PSN, (uint *)&nLen, szBuf);
			if((nRet==NDK_OK)&&(nLen>0))
			{
				NDK_ScrPrintf("PN:%s\n",szBuf);
			}
			else 
				NDK_ScrPrintf("PN:000000000000000\n");
	       NDK_ScrPrintf("固件版本号:%s\r\n", pszAppVer);
        }
        else if(nPageFlag==1)
        {
            me_ScrClrs();
            strcpy(szBuf, " ("__DATE__" " __TIME__")");
            NDK_ScrPrintf("固件时间:%s", szBuf);
        }

rep:
        NDK_SysMsDelay(100);
        if((NDK_KbHit(&nKey)==NDK_OK)&&(nKey!=0)){
            nTimeCount = 0;
            if(nKey==K_ESC)
                break;
            else if(nKey==K_ENTER)
                nPageFlag^=0x01;
             else
                 goto rep;
        }
        else{
             nTimeCount++;
            if(nTimeCount>100)
                break;
             goto rep;
        }
    }
    return ME_ERR_FALLBACK;
}



/*
***********************************************************************************************
函数原型：ME_ParaInit
功能描述：
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
void ME_ParaInit()
{
	g_METPEDCtl.ucDefaultMode = COMMODE_USB;
	g_METPEDCtl.ucMainItemCount = sizeof(stMainItem)/sizeof(ME_KbItem_t);	
	g_METPEDCtl.ucPasswordItemCount= sizeof(stPasswordItem)/sizeof(ME_KbItem_t);
	g_METPEDCtl.ucFuncItemCount= sizeof(stFuncItem)/sizeof(ME_GuiItem_t);
	g_METPEDCtl.ucSysItemCount= sizeof(stSysItem)/sizeof(ME_GuiItem_t);
	g_METPEDCtl.ucCommItemCount= sizeof(stCommItem)/sizeof(ME_GuiItem_t);	
    return;
}

