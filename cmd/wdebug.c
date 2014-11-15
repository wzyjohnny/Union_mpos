
#include "wdebug.h"
#include "NDK.h"

#ifdef APP_DEBUG

static int s_DbgComInitFlag=0;

int putchar(int ch)
{
#if 1
	if (!s_DbgComInitFlag){
		if (NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1")!=NDK_OK){
			return 0;
		}
		s_DbgComInitFlag = 1;
	}
#endif

	NDK_PortWrite(PORT_NUM_COM1, 1, (char *)&ch);
//	NDK_PortWrite(PORT_NUM_COM2, 1, (char *)&ch);
	return 0;
}

#endif
