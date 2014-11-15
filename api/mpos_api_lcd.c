#if 1
 /*
***********************************************************************************************
版权说明：API_KB
版本号：
生成日期：
作者：
模块目的/功能：
主要函数及其功能：
修改日志：
***********************************************************************************************
*/
#include <string.h>
#include "NDK.h"
#include "mpos_command.h"
#include "mpos_api_lcd.h" 
#include "unistd.h"
#include "key.h"




/*
***********************************************************************************************
* 宏定义
***********************************************************************************************
*/
#define me_isdigit(x) (((x) >= '0') && ((x) <= '9'))
#define me_isprint(x) ( (((x) >= '0') && ((x) <= '9')) || (((x) >= 'a') && ((x) <= 'z')) || (((x) >= 'A') && ((x) <= 'Z')) )

#define MIN_CHAR_ASCII	0x20
#define MAX_CHAR_ASCII	0x7f

#define MIN_HZ_ASCII	0x80
#define MAX_HZ_ASCII	0xbc

/*
***********************************************************************************************
* 变量定义
***********************************************************************************************
*/

const char bat_icon[4][16] = {
/*--  宽度x高度=16x8  --*/
    {
	0x00,0x1C,0x14,0x7F,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x7F,0x00,
    },
    {
        0x00,0x1C,0x14,0x7F,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x5D,0x5D,0x41,0x7F,0x00,
    },
    {
        0x00,0x1C,0x14,0x7F,0x41,0x41,0x41,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x7F,0x00,
    },	
    {
        0x00,0x1C,0x14,0x7F,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x7F,0x00,
    },  
};

const char clr_icon[]= {
	/*--  宽度x高度=8x16  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const char bt_icon[] ={
	/*--  宽度x高度=8x16  --*/
	0x00,0x08,0x90,0xFE,0x22,0x54,0x88,0x00,0x00,0x01,0x00,0x03,0x02,0x01,0x00,0x00,
};



int ClrBackChar(void);
int ClrLine(void);
int DispOneChar(unsigned char c);
int DispAlphaChar(unsigned char ucAlphanumericEn, unsigned char ucData, unsigned char ucMaskFlg, unsigned char ucMask);

/*
***********************************************************************************************
函数原型：signed char ClrBackChar(void)
功能描述：清除文本界面当前坐标前一个字符，按照ASCII字符大小为单位清除
输入参数：无
输出参数：无
返回值：
调用关系：
***********************************************************************************************
*/
int ClrBackChar(void)
{
	uint xpos, ypos, lcd_width, lcd_heigth, zm_width,zm_heigth;
	
	NDK_ScrGetxy(&xpos, &ypos);
	NDK_ScrGetLcdSize(&lcd_width, &lcd_heigth);
	NDK_ScrGetFontSize(&zm_width, &zm_heigth);
	
	zm_width = zm_width/2;
	if(xpos <zm_width)
	{
		if(ypos >= zm_heigth)
		{
			ypos -= zm_heigth;
			xpos =  lcd_width-zm_width;
		}
	}
	else
	{
		xpos -= zm_width;
	}
	
	NDK_ScrDispString(xpos, ypos, " ", 0);
	NDK_ScrGotoxy(xpos, ypos);
	
	return 0;
}

/*
***********************************************************************************************
函数原型：ClrLine
功能描述：
输入参数：无
输出参数：无
返回值：
调用关系：
***********************************************************************************************
*/
int ClrLine(void)
{
	uint xpos, ypos, zm_width,zm_heigth;
	NDK_ScrGetxy(&xpos, &ypos);
	NDK_ScrGetFontSize(&zm_width, &zm_heigth);
	NDK_ScrClrLine(ypos,ypos+zm_heigth-1);
	return 0;
}
/*
***********************************************************************************************
函数原型：DispOneChar
功能描述：
输入参数：无
输出参数：无
返回值：
调用关系：
***********************************************************************************************
*/
int DispOneChar(uchar c)
{
	uint xpos, ypos;
	char tmp[2];
	
	tmp[0] = c;
	tmp[1] = '\0';
	NDK_ScrGetxy(&xpos, &ypos);
	NDK_ScrDispString(xpos, ypos, tmp, 0);
	
	return 0;
}
/*
***********************************************************************************************
函数原型：DispAlphaChar
功能描述：
输入参数：无
输出参数：无
返回值：
调用关系：
***********************************************************************************************
*/
int DispAlphaChar(unsigned char ucAlphanumericEn, unsigned char ucData, unsigned char ucMaskFlg, unsigned char ucMask)
{
	uchar ucDispChar;
	if(ucMaskFlg) {
		ucDispChar = ucMask;	/*用覆盖字符显示*/
	}
	else {
		ucDispChar = ucData;
	}
	if(ucAlphanumericEn) {
		if(me_isprint(ucData))	{
			DispOneChar(ucDispChar);	/*允许非数字字符*/
		}
		else {
			return FAIL;
		}
	}
	else {								/*只允许数字字符*/
		if(me_isdigit(ucData)) {
			DispOneChar(ucDispChar);
		}
		else {
			return FAIL;
		}
	}
	return SUCC;
}

int IsAscii(unsigned char ucChar)
{
	if((ucChar >= MIN_CHAR_ASCII) && (ucChar <= MAX_CHAR_ASCII)) /*处于字符区间*/
	{
		return SUCC;
	}
	else
	{
		return FAIL;
	}
}

int IsHZ(unsigned char ucChar)
{
	if((ucChar >= MIN_HZ_ASCII) && (ucChar <= MAX_HZ_ASCII))/*处于汉字区间*/
	{
		return SUCC;
	}
	else
	{
		return FAIL;
	}
}

void me_ScrClrs()
{
	uint lcdx,lcdy;
	ME_DispStaBar();
	NDK_ScrGetLcdSize(&lcdx, &lcdy);
	NDK_ScrClrLine(nlMpos_Command.Lcd_StartY, lcdy);
	NDK_ScrGotoxy(0, nlMpos_Command.Lcd_StartY);
	return;
}


int me_ScrGotoxy(uint unX,uint unY)
{
	uint lcdx,lcdy;
	NDK_ScrGetLcdSize(&lcdx, &lcdy);
	if(unY+nlMpos_Command.Lcd_StartY>lcdy)
		return -1;
	return NDK_ScrGotoxy(unX, unY+nlMpos_Command.Lcd_StartY);
}

int me_ScrGetxy(uint *x,uint *y)
{
	uint tmpx,tmpy;
	NDK_ScrGetxy(&tmpx, &tmpy);
	*x = tmpx;
	if(tmpy>=nlMpos_Command.Lcd_StartY)
		*y = tmpy-nlMpos_Command.Lcd_StartY;
	else
		*y = tmpy;
	return 0;
}


int me_ScrDispString(uint unX, uint unY, const char * pszS, uint unMode)
{
	uint lcdx,lcdy;
	NDK_ScrGetLcdSize(&lcdx, &lcdy);
	if(unY+nlMpos_Command.Lcd_StartY>lcdy)
		return -1;
	return NDK_ScrDispString(unX, unY+nlMpos_Command.Lcd_StartY,pszS,unMode);
}


void mpos_DispStrXY( int nStarty, char *strLineText, char uLen )
{
	int nMaxNum,nOffset;
	uint unLcdWidth,unLcdHeight;
	uint unFontWidth,unFontHeight;	
	char szDispStr[24];			//最大128/6

	memset(szDispStr,0x00,sizeof(szDispStr));
	NDK_ScrGetLcdSize(&unLcdWidth, &unLcdHeight);
	NDK_ScrGetFontSize(&unFontWidth, &unFontHeight);
	nMaxNum = unLcdWidth*2/unFontWidth;
	if(uLen > nMaxNum)
        uLen = nMaxNum;	
	memcpy(szDispStr,strLineText,uLen);
	nOffset = (unLcdWidth-uLen*unFontWidth/2)/2;
	NDK_ScrClrLine(nStarty,nStarty+unFontHeight);
	NDK_ScrDispString(nOffset, nStarty,strLineText,0);
	return;
}

/*
***********************************************************************************************
函数原型：signed char mpos_DispImage(unsigned int uixpos, unsigned int uiypos, unsigned int uiWidth, unsigned int uiHeight, const unsigned char *pucImageBuf)
功能描述：对面向液晶显示的点阵数据（点阵规则是为以字节为单位，表示一列显示数据，按照列单位从左到
		  右，从上至下排列）进行显示。
输入参数：uixpos：显示起始x坐标
		  uiypos：显示起始y坐标
		  uiWidth：显示宽度
		  uiHeight：显示高度
		  pucImageBuf：点阵数据
输出参数：无
返回值：  
调用关系：
***********************************************************************************************
*/
signed char mpos_DispImage(unsigned int uixpos, unsigned int uiypos, unsigned int uiWidth, unsigned int uiHeight, const char *pucImageBuf)
{
	int iHCount, iWCount, iPCount;/*高度计数、宽度计数、点计数*/
	unsigned char uc,uctmp;
	EM_TEXT_ATTRIBUTE text_attr;

	NDK_ScrSetAttr(TEXT_ATTRIBUTE_NORMAL,&text_attr);
	NDK_ScrSetAttr(text_attr, NULL);
	for(iHCount = 0; iHCount < uiHeight; iHCount += 8)/*液晶显示点阵默认都是以一列8点为单位*/
	{
		if(iHCount+8>uiHeight)
			uctmp = uiHeight-iHCount;
		else
			uctmp = 8;
		for(iWCount = 0; iWCount < uiWidth; iWCount++)
		{
			uc = *pucImageBuf;
			for(iPCount = 0; iPCount < uctmp; iPCount++)
			{
				if(text_attr == TEXT_ATTRIBUTE_NORMAL)
				{
					NDK_ScrPutPixel(uixpos+iWCount, uiypos+iHCount+iPCount, (uc&1)^1);
				}
				else
				{
					NDK_ScrPutPixel(uixpos+iWCount, uiypos+iHCount+iPCount, uc&1);
				}
				uc>>=1;
			}
			pucImageBuf++;
		}
	}
	return 0;
}



void me_show_bt_icon(void)
{
	int ret;
	int status;
//	ret = NDK_BTStatus(&status);
	//ME31_DEBUG("\r\nret %02x,status %02x",ret,status);
	if((ret == NDK_OK)&&(status==0)) {
		mpos_DispImage(100, 2, 8, 12, bt_icon);
		return;
	}
	mpos_DispImage(100, 2, 8, 12, clr_icon);
	return;
}


int me_SysGetPowerVol(uint *punChargeFlag,uint *punVol)
{
	uint nRet,unVol;

	*punVol = 0;	
	*punChargeFlag = 0;	

	nRet = NDK_SysGetPowerVol(&unVol);
	if(nRet!=0){
		return nRet;
	}
	if(unVol==0)
		*punChargeFlag = 1;
	
	if((unVol&0x80000000)!=0){
		unVol &= 0x7fffffff;			
		*punChargeFlag = 1;
	}

	*punVol = unVol;
	return 0;
}

#if 0
/*
***********************************************************************************************


void me_show_bat_icon(void)
{
	static int nCount = 0;
	static int bat_cur;
	static int nLowFlag=0;
	uint unVol,unMinVol,unMaxVol,unVolPer,unIsCharged;

	if(me_SysGetPowerVol(&unIsCharged,&unVol)!=0)
	    return;
	NDK_SysGetPowerVolRange(&unMaxVol, &unMinVol);

	
	if(unVol>unMinVol)
		unVolPer = ((unVol-unMinVol)*100)/(unMaxVol-unMinVol);
	else
		unVolPer = 0;
	//ME31_DEBUG("unIsCharged=%d  unVolPer=%d\n",unIsCharged,unVolPer);
	if((unIsCharged==1)) 								/*充电中*/
	//{
		//if(unVolPer>=90) { 							/*充满电*/
		//	NDK_LedStatus(LED_RFID_RED_OFF);		
		//	NDK_LedStatus(LED_RFID_GREEN_ON);	
	//		mpos_DispImage(110, 4, 16, 8, bat_icon[3]);
		//}else {
		//	mpos_DispImage(110, 4, 16, 8, bat_icon[bat_cur&0x03]);
	//		bat_cur ++;
	//		NDK_LedStatus(LED_RFID_RED_ON);
//		}
		
//	}
//	else 	/*拔除充电线*/
//	{
/*
		NDK_LedStatus(LED_RFID_RED_OFF);
		NDK_LedStatus(LED_RFID_GREEN_OFF);

		if(unVolPer<10)								/*电量检测*/
/*		{ 									
			nCount++;
			if(nCount>30)
			{
				me_ScrClrs();
				NDK_LedStatus(LED_RFID_RED_FLICK);
				mpos_DispImage(110, 4, 16, 8, bat_icon[0]);	
				mpos_DispStrXY(16, "电量低,关机中!", 14);
				NDK_SysMsDelay(3000);
				NDK_BTDisconnect();				
				NDK_SysShutDown();	
			}
		}else{
			nCount=0;
		}

/*		if(unVolPer > 75) { 						/* 3格*/
/*			mpos_DispImage(110, 4, 16, 8, bat_icon[3]);
		}
		else if(unVolPer > 50) {						/* 2格*/
	/*		mpos_DispImage(110, 4, 16, 8, bat_icon[2]);
		}
		else if(unVolPer > 25) {						/* 1格*/
/*			mpos_DispImage(110, 4, 16, 8, bat_icon[1]);
		}
		else if(unVolPer > 10) {						/* 0格*/
/*			NDK_LedStatus(LED_RFID_RED_FLICK);
			if(nLowFlag)
				mpos_DispImage(110, 4, 16, 8, bat_icon[1]);
			nLowFlag^=0x01;
		}
		
	}
	
	return;
}


***********************************************************************************************
*/
#endif

/*
***********************************************************************************************
函数原型：ME_DispStaBar
功能描述：显示状态栏
输入参数：
输出参数：无
返回值：  无
调用关系：
***********************************************************************************************
*/
void ME_DispStaBar(void)
{
	uint tmpx,tmpy;
	uint fontx,fonty;
	NDK_ScrGetxy(&tmpx,&tmpy);
	NDK_ScrGetFontSize(&fontx, &fonty);
	if(nlMpos_Command.Lcd_StartY>fontx)
	{
		NDK_ScrClrLine(0, nlMpos_Command.Lcd_StartY-1);
		ME_SysExtraAction(ME_SYS_ACTION_LCD_STABAR);
	}
	me_show_bt_icon();
	//me_show_bat_icon();
	NDK_ScrGotoxy(tmpx,tmpy);
	return;
}

#endif

