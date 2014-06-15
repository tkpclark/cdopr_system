
#include "lib.h"



static char heapmogw[128];
static int readfd;
static int writefd;

char mdname[]="unimo";
char logpath[] = "../../../logs/unicomgw/";
char version[]="1.00";

static char ip[32];
static char port[8];
static char db[32];
static char user[32];
static char pass[32];
static char gwid[4];

static MYSQL mysql;

static void read_config()
{
	struct ccl_t config;
	const struct ccl_pair_t *iter;
	config.comment_char = '#';
	config.sep_char = '=';
	config.str_char = '"';
	ccl_parse(&config, "app.config");
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

	}
	ccl_release(&config);

	//proclog("[%s][%s][%s][%s][%s]",ip,db,user,pass,gwid);
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
	*(long *)p=htonl(len);
	*(long *)(p+4)=htonl(cmd);
	*(long *)(p+8)=htonl(*(int*)seq);
	*(long *)(p+12)=htonl(*(int*)(seq+4));
	*(long *)(p+16)=htonl(*(int*)(seq+8));
	//if(writeall(writefd,buf,len)==-1)
	if(write(writefd,buf,len)==-1)
	{
		proclog("cmd %X write error!",cmd); 
		exit(0);
	}
}


sgip_init()
{
	//struct sigaction signew;

	read_config();

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
	if((n=read(readfd,buffer,20))!=20)
	{
		proclog("Read Header Error! return [%d]",n);
		exit(0);
	}
	//proclog_HEX(buffer,20);
	seq=(void*)malloc(12);
	len=ntohl(*((long *)buffer));
	cmd=ntohl(*((unsigned long *)(buffer+4)));
	
	*(int*)seq=ntohl(*((unsigned long *)(buffer+8)));
	*(int*)(seq+4)=ntohl(*((unsigned long *)(buffer+12)));
	*(int*)(seq+8)=ntohl(*((unsigned long *)(buffer+16)));
	
	proclog("header:len:[%d] CMD:[%X] seq:[%d][%d][%d]",len,cmd,*(int*)seq,*(int*)(seq+4),*(int*)(seq+8));


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
		strncpy(SPNumber,buffer+21,21);

		pid=*(unsigned char *)(buffer+42);
		udhi=*(unsigned char *)(buffer+43);
		MessageCoding=*(unsigned char *)(buffer+44);
		memset(MessageContent,0,sizeof(MessageContent));
		memcpy(MessageContent,buffer+49,MessageLength);
		strcpy(linkid,buffer+49+MessageLength);
		if (MessageCoding==8)
		{
			convt(MessageContent,MessageContent_utf8,"ucs-2be","utf-8");
			//ucs2_to_utf8(MessageContent,MessageContent_utf8);
		}
		else if(MessageCoding==15)
		{
			convt(MessageContent,MessageContent_utf8,"gb2312","utf-8");
		}
		else
		{
			strcpy(MessageContent_utf8,MessageContent);
		}
		proclog("MO:UserNumber[%s]SPNumber[%s]Messagelen[%d]Content[%s]MessageCoding[%d]linkid[%s]pid[%d]udhi[%d]\n",UserNumber,SPNumber,MessageLength,MessageContent_utf8,MessageCoding,linkid,pid,udhi);
		
		char sql[512];
		sprintf(sql,"insert into wraith_mo( in_date, phone_number, message, sp_number, linkid, gwid ) values (NOW(),'%s', '%s', '%s', '%s', '%s');",
				UserNumber,
				MessageContent_utf8,
				SPNumber,
				linkid,
				gwid
				);
		//proclog(sql);
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
		unsigned long seq;
		seq=ntohl(*((unsigned long *)(buffer+8)));
		unsigned char state,report_code;
		//state=*(int *)(buffer+34);
		state=*(unsigned char *)(buffer+34);
		report_code=*(unsigned char *)(buffer+35);
		proclog("REPORT: seq[%d]usernumber[%s]state[%d]errorcode:[%d]\n",seq,UserNumber,state,report_code);
//		write_to_heapfile(heapstatdbfd,buffer,sizeof(buffer));
		char sql[512];
		sprintf(sql,"update wraith_mt set report='%d', report_time=NOW() where id='%d'",report_code,seq);
		proclog(sql);
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

main()
{
	int n=0;
	int i=0;
	fd_set fds1;
	struct timeval tv;

	sgip_init();
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

