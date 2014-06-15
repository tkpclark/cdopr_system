

#include "lib.h"
#include "sk.h"
#include "sgip.h"

static const char *pidfile="mt.pid";
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


char mdname[]="unimt";
char logpath[128];
char version[]="1.00";

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
	memset(response,0,sizeof(response));
	pp=pak;
	*(long *)pp=htonl(61);
	*(long *)(pp+4)=htonl(0x00000001);
	*(long *)(pp+8)=htonl(nodeId);
	nownow[0]=0;
	tstring(nownow);
	*(long *)(pp+12)=htonl(atoll(nownow));
	*(long *)(pp+16)=htonl(seq);
	*(pp+20)=(unsigned char)1;
	strcpy(pp+21,username);
	//strcpy(pp+37,gshare_key);
	strcpy(pp+37,password);
	//proclog( "login... ip:[%s:%d] name[%s] passwd[%s]",gateip,port,username,password);
	
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
	if((i=recv(sp->sd,response,29,MSG_WAITALL))==-1) 
	{
		proclog( "failed to recv login response!");
		exit(0);
	}
	//cmd=ntohl(*((unsigned long *)(response+4)));
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
	*(long *)pp=htonl(20);
	*(long *)(pp+4)=htonl(0x00000002);
	*(long *)(pp+8)=htonl(atoi(nodeid));
	//*(long *)(pp+12)=htonl(825140000);
	nownow[0]=0;
	tstring(nownow);
	*(long *)(pp+12)=htonl(atol(nownow));
	*(long *)(pp+16)=htonl(seq);//seq

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
unsigned long s4,
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

	unsigned long pkg_len=0;
	int n=0;
	int cmd=0x00000003;

	memset(buffer,0,sizeof(buffer));
	pp=buffer;
	*(long *)(pp+4)=htonl(cmd);
	*(long *)(pp+8)=htonl(nodeId);
	nownow[0]=0;
	tstring(nownow);
	*(long *)(pp+12)=htonl(atol(nownow));
	*(long *)(pp+16)=htonl(p_submit_pkg->seq);
	strcpy(pp+20,p_submit_pkg->SPNumber);
	strcpy(pp+41,p_submit_pkg->ChargeNumber);
	*(pp+62)=(unsigned char)1;//
	strcpy(pp+63,p_submit_pkg->UserNumber);
	strcpy(pp+84,p_submit_pkg->CorpId);
	strcpy(pp+89,p_submit_pkg->ServiceType);
	*(pp+99)=p_submit_pkg->FeeType;
	strcpy(pp+100,p_submit_pkg->FeeValue);
	strcpy(pp+106,"0"); //
	*(pp+112)=(unsigned char)0;
	*(pp+113)=(unsigned char)3;//0;
	*(pp+114)=(unsigned char)8;//
	*(pp+147)=(unsigned char)1;//1;
	*(pp+150)=(unsigned char)15;//
	*(long *)(pp+152)=htonl(p_submit_pkg->MessageLength);
	pkg_len=p_submit_pkg->MessageLength+164;
	
	*(long *)pp=htonl(pkg_len);
	strcpy(pp+156,p_submit_pkg->MessageContent);
	strcpy(pp+156+p_submit_pkg->MessageLength,p_submit_pkg->linkid);
	
	proclog("seq[%u]ChargeNumber[%s]CorpId[%s]nodeid[%u]FeeType[%d]FeeValue[%s]MessageContent[]MessageLength[%d]ServiceType[%s]SPNumber[%s]UserNumber[%s]linkid[%s]bind[%d]",
			p_submit_pkg->seq,
			p_submit_pkg->ChargeNumber,
			p_submit_pkg->CorpId,
			nodeId,
			p_submit_pkg->FeeType,
			p_submit_pkg->FeeValue,
		//	p_submit_pkg->MessageContent,
			p_submit_pkg->MessageLength,
			p_submit_pkg->ServiceType,
			p_submit_pkg->SPNumber,
			p_submit_pkg->UserNumber,
			p_submit_pkg->linkid,
			bind_flag);

	proclog_HEX(buffer,pkg_len);
	if(writeall(sp->sd,buffer,pkg_len)==-1)
	{
		proclog("submit failed! %s",strerror(errno));
		exit(0);
	}

	return;
}
static read_response(int seq)
{
	char buffer[256];
	memset(buffer,9,sizeof(buffer));
	int n=-2;
	n=read(sp->sd,buffer,sizeof(buffer)-1);
	//proclog("resp:%d bytes,cmd:0x%X,result:%d",n,ntohl(*(int*)(buffer+4)),buffer[20]);
	proclog("rep_len:%d,result:%d",n,buffer[20]);

	char sql[512];
	sprintf(sql,"update wraith_message set gw_resp='%d',gw_resp_time=NOW() where id='%d'",buffer[20],seq);
	proclog(sql);
	mysql_exec(&mysql, sql);
}
static int new_data()
{
	char sql[256];
	sprintf(sql,"select * from wraith_message where ID > %d and mo_status='ok' and gwid=%s limit 500",(off_t)psoff, gwid);
	proclog(sql);
	mysql_exec(&mysql,sql);
	result=mysql_store_result(&mysql);
	return mysql_num_rows(result);
}
static void my_nano_sleep(unsigned long nsec)
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
		if(row[0])
			submit_pkg.seq=atoi(row[0]);
		if(row[4])
			strcpy(submit_pkg.SPNumber, row[4]);
		if(row[2])
			strcpy(submit_pkg.ChargeNumber, row[2]);
		if(row[2])
			strcpy(submit_pkg.UserNumber, row[2]);
		if(row[11])
			strcpy(submit_pkg.ServiceType, row[11]);
		if(row[8])
			strcpy(submit_pkg.FeeValue, row[8]);
		if(row[10])
			strcpy(submit_pkg.MessageContent, row[10]);
		if(row[5])
			strcpy(submit_pkg.linkid, row[5]);
		if(row[9])
			submit_pkg.FeeType=atoi(row[9]);
		submit_pkg.MessageLength=strlen(submit_pkg.MessageContent);
		strcpy(submit_pkg.CorpId,corpId);


		sgip_submit(&submit_pkg,atoll(nodeId));
		read_response(submit_pkg.seq);
		//printf("row0:%s\n",row[0]);
		if(row[0])
			psoff=atoi(row[0]);

		//my_nano_sleep(300000000);
		sleep(1);
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

