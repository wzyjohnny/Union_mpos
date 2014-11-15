/*
 ***********************************************************************************************
 版权说明：ME31指令集
 版本号：
 生成日期：
 作者：
 模块目的/功能：安全认证类
 主要函数及其功能：
 修改日志：
 ***********************************************************************************************
 */
#include <string.h>
#include "NDK.h"
#include "mpos_command.h"
#include "unistd.h"
#include "lcup.h"
#include "wdebug.h"

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

#define MAXLEN_8583PACK				1024				//8583 最大长度

//---------------------------调试宏定义-----------------------------------//
/*#define PDEBUG_DATA(info, data, len) 	{int i;\     */
/*NDK_ScrPrintf("[%s %d]%s: len=%d data=", __FILE__, __LINE__, (info), (len));\*/
/*for(i=0; i<(len); i++){\*/
/*NDK_ScrPrintf("%02x ", *(data+i));\*/
/*}\*/
/*NDK_ScrPrintf("\n\r");\*/
/*}*/

extern ME31_CONFIG_T g_me31conf;
extern const char *pszAppVer;

static int nGlobalStep = 1;
char RN1originalplaintext[16];
char RN3origalplaintext[16];
char geoposition[32] = {0};
int GeoPositionLen=0;
int AuthOK = 0;
extern uchar ME_Host_Command_Buf[ME_MAX_Host_Command];
extern ME_TPEDCTL g_METPEDCtl;

int ME_SysGetPosInfo(EM_SYS_HWINFO_ME emFlag, uint * punLen, char * psBuf);
extern void me_ScrClrs();

int UnionPay_Send(EM_PORT_NUM emPort, uint unLen, const char *pszInbuf);
int UnionPay_Recv(EM_PORT_NUM emPort, char *pszOutBuf, int nTimeoutMs, int *pnReadLen);

UnionParam_t stUnionParam = {
		.StationLen   =  16,
		.StationBuf	  =  "123456789",
		.UnionPay_Send = UnionPay_Send,
		.UnionPay_Recv = UnionPay_Recv,
};


/*
 ***********************************************************************************************
 函数原型：双向认证
 功能描述：设备认证阶段1
 输入参数：
 输出参数：无
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */

int CommandParse_MutualAuth1(puchar pbuf, int buf_len,
		const pSysParam_t pTParam) {
	int iRet;
	char nStep; //当前认证步骤
	int offset = 0;
	char ret_code[2 + 1]; //返回响应码
	char sysinfo[32]; //大小设置为多大
	uchar szSendbuff[64];  //大小确定一下，SN的大小也确定一下
	uint SysPosInfolen; //T_info信息长度

	NDK_ScrClrs();
	NDK_ScrPrintf("\n   Auth step 1..\n");

	/* ==========================解析数据==========================*/
	nStep = pbuf[MPOS_VARIABLE_OFFSET + offset]; /*当前认证步骤*/
	if (nStep != 0x01) {
		ME31_DEBUG("nStep=%d \n", nStep);
		memcpy(ret_code, CMD_ERR_STEP, 2);
		goto on_err_ack;
	}

	/* ==========================获取终端设备信息T_info==========================*/
	memset(sysinfo, 0x00, sizeof(sysinfo));
	iRet = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &SysPosInfolen, sysinfo); //获取终端信息
	if( (NDK_OK != iRet) || (SysPosInfolen == 0)){
		NDK_ScrClrs();
		NDK_ScrPrintf("获取终端信息失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_TINFO, 2);
		goto on_err_ack;
	}

	/*========================获取随机数RN1====================================*/
	iRet = NDK_SecGetRandom(sizeof(RN1originalplaintext), RN1originalplaintext);
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("获取随机数RN1失败");
		NDK_KbGetCode(2, NULL);
		PDEBUG("iRet = %d", iRet);
		memcpy(ret_code, CMD_ERR_RAND, 2);
		goto on_err_ack;
	}

	/* ==========================输出==========================*/
	offset = 0;
	memcpy(szSendbuff + offset, &nStep, 1); /*表明当前进行的认证步骤*/
	offset += 1;

	nlMpos_Command.mpos_writelen(szSendbuff + offset, SysPosInfolen,
	_VAR_BIT16); //把sysposinfolen变为VAR-16的形式，同时发送到缓冲区
	offset += 2;

    if(SysPosInfolen > 32)
        goto on_err_ack;
	memcpy(szSendbuff + offset, sysinfo, SysPosInfolen); /*终端信息*/
	offset += SysPosInfolen;

	memcpy(szSendbuff + offset, RN1originalplaintext, 16); /*随机数RN1*/
	offset += 16;

	nlMpos_Command.mpos_sdata_to_host((uchar*) szSendbuff, offset, CMD_OK);
	nGlobalStep = 2;
	return 0;

on_err_ack: 
	nlMpos_Command.mpos_sdata_to_host(NULL, 0, ret_code);
	nGlobalStep = 1;
	return 0;
}

/*
 ***********************************************************************************************
 函数原型：双向认证
 功能描述：设备认证阶段2
 输入参数：
 输出参数：无
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */
int CommandParse_MutualAuth2(puchar pbuf, int buf_len,
		const pSysParam_t pTParam) {
	int iRet;
	int nStep;
	int offset = 0;
	char ret_code[2 + 1];
	uchar RN1ciphertext[16]; //  接收的 RN1密文
	uchar RN1plaintext[16]; //解密出来的RN1明文，与RN1originalplaintext对比
	uchar RN2plaintext[16]; //接收的RN2明文，该RN2明文是由业务前置产生
	uchar RN2ciphertext[16];  //	加密的RN2密文，用于发送

	char sysinfo[32]; //保存T_info终端信息
	uint SysPosInfolen; //T_info   终端信息长度
	uchar szSendbuff[64];

	int keyid = 255;
	int data_len = 16;

	NDK_ScrClrs();
	NDK_ScrPrintf("\n   Auth step 2..\n");

	/*=========================判断认证步骤===============================*/
	if (nGlobalStep != 2) {
		NDK_ScrClrs();
		NDK_ScrPrintf("认证步骤错误");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_STEP, 2);
		goto on_err_ack;
	}

	/* ==========================解析数据==========================*/

	nStep = pbuf[MPOS_VARIABLE_OFFSET + offset]; /*认证步骤*/
	if (nStep != 0x02) {
		ME31_DEBUG("nStep=%d \n", nStep);
		memcpy(ret_code, CMD_ERR_STEP, 2);
		goto on_err_ack;
	}
	offset++;

	memcpy(RN1ciphertext, pbuf + MPOS_VARIABLE_OFFSET + offset, 16); //获取RN1密文
	offset += 16;

	memcpy(RN2plaintext, pbuf + MPOS_VARIABLE_OFFSET + offset, 16); //获取  业务前置产生的  RN2明文
	offset += 16;

	/* ==========================获取终端信息==========================*/

	memset(sysinfo, 0x00, sizeof(sysinfo));
	iRet = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &SysPosInfolen, sysinfo); //获取终端信息
	if( (NDK_OK != iRet) || (SysPosInfolen == 0)){
		NDK_ScrClrs();
		NDK_ScrPrintf("获取终端信息失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_TINFO, 2);
		goto on_err_ack;
	}

	/* ==========================解密RN1密文，验证RN1==========================*/

	iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TDK, keyid, RN1ciphertext, data_len,
			RN1plaintext, SEC_DES_DECRYPT); //解密RN1
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("解密随机数RN1失败");
		PDEBUG("iRet = %d", iRet);
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_DECRYPTRN, 2);
		goto on_err_ack;
	}
	if (memcmp(RN1originalplaintext, RN1plaintext, 16)) {
		NDK_ScrClrs();
		NDK_ScrPrintf("验证随机数RN1失败");    //验证随机数错误码还没成功 
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_VERIFYRN, 2);
		goto on_err_ack;
	}

	/* ==========================生成随机数RN2密文==========================*/

	iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TDK, keyid, RN2plaintext, data_len,
			RN2ciphertext, SEC_DES_ENCRYPT); //加密RN2
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("加密随机数RN2失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_ENCRYPTRN, 2);
		goto on_err_ack;
	}

	/* ==========================生成随机数RN3明文==========================*/
	iRet = NDK_SecGetRandom(sizeof(RN3origalplaintext), RN3origalplaintext);
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("生成随机数RN3失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_RAND, 2);
		goto on_err_ack;
	}

	/* ==========================输出==========================*/

	offset = 0;
	memcpy(szSendbuff + offset, &nStep, 1); /*表明当前进行的认证步骤*/
	offset += 1;

	nlMpos_Command.mpos_writelen(szSendbuff + offset, SysPosInfolen,
	_VAR_BIT16);

	offset += 2;
	memcpy(szSendbuff + offset, sysinfo, SysPosInfolen); /*终端信息*/
	offset += SysPosInfolen;

	memcpy(szSendbuff + offset, RN2ciphertext, 16); /*随机数RN2密文*/
	offset += 16;

	memcpy(szSendbuff + offset, RN3origalplaintext, 16); /*随机数RN3明文*/
	offset += 16;

	nlMpos_Command.mpos_sdata_to_host((uchar*) szSendbuff, offset, CMD_OK);
	nGlobalStep = 3;
	return 0;

on_err_ack: 
	nlMpos_Command.mpos_sdata_to_host(NULL, 0, ret_code);
	nGlobalStep = 1;
	return 0;
}

/*
 ***********************************************************************************************
 函数原型：双向认证
 功能描述：设备认证阶段3
 输入参数：
 输出参数：无
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */
int CommandParse_MutualAuth3(puchar pbuf, int buf_len,
		const pSysParam_t pTParam) {
	int iRet;
	int nStep;
	int offset = 0;
	char ret_code[2] = { '0', '6'};
	char RN3ciphertext[16]; //接收的RN3密文

	uchar R3plaintext[16 + 1];  //解密出来的RN3明文
	uchar TSK_DATA[24];  //密文形式的TSK_DATA，TSK_DATA用于终端数据加密
	uchar TSK_MAC[24];  //密文形式的TSK_MAC，TSK_MAC用于终端MAC加密
	uchar szSendbuff[64];
	int iDatalen1;
	int iDatalen2;

	int keyid = 255;
	int data_len = 16;

	ST_SEC_KEY_INFO keyst;
	ST_SEC_KCV_INFO kcv;


	NDK_ScrClrs();
	NDK_ScrPrintf("\n   Auth step 3..\n");
	
	/*=========================判断认证步骤===============================*/

	if (nGlobalStep != 3) {
		NDK_ScrClrs();
		NDK_ScrPrintf("认证步骤错误");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_STEP, 2);
		goto on_err_ack;
	}

	/* ==========================解析数据==========================================*/
	nStep = pbuf[MPOS_VARIABLE_OFFSET + offset]; /*认证步骤*/
	if (nStep != 0x03) {
		PDEBUG("nStep=%d \n", nStep);
		memcpy(ret_code, CMD_ERR_PARAM, 2);
		goto on_err_ack;
	}

	offset++;

	memcpy(RN3ciphertext, pbuf + MPOS_VARIABLE_OFFSET + offset, 16); //获取RN3密文

	offset += 16;

	iDatalen1 = nlMpos_Command.mpos_readlen(
			pbuf + MPOS_VARIABLE_OFFSET + offset, _VAR_BIT16); /* TSK_DATA密钥输入数据长度，针对LL*/
	if(iDatalen1 > sizeof(TSK_DATA))
	    goto on_err_ack;
	offset += 2;
	memcpy(TSK_DATA, pbuf + MPOS_VARIABLE_OFFSET + offset, iDatalen1);
	offset += iDatalen1;

	iDatalen2 = nlMpos_Command.mpos_readlen(
			pbuf + MPOS_VARIABLE_OFFSET + offset, _VAR_BIT16); /* TSK_MAC密钥输入数据长度，针对LL*/
	if(iDatalen2 > sizeof(TSK_MAC))
	    goto on_err_ack;
	offset += 2;
	memcpy(TSK_MAC, pbuf + MPOS_VARIABLE_OFFSET + offset, iDatalen2);
	offset += iDatalen2;

	/* ==========================解密RN3密文 ，验证==========================*/

	iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TDK, keyid, RN3ciphertext, data_len,
			R3plaintext, SEC_DES_DECRYPT); //解密RN3
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("解密随机数RN3失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_DECRYPTRN, 2);
		goto on_err_ack;
	}
	//先把验证注释掉，跑一下程序
	if( memcmp(R3plaintext, RN3origalplaintext, 16)){
			NDK_ScrClrs();
			NDK_ScrPrintf("验证随机数RN3失败");    //验证随机数错误码还没成功 
			NDK_KbGetCode(2, NULL);
			memcpy(ret_code, CMD_ERR_VERIFYRN, 2);
            goto on_err_ack;
	}

#if 0
	/* ==========================用TEK 解密TSK_DATA和TSK_MAC==========================*/
	iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TMK, keyid, TSK_DATA, data_len,
			TSK_DATA_plaintext, SEC_DES_DECRYPT); //解密TSK_DATA
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("解密密钥TSK_DATA失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_DECRYPTKEY, 2);
		goto on_err_ack;
	}

	iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TMK, keyid, TSK_MAC, data_len,
			TSK_MAC_plaintext, SEC_DES_DECRYPT); //解密TSK_DATA
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("解密密钥TSK_MAC失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_DECRYPTKEY, 2);
		goto on_err_ack;
	}
#endif
	/* ==========================安装密钥TSK_DATA和TSK_MAC==========================*/

	memset(&keyst, 0, sizeof(ST_SEC_KEY_INFO));
	memset(&kcv, 0, sizeof(ST_SEC_KCV_INFO));
	keyst.nDstKeyLen = iDatalen1; //目的密钥长度
	memcpy(keyst.sDstKeyValue, TSK_DATA, iDatalen1); //写入的密钥内容
	keyst.ucDstKeyType = SEC_KEY_TYPE_TDK; //目的密钥类型
	keyst.ucScrKeyIdx = 255; 
	keyst.ucDstKeyIdx = 0; //ID0     目的密钥索引
	keyst.ucScrKeyType = SEC_KEY_TYPE_TMK; //源密钥类型

	kcv.nCheckMode = 0;  //校验模式
	kcv.nLen = 0; //校验数据区长度

	iRet = NDK_SecLoadKey(&keyst, &kcv);
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("安装TSK_DATA密钥失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_INSTALLKEY, 2);
		goto on_err_ack;
	}


	memset(&keyst, 0, sizeof(ST_SEC_KEY_INFO));
	memset(&kcv, 0, sizeof(ST_SEC_KCV_INFO));
	keyst.nDstKeyLen = iDatalen2; //目的密钥长度
	memcpy(keyst.sDstKeyValue, TSK_MAC, iDatalen2); //写入的密钥内容
	keyst.ucDstKeyType = SEC_KEY_TYPE_TAK; //目的密钥类型
	keyst.ucScrKeyIdx = 255; 
	keyst.ucDstKeyIdx = 0; //ID0     目的密钥索引
	keyst.ucScrKeyType = SEC_KEY_TYPE_TMK; //源密钥类型

	kcv.nCheckMode = 0;  //校验模式
	kcv.nLen = 0; //校验数据区长度

	iRet = NDK_SecLoadKey(&keyst, &kcv);
	if (NDK_OK != iRet) {
		NDK_ScrClrs();
		NDK_ScrPrintf("安装 TSK_MAC  密钥失败");
		NDK_KbGetCode(2, NULL);
		memcpy(ret_code, CMD_ERR_INSTALLKEY, 2);
		goto on_err_ack;
	}

	/* ==========================输出==========================*/
	offset = 0;
	memcpy(szSendbuff + offset, &nStep, 1); /*表明当前进行的认证步骤*/
	offset += 1;

	nlMpos_Command.mpos_sdata_to_host((uchar*) szSendbuff, offset, CMD_OK);
	nGlobalStep = 1;
	AuthOK = 1;
	return 0;

on_err_ack: 
	nlMpos_Command.mpos_sdata_to_host(NULL, 0, ret_code);
	nGlobalStep = 1;
	return -1;
}

/*
 ***********************************************************************************************
 函数原型：地理位置信息
 功能描述：获取地理位置信息
 输入参数：
 输出参数：无
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */
int CommandParse_SetGeoPosition(puchar pbuf, int buf_len,
		const pSysParam_t pTParam) {

	int offset = 0;
	char szSendbuff[32];
	int iDatalen;

	NDK_ScrClrs();
	NDK_ScrPrintf("\n  Set GeoPosition ...\n");
	
	/* ==========================解析数据==========================*/
	iDatalen = nlMpos_Command.mpos_readlen(pbuf + MPOS_VARIABLE_OFFSET + offset, _VAR_BIT16); //地理位置信息输入长度，针对LL
	offset += 2;

	if(iDatalen == 0)
	    goto on_err_ack;
	    
	if(iDatalen > sizeof(geoposition))
	    goto on_err_ack;
	    
	memcpy(geoposition, pbuf + MPOS_VARIABLE_OFFSET + offset, iDatalen); //获取地理位置信息
	GeoPositionLen=iDatalen;
	offset += iDatalen;

	/* ==========================输出==========================*/
	offset = 0;
	nlMpos_Command.mpos_sdata_to_host((uchar*) szSendbuff, offset, CMD_OK); //成功接收了地理位置信息
	return 0;

on_err_ack: 
    GeoPositionLen = 0;
	nlMpos_Command.mpos_sdata_to_host(NULL, 0, CMD_ERR_GEOPOSITION);
	return -1;
}

/*
 ***********************************************************************************************
 函数原型：直联程序通信类
 功能描述：直联程序发送信息
 输入参数：8583包
 输出参数：密文8583包+终端信息+mac校验
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */

int UnionPay_Send(EM_PORT_NUM emPort, uint unLen, const char *pszInbuf) {

	int iRet = 0;
	int offset = 0;
	int outDatalen;
	uchar plaintext8583[MAXLEN_8583PACK + 256] = { 0 }; //明文形式的8583包
	uchar ciphertext8583[MAXLEN_8583PACK + 256]; //密文形式的8583包
	int SysPosInfolen; //T_info   终端信息长度
	uchar tmpdata[MAXLEN_8583PACK + 256] = { 0 }; //  存放8583密文包、终端信息、mac校验信息
	char sysinfo[64]; //存放终端信息
	uchar mac[8];
	int padlen = (0x8 - (unLen) & 0x7) % 0x8;
	
	memset(plaintext8583, 0, sizeof(plaintext8583));
	memset(ciphertext8583, 0, sizeof(ciphertext8583));	
	memset(tmpdata, 0, sizeof(tmpdata));
	memset(sysinfo, 0, sizeof(sysinfo));
	
	memcpy(plaintext8583, pszInbuf, unLen);

	PDEBUG("========Sending==========");
	PDEBUG_DATA("8583 plaintext:", pszInbuf, unLen);


	/*=====================  获取终端信息， 在tmpdata中加入终端信息 ===================*/
	iRet = NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &SysPosInfolen, sysinfo); //获取终端信息
	if( (NDK_OK != iRet) || (SysPosInfolen == 0)){
		goto on_err_ack;
	}

	nlMpos_Command.mpos_writelen(tmpdata + offset, SysPosInfolen, _VAR_BIT16); //终端信息长度
	offset += 2;

	memcpy(tmpdata + offset, sysinfo, SysPosInfolen); // 将终端信息放入tmpdata中
	offset += SysPosInfolen;

    /*=====================  添加地理位置信息===================*/
	nlMpos_Command.mpos_writelen(tmpdata+offset, GeoPositionLen,_VAR_BIT16);
	offset+=2;
	memcpy(tmpdata+offset, geoposition, GeoPositionLen); 
	offset+=GeoPositionLen;

	/*===========================使用TSK_DATA加密8583包，并存在tmpdata中 ================*/
	iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 0, plaintext8583,
			unLen + padlen, ciphertext8583, SEC_DES_ENCRYPT); //加密8583包
	if (NDK_OK != iRet) {
		goto on_err_ack;
	}
	PDEBUG_DATA("8583 ciphertext:", ciphertext8583, unLen+padlen);

	nlMpos_Command.mpos_writelen(tmpdata + offset, unLen, _VAR_BIT16); //8583长度
	offset += 2;
	memcpy(tmpdata + offset, ciphertext8583, unLen + padlen); // 将密文形式的8583包放入tepdata中
	offset += (unLen + padlen);

	/*===========================加入MAC校验==================================*/
	padlen = (0x8 - offset & 0x07) % 0x8;
	iRet = NDK_SecGetMac(0, tmpdata, offset + padlen, mac, SEC_MAC_X919); //大小再确认一下
	if (NDK_OK != iRet) {
		goto on_err_ack;
	}
	PDEBUG_DATA("mac data:", tmpdata, offset+padlen);
	PDEBUG_DATA("mac value:", mac, 8);
	
    memcpy(tmpdata + offset, mac, 8); //加入MAC校验信息
	offset += 8;

	/*===========================输出==================================*/
	nlMpos_Command.mpos_sdata_to_host((uchar*) tmpdata, offset, CMD_OK);
	return 0;

on_err_ack: 
	return -1;
}

/*
 ***********************************************************************************************
 函数原型：直联程序通信类
 功能描述：直联程序接收信息
 输入参数：加密全报文
 输出参数：明文形式的8583报文
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */

int UnionPay_Recv(EM_PORT_NUM emPort, char *pszOutBuf, int nTimeoutMs,
		int *pnReadLen) {

	int nRet;
	int offset;
	int iRet;
	int iDatalen; //后台返回的加密的响应码长度
	uchar plaintextResponse[MAXLEN_8583PACK + 256];
	int unTicks = 0;
	int padlen = 0;
														
	memset(plaintextResponse, 0, sizeof(plaintextResponse));

	NDK_SysStopWatch(NULL);
	NDK_SysStartWatch();
	while (1) {
		nRet = nlMpos_Command.mpos_rdata_from_host(ME_Host_Command_Buf, sizeof(ME_Host_Command_Buf), 100);
		NDK_SysReadWatch(&unTicks);
		if (unTicks > nTimeoutMs) {
			NDK_SysStopWatch(&unTicks);
		    PDEBUG("here!!!!!");
			return NDK_ERR_TIMEOUT;
		}

		if (nRet > 0) {

			PDEBUG("^^^^^^^^^^^^^RECV^^^^^^^^^^^^^");
		    
			g_METPEDCtl.u1MposSn = ME_Host_Command_Buf[MPOS_SN_OFFSET]; // 当前的SN
			memcpy(g_METPEDCtl.u1MposCmd, ME_Host_Command_Buf + MPOS_COMMAND_OFFSET, MPOS_COMMAND_SIZE); // 当前命令码
			if (nlMpos_Command.mpos_edc_check(ME_Host_Command_Buf, nRet) != NDK_OK) {
				NDK_SysStopWatch(&unTicks);
			    PDEBUG("here!!!!!");
				return NDK_ERR;
			}

			if ((0x1E == ME_Host_Command_Buf[MPOS_COMMAND_OFFSET])
					&& (0x01 == ME_Host_Command_Buf[MPOS_COMMAND_OFFSET + 1])) {
				offset = 0;
				iDatalen = nlMpos_Command.mpos_readlen(
						ME_Host_Command_Buf + MPOS_VARIABLE_OFFSET + offset,
						_VAR_BIT16);
				if(iDatalen > MAXLEN_8583PACK){
				    PDEBUG("here!!!!!");
					NDK_SysStopWatch(&unTicks);
				    return NDK_ERR;
				}
			    padlen = (0x8 - (iDatalen) & 0x7) % 0x8;
				offset += 2;
    			PDEBUG_DATA("8583 ciphertext:", &ME_Host_Command_Buf[MPOS_VARIABLE_OFFSET + offset], iDatalen + padlen);
				iRet = NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 0,
						&ME_Host_Command_Buf[MPOS_VARIABLE_OFFSET + offset],
						iDatalen + padlen, plaintextResponse, SEC_DES_DECRYPT); //解密8583包
				if (NDK_OK != iRet) {
				    PDEBUG("here!!!!!");
					NDK_SysStopWatch(&unTicks);
					return NDK_ERR;
				}
				if(*(ME_Host_Command_Buf + MPOS_VARIABLE_OFFSET + offset + iDatalen + padlen) != 0x03){
				    PDEBUG("here!!!!!");
				    NDK_SysStopWatch(&unTicks);
				    return NDK_ERR;
                }
				memcpy(pszOutBuf, plaintextResponse, iDatalen);
				*pnReadLen = iDatalen;
				NDK_SysStopWatch(&unTicks);
				PDEBUG_DATA("8583 plaintext:",  pszOutBuf, *pnReadLen);
				return 0;
			} else {
				continue;
			}
		}
	}
}

/*
 ***********************************************************************************************
 函数原型：启动透传指令，调用直联合程序
 功能描述：获取地理位置信息
 输入参数：
 输出参数：无
 返回值：  无
 调用关系：
 ***********************************************************************************************
 */

int CommandParse_UnionPayCall(puchar pbuf, int buf_len,
		const pSysParam_t pTParam)  //把参数整合成一个数据结构
{
	int nRet = 0;
	int nKey = 0;

	if (AuthOK != 1) {
		NDK_ScrClrs();
		NDK_ScrPrintf("\n  Pls do Auth first \n");
		NDK_KbGetCode(2, NULL);
		nRet = -1;
		goto quit;
	}
	
	if (GeoPositionLen == 0) {
		NDK_ScrClrs();
		NDK_ScrPrintf("\nPls set location info\n");
		NDK_KbGetCode(2, NULL);
		nRet = -1;
		goto quit;
	}

	stUnionParam.StationLen = GeoPositionLen;
	memcpy(stUnionParam.StationBuf, geoposition, GeoPositionLen);	
    //启动之前清除一次串口缓冲
	NDK_PortClrBuf(PORT_NUM_USB);
	Union_main(&stUnionParam);
	
	//清零标志
	AuthOK = 0;
	GeoPositionLen = 0;
	
	//发送响应码给上位机 
quit:
	g_METPEDCtl.u1MposCmd[0] = 0x1e;
	g_METPEDCtl.u1MposCmd[1] = 0x02;
	nlMpos_Command.mpos_sdata_to_host(NULL, 0, CMD_OK);
	return nRet;
}

