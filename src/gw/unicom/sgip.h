#include "lib.h"
typedef struct
{
	int seq;
	char SPNumber[16];
	char ChargeNumber[16];
	char UserNumber[16];
	char CorpId[8];
	char ServiceType[20];
	unsigned char FeeType;
	char FeeValue[8];
	int MessageLength;
	char MessageContent[140];
	char linkid[10];
}SUBMIT_PKG;


