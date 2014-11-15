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
#ifndef _MPOS_COMMAND_H_
#define _MPOS_COMMAND_H_
#include "NDK.h"
#include "mpos_api_frame.h"

#ifndef FAIL
#define FAIL 		(-1)
#endif

#ifndef SUCC
#define SUCC 		(0)
#endif

/* ====================================指令集====================================================*/
#define ME_PRN_NOPAPER    (-97)             /*打印缺纸*/
#define ME_ERR_FALLBACK   (-98)             /*快速返回上一级菜单,需要上一级处理*/
#define ME_ERR_BROKEN      (-99)            /*收到指令，当前流程被中断*/



#define ME_COUNTDOWN_MODE_COM			(1<<0)	/*允许中断去处理串口*/
#define ME_COUNTDOWN_MODE_NOCOUNTDOWN	(1<<1)	/*不显示倒计时*/
#define ME_COUNTDOWN_MODE_COM_NORUN  	(1<<2)	/*允许中断去处理串口,但不运行*/



#define CONFIG_MAGIC_ME31APP  		"NLMAG301"			/*魔数，配置文件格式修改时请修改后3位.后续考虑不同应用升级时更新配置信息(如蓝牙名),将第6位作为厂商标识，7 8位作为版本标识*/


#define 	PIN_KSN_FILE			"/appfs/pinksn.in"          /*PIN    KSN,银商总公司*/
#define 	TRACK_KSN_FILE			"/appfs/trackksn.in"        /*TRACK  KSN,银商总公司*/
#define 	PBOC_KSN_FILE			"/appfs/pbocksn.in"        /*PBOC  KSN,银商总公司*/
#define 	SN_FILE					"/appfs/yssn.in"		/*设备序列号CSN,银商总公司*/

#define 	REVERSAL_FILE			"/appfs/reversal.in"	/*55域冲正信息*/
#define 	REVERSAL_TC_FILE		"/appfs/revertc.in"		/*TC 冲正信息*/
#define 	REVERSAL_SPR_FILE		"/appfs/reverspr.in"	/*脚本 冲正信息*/


#define ME_TLV_FILE					"/appfs/Terminal.in"        /*存放终端参数*/
#define ME_APP_FILE					"/appfs/app.in"                /*应用升级文件*/
#define ME_CONF_FILE				"/appfs/mConfig.in"             /*配置信息*/
/* ====================================结构体定义====================================================*/

/*设备信息，存储设备应用编号、应用程序版本等信息*/
typedef struct {
	uchar							DevicePersonalStatus ;			/**< 设备个人化状态	 	0xFF：出厂默认状态 	0x00：个人化完成 */
	uchar 							DeviceRersved[16];				/**< 应用版本。留着感觉也没什么用，不够用的时候可以把它去掉??????*/
	uchar 							DeviceAppSN[10];				/**< 设备应用编号（UDID）	*/
	uchar							DeviceWorkStatus;				/**< 设备状态信息		0x00：无源待工状态	0x01：无源工作状态 	0x10: 有源设备待工状态 	0x11  有源设备工作状态 */
	uchar							DeviceValidity[4];				/**< 设备有效期	YYMM */
	uchar 							DeviceAdditionalInfo[20];	    /**< 设备附加信息 */
	uchar 							PublicKeySerialNumber[5];		/**< 公钥序号 5字节 */
	uchar 							PublicKey[1024>>3];				/**< 敏感信息（磁道）加密公钥（1024）	密钥长度为1024比特 */
	uchar							PublicKeyValidity[6];			/**< 公钥有效期	YYMMDD */
	uchar							AuthenticationKey[16];			/**< 认证密钥	对称16字节 */
	uchar							MagneticTrackPubKeySerNum[5];	/**< 磁道公钥序号 5字节 */
	uchar 							PINPublicKeySerialNumber[5];	/**< PIN公钥序号（预留） */
}DEVICE_DATA;		///1+16+10+1+4+20+5+128+6+16+5+5=217

/*不需要存储的配置信息*/
typedef struct
{
	int ucKey;                                                     /*主界面当前按下的键,保存供B1 01指令读取*/
	int u1SecTamperStatus;                              /*安全触发状态*/
	unsigned int unLcdTimeout;                          /*主界面液晶显示超时,超过此时间自动返回欢迎使用界面*/
	unsigned char m_u1fonttype;				    /*字体类型*/
	EM_PRN_MODE   m_PrnMode;				    /* 字体设置 */
	unsigned char u1MposSn;					    /*当前指令包序列号*/
	unsigned char u1MposCmd[2]; 	                        /*当前指令包密令码*/
	unsigned char u1RandCode[8];				 /*当前随机码*/
	unsigned char uszWaterNo[12];				/*流水号,PIN输入没有流水号,保存磁卡流水号*/
	char u1BTName[20];	                              /*蓝牙名,保存避免反复读取*/
	char u1BTPassword[16];	                       /*蓝牙密码,保存避免反复读取*/
	char cIsDebug;                                          /*debug开关*/
	char cCardType;								//当前操作的卡类型
	unsigned char ucDefaultMode;			  /*默认模式,usb 或蓝牙*/
	unsigned char ucMainItemCount;
	unsigned char ucPasswordItemCount;
	unsigned char ucFuncItemCount;
	unsigned char ucSysItemCount;
	unsigned char ucCommItemCount;
}ME_TPEDCTL;	/*PED控制结构*/


/*旧格式的配置文件，大小固定为256,已不用，升级新版本时强制转化为新格式的配置文件*/
typedef struct
{
    unsigned char u1BTConfigFlag;
	unsigned char u1CommuMode;
	DEVICE_DATA   ME_TDeviceData;
	unsigned char u1SwitchFlag;
	unsigned char  uszRerseved[256-5-sizeof(unsigned short)-sizeof(unsigned short)-sizeof(DEVICE_DATA)];
	unsigned short u1StandByTime;
	unsigned short u1SleepTime;
	unsigned char  uszCrcValue[2];
}ME31_OLDCONFIG_T;


/*新格式的配置文件*/
typedef struct {
    unsigned char	magic[8];           //"NLMAG000",可以通过检测此域实现程序参数的更新
    unsigned char	u1AppOwner;         //预留
    unsigned char	u1CommMode;         //通信方式: 0 无（串口） 、1 USB、 2 蓝牙、4 音频
    unsigned short	u1StandByTime;	    //进入待机的时间
    unsigned short	u1SleepTime;		//进入休眠的时间
    unsigned short  u1ShutDownTime;	//自动关机时间
    unsigned short  u1LcdContrast;          //lcd对比度
    char  szRersved[12];                          //预留
    char  szAdminPassword[16];          //管理员密码,预留
    char  szKlaPassword[16];            //KLA程序密码，预留
    char  szAppExitPassword[16];        //应用程序退出密码 ,预留
    DEVICE_DATA   ME_TDeviceData;       //设备信息
    unsigned char u1RersevedNo;         //uszRersved数组的大小 ,如果参数增加无法存放可将内容放到uszRersved
    unsigned char  uszRersved[0];       //后续使用
}ME31_CONFIG_T;


typedef struct {
    unsigned int  unTotalTicks;
    unsigned int  unStabarTicks;
    unsigned int  unFrushTicks;
    unsigned int  unSecTicks;	
    unsigned int  unKeyTicks;	
    unsigned int  unLastTicks;
}ME_TICKS_T;


#define MAX_MENU_NAME	24

typedef struct {
        int nKey;               //按键值，当此按键按下时执行对应的函数
	 char szKeyStr[12];             //字符串，当按下的字符串等于此字符串时执行对应函数
	 int (*func)(void);             //对应的函数
}ME_KbItem_t;

typedef struct {
        int nKey;                           //按键值，当此按键按下时显示对应的内容
	 char name[MAX_MENU_NAME];              //显示内容
        uchar xy[2];                                //显示坐标.NOTE: 当Y坐标大于60时会将显示内容隐藏
	 int (*func)(void);                         //对应的函数
}ME_GuiItem_t;



typedef enum {
	SYS_HWINFO_GET_PIN_KSN=100,			/*PIN KSN (银商总公司)*/	//从100开始避免冲突
	SYS_HWINFO_GET_TRACK_KSN,			/*TRACK KSN (银商总公司)*/
	SYS_HWINFO_GET_PBOC_KSN,			/*PBOC KSN (银商总公司)*/
	SYS_HWINFO_GET_KSN,					/*ALL KSN (银商总公司)*/
	SYS_HWINFO_GET_CSN,					/*CSN (银商总公司)*/
	SYS_HWINFO_GET_PRODUCE_SN,			/*生产SN*/
	SYS_HWINFO_GET_VID,					/*厂家ID*/
	SYS_HWINFO_GET_KCV,					/*检测密钥是否存在*/
	SYS_HWINFO_GET_TLVSWITCH,			/*终端参数读写开关,参数读写开关关闭时无法写终端号和商户号(青岛银商需求)*/
	SYS_HWINFO_GET_TLVSWITCH1,			/*终端参数读写开关,参数读写开关关闭时无法写终端号和商户号(青岛银商双终端需求)*/	
} EM_SYS_HWINFO_ME;


typedef enum {
	SYS_APPVID_LAKALA  =  0,      	    /*拉卡拉*/
	SYS_APPVID_CHINAUMS ,      		    /*银商总公司*/
	SYS_APPVID_JSUMS,      		        /*江苏银商*/
	SYS_APPVID_99BILL,      		    /*块钱*/	
	SYS_APPVID_FUIOU,      		        /*富友*/	
	SYS_APPVID_YEEPAY,      		    /*易宝*/		
	SYS_APPVID_QINGDAO,      		    /*青岛*/
	SYS_APPVID_EEEPAY,      		    /*移联*/
	SYS_APPVID_EPTOK,      		    	/*银盛*/
} EM_SYS_APPVID;


typedef enum {
		ME_SYS_ACTION_SWIPED_START=0,      		/*刷卡开始*/
		ME_SYS_ACTION_SWIPED_SUCC,				/*刷卡成功*/
		ME_SYS_ACTION_SWIPED_FAIL,				/*刷卡失败*/
		ME_SYS_ACTION_SWIPED_CHECK,				/*刷卡二磁道检测*/        
		ME_SYS_ACTION_SWIPED_TIMEOUT,			/*刷卡超时*/	
		ME_SYS_ACTION_PIN_CANCEL,				 /*输密码超时*/	
		ME_SYS_ACTION_PIN_SUCC,				     /*输密成功*/
		ME_SYS_ACTION_PIN_FAIL,				     /*输密失败*/
		ME_SYS_ACTION_PIN_TIMEOUT,				 /*输密码超时*/	
		ME_SYS_ACTION_PBOC_SUCC,				/*pboc成功*/
	 ME_SYS_ACTION_PBOC_CANCEL,
	 ME_SYS_ACTION_PBOC_FAIL,
		ME_SYS_ACTION_LCD_STABAR,				/*状态栏*/
		ME_SYS_ACTION_PRN_START,				/*打印开始*/
		ME_SYS_ACTION_PRN_FINISH,				/*打印结束*/
		ME_SYS_ACTION_CHECK_MAGIC,				/*检测mag 是否支持IC*/ 
	 ME_SYS_ACTION_QPBOC_SUCC,
	 ME_SYS_ACTION_QPBOC_FAIL,
	 ME_SYS_ACTION_QPBOC_CANCEL,
		ME_SYS_ACTION_PAN_CHECK,			   /*检测mag 是否支持IC*/
		
		ME_SYS_ACTION_PAN_TIMEOUT,			   /*检测mag 是否支持IC*/
} EM_SYS_ACTION_ME;


/* ====================================帧发送接收相关接口====================================================*/
extern void ME_ParaInit();

extern void ME31_PEDMain(void);
extern int ME_Sysmanage(void);
extern int ME_DeviceInfo();
//extern int ME_FsDel();
extern int ME_SetDateTime();
extern int ME_SetCommMode();
//extern int ME_SetSleepTime();
extern int ME_UsbParaSet();
extern int ME_AudioParaSet();
extern int ME_FuncMenu();
extern int ME_DebugSwitch();
extern int ME_PasswordMenu();
extern int ME_MKSetup();
extern int ME_PbocInfo();
extern int ME_CsnInput(void);
extern int ME_ScrSetContrast();
extern int ME_SetFrushTime(int nMs);
extern int ME_SysGetPosInfo(EM_SYS_HWINFO_ME emFlag,uint * punLen,char * psBuf);
//extern int ME_SysSetPosInfo(EM_SYS_HWINFO_ME emFlag,char * psBuf);
extern int ME_SysExtraAction(EM_SYS_ACTION_ME emFlag);
extern int ME_SysCountdown(unsigned char ucMode,int nTimeout,int nStartY,int(* application)(void));
extern int ME_FsRead(const char * pszName,unsigned char * psBuffer,uint unOffset,uint unLength);
extern int ME_FsWrite(const char * pszName,const unsigned char * psBuffer,uint unOffset,uint unLength);
extern int ME_BtCheck(void);
extern int ME_KeyManagement(void);



extern ME_TPEDCTL g_METPEDCtl;	/*PED控制结构*/


#endif
