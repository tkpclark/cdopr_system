#include "lib.h"
#include "sgip.h"

main(int argc,char **argv)
{
	if(argc<2)
	{
		printf("i want to know the user's phone number\n");
		exit(0);
	}
	int fd;
	char mt[PKG_LENGTH];
	int n;
	char heapmtgw[128];
	read_config("heapmtgw",heapmtgw);
	
	fd=open(heapmtgw,O_WRONLY|O_APPEND);
	if(fd<0)
	{
		printf("open error!\n");
		exit(0);
	}

	SUBMIT_PKG submit_pkg;
	/*
	char SPNumber[16];
	char ChargeNumber[16];
	char UserNumber[16];
	char CorpId[8];
	char ServiceType[10];
	unsigned char FeeType;
	char FeeValue[8];
	int MessageLength;
	char MessageContent[140];
	char linkid[10];
	*/

	strcpy(submit_pkg.SPNumber,"10627041");
	strcpy(submit_pkg.ChargeNumber,argv[1]);
	strcpy(submit_pkg.UserNumber,argv[1]);
	strcpy(submit_pkg.CorpId,"51405");
	strcpy(submit_pkg.ServiceType,"9091014400");
	submit_pkg.FeeType=3;
	strcpy(submit_pkg.FeeValue,"1000");
	if(argc==3)
	{
		strcpy(submit_pkg.MessageContent,argv[2]);
	}
	else
	{
		strcpy(submit_pkg.MessageContent,"abc");
	}
	submit_pkg.MessageLength=strlen(submit_pkg.MessageContent);

	memset(mt,0,sizeof(mt));
	memcpy(mt,&submit_pkg,sizeof(SUBMIT_PKG));
	n=write(fd,mt,sizeof(mt));
	printf("wrote %d bytes\n",n);
	
	close(fd);

}
