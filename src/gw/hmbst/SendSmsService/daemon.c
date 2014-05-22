#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ccl/ccl.h>
#include <sys/stat.h>
#include <mysql.h>

char mdname[]="daemon";
char logpath[256];
char version[]="1.00";
static int rcdlen;
static char chdname[64];
static unsigned int mmapsize;
static char mmapfile[128];


static char dbip[16];
static char dbuser[16];
static char dbpass[32];
static char dbname[32];
static char gwid[4];

static MYSQL mysql;


static int gstop=0;
static int chdnum;

static char *p_map;

static int *ppid;

static const char *pidfile="pgid_hm.pid";

char *init_mmap(char *pathname,unsigned int msize);

static create_children(char **argv)
{
	int i;
	int pid;
		for(i=0;i<chdnum;i++)
		{
			if(*(ppid+i))
				continue;
			pid=fork();
			if(pid<0)
			{
				proclog("ERROR:fork error!");
				exit(0);
			}
			else if(!pid)//child
			{
				if(execv(chdname,argv)==-1)
					proclog("ERROR:exec error!");
				exit(0);
			}
			else
			{
				*(ppid+i)=pid;
				proclog("MESSAGE:created child [%d]",pid);
			}
		}
}
static int check_children_state()//return the number of dead children
{
	int i;
	int n;
	int num=0;
	for(i=0;i<chdnum;i++)
	{
		n=waitpid(*(ppid+i),NULL,WNOHANG);
		if(n>0)
		{
			//sprintf(logbuf,"%d quited ",*(ppid+i));
			//proclog(logbuf);
			*(ppid+i)=0;
			num++;
		}
		else if(n==-1)
		{
			proclog("ERROR:waitpid error!");
			exit(0);
		}
		else ;
	}
	return num;
}
static void acquit(int signo)
{
	gstop=1;
	//proclog("got quiting signal");
}
static void wakeup(int signo)
{
	;//proclog("get data from heapfile");
}
static void procquit(void)
{
 	unlink(pidfile);
  	proclog("MESSAGE:group quited!");
}
static kill_children(int sig)
{
	int i;
	for(i=0;i<chdnum;i++)
	{
		if(*(ppid+i))
			kill(*(ppid+i),sig);
	}
}
static wait_to_quit()
{
	int i;
	proclog("MESSAGE:waiting all children to quit");
	kill_children(SIGINT);
	while(1)
	{
		sleep(1);
		if(check_children_state()==chdnum)
		{		
			exit(0);
		}
		else
		{
			proclog("MESSAGE:still waiting");
			sleep(1);
		}
	}
}

static int fetch_data()
{
	int n;
	char *tmp;
	MYSQL_RES *result;
  MYSQL_ROW row;
  int gotnum=0;
	
	tmp=malloc(rcdlen);
	memset(p_map+4,0,mmapsize-4);
	//lseek(heapfd,rcdlen*( (off_t)(*(unsigned int*)p_map) ),SEEK_SET);
	//	sprintf(logbuf,"fetch from %d \n",rcdlen*( (off_t)(*(unsigned int*)p_map) ) );
	//	proclog(logbuf);
	char sql[256];

	sprintf(sql,"select * from wraith_mt where ID > %d and gwid=%s limit 500",(off_t)(*(unsigned int*)p_map), gwid);
	proclog(sql);
	mysql_query(&mysql,"set names utf8");
	mysql_query(&mysql,sql);
	result=mysql_store_result(&mysql);
	gotnum=mysql_num_rows(result);
//	sprintf(logbuf,"got [%d] rows",gotnum);
//	proclog(logbuf);
	
	int i=0;
	while(row=mysql_fetch_row(result))
	{
		memset(tmp,0,rcdlen);

		if(row[0]!=NULL)//seqid
			*(int*)(tmp+350)=atol(row[0]);
		
		if(row[4]!=NULL)//
			strcpy(tmp+260,row[4]);
		
		if(row[2]!=NULL)//senderName
			strcpy(tmp+40,row[2]);
		
		if(row[3]!=NULL)//message
			strcpy(tmp+60,row[3]);
			
		if(row[6]!=NULL)//productID
			strcpy(tmp,row[6]);
			
		if(row[5]!=NULL)//linkID
			strcpy(tmp+320,row[5]);



		memcpy(p_map+512+rcdlen*i,tmp,rcdlen);
		++i;
	}
	mysql_free_result(result);
	free(tmp);
	*(unsigned int*)(p_map+sizeof(unsigned int))=0;
	*(unsigned int*)(p_map+2*sizeof(unsigned int))=gotnum;
	return gotnum;

}
static void read_config()
{
	struct ccl_t config;
	const struct ccl_pair_t *iter;
	config.comment_char = '#';
	config.sep_char = '=';
	config.str_char = '"';
	ccl_parse(&config, "../conf/hmgw.ccl");
	while((iter = ccl_iterate(&config)) != 0)
	{

		if(!strcmp(iter->key,"chdnum"))
			chdnum=atoi(iter->value);
		else if(!strcmp(iter->key,"mmapfile"))
			strcpy(mmapfile,iter->value);
		else if(!strcmp(iter->key,"logpath"))
			strcpy(logpath,iter->value);
		else if(!strcmp(iter->key,"chdname"))
			strcpy(chdname,iter->value);
		else if(!strcmp(iter->key,"mmapsize"))
			mmapsize=atoi(iter->value);
		else if(!strcmp(iter->key,"rcdlen"))
			rcdlen=atoi(iter->value);
			
		else if(!strcmp(iter->key,"ip"))
			strcpy(dbip,iter->value);
		else if(!strcmp(iter->key,"user"))
			strcpy(dbuser,iter->value);
		else if(!strcmp(iter->key,"pass"))
			strcpy(dbpass,iter->value);
		else if(!strcmp(iter->key,"db"))
			strcpy(dbname,iter->value);
		else if(!strcmp(iter->key,"gwid"))
			strcpy(gwid,iter->value);
	}
	ccl_release(&config);
}
static daemain(int argc,char **argv)
{
	int i;
	struct sigaction signew;
	
	if(atexit(&procquit))
	{
	   printf("quit code can't install!");
	   exit(0);
	}
	read_config();
	//printf("\nmdname=%s\nmmapfile=%s\nchdname=%s\nchdnum=%d\nmmapsize=%d\nrcdlen=%d\n",mdname,mmapfile,chdnum,mmapsize,rcdlen);


	proclog("MESSAGE:starting....");
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
  	sigaction(SIGINT,&signew,0);
	
	signew.sa_handler=acquit;
	sigemptyset(&signew.sa_mask);
	signew.sa_flags=0;
	sigaction(SIGTERM,&signew,0);
	
	signew.sa_handler=wakeup;
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
		exit(0);
	memset(p_map+4,0,mmapsize-4);
	ppid=malloc(chdnum*sizeof(int));
	
	for(i=0;i<chdnum;i++)
	{
		*(ppid+i)=0;
	}
	create_children(argv);
	proclog("MESSAGE:all children created!");
	sleep(1);//very important..wait children created
	while(11)
	{
		if(gstop)
		{
			wait_to_quit();
		}
		if (!*(unsigned int*)(p_map+2*sizeof(unsigned int)))//left
		{
			i=fetch_data();
			if(i==-1)
				wait_to_quit();
			else if(i)
				kill_children(SIGUSR1);
			else;
		}
		if(check_children_state())
			create_children(argv);
		sleep(1);//quit/fetch data/create child/
		//proclog("sleeping");
	}
}

main(int argc,char **argv)
{
	int pid;
	int fd;
	fd=open(pidfile,O_CREAT|O_EXCL|O_WRONLY,0600);
	if(fd<0)
	{
		printf("program is running!\n");
		exit(0);
	}
	pid=fork();
	if(pid>0)
	{
		char tmp[8]={0};
		sprintf(tmp,"%d",pid);
		write(fd,tmp,strlen(tmp));
		close(fd);
		exit(0);
	}
	else if(pid<0)
	{
		printf("fork error!\n");
		exit(0);
	}
	else//child
	{
		setpgid(0,0);
		daemain(argc,argv);
	}
}
