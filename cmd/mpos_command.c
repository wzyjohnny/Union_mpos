 /*
***********************************************************************************************
版权说明：ME31指令集
版本号：
生成日期：
作者：
模块目的/功能：
主要函数及其功能：
修改日志：
***********************************************************************************************
*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "stdlib.h"
#include "unistd.h"
#include "NDK.h"
#include "mpos_command.h"
#include "mpos_api_frame.h"
#include "lcup.h"
#include "wdebug.h"
/*
***********************************************************************************************
* 宏定义
***********************************************************************************************
*/

#define ME_TIME_INTERVAL	(2)

#define ME_PASSWORD_ESC   	 	"123456"
#define ME_PASSWORD_ADMIN   	"123456"
#define ME_PASSWORD_KLA   	 	"150349"
#define ME_CFG_LEN 6
/*
***********************************************************************************************
* 函数申明
***********************************************************************************************
*/

/*
***********************************************************************************************
* 变量定义
***********************************************************************************************
*/
ME_TPEDCTL g_METPEDCtl;	/*PED控制结构*/
ME31_CONFIG_T g_me31conf;
#define	PinpadVersion				"02.01_00_10111400"

extern ME_COM_T ME_Com_Para;


 uchar ME_Host_Command_Buf[ME_MAX_Host_Command]; //发送缓冲和接受缓冲公用。


extern const char *pszAppVer ;
extern const char *pszHello  ;
extern const ME_KbItem_t stMainItem[];
extern const ME_KbItem_t stPasswordItem[];

extern const ME_GuiItem_t stFuncItem[];
extern const ME_GuiItem_t stSysItem[];
extern const ME_GuiItem_t stCommItem[];

extern UnionParam_t stUnionParam;

#ifdef NOAUDIO
extern int Audio_Protocol_Rx(unsigned char *outdata, int indata_len, int ComID);
extern int Audio_Match_Device(int ComID);
extern int Audio_Protocol_Srartup(int ComID, unsigned char attr);
#endif
static int DefaultFunc(unsigned char type){return NDK_ERR_NOT_SUPPORT;}






/*==============================自定义信息获取，如银商的CSN和KSN等=====================================*/
int ME_SysGetPosInfo(EM_SYS_HWINFO_ME emFlag,uint * punLen,char * psBuf)	__attribute__((weak, alias("DefaultFunc")));
/*==============================自定义信息设置，如银商的CSN和KSN等=====================================*/
//int ME_SysSetPosInfo(EM_SYS_HWINFO_ME emFlag,char * psBuf)	__attribute__((weak, alias("DefaultFunc")));
/*==============================自定义操作，如UI界面等=====================================*/
int ME_SysExtraAction(EM_SYS_ACTION_ME emFlag) __attribute__((weak, alias("DefaultFunc")));
/*==============================主密钥装载====================================*/
int ME_MKSetup() __attribute__((weak, alias("Default_ME_MKSetup")));

/*
***********************************************************************************************
函数原型：ME_SecKekInit
功能描述：初始化KEK,默认16字节0x31
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_SecKekInit()
{
	   int nRet;
	ST_SEC_KEY_INFO stKeyInfoIn;
	ST_SEC_KCV_INFO stKcvInfoIn;

	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfoIn,0,sizeof(stKeyInfoIn));
	stKeyInfoIn.ucScrKeyIdx = 0;
	stKeyInfoIn.ucScrKeyType = SEC_KEY_TYPE_TLK;
	stKeyInfoIn.nDstKeyLen = 16;
	stKeyInfoIn.ucDstKeyIdx = 1;
	stKeyInfoIn.ucDstKeyType = SEC_KEY_TYPE_TLK;
	memset(stKeyInfoIn.sDstKeyValue, 0x31, stKeyInfoIn.nDstKeyLen);
	//memcpy(stKeyInfoIn.sDstKeyValue, "\x57\x8E\x0F\xC9\x0F\x09\x47\x54\xBE\xCB\xBE\x87\xF6\x4F\x0F\xBE", stKeyInfoIn.nDstKeyLen);
	stKcvInfoIn.nCheckMode = 0;
	nRet = NDK_SecLoadKey(&stKeyInfoIn, &stKcvInfoIn);
	return nRet;
}


/*
***********************************************************************************************
函数原型：int ME_FsRead(const char * pszName,unsigned char * psBuffer,uint unOffset,uint unLength)
功能描述：文件写.不建议使用反复写同一个文件,会降低读写效率,可保存到缓冲后只写一次
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_FsWrite(const char * pszName,const unsigned char * psBuffer,uint unOffset,uint unLength)
{
	int fd;
	int nRet;

	fd = NDK_FsOpen(pszName, "w");
	if(fd<0) {
		return -1;
	}


	nRet = NDK_FsSeek(fd, unOffset, SEEK_SET);
	if(nRet!=0){
		ME31_DEBUG("seek err =%d\n",nRet);
		NDK_FsClose(fd);
		return -2;
	}
	
	nRet = NDK_FsWrite(fd,(char *)psBuffer, unLength);
	if(nRet!=unLength){
		NDK_FsClose(fd);
		return -3;
	}
	NDK_FsClose(fd);
	return 0;
}


/*
***********************************************************************************************
函数原型：int ME_FsRead(const char * pszName,unsigned char * psBuffer,uint unOffset,uint unLength)
功能描述：文件读
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_FsRead(const char * pszName,unsigned char * psBuffer,uint unOffset,uint unLength)
{
	int fd;
	int nRet;

	memset(psBuffer, 0x00, unLength);
	nRet = NDK_FsExist(pszName);
	if(nRet!=0)
	{
		ME31_DEBUG("file not exist err\n");
		return nRet;
	}
	fd = NDK_FsOpen(pszName, "r");
	if(fd<0) {
		ME31_DEBUG("open err\n");
		return -1;
	}

	nRet = NDK_FsSeek(fd, unOffset, SEEK_SET);
	if(nRet!=0){
		ME31_DEBUG("seek err =%d\n",nRet);
		NDK_FsClose(fd);
		return -2;
	}
	
	nRet = NDK_FsRead(fd,(char *)psBuffer, unLength);
	if(nRet!=unLength){
		ME31_DEBUG("read err =%d\n",nRet);
		NDK_FsClose(fd);
		return -3;
	}
	NDK_FsClose(fd);
	return 0;
}









/*
***********************************************************************************************
函数原型：Default_ME_MKSetup
功能描述：通用版本，密钥手输
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int Default_ME_MKSetup(void)
{
	int nKey;
	int nRet;
	unsigned int  unLen;
	unsigned int  ucMKNO;
	char  szAsciiBuf[33];
	unsigned char  uszKeyValue[16];
	ST_SEC_KEY_INFO stKeyInfoIn;
	ST_SEC_KCV_INFO stKcvInfoIn;

	while(1)
	{
		me_ScrClrs();
		NDK_ScrPrintf("请输入主密钥索引:\n");/*"请输入主密钥号"*/
		NDK_ScrBackLight(BACKLIGHT_ON);
		NDK_SysBeep();

		nRet= NDK_KbGetInput(szAsciiBuf, 1, 3, &unLen, INPUTDISP_NORMAL, 10000, INPUT_CONTRL_NOLIMIT_ERETURN);
		if(nRet!=0)
		{
	       strcpy(szAsciiBuf,"输入主密钥 索引失败!");
	       goto on_err;
		}
		ucMKNO = atoi((char *)szAsciiBuf);
		if( (ucMKNO == 0) || (ucMKNO >= 255) )		/*1 - 255 */
		{
            strcpy(szAsciiBuf,"不合法的主密钥号");
            goto on_err;
		}

		
		me_ScrClrs();
		NDK_ScrPrintf("请输入主密钥:\n");/*"请输入主密钥"*/
		nRet = NDK_KbGetInput(szAsciiBuf, 32, 32, &unLen, INPUTDISP_PASSWD, 10000, INPUT_CONTRL_NOLIMIT_ERETURN);
		if((unLen!=32)||(nRet!=0))
		{
		       strcpy(szAsciiBuf,"输入主密钥 索引失败!");
		       goto on_err;
		}
		
		memset(uszKeyValue, 0, sizeof(uszKeyValue));
		NDK_AscToHex((uchar *)szAsciiBuf, unLen, 0, uszKeyValue);

		memset(&stKeyInfoIn, 0x00, sizeof(stKeyInfoIn));
		memset(&stKcvInfoIn, 0x00, sizeof(stKcvInfoIn));
		
		stKeyInfoIn.ucDstKeyType = SEC_KEY_TYPE_TMK; 
		stKeyInfoIn.ucDstKeyIdx = ucMKNO; 	// 目的密钥索引
		memcpy(stKeyInfoIn.sDstKeyValue, uszKeyValue, 16);
		stKeyInfoIn.nDstKeyLen = 16;
		stKeyInfoIn.ucScrKeyIdx = 0; 		// 密钥索引
		stKeyInfoIn.ucScrKeyType = SEC_KEY_TYPE_TMK;
		stKcvInfoIn.nLen = 0;
		stKcvInfoIn.nCheckMode = 0;
		
		ME31_DEBUG_BUF(stKeyInfoIn.sDstKeyValue, stKeyInfoIn.nDstKeyLen);

		if((nRet= NDK_SecLoadKey(&stKeyInfoIn, &stKcvInfoIn)) != NDK_OK)
		{
			me_ScrClrs();
			NDK_ScrPrintf("主密钥 保存失败%02x!",nRet);/*"请输入主密钥号"*/
			NDK_KbGetCode(2, &nRet);
			return 0;
		}
		me_ScrClrs();
		NDK_ScrPrintf("主密钥 保存成功!");/*"请输入主密钥号"*/
		me_ScrGotoxy(0, 36);
		NDK_ScrPrintf("退出             继续");		
		if(NDK_KbGetCode(0, &nKey)==NDK_OK)
		{
			if(nKey==K_ESC)
				return 0;
		}
	}
on_err:
	me_ScrClrs();
	NDK_ScrPrintf(szAsciiBuf);
	NDK_KbGetCode(2, NULL);
	return 0;
}	

/*
***********************************************************************************************
函数原型：ME_ConfigNew
功能描述：创建一个新的配置文件
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_ConfigNew(void)
{
	int fd;
	int iLen;
	ME31_DEBUG("\r\nFILE NO EXIST");
	memcpy(g_me31conf.magic,CONFIG_MAGIC_ME31APP,8);
#ifndef NOAUDIO    
	g_me31conf.u1CommMode = g_METPEDCtl.ucDefaultMode;		/*默认为USB通讯方式，下载ksn后自动切换成蓝牙*/
#else
    g_me31conf.u1CommMode = COMMODE_AUDIO;      // 音频
    //g_me31conf.u1CommMode = COMMODE_USB;      // USB
#endif
	ME_SecKekInit();
	g_me31conf.u1SleepTime = 30;
	g_me31conf.u1StandByTime = 5;
	g_me31conf.u1LcdContrast = 19;
	g_me31conf.u1ShutDownTime= 60*5;
	strcpy(g_me31conf.szAdminPassword,ME_PASSWORD_ADMIN);
	strcpy(g_me31conf.szAppExitPassword,ME_PASSWORD_ESC);
	strcpy(g_me31conf.szKlaPassword,ME_PASSWORD_KLA);
	g_me31conf.ME_TDeviceData.DevicePersonalStatus = 0xff;	//设备个人化状态
	strcpy((char *)(g_me31conf.ME_TDeviceData.DeviceValidity),"2012");	//2020 年12月
	g_me31conf.ME_TDeviceData.DeviceWorkStatus = 0x11;
	fd = NDK_FsOpen(ME_CONF_FILE, "w");
	if(fd<0)
		return -1;

	if(NDK_FsSeek(fd,0,0)<0)
		return -1;

	iLen = NDK_FsWrite(fd,(char *)&g_me31conf, sizeof(ME31_CONFIG_T));
	NDK_FsClose(fd);
	if(iLen!=sizeof(ME31_CONFIG_T))
		return -1;
	return 0;
}


/*
***********************************************************************************************
函数原型：ME_ConfigConvert
功能描述：配置信息转换,旧版本ME31的配置信息转换为新版本
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_ConfigConvert(void)
{
	int fd;
	int iLen;
	ME31_OLDCONFIG_T me_oldconfig_t;

	fd = NDK_FsOpen(ME_CONF_FILE, "w");
	if(fd<0)
		return -1;
	if(NDK_FsSeek(fd,0,0)<0)
		goto err_ack;

	iLen = NDK_FsRead(fd, (char *)&me_oldconfig_t, sizeof(ME31_OLDCONFIG_T));
	if(iLen!=sizeof(ME31_OLDCONFIG_T))
		goto err_ack;

	if(NDK_FsSeek(fd,0,0)<0)
		goto err_ack;

	memset(&g_me31conf,0x00,sizeof(ME31_CONFIG_T));
	memcpy(g_me31conf.magic,CONFIG_MAGIC_ME31APP,8);

	if(me_oldconfig_t.u1CommuMode==0){//usb
		g_me31conf.u1CommMode = COMMODE_USB;
	}
	else{ 						//蓝牙
		g_me31conf.u1CommMode = COMMODE_BT;
	}

	if(me_oldconfig_t.u1SleepTime==0)
		g_me31conf.u1SleepTime = 15*60;
	else
		g_me31conf.u1SleepTime = me_oldconfig_t.u1SleepTime;

	if(me_oldconfig_t.u1StandByTime==0)
		g_me31conf.u1StandByTime = 60;
	else
		g_me31conf.u1StandByTime = me_oldconfig_t.u1StandByTime;

	strcpy(g_me31conf.szAdminPassword,ME_PASSWORD_ADMIN);
	strcpy(g_me31conf.szAppExitPassword,ME_PASSWORD_ESC);
	strcpy(g_me31conf.szKlaPassword,ME_PASSWORD_KLA);
	g_me31conf.ME_TDeviceData.DevicePersonalStatus = 0xff;	//设备个人化状态
	strcpy((char *)(g_me31conf.ME_TDeviceData.DeviceValidity),"2012");	//2020 年12月
	g_me31conf.ME_TDeviceData.DeviceWorkStatus = 0x11;

	iLen = NDK_FsWrite(fd, (char *)&g_me31conf, sizeof(g_me31conf));
	if(iLen!=sizeof(g_me31conf))
		goto err_ack;
	NDK_FsClose(fd);

	return 0;
err_ack:
	NDK_FsClose(fd);
	return -1;
}


/*
***********************************************************************************************
函数原型：ME_ConfigInit
功能描述：配置初始化
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_ConfigInit(void)
{
	int fd = -1;
	int iRet = 0;
	uint iLen = 0;
	char buf[12];

	iRet = NDK_FsExist(ME_CONF_FILE);

	if(iRet==0){				//文件已经存在
		ME31_DEBUG("\r\n ME_CONF_FILE EXISTS\n");
		NDK_FsFileSize(ME_CONF_FILE, &iLen);
		fd = NDK_FsOpen(ME_CONF_FILE, "w");
		if(fd<0)
			goto on_err_ack;

		if(NDK_FsSeek(fd,0,0)<0)
			goto on_err_ack;

		if(NDK_FsRead(fd, buf, 10)!=10)
			goto on_err_ack;


		if(memcmp(buf,CONFIG_MAGIC_ME31APP,5)!=0){		//旧格式的配置文件，读出数据重新初始化
			ME31_DEBUG("\r\nold format config");
			NDK_FsClose(fd);
			if(iLen!=sizeof(ME31_OLDCONFIG_T)){
				ME31_DEBUG("ME_CONF_FILE iLen err=%d--!!\n",iLen);
				return -1;
			}
			else {
				ME31_DEBUG("ME_ConfigConvert!!\n");
				return ME_ConfigConvert();
			}
		}
		else{
			if(iLen!=sizeof(ME31_CONFIG_T)){
				goto on_err_ack;
			}
			if(NDK_FsSeek(fd,0,0)<0)
				goto on_err_ack;

			if(NDK_FsRead(fd, (char *)&g_me31conf, sizeof(ME31_CONFIG_T))!=sizeof(ME31_CONFIG_T))
				goto on_err_ack;

			NDK_FsClose(fd);
		}

		if(memcmp(g_me31conf.magic,CONFIG_MAGIC_ME31APP,8)!=0){		//旧版本的配置文件，对于新增的部分需要重新初始化
			memcpy(g_me31conf.magic,CONFIG_MAGIC_ME31APP,8);
			g_me31conf.u1LcdContrast=19;
			g_me31conf.u1ShutDownTime = 60*5;
		}
		return 0;
	}
on_err_ack:
	NDK_FsClose(fd);
	return -1;
}


int ME_JudgeValue(char *buf,int min,int max)
{
    int tmp;

    if(((buf[0]<'0')&&(buf[0]!=0))||(buf[0]>'9')||((buf[1]<'0')&&(buf[1]!=0))||(buf[1]>'9'))
        return 0;

    if(buf[0]==0)
    {
        if((min==0)||(max==0))
            return 1;
    }

    if(buf[1])
    {
        tmp=(buf[0]-'0')*10+(buf[1]-'0');
    }
    else
        tmp=buf[0]-'0';

    if((tmp>=min)&&(tmp<=max))
        return 1;
    return 0;

}

void ME_ChangePosDate(void)
{
    struct tm pt;
    int i,ret = 0;
    char sbuf[30], bufyear[6], bufmon[4], bufdate[4], buftmp[6];
    uint len;

    strcpy(bufyear,"0000");
    strcpy(bufmon,"00");
    strcpy(bufdate,"00");

    NDK_SysGetPosTime(&pt);
    me_ScrClrs();
    me_ScrDispString(0, 0, "当前日期:  ", 0);
    sprintf(sbuf,"%d/%d/%d",1900 + pt.tm_year, pt.tm_mon + 1, pt.tm_mday);
    me_ScrDispString(0, 12, sbuf, 0);
    me_ScrDispString(0, 24, "输入日期,ESC退出", 0);

    do {
        me_ScrDispString(0, 36, "20  年  月  日", 0);
        me_ScrGotoxy(12, 36);
        if ((NDK_KbGetInput(buftmp, 2, 2, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT)) < 0)
            return;
    } while (ME_JudgeValue(buftmp, 0, 37) == 0);
    bufyear[0]='2';
    bufyear[1]='0';
    memcpy(bufyear+2, buftmp, 2);

    do {
        sprintf(sbuf,"%s年  月  日",bufyear);
        me_ScrDispString(0, 36, sbuf, 0);
        me_ScrGotoxy(36, 36);
        if ((NDK_KbGetInput(buftmp, 1, 2, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT)) < 0)
            return;
    } while (ME_JudgeValue(buftmp, 1, 12) == 0);

    for (i=0; i<len; i++)
        bufmon[1-i] = buftmp[len-1-i];

    do {
        sprintf(sbuf,"%s年%s月  日",bufyear,bufmon);
        me_ScrDispString(0, 36, sbuf, 0);
        me_ScrGotoxy(60, 36);
        if ((NDK_KbGetInput(buftmp, 1, 2, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT)) < 0)
            return;
        switch (atoi(bufmon)) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            ret=ME_JudgeValue(buftmp, 1, 31);
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            ret=ME_JudgeValue(buftmp, 1, 30);
            break;
        case 2:
            if ((atoi(bufyear) & 3) == 0)
                ret=ME_JudgeValue(buftmp, 1, 29);
            else
                ret=ME_JudgeValue(buftmp, 1, 28);
            break;
        }
    } while (ret == 0);
    for (i=0; i<len; i++)
        bufdate[1-i]=buftmp[len-1-i];

    sprintf(sbuf,"%s年%s月%s日",bufyear,bufmon,bufdate);
    me_ScrDispString(0, 36, sbuf, 0);
    pt.tm_year=atoi(bufyear)-1900;
    pt.tm_mon=atoi(bufmon)-1;
    pt.tm_mday=atoi(bufdate);
    NDK_SysSetPosTime(pt);
    return;

}

void ME_ChangePosTime(void)
{
    struct tm pt;
    int i;
    uint len;
    char sbuf[30],buftmp[6];
    char bufhour[4],bufmin[4],bufsec[4];

    NDK_SysGetPosTime(&pt);
    me_ScrClrs();

    strcpy(bufhour,"00");
    strcpy(bufmin,"00");
    strcpy(bufsec,"00");
    me_ScrDispString(0, 0, "当前时间:", 0);
    sprintf(sbuf,"%02d:%02d:%02d ", pt.tm_hour, pt.tm_min, pt.tm_sec);
    me_ScrDispString(0, 12, sbuf, 0);
    me_ScrDispString(0, 24, "输入时间,ESC退出", 0);

    do {
        me_ScrDispString(0, 36, "  :  :   ", 0);
        me_ScrGotoxy(0, 36);
        if ((NDK_KbGetInput(buftmp, 1, 2, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT)) < 0)
            return;
    } while (ME_JudgeValue(buftmp, 0, 23) == 0);

    for (i=0; i<len; i++)
        bufhour[1-i]=buftmp[len-1-i];

    do {
        sprintf(sbuf,"%s:  :   ",bufhour);
        me_ScrDispString(0, 36, sbuf, 0);
        me_ScrGotoxy(18, 36);
        if ((NDK_KbGetInput(buftmp,1, 2, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT)) < 0)
            return;
    } while (ME_JudgeValue(buftmp,0,59) == 0);
    for (i=0; i<len; i++)
        bufmin[1-i]=buftmp[len-1-i];

    do {
        sprintf(sbuf,"%s:%s:   ",bufhour,bufmin);
        me_ScrDispString(0, 36, sbuf, 0);
        me_ScrGotoxy(36, 36);
        if ((NDK_KbGetInput(buftmp, 1, 2, &len, INPUTDISP_NORMAL, 0, INPUT_CONTRL_LIMIT)) < 0)
            return;
    } while (ME_JudgeValue(buftmp,0,59) == 0);
    for (i=0; i<len; i++)
        bufsec[1-i]=buftmp[len-1-i];

    sprintf(sbuf,"%s:%s:%s ",bufhour, bufmin, bufsec);
    me_ScrDispString(0, 36, sbuf, 0);
    pt.tm_hour=atoi(bufhour);
    pt.tm_min=atoi(bufmin);
    pt.tm_sec=atoi(bufsec);
    NDK_SysSetPosTime(pt);
}

int ME_SetDateTime(void)
{
    int key;

    while (1) {
        me_ScrClrs();
        me_ScrDispString(0, 0, "1.设置日期", 0);
        me_ScrDispString(0, 12, "2.设置时间", 0);
rep:
        NDK_KbGetCode(0, &key);
        if (key == '1') {
            ME_ChangePosDate();
        }
        else if (key  == '2') {
            ME_ChangePosTime();
        }
        else if (key == K_ESC) break;
        else goto rep;
    }
    return 0;
}



/*
***********************************************************************************************
函数原型：ME_DebugSwitch
功能描述：调试开关
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_DebugSwitch(void)
{
	int nKey;
	me_ScrClrs();
	me_ScrDispString(0, 0, "         调试      ", 0);
	me_ScrDispString(0, 12, "1.开", 0);
	me_ScrDispString(0, 24, "2.关", 0);
	NDK_KbGetCode(0, &nKey);
	if(nKey==K_ONE){
		g_METPEDCtl.cIsDebug = 1;
		NDK_PortOpen(PORT_NUM_USB, NULL);
		me_ScrDispString(0, 36, "调试已开", 0);
	}
	else if(nKey==K_TWO){
		g_METPEDCtl.cIsDebug = 0;
		NDK_PortClose(PORT_NUM_USB);
		me_ScrDispString(0, 36, "调试已关", 0);
	}
	NDK_KbGetCode(1, NULL);
	return 0;
}







/*
***********************************************************************************************
函数原型：ME_ScrSetContrast
功能描述：设置屏幕对比度
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_ScrSetContrast(void)
{
	int iKey;
	int fd;
	int nlen;

	while(1)
	{
		me_ScrClrs();
		me_ScrDispString(0, 0,    "1.增加对比度", 0);
		me_ScrDispString(0, 12,  "2.减小对比度", 0);
		NDK_ScrGotoxy(64,52);
		NDK_ScrClrLine(52,64);
		NDK_ScrPrintf("当前对比度:%d",g_me31conf.u1LcdContrast);
		if(NDK_KbGetCode(0, &iKey)==NDK_OK)
		{
		    if(iKey==K_ONE){
				if(g_me31conf.u1LcdContrast<64)
		    		g_me31conf.u1LcdContrast++;
		    	ME31_DEBUG("nbrightVal=%d\n",g_me31conf.u1LcdContrast);
		       	NDK_ScrSetContrast(g_me31conf.u1LcdContrast);
		    }
		    else if(iKey==K_TWO){
				if(g_me31conf.u1LcdContrast>0x00)
		    		g_me31conf.u1LcdContrast--;
		    	ME31_DEBUG("nbrightVal=%d\n",g_me31conf.u1LcdContrast);
				NDK_ScrSetContrast(g_me31conf.u1LcdContrast);
		    }
		    else if(iKey==K_ESC)
		        break;
			else
				break;
			NDK_ScrGotoxy(64,52);
			NDK_ScrClrLine(52,64);
			NDK_ScrPrintf("当前对比度:%d",g_me31conf.u1LcdContrast);
		}
	}
	fd = NDK_FsOpen(ME_CONF_FILE, "w");
	nlen = NDK_FsWrite(fd, (char *)(&g_me31conf), sizeof(g_me31conf));
	if(nlen != sizeof(g_me31conf))
	{
		me_ScrClrs();
		NDK_ScrPrintf("fs write error");
		NDK_FsClose(fd);
		return 0;
	}
	NDK_FsClose(fd);
	return 0;
}



/*
***********************************************************************************************
函数原型：ME_FuncMenu
功能描述：一级菜单,按功能键进入
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_FuncMenu()
{
    int i;
    int nKey;
	int nRet;
	int nCount;
	unsigned int unTotalTicks;

	nCount = g_METPEDCtl.ucFuncItemCount;

	NDK_SysStopWatch(&unTotalTicks);
	NDK_SysStartWatch();	
    while (1) {		
		NDK_ScrClrs();
        for (i = 0; i < nCount; i ++) {
            NDK_ScrDispString(stFuncItem[i].xy[0],stFuncItem[i].xy[1], stFuncItem[i].name, 0);
        }
rep:
        nRet = nlMpos_Command.mpos_rdata_from_host(ME_Host_Command_Buf, sizeof(ME_Host_Command_Buf), 100);    //*读取报文
		if((nRet > 0)) {
			ME31_DEBUG("\r\nrecv msg:");
			ME31_DEBUG_BUF(ME_Host_Command_Buf, nRet);
			ME31_DEBUG("\r\n");			
			NDK_SysStopWatch(&unTotalTicks);
			nlMpos_Command.mpos_command_parse(ME_Host_Command_Buf, nRet);	//对接收到的数据进行解析并执行相应操作
			break;
		}

		nRet = NDK_KbHit(&nKey);
        NDK_SysReadWatch(&unTotalTicks);
		
		
		if(unTotalTicks>10000)
        	break;
		else if((nRet!=0)||(nKey==0))
			goto rep;
		else if (nKey== K_ESC)
            break;
        else{
             for (i = 0; i < nCount; i ++) {
                if(nKey==stFuncItem[i].nKey) {
					if((stFuncItem[i].xy[1]<60)){
						me_ScrClrs();
					}
                    if(stFuncItem[i].func()==ME_ERR_FALLBACK)
						return 0;
                    break;
                }
            }
            if(i==nCount)
                goto rep;
        }
    }	
	NDK_SysStopWatch(&unTotalTicks);
    NDK_ScrClrs();
    return 0;
}

#if 0
int ME_KeyManagement()
{
		unsigned char  ucBuf[33];
		unsigned char  ucLenCtl[6];
		unsigned int  uiMKNO;
		unsigned int  uiLen;
		int nkey,nkey1;
		int iRet;
		char			 szTerminalID[8 + 1];
		char			 szMerchantID[15 + 1];
		char			 szMerchantName[64+1];
		tlv_t			 tlv_tmp;
		unsigned char szCmdBuf[64];
		
		uchar szbuf[TERMINAL_SIZE];
	
		memset(szTerminalID,0,sizeof(szTerminalID));
		memset(szMerchantID,0,sizeof(szMerchantID));
		memset(szMerchantName,0,sizeof(szMerchantName));
		memset(&tlv_tmp,0,sizeof(tlv_t));
	
loop1:
		
	ME_DispStaBar();
	me_ScrClrs();
    mpos_DispStrXY(16, "请选择要导入的密钥",strlen("请选择要导入的密钥"));
	mpos_DispStrXY(28, "1.IC卡导密钥",strlen("1.IC卡导密钥"));
	mpos_DispStrXY(40, "2.IC卡导主控密钥",strlen("2.IC卡导主控密钥"));
		
		while (1) {
				NDK_KbHit(&nkey);
				if (nkey == K_ESC)
				{
					return 0;
				}
				if(nkey == K_ONE) 
				{
					SetMerchant(0);
					me_ScrClrs();
					mpos_DispStrXY(16, "IC卡导密钥",strlen("IC卡导密钥"));
					break;
				}
				if(nkey == K_TWO)
				{
					//SetMerchant(1);
					me_ScrClrs();
					mpos_DispStrXY(16, "IC卡导主控密钥",strlen("IC卡导主控密钥"));
					break;
				}
			}
		
		mpos_DispStrXY(32,"请插入密钥IC卡",14);
		NDK_IccPowerDown(ICTYPE_IC);
		while (1) {
				NDK_KbHit(&nkey1);
				if (nkey1 == K_ESC) {
					goto loop1;
				}
	
				NDK_IccDetect(&iRet);
				if (iRet & 0x01) {	/**< 有IC卡插入 */
					me_ScrClrs();
					NDK_ScrPrintf("检测到IC卡!");
					break;
				}
		}
//loop:
		ME_DispStaBar();
		me_ScrClrs();

		/*
		if(GetMerchantFlag() == 0){
			mpos_DispStrXY(16, "商户1相关设置\n",14);
		}
		else{
			mpos_DispStrXY(16, "商户2相关设置\n",14);
		}
		*/
		/*
		NDK_ScrPrintf("1.导入主密钥\r\n");
		NDK_ScrPrintf("2.导入主控密钥\r\n");
		NDK_ScrPrintf("3.设置终端参数\r\n");
		*/
		//NDK_ScrPrintf("3.终端参数开关设置\r\n");
		
		memset(szbuf,0,sizeof(szbuf));
		//iRet = NDK_KbGetCode(0, &nkey);
		//if(iRet==0)
		//{
			NDK_IccPowerDown(ICTYPE_IC);
			if(nkey==K_ONE){
				me_ScrClrs();
				NDK_ScrPrintf("请输入主密钥索引:");
				NDK_ScrBackLight(BACKLIGHT_ON);
				me_ScrGotoxy(0, 12);
				ucLenCtl[0] = 3;
				me_SetRangeBits(ucLenCtl+1, 1, ucLenCtl[0]);
				memset(ucBuf, 0, sizeof(ucBuf));
				if(me_getline_nl(ucBuf, ucLenCtl, ME_KEY_MODE_MOVE|ME_WAITENTER, 0, 20) <= 0)
				{
					me_ScrClrs();
					NDK_ScrPrintf("输入主密钥索引 失败!");
					NDK_KbGetCode(2, &nkey);
					goto loop1;
				}
				uiMKNO = atoi((char *)ucBuf);
				if( (uiMKNO == 0) || (uiMKNO >= 255) )		/*1 - 255 */
				{
					me_ScrClrs();
					NDK_ScrPrintf("获取主密钥索引 失败! = %d",uiMKNO);	
					NDK_KbGetCode(2, &nkey);
					goto loop1;
				}
	
				memset(&tlv_tmp,0x00,sizeof(tlv_t));
				tlv_tmp.tagname = 0x5f0d;
				tlv_tmp.pvalue = szMerchantID;
				mpos_readonetlv(ME_TLV_FILE,&tlv_tmp);

				memset(&tlv_tmp,0x00,sizeof(tlv_t));
				tlv_tmp.tagname = 0x5f0e;
				tlv_tmp.pvalue = szTerminalID;
				mpos_readonetlv(ME_TLV_FILE,&tlv_tmp);
#if 0	
				me_ScrClrs();
				NDK_ScrPrintf("请输入商户号:");
				me_ScrGotoxy(0, 12);
				memset(szMerchantID, 0, sizeof(szMerchantID));
				memset(szTerminalID, 0, sizeof(szTerminalID));
				memset(szMerchantName, 0, sizeof(szMerchantName));
				memset(szCmdBuf, 0, sizeof(szCmdBuf));
				
				iRet = NDK_KbGetInput((char *)szMerchantID, 0, 15, &uiLen, INPUTDISP_NORMAL, 10000, INPUT_CONTRL_NOLIMIT_ERETURN);
				if(iRet!=0)
				{
					me_ScrClrs();
					NDK_ScrPrintf("输入商户号 失败!");
					memset(szMerchantID,0x00,sizeof(szMerchantID));
					NDK_KbGetCode(1, &nkey);
					goto loop1;
				}
	
				me_ScrClrs();
				NDK_ScrPrintf("请输入终端号:");
				me_ScrGotoxy(0, 12);
				iRet = NDK_KbGetInput((char *)szTerminalID, 0, 8, &uiLen, INPUTDISP_NORMAL, 10000, INPUT_CONTRL_NOLIMIT_ERETURN);
				if(iRet!=0)
				{
					me_ScrClrs();
					NDK_ScrPrintf("输入终端号 失败!");
					memset(szTerminalID,0x00,sizeof(szTerminalID));
					NDK_KbGetCode(1, &nkey);
					goto loop1;
				}
#endif
				szCmdBuf[0] = 0x02;
				szCmdBuf[1] = 0x00;
				szCmdBuf[2] = 0x29;
				szCmdBuf[3] = 0xe1;
				szCmdBuf[4] = 0x05;
				szCmdBuf[5] = 0x2F;
				szCmdBuf[6] = 0x01;
				szCmdBuf[7] = 0x00;
				uiMKNO+=32;
				szCmdBuf[8] = uiMKNO;
				memcpy(&szCmdBuf[9],szMerchantID,15);
				memcpy(&szCmdBuf[9+15],szTerminalID,8);
				CommandParse_SetMainKeyByIC(szCmdBuf,40);
			}
			else if(nkey==K_TWO){
				me_ScrClrs();
				CommandParse_SetTagKeyByIC();
			}
			//else if(nkey==K_ESC)
			//	goto loop1;
			goto loop1;
		//}
		return SUCC;
	

}
#endif


 /*
 ***********************************************************************************************
 函数原型：ME_PasswordMenu
 功能描述：进入二级菜单的密码输入界面
 输入参数：
 输出参数：无
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */
 int ME_PasswordMenu()
 {
	 int i;
	 int nCount;
	 uint unLen;
	 char szKeyBuf[16];

	nCount = g_METPEDCtl.ucPasswordItemCount;
	for (i = 0; i < g_METPEDCtl.ucFuncItemCount; i ++) {
		if(stFuncItem[i].xy[1]>60)      //如果有y坐标大于60，则不显示"请输入密码"界面
			break;
	}
	if(i==g_METPEDCtl.ucFuncItemCount){
		 me_ScrClrs();
		 me_ScrDispString(0, 0, "请输入密码:", 0);
		 me_ScrGotoxy(2, 12);
	}
	else
		NDK_ScrGotoxy(0,63);
	 if ((NDK_KbGetInput(szKeyBuf, 0, 8, &unLen, INPUTDISP_PASSWD , 10, INPUT_CONTRL_LIMIT_ERETURN)) == 0){
		for (i = 0; i < nCount; i ++) {
			 if(strcmp(stPasswordItem[i].szKeyStr,szKeyBuf)==0) {
				 stPasswordItem[i].func();
				 break;
			 }
		 }
	 }
	me_ScrClrs();
	ME_DispStaBar();
	ME_Disp_Bitmap();
	return 0;
 }

/*
***********************************************************************************************
函数原型：ME_Sysmanage
功能描述：系统设置菜单，设置时间和自动休眠时间等
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
 int ME_Sysmanage(void)
{
    int i;
    int nKey;
	int nCount;

	nCount = g_METPEDCtl.ucSysItemCount;
    while (1) {
        me_ScrClrs();
        for (i = 0; i < nCount; i ++) {
            NDK_ScrDispString(stSysItem[i].xy[0],stSysItem[i].xy[1], stSysItem[i].name, 0);
        }
  rep:
        NDK_KbGetCode(0, &nKey);

         if (nKey== K_ESC)
            break;
        else{
             for (i = 0; i < nCount; i ++) {
                if(nKey==stSysItem[i].nKey) {
                    stSysItem[i].func();
                    break;
                }
            }
            if(i==nCount)
                goto rep;
        }

    }

    return 0;
}

/*
***********************************************************************************************
函数原型：ME_SetCommMode
功能描述：设置通讯模式，蓝牙\USB\音频
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_SetCommMode()
{
    int i;
    int nKey;
	int nCount;

	nCount = g_METPEDCtl.ucCommItemCount;

    while (1) {
        me_ScrClrs();
        for (i = 0; i < nCount; i ++) {
            NDK_ScrDispString(stCommItem[i].xy[0],stCommItem[i].xy[1], stCommItem[i].name, 0);
        }
rep:
        NDK_KbGetCode(0, &nKey);

         if (nKey== K_ESC)
            break;
        else{
             for (i = 0; i < nCount; i ++) {
                if(nKey==stCommItem[i].nKey) {
                    stCommItem[i].func();
                    break;
                }
            }
            if(i==nCount)
                goto rep;
        }
    }
    return 0;
}


/*
***********************************************************************************************
函数原型：ME_Disp_Bitmap
功能描述：显示主界面
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
void ME_Disp_Bitmap(void)
{
	uint width, height;
	struct tm ltm;
	char szBTName[30];
	char sysTime[30];

	NDK_ScrGetLcdSize(&width, &height);
	if(NDK_SysGetPosTime(&ltm) != NDK_OK) {
		return;
	}
	NDK_ScrClrLine(nlMpos_Command.Lcd_StartY, height);
	mpos_DispStrXY(4+16,(char *)pszHello,strlen(pszHello));
	if(g_me31conf.u1CommMode==COMMODE_BT){
		memset(szBTName, 0x00, sizeof(szBTName));
		if(!strlen(g_METPEDCtl.u1BTName)){
			sprintf(szBTName,"正在获取蓝牙名...");
		}
		else if(strlen(g_METPEDCtl.u1BTName)>21-7){       //银盛的蓝牙名太长了，做个特殊处理吧
		    sprintf(szBTName,"蓝牙:%s",g_METPEDCtl.u1BTName);
		}
		else{
			sprintf(szBTName,"蓝牙名:%s",g_METPEDCtl.u1BTName);
		}
		mpos_DispStrXY(32,szBTName,strlen(szBTName));
	}
	else if(g_me31conf.u1CommMode==COMMODE_AUDIO){
		mpos_DispStrXY(32,"音频通信模式",strlen("音频通信模式"));
	}
	else{
	        g_me31conf.u1CommMode = COMMODE_USB;
		 mpos_DispStrXY(32,"USB通信模式",strlen("USB通信模式"));
	}
	memset(sysTime, 0x00, sizeof(sysTime));
	sprintf(sysTime,"%d-%02d-%02d %02d:%02d:%02d", ltm.tm_year+1900, ltm.tm_mon+1, ltm.tm_mday, ltm.tm_hour, ltm.tm_min,ltm.tm_sec);
	mpos_DispStrXY(44,sysTime,strlen(sysTime));
	return;
}

/*
***********************************************************************************************
函数原型：ME_UsbParaSet
功能描述：USB参数设置
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_UsbParaSet()
{
    int fd,nLen;
	ME_API_COMClose();
    fd = NDK_FsOpen(ME_CONF_FILE, "w");
    if(fd<0)
    {
        me_ScrClrs();
        NDK_ScrPrintf("fs error");
        return 0;
    }
    NDK_FsSeek(fd, 0, 0);
    g_me31conf.u1CommMode = COMMODE_USB;
    nLen = NDK_FsWrite(fd, (char *)(&g_me31conf), sizeof(g_me31conf));
     me_ScrClrs();
    if(nLen == sizeof(g_me31conf)) {
        NDK_ScrPrintf("设置成功");
    }else {
        NDK_ScrPrintf("设置失败");
    }
	NDK_FsClose(fd);
    ME_API_COMInit(COMMODE_USB);
	return 0;
}


/*
***********************************************************************************************
函数原型：ME_AudioParaSet
功能描述：音频参数设置
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_AudioParaSet()
{
    int fd,nLen;
    int nRet;

    nRet = NDK_PortOpen(PORT_NUM_AUDIO,NULL); 	//音频
    if((nRet==NDK_ERR_NOT_SUPPORT)||(nRet==NDK_ERR_NO_DEVICES)||(nRet==NDK_ERR_OPEN_DEV)||(nRet==NDK_ERR_PARA)){
        me_ScrClrs();
        NDK_ScrPrintf("暂不支持");
        NDK_KbGetCode(1, NULL);
        return 0;
    }

    ME_API_COMClose();
    fd = NDK_FsOpen(ME_CONF_FILE, "w");
    if(fd<0)
    {
        me_ScrClrs();
        NDK_ScrPrintf("fs error");
        return 0;
    }
    NDK_FsSeek(fd, 0, 0);
    g_me31conf.u1CommMode = COMMODE_AUDIO;
    nLen = NDK_FsWrite(fd, (char *)(&g_me31conf), sizeof(g_me31conf));
    me_ScrClrs();
    if(nLen == sizeof(g_me31conf)) {
        NDK_ScrPrintf("设置成功");
    }else {
        NDK_ScrPrintf("设置失败");
    }
	NDK_FsClose(fd);
    ME_API_COMInit(COMMODE_USB);
	return 0;
}


/*
***********************************************************************************************
函数原型：int ME_KbHit()
功能描述：获取按键值并保存按键到全局缓冲，供B1 01读取
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_KbHit()
{
    int nKey;
    if((NDK_KbHit(&nKey) != NDK_OK)||(nKey==0))
    {
        return -1;
    }
    if(((nKey >= K_ZERO) && (nKey <= K_NINE)) || (nKey == K_F2))
        g_METPEDCtl.ucKey = nKey;
    else
        g_METPEDCtl.ucKey = -1;
    return nKey;
}


/*
***********************************************************************************************
函数原型：int ME_SetFrushTime(int nMs)
功能描述：设置主界面刷新时间间隔
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
int ME_SetFrushTime(int nMs)
{
    g_METPEDCtl.unLcdTimeout = nMs;
    return 0;
}






/*
***********************************************************************************************
函数原型：ME31_PEDInit
功能描述：POS配置初始化
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
   int ME31_PEDInit(void)
{
	int iRet;
	uint unLen;
	char szMasterVer[64];

	memset(szMasterVer,0x00,sizeof(szMasterVer));
	ME_ParaInit();
	if(ME_ConfigInit() < 0)   /* 读配置文件 失败 */
	{
		NDK_FsDel(ME_CONF_FILE);
		if(ME_ConfigNew()!=0)
	   		return -1;
	}
	NDK_SysGetPosInfo(SYS_HWINFO_GET_BIOS_VER, &unLen, (char *)szMasterVer); /*固件版本*/
	if(memcmp(szMasterVer,"Scorpio 1.2.22",strlen("Scorpio 1.2.22"))>=0)
		NDK_ScrSetContrast(g_me31conf.u1LcdContrast);
	//NDK_SysSetShutdownDelay(g_me31conf.u1ShutDownTime);

    if(ME_API_COMInit(g_me31conf.u1CommMode) < 0)   /* 初始化串口*/
    {
		ME31_DEBUG("\nME_InitPortPara err !!\n");
        return -1;
    }

    ME_TlvInit();
    return 0;
}

 /*
功能:    ME31  K21 平台1.0 .  写入SN
输入:    
输出:    
返回:    
*/  
int _NDK_SysSetPosInfo(EM_SYS_HWINFO emFlag,char *psBuf)
{
	if(emFlag==SYS_HWINFO_GET_POS_USN)
		return Private_SysSetPosInfo(0x02, psBuf);
	else if(emFlag==SYS_HWINFO_GET_POS_PSN)	
		return Private_SysSetPosInfo(0x01, psBuf);
	else if(emFlag==SYS_HWINFO_GET_BOARD_VER)	
		return Private_SysSetPosInfo(0x04, psBuf);
	else
		return NDK_ERR_NOT_SUPPORT;
}


/*
***********************************************************************************************
函数原型：ME31_PEDMain
功能描述：
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/

extern int origin_call(void);
void ME31_PEDMain(void)
{
	int i,nRet, nKey = 0;
	int TamperStatus = 0;

	memset(&g_METPEDCtl,0x00,sizeof(ME_TPEDCTL));
	memset(&g_me31conf,0x00,sizeof(ME31_CONFIG_T));

	NDK_ScrClrs();
	NDK_ScrPrintf("       Init..\n");

	if(ME31_PEDInit() < 0) { /* 读配置文件 失败 */
		ME31_DEBUG("--ME31_PEDInit err---\n");
		NDK_KbGetCode(3, NULL);
		return;
	}

	g_METPEDCtl.unLcdTimeout = 500;
	NDK_ScrClrs();
	NDK_ScrPrintf("\n       Welcome\n");
	NDK_ScrPrintf("  V");
	NDK_ScrPrintf("%s", PinpadVersion);
	while(1)
	{
		/* ====================================接收命令并进行解析，返回响应报文=====================================================*/
		//nRet返回数据的长度
		NDK_SecGetTamperStatus(&TamperStatus);
		if(TamperStatus != SEC_TAMPER_STATUS_NONE){
			PDEBUG("tamper status:%d", TamperStatus);
			return -1;
		}
		nRet = nlMpos_Command.mpos_rdata_from_host(ME_Host_Command_Buf, sizeof(ME_Host_Command_Buf), 100);    //*读取报文
		if (nRet > 0)
		{
			nlMpos_Command.mpos_command_parse(ME_Host_Command_Buf, nRet);//对接收到的数据进行解析并执行相应操作
			NDK_ScrClrs();
			NDK_ScrPrintf("\n       Welcome\n");
			NDK_ScrPrintf("  V");
			NDK_ScrPrintf("%s", PinpadVersion);
		}
		NDK_KbHit(&nKey);
		if(nKey == 0x1b)
			break;
	}	    
}



