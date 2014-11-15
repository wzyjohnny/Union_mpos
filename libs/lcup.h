/**
* @file lcup.h
* @brief 银联应用
* @version  1.0
* @author 林礼达
* @date 2014-10-30
*/

#ifndef _L_CUP_
#define _L_CUP_

typedef struct {
    int StationLen;
    char StationBuf[32];
    int (*UnionPay_Send)(EM_PORT_NUM emPort, uint unLen,const char *pszInbuf);
    int (*UnionPay_Recv)(EM_PORT_NUM emPort, char *pszOutBuf,int nTimeoutMs, int *pnReadLen);
}UnionParam_t;

int Union_main(UnionParam_t *pstUnionParam);

#endif

