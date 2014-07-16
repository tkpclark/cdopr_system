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

	unsigned char UserCount;
	char GivenValue[8];
	unsigned char AgentFlag;
	unsigned char MorelatetoMTFlag;
	unsigned char Priority;
	char ExpireTime[32];
	char ScheduleTime[32];
	unsigned char ReportFlag;
	unsigned char TP_pid;
	unsigned char TP_udhi;
	unsigned char MessageCoding;
	unsigned char MessageType;


}SUBMIT_PKG;


