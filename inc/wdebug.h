
#ifndef WDEBUG_INCLUDE_H
#define WDEBUG_INCLUDE_H

#define APP_DEBUG 
#ifdef APP_DEBUG

#define DPRINTF(fmt, args...)	printf("[%s %d]"fmt"\n", __FILE__, __LINE__, ##args);
#define PDEBUG(fmt, args...)	printf("[%s %d]"fmt"\n", __FILE__,__LINE__, ##args);

#define PDEBUG_DATA(info, data, len) 	{int i;\
											printf("[%s %d]%s: len=%d data=", __FILE__, __LINE__, (info), (len));\
											for(i=0; i<(len); i++){\
												printf("%02x ", *(data+i));\
											}\
											NDK_SysMsDelay(10);\
											printf("\n\r");\
											NDK_SysMsDelay(10);\
										}
#else
#define DPRINTF(fmt, args...)
#define PDEBUG(fmt, args...)
#define PDEBUG_DATA(info, data, len)
#endif

#endif
