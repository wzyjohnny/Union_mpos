#ifndef _API_BT_H_
#define _API_BT_H_


int ME_GetBtPort();
int ME_BTGetVer();


int NDK_BTGetPIN(char *pinstr);
int ME_BtInit(void);
void ME_BtSetPIN(void);
void ME_BtSetName(void);
int ME_BtGetPinNAME(char *PIN, char *NAME);
int ME_BtFormat(char *namebuf);

#endif
