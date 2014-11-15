 /*
***********************************************************************************************
版权说明：ME31指令集
版本号：1.0
生成日期：
作者：
模块目的/功能：ME31    NDK接口补齐
主要函数及其功能：对K21 平台1.0的ME31,NDK接口与ME30存在不一致，
                                    尤其是蓝牙方面，将旧的接口转换为新的NDK接口.
                                    非K21 平台1.0的ME31请勿包含此文件
修改日志：
***********************************************************************************************
*/
#include "NDK.h"
#include "string.h"



 /**
 *@brief   ME31 1.0  BT模块常用命令定义
 */
 typedef enum {
	 BT_CMD_PIN=0,						 //蓝牙获取当前配对密码
	 BT_CMD_NAME=1, 					 //蓝牙获取当前设备名称
	 BT_CMD_PIN_SET=2,					 //蓝牙设置配对密码
	 BT_CMD_NAME_SET=3, 				 //蓝牙设置名称
	 BT_CMD_CLR=4,						 //蓝牙清除配对列表
	 BT_CMD_RST=5,						 //蓝牙恢复出厂设置（清除配对记录、恢复名称、配对密码）
	 BT_CMD_QUIT=6, 					 //蓝牙退出设置模式（建议重启蓝牙，目前退出设置模式，是回到可连接状态，不是可查找状态）
	 BT_CMD_SCAN0=7,					 //是否被手机识别到
	 BT_CMD_SCAN1=8,					 //是否被手机识别到
	 BT_CMD_BD_ADDR_GET=9,				 //获取蓝牙模块MAC地址
	// BT_CMD_BD_PEER_ADDR_GET=10,		   //获取蓝牙模块MAC地址
	 BT_CMD_BD_SUSPEND, 				 //休眠指令AT
	 BT_CMD_BD_SUSPEND_DIS, 			 //休眠指令AT
	 BT_CMD_GPIO1,						 //gpio设置
	 BT_CMD_GPIO2,						 //gpio设置
	 BT_CMD_DISCON, 					 //断开BT模块连接
	 BT_CMD_BYPASS, 					 //传输通道
	 BT_CMD_PINMODE4,					 //关闭PIN模式
	 BT_CMD_PINMODE2,					 //打开PIN模式
	 BT_CMD_VER,						 //得到版本
	 BT_CMD_ENABLEENCRYPTION,
	 BT_CMD_DISABLEENCRYPTION,
	 BT_CMD_END,						 //<命令结束，如果新增命令都放这之前位置
	 BT_CMD_UNDEFINE=1024				 //<列表中未定义
 } BTATCMDLIST;



// ME31 private
extern int Private_SysSetPosInfo(int emFlag, const char *psBuf);
// ME31 private bt24

// ME31  private ioctl
extern int Private_Ioctl(int nHandle, int cmd, void *arg);

static char g_CBtType;          //ME31 bt24和bm77存在差异，开机判断蓝牙模块.0 为BT24，1为BM77



/*
功能:    仅用于ME31  K21 平台1.0 .    获取蓝牙对应的串口号
输入:    无
输出:    无

/*
功能:    仅用于ME31  K21 平台1.0 .   获取蓝牙型号
输入:    无
输出:    无
返回:    无
说明:将检测到的蓝牙型号置为全局变量g_CBtType. 0代表 BT24, 1代表BM77

/*
功能:    仅用于ME31  K21 平台1.0 .   获取PIN
输入:    无
输出:    char * pinstr,
返回:    


/*
功能:    仅用于ME31  K21 平台1.0 .   获取MAC
输入:    无
输出:    char * name
返回:    

 /*
功能:   仅用于 ME31  K21 平台1.0 .   设置蓝牙名称
输入:    const char * name
输出:    
返回:    
*/

 /*
功能:    仅用于ME31  K21 平台1.0 .   获取蓝牙连接状态,0 已连接 ,1 未连接
输入:    
输出:    
返回:    
*/ 	

 /*
功能:    仅用于ME31  K21 平台1.0 .   蓝牙断开
输入:    
输出:    
返回:    
*/  

 /*
功能:    仅用于ME31  K21 平台1.0 .  ME31 获取电量范围
                127-134			0格
                134-144			1格
                141-148			2格
                148-155			3格
输入:    
输出:    
返回:    
*/  
 int NDK_SysGetPowerVolRange(uint * punMax, uint * punMin)
 {
 	*punMin = 127;		
 	*punMax = 155;
	 return 0;
 }


 /*
功能:    ME31  K21 平台1.0 .  设置自动关机时间，目前不支持
输入:    
输出:    
返回:    
*/ 
int NDK_SysSetShutdownDelay(uint unSeconds)
{
    return NDK_ERR_NOT_SUPPORT;
}


 /*
功能:    ME31  K21 平台1.0 .  IOCTL
输入:    
输出:    
返回:    
*/ 
int ioctl(int fd,int request,void * arg)
{
	return Private_Ioctl(fd,request,arg);
}



/*
功能:    ME31  K21 平台1.0 .  NDK_KbScanCode
输入:    
输出:    
返回:    
*/ 
 int NDK_KbScanCode(int * pnCode)
{
	return NDK_KbHit(pnCode);
}



