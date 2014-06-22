#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ccl/ccl.h>
#include <mysql.h>
#include "soapH.h"
#include "SendSmsBinding.nsmap"

	
char mdname[32]="sendsms";
char logpath[256];
char version[]="1.02";
static int quit;
static int prtpid;
static int rcdlen;
static char mmapfile[128];
static char logfile[128];
static MYSQL mysql;
static char serverurl[256];
static char spid[32];
static char key[32];

static char dbip[16];
static char dbuser[16];
static char dbpass[32];
static char dbname[32];

static char *p_map;
static unsigned int mmapsize;

char *init_mmap(char *pathname,unsigned int msize);

static void acquit(int signo)
{
	quit=1;
	//proclog(logfd,"MESSAGE:got quiting signal");
}
static void gofulfil(int signo)
{
	;//proclog(logfd,"MESSAGE:go fetch data!\n");
}
static void procquit(void)
{
  	proclog("MESSAGE:quited!");
}
static void read_config(char *confile)
{
	struct ccl_t config;
	const struct ccl_pair_t *iter;
	config.comment_char = '#';
	config.sep_char = '=';
	config.str_char = '"';
	ccl_parse(&config, confile);
	while((iter = ccl_iterate(&config)) != 0)
	{
		if(!strcmp(iter->key,"mmapfile"))
			strcpy(mmapfile,iter->value);
		else if(!strcmp(iter->key,"logpath"))
			strcpy(logpath,iter->value);
		else if(!strcmp(iter->key,"mmapsize"))
			mmapsize=atoi(iter->value);
		else if(!strcmp(iter->key,"rcdlen"))
			rcdlen=atoi(iter->value);
		else if(!strcmp(iter->key,"serverurl"))
			strcpy(serverurl,iter->value);
		else if(!strcmp(iter->key,"spid"))
			strcpy(spid,iter->value);
		else if(!strcmp(iter->key,"key"))
			strcpy(key,iter->value);
			
		else if(!strcmp(iter->key,"ip"))
			strcpy(dbip,iter->value);
		else if(!strcmp(iter->key,"user"))
			strcpy(dbuser,iter->value);
		else if(!strcmp(iter->key,"pass"))
			strcpy(dbpass,iter->value);
		else if(!strcmp(iter->key,"db"))
			strcpy(dbname,iter->value);
	}
	ccl_release(&config);
}


char* getTimeStamp(char *ts)
{
	time_t tt;
	tt=time(0);
	strftime(ts,16,"%m%d%H%M%S",localtime(&tt));
	return ts;
}

char* getspPwd(char* pwd,char* ts)
{
	int i;
	//char *key="351001355";
	char buf[64]={0};
	char tmp[64]={0};
	strcpy(buf,spid);
	strcpy(buf+strlen(spid),key);
	strcpy(buf+strlen(spid)+strlen(key),ts);
	MD5(buf,strlen(spid)+strlen(key)+strlen(ts),tmp);
	for(i=0;i<16;i++)
		sprintf(pwd+2*i,"%02X",*((unsigned char*)(tmp+i)));
	//printf("%s\n",pwd);
	return pwd;
}

char *tostr(int seqid,char *strseqid)
{
	sprintf(strseqid,"%d",seqid);
	return strseqid;
}


static fulfil(char *p_data)
{
			char *addresses[1];
			char spPassword[128];
			char timeStamp[16];
			//char *SAN="10661333";
			char *transactionId="";
			enum ns4__EndReason transEnd=ns4__EndReason__0;
			char *FA="";
			enum xsd__boolean multicastMessaging=xsd__boolean__false_;
			char utfcontent[512];
			char *description="";
			char *currency="";
			char *interfaceName="smssend";
	
			struct soap soap;
			struct ns2__sendSms ns2__sendSms;
			struct ns2__sendSmsResponse ns2__sendSmsResponse;
			struct ns4__RequestSOAPHeader ns4_RequestSOAPHeader;
			struct ns4__SimpleReference receiptRequest;
			struct ns4__ChargingInformation charging;
			
			char pn[32]="tel:";
			strcat(pn,p_data+260);
			addresses[0]=pn;
			ns4_RequestSOAPHeader.spId=spid;
			getTimeStamp(timeStamp);
			memset(spPassword,0,sizeof(spPassword));
			getspPwd(spPassword,timeStamp);
			ns4_RequestSOAPHeader.spPassword=spPassword;
			ns4_RequestSOAPHeader.timeStamp=timeStamp;
	
			ns4_RequestSOAPHeader.productId=p_data;
			ns4_RequestSOAPHeader.linkId=p_data+320;
			ns4_RequestSOAPHeader.transactionId=transactionId;
			
			char senderName[32];
			sprintf(senderName,"%s",p_data+40);
			ns4_RequestSOAPHeader.SAN="";
			
			ns4_RequestSOAPHeader.OA=addresses[0];
			ns4_RequestSOAPHeader.FA=FA;
			ns4_RequestSOAPHeader.transEnd=&transEnd;
			ns4_RequestSOAPHeader.multicastMessaging=&multicastMessaging;
	
			charging.description=description;
			charging.currency=currency;
			charging.amount=p_data+280;
			charging.code="";
			receiptRequest.endpoint=addresses[0];
			receiptRequest.interfaceName=interfaceName;
			char strseqid[16];
			receiptRequest.correlator=tostr(*(int*)(p_data+350),strseqid);
			//*(unsigned int*)(p_map)=*(int*)(p_data+350);//tell daemon where the child has processed
			
			//memset(utfcontent,0,sizeof(utfcontent));
			//to_utf(p_data+60,utfcontent);
			//gb2u("utf8",mySmsDS.MsgContent,strlen(mySmsDS.MsgContent),gcontent,&len);
			//to_uc(mySmsDS.MsgContent,uc2msg);
			ns2__sendSms.__sizeaddresses=1;
			ns2__sendSms.addresses=addresses;
			ns2__sendSms.senderName=senderName;
			ns2__sendSms.charging=&charging;
			ns2__sendSms.message=p_data+60;
			//ns2__sendSms.message=p_data+60;
			//ns2__sendSms.message=message;
			ns2__sendSms.receiptRequest=&receiptRequest;
			
			
			proclog("cor[%s]spid[%s]pwd[%s]ts[%s]tran[%s]san[%s]fa[%s]src[%s]dest[%s]pid[%s]linkid[%s]amount[%s]code[%s]msg[%s]",
											receiptRequest.correlator,
											ns4_RequestSOAPHeader.spId,
											ns4_RequestSOAPHeader.spPassword,
											ns4_RequestSOAPHeader.timeStamp,
											ns4_RequestSOAPHeader.transactionId,
											ns4_RequestSOAPHeader.SAN,
											ns4_RequestSOAPHeader.FA,
											ns2__sendSms.senderName,
											receiptRequest.endpoint,
											ns4_RequestSOAPHeader.productId,
											ns4_RequestSOAPHeader.linkId,
											ns2__sendSms.charging->amount,
											charging.code,
											p_data+60
											);

			SOAP_SOCKET m, s; /* master and slave sockets */
			soap_init(&soap);
			soap.header = (struct SOAP_ENV__Header *)soap_malloc(&soap, sizeof(struct SOAP_ENV__Header));
			soap_set_mode(&soap, SOAP_C_UTFSTRING);
			soap.header->ns4__RequestSOAPHeader=&ns4_RequestSOAPHeader;

			//proclog("server:[%s]", serverurl);
			soap_call___ns1__sendSms(&soap, serverurl, NULL,&ns2__sendSms,&ns2__sendSmsResponse);

			if (soap.error)
			{
				//printf("SendSms:\n");
			   	soap_print_fault(&soap, stderr);
			   	proclog("soap error!");
			}
			
			proclog("correlator[%s]result[%s]", receiptRequest.correlator, ns2__sendSmsResponse.result);
			//update result
			char sql[256];
			sprintf(sql, "update wraith_message set gw_resp='%s', gw_resp_time=NOW()  where ID='%s'",
										ns2__sendSmsResponse.result,
										receiptRequest.correlator
										);
				mysql_exec(&mysql,sql);
			
			
				soap_destroy(&soap);
				soap_end(&soap);
				soap_done(&soap);


			
			
			

			///
}
main(int argc, char **argv)
{
	int i;
	int lockfd;
	unsigned int curnum=20000;
	unsigned int leftnum=0;
	void *p_chd;
	char tmp[8]={0};
	struct sigaction signew;
	
	sprintf(mdname, "%s-%d", mdname, getpid());

	prtpid=getppid();
	if(atexit(&procquit))
	{
	   printf("gstop code can't install!");
	  kill(prtpid,SIGINT);
		pause();
	}
	read_config(argv[1]);
	//sprintf(tmp,"|%d",getpid());
	//strcat(mdname,tmp);


	lockfd=open("tkp.lck",O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
	if(lockfd==-1)
	{
		printf("open lock file error!\n");
		kill(prtpid,SIGINT);
		pause();
	}
	
	signew.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&signew,0);
	
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
  	sigaction(SIGINT,&signew,0);
	
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
	sigaction(SIGTERM,&signew,0);
	
	signew.sa_handler=gofulfil;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
	sigaction(SIGUSR1,&signew,0);
	
	mysql_init(&mysql);
	   if(!mysql_real_connect(&mysql,dbip,dbuser,dbpass,dbname,0,NULL,0))
	   {
		sql_err_log(&mysql);
		exit(0);
	   }
	
	
	p_map=init_mmap(mmapfile,mmapsize);
	if(!p_map)
	{
		kill(prtpid,SIGINT);
		pause();
	}
	p_chd=malloc(rcdlen);
	while(1)
	{
		if(quit)
			exit(0);
		alarm(200);
		if(flock(lockfd,LOCK_EX))
		{
			proclog("lock error! %s",strerror(errno));
		    	sleep(1);
		    	continue;
		}
		if(!*(unsigned int*)(p_map+2*sizeof(unsigned int)))
		{
			myflock(lockfd,2);
			//proclog(MESSAGE:logfd,"no data now");
			//pause();
			//mysql_exec(&mysql,"set names gbk");
			mysql_ping(&mysql);
			sleep(60);
			continue;
		}
		proclog("mmap old:[%d][%d][%d]",*(unsigned int*)(p_map),(*(unsigned int*)(p_map+sizeof(unsigned int))),(*(unsigned int*)(p_map+2*sizeof(unsigned int))));
		curnum=*(unsigned int*)(p_map+sizeof(unsigned int));
		leftnum=*(unsigned int*)(p_map+2*sizeof(unsigned int));
		memcpy(p_chd,p_map+512+curnum*rcdlen,rcdlen);

		*(unsigned int*)(p_map)=*(unsigned int*)(p_chd+350);
		(*(unsigned int*)(p_map+sizeof(unsigned int)))+=1;
		(*(unsigned int*)(p_map+2*sizeof(unsigned int)))-=1;
		proclog("mmap new:[%d][%d][%d]",*(unsigned int*)(p_map),(*(unsigned int*)(p_map+sizeof(unsigned int))),(*(unsigned int*)(p_map+2*sizeof(unsigned int))));
		if(flock(lockfd,LOCK_UN))
		{
			proclog("unlock error! %s",strerror(errno));
		}
		if(leftnum==1)
		{
			kill(prtpid,SIGUSR1);
		}
		fulfil(p_chd);
		alarm(0);
	}
}
