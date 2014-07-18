

#include "lib.h"
#include "sk.h"
#include "sgip.h"


static int sd;
static unsigned int psoff;
static unsigned int seq=0;
static char nodeId[16];//="3024051405"
static char corpId[16];//="51405"
static char gateip[32];//="218.60.136.119"
static char port[16];//="8801"
static char username[16];//="syqp51405"
static char password[16];//="syqp51405"
static unsigned int idle_point;//time of no date
static int bind_flag;
static char gwid[4];
skt_s *sp;


char mdname[]="unimt_fee";
char logpath[128];
char version[]="1.01";

static char dbip[32];
static char dbname[32];
static char dbuser[32];
static char dbpass[32];

MYSQL mysql;
static MYSQL_RES *result;
static void acquit(int signo)
{
	exit(0);
}
static void procquit(void)
{
  	proclog("quiting!");
  	//unlink(pidfile);
}
init()
{
	struct sigaction signew;

	
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
  	sigaction(SIGINT,&signew,0);
	
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
	sigaction(SIGTERM,&signew,0);
	
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
	sigaction(SIGQUIT,&signew,0);
	
	//write_pid(pidfile);
	
	if(atexit(&procquit))
	{
	   printf("gstop code can't install!");
	   exit(0);
	}
	
	//read_config();


	 mysql_init(&mysql);
	  if(!mysql_real_connect(&mysql,dbip,dbuser,dbpass,dbname,0,NULL,0))
	  {
	       sql_err_log(&mysql);
	       exit(0);
	  }

	
	proclog("starting...\n");

	
}
static int sgip_bind(char *gateip,int port,unsigned int nodeId,char *username,char *password,unsigned int seq)
{
	unsigned char pak[128];
	unsigned char nownow[256];
	unsigned char response[64];
	unsigned char *pp;
	int i;

	memset(pak,0,sizeof(pak));
	memset(response,9,sizeof(response));
	pp=pak;
	*(int *)pp=htonl(61);
	*(int *)(pp+4)=htonl(0x00000001);
	*(int *)(pp+8)=htonl(nodeId);
	nownow[0]=0;
	tstring(nownow);
	*(int *)(pp+12)=htonl(atoll(nownow));
	*(int *)(pp+16)=htonl(seq);
	*(pp+20)=(unsigned char)1;
	strcpy(pp+21,username);
	//strcpy(pp+37,gshare_key);
	strcpy(pp+37,password);
	proclog( "login... ip:[%s:%d] name[%s] passwd[%s]",gateip,port,username,password);
	
	sp=(skt_s*)sopen();
	if(sclient(sp,gateip,port)==-1) 
	{
		proclog("failed to connect to %s:%d,%s",gateip,port,strerror(errno));
		exit(0);
	}
	//if((cmppsd=sclient(sp,ggate_ip,5088))==-1) exit(0);
	
	if(write(sp->sd,pak,61)==-1)
	{
		proclog("failed to send login cmd!%s",strerror(errno));
		exit(0);
	}
	//proclog( "bind before is:%d",response[20]);
	if((i=recv(sp->sd,response,29,MSG_WAITALL))==-1) 
	{
		proclog( "failed to recv login response!");
		exit(0);
	}
	//cmd=ntohl(*((unsigned int *)(response+4)));
	//if(cmd!=0x80000001) exit(0);
	if(response[20]==0)
	{
		//proclog( "bind status is:%d",response[20]);
		bind_flag=1;
	}
	else
	{
		proclog( "bind failed! return:%d",response[20]);
		exit(0);
	}
	
	return (int)response[20]; 
}


static int sgip_unbind(char *nodeid,unsigned int seq)
{
	unsigned char pak[70];
	unsigned char nownow[64];
	unsigned char response[40];
	unsigned char *pp;
	int n;

	memset(pak,0,70);
	memset(response,0,40);
	pp=pak;
	//proclog("sending unbind cmd...");
	*(int *)pp=htonl(20);
	*(int *)(pp+4)=htonl(0x00000002);
	*(int *)(pp+8)=htonl(atoi(nodeid));
	//*(int *)(pp+12)=htonl(825140000);
	nownow[0]=0;
	tstring(nownow);
	*(int *)(pp+12)=htonl(atol(nownow));
	*(int *)(pp+16)=htonl(seq);//seq

	bind_flag=0;
	
	if(writeall(sp->sd,pak,20)==-1)
	{
		proclog("failed int unbind:%s",strerror(errno));
		return;
	}

	n=recv(sp->sd,response,20,MSG_WAITALL);
	
	//proclog("recved %d bytes",n);
	sclose(sp);

}

/*
unsigned int s4,
int mtclen,
char *content,
char *service_id,
char *src_id,
char feetype,
char MTFlag,
char RPFlag,
char *feecode,
char *des_id,
char *l88,
char *linkid
*/
static void sgip_submit(SUBMIT_PKG *p_submit_pkg,int nodeId)
{
	//proclog("nodeid:%u\n",nodeId);
	unsigned char buffer[1024];
	unsigned char nownow[300];
	unsigned char *pp;

	unsigned int pkg_len=0;
	int n=0;
	int cmd=0x00000003;

	memset(buffer,0,sizeof(buffer));
	pp=buffer;
	*(int *)(pp+4)=htonl(cmd);
	*(int *)(pp+8)=htonl(nodeId);
	nownow[0]=0;
	tstring(nownow);
	*(int *)(pp+12)=htonl(atol(nownow));
	*(int *)(pp+16)=htonl(p_submit_pkg->seq);
	strcpy(pp+20,p_submit_pkg->SPNumber);
	//sprintf(pp+41,"86%s",p_submit_pkg->ChargeNumber);
	strcpy(pp+41,p_submit_pkg->ChargeNumber);
	*(pp+62)=p_submit_pkg->UserCount;//
	//sprintf(pp+63,"86%s",p_submit_pkg->UserNumber);
	strcpy(pp+63,p_submit_pkg->UserNumber);
	strcpy(pp+84,p_submit_pkg->CorpId);
	strcpy(pp+89,p_submit_pkg->ServiceType);
	*(pp+99)=(unsigned char)p_submit_pkg->FeeType;
	strcpy(pp+100,p_submit_pkg->FeeValue);
	strcpy(pp+106,p_submit_pkg->GivenValue); //
	*(pp+112)=p_submit_pkg->AgentFlag;

	*(pp+113)=p_submit_pkg->MorelatetoMTFlag;//包月话单 	//引起MT消息的原因 0-MO点播引起的第一条MT消息；
	*(pp+114)=p_submit_pkg->Priority;//
	*(pp+147)=p_submit_pkg->ReportFlag;//包月话单
	*(pp+148)=p_submit_pkg->TP_pid;
	*(pp+149)=p_submit_pkg->TP_udhi;
	*(pp+150)=p_submit_pkg->MessageCoding;//
	*(pp+151)=p_submit_pkg->MessageType;
	*(int *)(pp+152)=htonl(p_submit_pkg->MessageLength);
	pkg_len=p_submit_pkg->MessageLength+164;

	*(int *)pp=htonl(pkg_len);
	strcpy(pp+156,p_submit_pkg->MessageContent);
	//strcpy(pp+156+p_submit_pkg->MessageLength,p_submit_pkg->linkid);
	
	/*
	proclog("MT:seq[%u]ChargeNumber[%s]CorpId[%s]nodeid[%u]FeeType[%d]FeeValue[%s]cnt[gbk]len[%d]ServiceType[%s]SPNumber[%s]UserNumber[%s]linkid[%s]bind[%d]",
			p_submit_pkg->seq,
			p_submit_pkg->ChargeNumber,
			p_submit_pkg->CorpId,
			nodeId,
			p_submit_pkg->FeeType,
			p_submit_pkg->FeeValue,
			//p_submit_pkg->MessageContent,
			p_submit_pkg->MessageLength,
			p_submit_pkg->ServiceType,
			p_submit_pkg->SPNumber,
			p_submit_pkg->UserNumber,
			p_submit_pkg->linkid,
			bind_flag);

	//proclog_HEX(buffer,pkg_len);
	*/
	/*
	 * submit_pkg.UserCount=1;
		strcpy(submit_pkg.GivenValue,"0");
		submit_pkg.AgentFlag=0;
		submit_pkg.MorelatetoMTFlag=3;
		submit_pkg.Priority=0;
		submit_pkg.ReportFlag=3;
		submit_pkg.TP_pid=0;
		submit_pkg.TP_udhi=0;
		submit_pkg.MessageCoding=15;
		submit_pkg.MessageType=0;
	 */
	proclog("MT:seq[%u]nodeid[%u]cmd[0x%x]SPNumber[%s]ChargeNumber[%s]UserCount[%d]UserNumber[%s]CorpId[%s]ServiceType[%s]FeeType[%d]FeeValue[%s]GivenValue[%s]AgentFlag[%d]MorelatetoMTFlag[%d]Priority[%d]ReportFlag[%d]TP_pid[%d]TP_udhi[%d]MessageCoding[%d]MessageType[%d]MessageLength[%d]bind[%d]",
				p_submit_pkg->seq,nodeId,cmd,p_submit_pkg->SPNumber,
				p_submit_pkg->ChargeNumber,p_submit_pkg->UserCount,p_submit_pkg->UserNumber,
				p_submit_pkg->CorpId,p_submit_pkg->ServiceType,
				p_submit_pkg->FeeType,
				p_submit_pkg->FeeValue,
				p_submit_pkg->GivenValue,
				p_submit_pkg->AgentFlag,
				p_submit_pkg->MorelatetoMTFlag,
				p_submit_pkg->Priority,
				p_submit_pkg->ReportFlag,
				p_submit_pkg->TP_pid,
				p_submit_pkg->TP_udhi,
				p_submit_pkg->MessageCoding,
				p_submit_pkg->MessageType,
				p_submit_pkg->MessageLength,
				bind_flag);
	if(writeall(sp->sd,buffer,pkg_len)==-1)
	{
		proclog("submit failed! %s",strerror(errno));
		exit(0);
	}

	return;
}
static read_response()
{
	char buffer[256];
	memset(buffer,9,sizeof(buffer));
	int n=-2;

	int len=0;
	int cmd=0;
	int sbm_seq=0;

	//message header
	if((n=read(sp->sd,buffer,20))!=20)
	{
		proclog("Read Header Error! return [%d]",n);
		return;
	}
	len=ntohl(*((unsigned int *)buffer));
	cmd=ntohl(*((unsigned int *)(buffer+4)));
	sbm_seq=ntohl(*((unsigned int *)(buffer+16)));


	//message body
	memset(buffer,0,sizeof(buffer));
	if((n=read(sp->sd,buffer,len-20))!=(len-20))
	{
		proclog("Read Body Error! return [%d]",n);
		exit(0);
	}
	unsigned char result=buffer[0];
	proclog("RESP:len[%d]cmd[0x%X]seq[%d]result[%d]",len,cmd,sbm_seq,result);

	//update
	char sql[512];
	sprintf(sql,"update wraith_message set gw_resp='%d',gw_resp_time=NOW() where id='%d'",result,sbm_seq);
	//proclog(sql);
	mysql_exec(&mysql, sql);
}
static int new_data()
{
	char sql[256];
	sprintf(sql,"select * from wraith_message where gwid=%s and mo_status='ok' and  gw_resp is null limit 500", gwid);
	//sprintf(sql,"select * from wraith_message where ID > 111229 and gwid=%s and report!='1' limit 500", gwid);
	//proclog(sql);
	mysql_exec(&mysql,"set names gbk");
	mysql_exec(&mysql,sql);
	result=mysql_store_result(&mysql);
	return mysql_num_rows(result);
}
static void my_nano_sleep(unsigned int nsec)
{
	struct timespec slptm;
	slptm.tv_sec = nsec/1000000000;
	slptm.tv_nsec = nsec%1000000000;
	nanosleep(&slptm, NULL);
}
static send_all_data()
{
	MYSQL_ROW row;
	SUBMIT_PKG submit_pkg;
	/*
	 * typedef struct
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
	 */
	while(row=mysql_fetch_row(result))
	{
		memset(&submit_pkg,0,sizeof(SUBMIT_PKG));

		//////
		submit_pkg.UserCount=1;
		strcpy(submit_pkg.GivenValue,"0");
		submit_pkg.AgentFlag=0;
		submit_pkg.MorelatetoMTFlag=3;
		submit_pkg.Priority=0;
		submit_pkg.ReportFlag=3;
		submit_pkg.TP_pid=0;
		submit_pkg.TP_udhi=0;
		submit_pkg.MessageCoding=15;
		submit_pkg.MessageType=0;


		///////////from db

		if(row[0])
			submit_pkg.seq=atoi(row[0]);
		if(row[4])
			strcpy(submit_pkg.SPNumber, row[4]);
		if(row[2])
		{
			char tmp[512];
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"86%s",row[2]);
			strcpy(submit_pkg.ChargeNumber, tmp);
			strcpy(submit_pkg.UserNumber, tmp);
		}
		if(row[11])
		{
			strcpy(submit_pkg.ServiceType,row[11]);
			//printf("servicetype:%s\n",submit_pkg.ServiceType);
		}
		if(row[8])
		{
			strcpy(submit_pkg.FeeValue, row[8]);
			//printf("value:%s\n",submit_pkg.FeeValue);
		}



		submit_pkg.FeeType= 3;
		submit_pkg.MessageLength=strlen(submit_pkg.MessageContent);
		strcpy(submit_pkg.CorpId,corpId);

		alarm(10);
		sgip_submit(&submit_pkg,atoll(nodeId));
		alarm(10);
		read_response();
		alarm(0);
		//printf("row0:%s\n",row[0]);
		if(row[0])
			psoff=atoi(row[0]);

		//my_nano_sleep(300000000);
		//sleep(1);
	}
}
/*
static send_mt()
{
	sgip_bind(gateip,atoi(port),atoll(nodeId),username,password,++seq);
	send_all_data();
	sgip_unbind(nodeId,++seq);
}
*/
static read_config(char *confile)
{
	struct ccl_t config;
	const struct ccl_pair_t *iter;
	config.comment_char = '#';
	config.sep_char = '=';
	config.str_char = '"';
	ccl_parse(&config, confile);
	while((iter = ccl_iterate(&config)) != 0)
	{
		if(!strcmp(iter->key,"nodeId"))
			strcpy(nodeId,iter->value);
		else if(!strcmp(iter->key,"corpId"))
			strcpy(corpId,iter->value);
		else if(!strcmp(iter->key,"gateip"))
			strcpy(gateip,iter->value);
		else if(!strcmp(iter->key,"port"))
			strcpy(port,iter->value);
		else if(!strcmp(iter->key,"gwid"))
			strcpy(gwid,iter->value);
		else if(!strcmp(iter->key,"username"))
			strcpy(username,iter->value);
		else if(!strcmp(iter->key,"password"))
			strcpy(password,iter->value);
		else if(!strcmp(iter->key,"ip"))
			strcpy(dbip,iter->value);
		else if(!strcmp(iter->key,"db"))
			strcpy(dbname,iter->value);
		else if(!strcmp(iter->key,"user"))
			strcpy(dbuser,iter->value);
		else if(!strcmp(iter->key,"pass"))
			strcpy(dbpass,iter->value);
		else if(!strcmp(iter->key,"logpath"))
			strcpy(logpath,iter->value);

	}
	ccl_release(&config);
}
main(int argc, char **argv)
{
	if(argc!=2)
	{
		printf("please tell me config file!\n");
		exit(0);
	}
	if(!is_file_exist(argv[1]))
	{
		printf("file %s doesn't exist!\n",argv[1]);
		exit(0);
	}
	read_config(argv[1]);
	proclog("starting...");
	init();

	idle_point=time(0);
	
	while(1)
	{
		if(new_data())
		{
			//printf("new data!\n");
			if(!bind_flag)
			{
				sgip_bind(gateip,atoi(port),atoll(nodeId),username,password,++seq);
			}
			send_all_data();
			idle_point=time(0);
			continue;
		}
		//printf("%d\n",time(0)-idle_point);
		else
		{
			if(bind_flag)
			{
				if(time(0)-idle_point > 5)
				{
					sgip_unbind(nodeId,++seq);
				}
			}
		}
		
		sleep(1);
	}	
}

