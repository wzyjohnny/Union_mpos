#if !defined(_API_FRAME_H)
#define _API_FRAME_H



typedef unsigned char*	puchar;
typedef char*			pchar;
typedef unsigned short* pushort;
typedef short* 			pshort;
typedef unsigned long* 	pulong;
typedef long* 			plong;
typedef unsigned int* 	puint;
typedef int* 			pint;

/*定义参数表单元结构*/
typedef struct {
	unsigned char m_ucType;				/*参数类型*/
	unsigned char m_ucData;				/*参数数据*/
}SysParam_t,*pSysParam_t;

#define ME31_EN_USBDEBUG			1

/* 通信方式 */
#define	 COMMODE_NONE 		0   	//无
#define  COMMODE_USB 		1		//usb
#define  COMMODE_BT 		2		//蓝牙
#define  COMMODE_AUDIO 		4		//音频

#define PARAM_NEED_TLV				0x01	/*将数据保存为TLV格式写入FLASH*/
#define PARAM_SPECIAL_PRINT			0x02	/*透明打印*/
#define PARAM_APPOWNER				0x03	/*应用程序厂商*/
#define SESSION_KEY_INDEX			255		//会话密钥索引，不可被其他工作密钥使用

extern unsigned int ME_GetCmdOffset();
extern unsigned int ME_GetVarOffset();
extern unsigned int ME_GetVarLen(void *pindata);
/*
***********************************************************************************************
* 宏定义
***********************************************************************************************
*/
#define ME_MAX_Host_Command			2048	/* 支持最大的接收长度 */

#define MPOS_CMD_OFFSET				ME_GetCmdOffset()
#define MPOS_VARIABLE_OFFSET    ME_GetVarOffset()	/*数据偏移量*/
#define MPOS_VARIABLE_READ(SRC) ME_GetVarLen(SRC)

#define MPOS_EXT_OFFSET				12

#define MPOS_VARIABLE_MINLEN		(4)		//(MPOS_CMD_SIZE+MPOS_SEP_SIZE+MPOS_SN_SIZE)		//可变数据的最小长度
/* ====================================返回码====================================================*/
#define CMD_OK					"00"	/* 处理成功 */
#define CMD_ERR_UNSUPPORT		"01"	/* 指令码不支持 */
#define CMD_ERR_PARAM			"02"	/* 参数错误 */
#define CMD_ERR_VARLEN			"03"	/* 可变数据域长度错误 */
#define CMD_ERR_FRAME			"04"	/* 帧格式错误 */
#define CMD_ERR_LRC				"05"	/* LRC交易失败 */
#define CMD_ERR_OTHER			"06"	/* 其他 */

#define CMD_ERR_TIMEOUT						"07"	/* 超时 */
#define CMD_ERR_OPENFILE					"08"	/* 打开文件失败 */
#define CMD_ERR_DEVICE_AUTHENTICATE			"09"	/* 设备认证失败 */
#define CMD_ERR_EXTERN_AUTHENTICATE			"0A"	/* 外部认证失败 */
#define CMD_ERR_PUBLIC_KEY					"0B"	/* 公钥灌装失败 */
#define CMD_ERR_GENERATE_KEYPAIR			"0C"	/* 生成密钥对失败 */


#define CMD_ERR_STEP                    "11"   /*步骤错误*/
#define CMD_ERR_TINFO                  "12"   /*获取终端信息错误*/
#define CMD_ERR_RAND                   "13"   /*生成随机数失败*/
#define CMD_ERR_DECRYPTRN       "14"   /*解密随机数失败*/
#define CMD_ERR_ENCRYPTRN       "15"  /*加密随机数失败*/
#define CMD_ERR_VERIFYRN           "16" /*验证随机数失败*/
#define CMD_ERR_INSTALLKEY        "17"    /*安装密钥失败*/   
#define CMD_QUIT                              "18" /*直联退出交易*/
#define CMD_ERR_ENCRYPT8583       "19"  /*加密8583包失败*/
#define CMD_ERR_DECRYPT8583      "20"  /*解密8583包失败*/
#define CMD_ERR_MAC				"21" /*MAC校验失败*/
#define CMD_ERR_GEOPOSITION      "22" /*获取地理位置信息失败*/
#define CMD_OK_GEOPOSITION      "23" /*获取地理位置信息成功*/
#define CMD_ERR_DECRYPTKEY      "24" /*解密密钥失败*/



/* ====================================大端小端相关接口====================================================*/
#define BIG_ENDIAN					1
#define LITTLE_ENDIAN				0

#define ENDIAN_MODE				1
#define BCD_MODE					0



#define _VAR_BIT8        				1
#define _VAR_BIT16       	 			2
#define _VAR_BIT24       	 			3
#define _VAR_BIT32        				4

#define _VAR_WRITE(addr,data,size) 		if(size == _VAR_BIT8)\
												*((unsigned char*)(addr)) = (unsigned char)(data) ;\
											else if(size == _VAR_BIT16)\
												*((unsigned short*)(addr)) = (unsigned short)(data) ;\
											else \
												*((unsigned long*)(addr)) = (unsigned long)(data)

#define _VAR_READ(addr,size) 			((size == _VAR_BIT8) ?  *((unsigned char*)(addr)) :\
											((size == _VAR_BIT16) ? *((unsigned short*)(addr)) :\
											*((unsigned long*)(addr))))

#define Endian_Swab16(x)   				((((unsigned short)(x) & 0xff00) >> 8)|(((unsigned short)(x) & 0x00ff) << 8))
#define Endian_Swab32(x)   				((( (unsigned long)(x) & 0xff000000) >> 24) | \
											(( (unsigned long)(x) & 0x00ff0000) >> 8) | \
											(( (unsigned long)(x) & 0x0000ff00) << 8) | \
											(( (unsigned long)(x) & 0x000000ff) << 24))


#define _BCD2INT(n)						(((((n) >> 4) & 0x0F) * 10) + ((n) & 0x0F))
#define _INT2BCD(n)						((((n) / 10) << 4) | ((n) % 10))


/* ====================================调试接口====================================================*/
typedef struct
{
	unsigned char* pu1DataBuf;		/*	数据缓冲区头指针 */
	unsigned int  u4DataLen;			/*	数据缓冲区大小	*/
}ME_DB_CELL, *PME_DB_CELL;

typedef struct
{
	unsigned char u1COMID;			/*	端口ID		*/
	unsigned int  u4BPS;				/*	波特率		*/
	unsigned int  u4Flag;				/*	参数		*/
	unsigned int  u4TimeOut;			/* 	超时 单位 ms*/
    unsigned char *pu1DataBuf;		/*	数据缓冲区头指针 */
    unsigned int  u4DataLen;		/*	数据缓冲区大小	*/
}ME_COM_T, *PME_COM_T;

typedef struct  {
	int	(*mpos_com_init)(unsigned char ucCommMode);
	int	(*mpos_com_read)(PME_COM_T);
	int	(*mpos_com_write)(PME_COM_T);
	int	(*mpos_com_readlen)(PME_COM_T);

	int (*mpos_edc_check)(unsigned char*, int);

	unsigned short (*mpos_endian_swab16)(unsigned short);
	unsigned int (*mpos_endian_swab32)(unsigned int);
	unsigned int (*mpos_getvar)(void*, int);
	unsigned int (*mpos_setvar)(void*, unsigned int, int);

	unsigned int (*mpos_int2bcd)(void*, unsigned int, int);
	unsigned int (*mpos_bcd2int)(void*, int);

	unsigned int (*mpos_readlen)(void*, int);
	unsigned int (*mpos_writelen)(void*, unsigned int, int);


	int (*mpos_rdata_from_host)(unsigned char*,int, int);
	int (*mpos_sdata_to_host)(unsigned char*, int, char*);
	int (*mpos_command_parse)(unsigned char*, int);
	int Lcd_StartY;
	int EndianMode;
	int LengthMode;
}nlmpos_command_t;

extern const nlmpos_command_t nlMpos_Command;
extern int ME_API_COMInit(unsigned char ucCommMode);
extern int ME_API_COMClose();

#if ME31_EN_USBDEBUG
     extern int ME_Printf( const char *format, ... ) ;
	#define ME31_DEBUG	ME_Printf
	#define ME31_DEBUG_BUF(BUF,LEN) {\
		int temp;\
		for(temp=0; temp< LEN; temp++) {\
			ME31_DEBUG(" %02x", BUF[temp]);\
		}\
	}
#else
	#define ME31_DEBUG(...)		{}
	#define ME31_DEBUG_BUF(BUF,LEN)	{}
#endif

#endif // !defined(_API_FRAME_H)

