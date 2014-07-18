#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
//#include <errmsg.h>
#include <stdarg.h>
#include <iconv.h>
#include <unistd.h>

extern char mdname[];
extern char logpath[];
extern char version[];


void proclog(const char *fmt,...)
{
	char ts[32];
	char buf[1024]; //logout
	char tmp[1024];//came in
	time_t tt;
	struct timeval tv;



	memset(tmp,0,sizeof(tmp));
	va_list vs;
	va_start(vs,fmt);
	vsprintf(tmp,fmt,vs);
	va_end(vs);

	//get log time
	tt=time(0);
	memset(buf,0,sizeof(buf));
	strftime(ts,30,"%F %X",(const struct tm *)localtime(&tt));

	char ts_nano[64];
	gettimeofday (&tv, NULL);
	sprintf(ts_nano,"%s.%06d",ts, tv.tv_usec);


	int fd;

	//log content
	sprintf(buf,"[%s][%s][%s]: %s\n",ts_nano,mdname,version,tmp);
	//printf("%s",buf);



	// get filename
	char filename[128];
	strftime(ts,30,"%Y%m%d",(const struct tm *)localtime(&tt));
	sprintf(filename,"%s/%s.log", logpath,ts);

	fd=open(filename, O_CREAT|O_RDWR|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
	if(fd <0)
	{
		printf("open %s failed!%s\n",filename,strerror(errno));
		return;
	}
	//T_DES(1,key,des_len,buf,buf);
	flock(fd,LOCK_EX);
	//write(fd,buf,sizeof(buf));
	write(fd,buf,strlen(buf));
	flock(fd,LOCK_UN);
	close(fd);

}
proclog_HEX(char *buffer, int len)
{
	int i;
	char logbuf[1024];
	memset(logbuf,0,sizeof(logbuf));
	for(i=0;i<len;i++)
	{
		sprintf(logbuf+3*i,"%02X ",*(unsigned char *)(buffer+i));
	}
	proclog(logbuf);
}
int convt(char *in,char *out,char *in_code,char *out_code)
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
	        cd=iconv_open(out_code,in_code);
	        if(cd==(iconv_t)-1)
	        {
	                printf("!!! can NOT cd!!");
	                exit(0);
	        }
	        iconv(cd,&pout,&ll1,&putfout,&ll2);
	        iconv_close(cd);
}
void ucs2_to_utf8(char *in,char *out)
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
        cd=iconv_open("utf-8","ucs-2be");
        if(cd==(iconv_t)-1)
        {
                printf("!!! can NOT cd!!");
                exit(0);
        }
        iconv(cd,&pout,&ll1,&putfout,&ll2);
        iconv_close(cd);
}
myflock(int lockfd,char type)
{
		if(type==1)
		{
	        if(flock(lockfd,LOCK_EX))
	        {
	        	proclog("lock error!\n");
	            exit(0);
	        }
	        //proclog(logfd,"lock");
	    }
	    else if(type==2)
	    {
	    	if(flock(lockfd,LOCK_UN))
	        {
	        	proclog("unlock error!\n");
	            exit(0);
	        }
	        //proclog(logfd,"unlock");
	    }
	    else;
}
char *init_mmap(char *pathname,unsigned int msize)
{
	int fd;
	struct stat statbuf;
	char *p_map=NULL;
	fd=open(pathname,O_RDWR|O_CREAT,0600);
	if(fd<0)
	{
		proclog("ERROR:open %s error!",pathname);
		return NULL;
	}
	if (fstat(fd, &statbuf) < 0)
	{
		proclog("ERROR:fstat %s error\n",pathname);
		return NULL;
	}
	if(statbuf.st_size!=msize)
	{
		void *p=NULL;
		p=malloc(msize);
		memset(p,0,msize);
		write(fd,p,msize);
	}
	if ((p_map = mmap(0, msize, PROT_READ|PROT_WRITE, MAP_SHARED,fd, 0)) == MAP_FAILED)
	{
		proclog("ERROR:%s mmap error!\n",pathname);
		return NULL;
	}
	if (p_map==(char*)-1)
		p_map=0;
	close(fd);
//	proclog("mmap size:%d",msize);
//	proclog(logfd,logbuf);
	//syslog(LOG_INFO,"%d",statbuf.st_size);
	return p_map;
}
char* init_mmap_read(char *pathname)
{
	int fd;
	char *map=NULL;
	struct stat statbuf;
	fd=open(pathname,0);
	if(fd<0)
	{
		proclog("ERROR:open %s error!",pathname);
		return NULL;
	}
	if (fstat(fd, &statbuf) < 0)
	{
		proclog("ERROR:fstat %s error!",pathname);
		return NULL;
	}
	if ((map = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED,fd, 0)) == MAP_FAILED)
	{
		proclog("ERROR:map %s error!",pathname);
		return NULL;
	}
	if (map==(char*)-1)
		map=0;
	close(fd);
	//syslog(LOG_INFO,"%d",statbuf.st_size);
	return map;
}
off_t get_file_size(int fd)
{
	struct stat statbuf;
	if(!fstat(fd,&statbuf))
	{
		return statbuf.st_size;
	}
	else
	{
		printf("ALERT:get file stat error! %s\n",strerror(errno));
		return 0;
	}

}


int getfilepid(char *pidfile)
{
	int fd;
	char buf[8]={0};
	fd=open(pidfile,0);
	if(fd<0)
	{
		return 999999999;
	}
	memset(buf,0,sizeof(buf));
	read(fd,buf,sizeof(buf)-1);
	close(fd);
	return atoi(buf);
}
int getdata(char* file,char* buffer,int size)
{
	int len;
	int fd;
	fd = open(file, 0);
	if(fd<0)
		return -1;
	len = read(fd, buffer, size);
	close(fd);
	buffer[len] = '\0';
//      printf("%s\n",buffer);
}
strrep(char *str,const char *src,const char *des)
{
	char *p=NULL;
	char tmp[4096];
	while(1)
	{
		p=strstr(str,src);
		if(!p)
			break;
		strcpy(tmp,p+strlen(src));
		strcpy(p,des);
		strcat(str,tmp);
	}
}
write_pid(char *pidfile)
{
	int fd;
	char tmp[10]={0};
	fd=open(pidfile,O_CREAT|O_EXCL|O_WRONLY,S_IRWXU);
	if(fd<0)
	{
		printf("program is running!\n");
		exit(0);
	}
	ftruncate(fd,0);
	lseek(fd,0,SEEK_SET);
	sprintf(tmp,"%d",getpid());
	write(fd,tmp,strlen(tmp));
	close(fd);
}
write_to_heapfile(int fd,const char *buffer,int len)
{
	int n;
	flock(fd,LOCK_EX);

lp:
	if((n=write(fd,buffer,len))!=len)
		if((n==-1)&&(errno==EINTR))
		{
			goto lp;
		}
		else
		{
			proclog("ALERT:wrote heapfile error!!!! wrote:[%d]",n);
			exit(0);
		}
	flock(fd,LOCK_UN);

}
void tstring(char *buffer)
{
   time_t curtime;
   struct tm *loctime;

   /* Get the current time. */
   curtime = time (NULL);

   /* Convert it to local time representation. */
   loctime = localtime (&curtime);

   /* Print out the date and time in the standard format. */
   //fputs (asctime (loctime), stdout);

   /* Print it out in a nice format. */
   //strftime (buffer, SIZE, "Today is  %m %d.\n", loctime);
   //strftime (buffer, SIZE, "Today is %A, %B %d.\n", loctime);
   //fputs (buffer, stdout);
   //strftime (buffer, SIZE, "The time is %I:%M %p.\n", loctime);
   //strftime (buffer, SIZE, "The time is %H:%M:%S .\n", loctime);
   //fputs (buffer, stdout);
   strftime (buffer, 256, "%m%d%H%M%S", loctime);
   //fputs (buffer+1, stdout);
}

int is_file_exist(char *filename)
{
	int fd;
	fd=open(filename,0);
	if(fd<=0)
	{
		return 0;
	}
	else
	{
		close(fd);
		return 1;
	}
}
void to_gb(char *in,char *out)
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
	cd=iconv_open("gb2312","utf-8");
	if(cd==(iconv_t)-1)
	{
		exit(0);
	}
	iconv(cd,&pout,&ll1,&putfout,&ll2);
	iconv_close(cd);
}

