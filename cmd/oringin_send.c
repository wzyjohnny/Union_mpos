#include "NDK.h"
#include "lcup.h"
#include "wdebug.h"

extern UnionParam_t stUnionParam;

#define APP_SUCC		(0)   	/**<成功      */
#define APP_FAIL		(-1)    /**<失败      */
#define APP_QUIT		(-2)    /**<退出、取消*/
#define APP_TIMEOUT 	(-3)    /**<超时      */
#define APP_FUNCQUIT	(-4)    /**<按功能键返回*/
#define APP_NULL		(-5)


#define FS			( 0x1C )
#define SEPARATOR	( 0x2f )
#define ACK 			( 0x06 )
#define NAK 			( 0x15 )
#define STX 			( 0x02 )
#define ETX 			( 0x03 )

#define KEY_ESC				0x1b		/**<取消键*/


int _PubBcdToByte(char ch, int *pnNum)
{
	if (pnNum == NULL)
	{
		return APP_FAIL;
	}
	if (((ch & 0x0F) > 9) || ((ch >> 4) > 9))
	{
		return APP_FAIL;
	}

	*pnNum = (ch >> 4) * 10 + (ch & 0x0f);
	return APP_SUCC;
}

int _PubBcdToInt(const char * psBcd, int *nNum)
{
	int nNum1, nNum2, ret;
	if (psBcd == NULL || nNum == NULL)
	{
		return APP_FAIL;
	}
	if (psBcd == NULL)
	{
		return APP_FAIL;
	}

	ret = _PubBcdToByte(psBcd[0], &nNum1);
	if (ret == APP_FAIL)
	{
		return APP_FAIL;
	}
	ret = _PubBcdToByte(psBcd[1], &nNum2);
	if (ret == APP_FAIL)
	{
		return APP_FAIL;
	}
	*nNum = nNum1 * 100 + nNum2;
	return APP_SUCC;
}

int Origin_UnionPay_Send(EM_PORT_NUM emPort, uint unLen, const char *pszInbuf) {
	int nRet = 0;
	nRet = NDK_PortWrite(emPort, unLen, pszInbuf);
	return nRet;
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

int Origin_UnionPay_Recv(EM_PORT_NUM emPort, char *pszOutBuf, int nTimeoutMs,
		int *pnReadLen) {

	int nRet;
	int s_nDownAux = emPort;
	char *sRecvBuf;
	sRecvBuf = pszOutBuf;
	int nReadLen;
	int unTime;
	int nKeyCode;
	int nLen2;
	int nLen;
	PDEBUG("recv...\n");
	while (1) {
		NDK_SysStartWatch();
		while (1) {
			sRecvBuf[0] = 0;
			nReadLen = 0;
			NDK_PortReadLen(s_nDownAux, &nReadLen);
			if (nReadLen > 0) {
				nRet = NDK_PortRead(s_nDownAux, 3, sRecvBuf,
						 nTimeoutMs, &nReadLen);
				if (NDK_OK != nRet) {
					break;
				}

				if (nReadLen <= 0) {
					break;
				}
				if (sRecvBuf[0] == STX) {
					break;
				}
			} else {
				NDK_KbHit(&nKeyCode);
				if (nKeyCode == KEY_ESC) {
					NDK_SysStopWatch(NULL);
					return -2;
				}
			}
			NDK_SysReadWatch(&unTime);
			if (unTime >= (nTimeoutMs)) {
				NDK_SysStopWatch(NULL);
				return -10;
			}
		}
		if (sRecvBuf[0] == STX) {
			nLen2 = 2;
			_PubBcdToInt(sRecvBuf + 1, &nLen2);
			nReadLen = 0;
			NDK_PortReadLen(s_nDownAux, &nReadLen);
			if (nReadLen > 0) {
				NDK_PortRead(s_nDownAux, nLen2 + 2, sRecvBuf + 3,
						nTimeoutMs, &nReadLen);
				if (nReadLen <= 0) {
					break;
				}
			} else {
				NDK_KbHit(&nKeyCode);
				if (nKeyCode == KEY_ESC) {
					NDK_SysStopWatch(NULL);
					return -2;
				}
			}
			nLen = nLen2 + 5;
		} else {
			break;
		}
		break;
	}
	*pnReadLen = nLen;
	NDK_SysStopWatch(NULL);
	PDEBUG_DATA("recvdata", pszOutBuf, *pnReadLen);
	return 0;
}



int origin_call(void)
{

	char tmp[32] = { 0xFE };
	memset(tmp, 0xff, 32);
	NDK_ScrClrs();
	NDK_ScrPrintf("try to run unionpay");
	NDK_KbGetCode(0, NULL);
	NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1");
	NDK_PortClose(PORT_NUM_USB);
	NDK_PortClose(PORT_NUM_USB);
	stUnionParam.StationLen = 32;
	memcpy(stUnionParam.StationBuf, tmp, 32);
	stUnionParam.UnionPay_Recv = Origin_UnionPay_Recv;
	stUnionParam.UnionPay_Send= Origin_UnionPay_Send;
	Union_main(&stUnionParam);
	NDK_ScrClrs();
	NDK_ScrPrintf("return succ");
	NDK_PortClose(PORT_NUM_COM1);
	NDK_KbGetCode(0, NULL);

    return 0;
}
