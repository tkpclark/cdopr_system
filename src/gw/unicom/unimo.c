
#include "lib.h"



static char heapmogw[128];
static int readfd;
static int writefd;

char mdname[]="unimo";
char logpath[128];
char version[]="1.09";

static char ip[32];
static char port[8];
static char db[32];
static char user[32];
static char pass[32];
static char gwid[4];

static MYSQL mysql;

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
		if(!strcmp(iter->key,"ip"))
			strcpy(ip,iter->value);
		else if(!strcmp(iter->key,"db"))
			strcpy(db,iter->value);
		else if(!strcmp(iter->key,"user"))
			strcpy(user,iter->value);
		else if(!strcmp(iter->key,"pass"))
			strcpy(pass,iter->value);
		else if(!strcmp(iter->key,"gwid"))
			strcpy(gwid,iter->value);
		else if(!strcmp(iter->key,"logpath"))
			strcpy(logpath,iter->value);
	}
	ccl_release(&config);

	//proclog("[%s][%s][%s][%s][%s]",ip,db,user,pass,gwid);
}

static void ucs2utf8(char *in,char *out)
{
             char *putfout;
             char *pout;
             size_t ll1;
             size_t ll2;
             iconv_t cd;
             ll1=1000;
             ll2=1000;
             putfout=out;
             pout=in;
             cd=iconv_open("UTF-8","ucs-2be");
             if(cd==(iconv_t)-1)
                {
                   exit(0);
                }
             iconv(cd,&pout,&ll1,&putfout,&ll2);
             iconv_close(cd);
}
static void gbk2utf8(char *in,char *out)
{
             char *putfout;
             char *pout;
             size_t ll1;
             size_t ll2;
             iconv_t cd;
             ll1=1000;
             ll2=1000;
             putfout=out;
             pout=in;
             cd=iconv_open("UTF-8","GBK");
             if(cd==(iconv_t)-1)
                {
                   exit(0);
                }
             iconv(cd,&pout,&ll1,&putfout,&ll2);
             iconv_close(cd);
}
static void procquit(void)
{
	close(readfd);
	close(writefd);
	//proclog( "quiting!\n");
}

static void sgip_resp(int cmd,void *seq,int len)
{
	unsigned char buf[32];
	unsigned char *p;
	memset(buf,0,sizeof(buf));
	p=buf;
	*(int *)p=htonl(len);
	*(int *)(p+4)=htonl(cmd);
	*(int *)(p+8)=htonl(*(int*)seq);
	*(int *)(p+12)=htonl(*(int*)(seq+4));
	*(int *)(p+16)=htonl(*(int*)(seq+8));
	//if(writeall(writefd,buf,len)==-1)
	if(write(writefd,buf,len)==-1)
	{
		proclog("cmd %X write error!",cmd); 
		exit(0);
	}
}

static sgip_read()
{
	//proclog("reading data...");
	
	int n=0;
	void *seq;
	int len,g;
	int cmd;
	int i;
	unsigned char buffer[PKG_LENGTH];
	//read header
	memset(buffer,0,20);
	//proclog("reading header...");

	n=read(readfd,buffer,20);
	if(n==0)
		exit(0);
	if(n!=20)
	{
		proclog("Read Header Error! return [%d]",n);
		exit(0);
	}
	//proclog_HEX(buffer,20);
	seq=(void*)malloc(12);
	len=ntohl(*((int *)buffer));
	cmd=ntohl(*((unsigned int *)(buffer+4)));
	
	*(int*)seq=ntohl(*((unsigned int *)(buffer+8)));
	*(int*)(seq+4)=ntohl(*((unsigned int *)(buffer+12)));
	*(int*)(seq+8)=ntohl(*((unsigned int *)(buffer+16)));
	
	proclog("HEADER:len[%d]cmd[0x%X]seq[%d][%d][%d]",len,cmd,*(int*)seq,*(int*)(seq+4),*(int*)(seq+8));


	///read body
	memset(buffer,0,PKG_LENGTH);
	//proclog("reading body...");
	if((n=read(readfd,buffer,len-20))!=(len-20))
	{
		proclog("Read Body Error! return [%d]",n);
		exit(0);
	}


	///////////////print binlog
	//proclog_HEX(buffer,len-20);
	////////////////////
	
	if(cmd==0x4)//deliver
	{
		sgip_resp(0x80000004,seq,29);
		char UserNumber[22]={0};
		char UserNumber_db[32];
		char SPNumber[22]={0};
		char MessageContent[256]={0};
		int MessageLength=0;
		char MessageContent_utf8[256]={0};
		memset(MessageContent_utf8,0,sizeof(MessageContent_utf8));
		unsigned char MessageCoding;
		unsigned char pid,udhi;
		char linkid[32];
		
		MessageLength=ntohl(*(unsigned int*)(buffer+45));
		strncpy(UserNumber,buffer,21);
		//proclog("UserNumber[%s]",UserNumber);
		strncpy(SPNumber,buffer+21,21);
		//proclog("SPNumber[%s]",SPNumber);
		pid=*(unsigned char *)(buffer+42);
		udhi=*(unsigned char *)(buffer+43);
		MessageCoding=*(unsigned char *)(buffer+44);
		memset(MessageContent,0,sizeof(MessageContent));
		memcpy(MessageContent,buffer+49,MessageLength);
		strcpy(linkid,buffer+49+MessageLength);
		//proclog("linkid[%s]",linkid);


		//convert to utf-8
		memset(MessageContent_utf8,0,sizeof(MessageContent_utf8));
		if (MessageCoding==8)
		{
			//convt(MessageContent,MessageContent_utf8,"ucs-2be","utf-8");
			ucs2_to_utf8(MessageContent,MessageContent_utf8);
		}
		else if(MessageCoding==15)
		{
			//convt(MessageContent,MessageContent_utf8,"gb2312","utf-8");
			gbk2utf8(MessageContent,MessageContent_utf8);
		}
		else
		{
			strcpy(MessageContent_utf8,MessageContent);
		}





/*
		//################# write to db#################
		memset(UserNumber_db,0,sizeof(UserNumber_db));
		if(strncmp(UserNumber,"86",2)==0)
			strcpy(UserNumber_db,UserNumber+2);
		else
			strcpy(UserNumber_db,UserNumber);
			*/
		proclog("MO:UserNumber[%s]SPNumber[%s]Messagelen[%d]Cont[%s]MessageCoding[%d]linkid[%s]pid[%d]udhi[%d]",UserNumber,SPNumber,MessageLength,MessageContent_utf8,MessageCoding,linkid,pid,udhi);

		char sql[512];
		sprintf(sql,"insert into wraith_message( motime, phone_number, mo_message, sp_number, linkid, gwid ) values (NOW(),'%s', '%s', '%s', '%s', '%s');",
				//UserNumber_db,
				UserNumber+2,
				MessageContent_utf8,
				SPNumber,
				linkid,
				gwid
				);
		proclog(sql);
		mysql_exec(&mysql,"set names utf8");
		mysql_exec(&mysql, sql);

		/*
		char cmd[128];
		sprintf(cmd,"./fakemt %s hello",UserNumber);
		proclog("%s\n",cmd);
		//system(cmd);
		*/
	}
	else if(cmd==0x5)//report
	{
		sgip_resp(0x80000005,seq,29);
		*(time_t*)(buffer+252)=time(0);
		char UserNumber[22]={0};
		strncpy(UserNumber,buffer+13,21);
		unsigned int seq;
		seq=ntohl(*((unsigned int *)(buffer+8)));
		unsigned char state,report_code;
		//state=*(int *)(buffer+34);
		state=*(unsigned char *)(buffer+34);
		report_code=*(unsigned char *)(buffer+35);
		proclog("REPORT: seq[%d]usernumber[%s]state[%d]errorcode[%d]",seq,UserNumber,state,report_code);
//		write_to_heapfile(heapstatdbfd,buffer,sizeof(buffer));
		char sql[512];
		//int report=report_code==0?1:2;
		int report=0;
		if(report_code==0||report_code==100)
		{
			report=1;
		}
		else
		{
			report=2;
		}
		sprintf(sql,"update wraith_message set report='%d', report_orig='%d', report_time=NOW() where id='%d'",report,report_code,seq);
		//proclog(sql);
		mysql_exec(&mysql, sql);

	}
	else if(cmd==0x1)//bind
	{
		sgip_resp(0x80000001,seq,29);
		//proclog("MESSAGE:got BIND command\n");
	}
	else if(cmd==0x2)//unbind
	{
		sgip_resp(0x80000002,seq,20);
		//proclog("MESSAGE:got UNBIND command!\n");
		exit(0);
	}
	else
	{
		proclog( "WARNING:Strange CMD:%x\n",cmd);
	}
	//proclog("returning...");
}

main(int argc ,char **argv)
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

	int n=0;
	int i=0;
	fd_set fds1;
	struct timeval tv;

	readfd=open("/dev/null",0);
	writefd=open("/dev/null",0);
	dup2(0,readfd);
	dup2(1,writefd);
	close(0);
	close(1);

	if(atexit(&procquit))
	{
		printf("quit code can't be load!\n");
		exit(0);
	}

	mysql_create_connect(&mysql, ip, user,pass,db);

	while(9)
	{
		FD_ZERO(&fds1);
		FD_SET(readfd,&fds1);
		tv.tv_sec = 20;
		tv.tv_usec = 0;
		//proclog("MESSAGE:waiting for belle.....");
		if((n=select(readfd+1,&fds1,NULL,NULL,NULL))>0)
		{
			sgip_read();
		}
		else if(n<0)
		{
			if(errno==EINTR)
				continue;
			proclog( "ALERT:fuck select error\n");
				continue;
		}
		else//return 0
		{
			proclog("MESSAGE:long time no mo!\n");
			exit(0);
		}
	}
}

