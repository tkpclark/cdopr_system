#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <iconv.h>
#include <unistd.h>

char *init_mmap(char *pathname,unsigned int msize)
{
	int fd;
	struct stat statbuf;
	char *p_map=NULL;
	fd=open(pathname,O_RDWR|O_CREAT,0600);
	if(fd<0)
	{
		printf("ERROR:open %s error!",pathname);
		return NULL;
	}
	if (fstat(fd, &statbuf) < 0)
	{
		printf("ERROR:fstat %s error\n",pathname);
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
		printf("ERROR:%s mmap error!\n",pathname);
		return NULL;
	}
	if (p_map==(char*)-1)
		p_map=0;
	close(fd);
//	printf("mmap size:%d",msize);
//	printf(logfd,logbuf);
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
		printf("ERROR:open %s error!",pathname);
		return NULL;
	}
	if (fstat(fd, &statbuf) < 0)
	{
		printf("ERROR:fstat %s error!",pathname);
		return NULL;
	}
	if ((map = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED,fd, 0)) == MAP_FAILED)
	{
		printf("ERROR:map %s error!",pathname);
		return NULL;
	}
	if (map==(char*)-1)
		map=0;
	close(fd);
	//syslog(LOG_INFO,"%d",statbuf.st_size);
	return map;
}
main(int argc,char **argv)
{
	char *p_map=NULL;
	if(argc==2)
	{
		p_map=(char*)init_mmap_read(argv[1]);
	}
	if(argc==3)
	{
		p_map=(char*)init_mmap(argv[1],10240000);
		*(int*)(p_map)=atoi(argv[2]);
	}
	printf("[%d][%d][%d]\n",*(int*)(p_map),*(int*)(p_map+sizeof(int)),*(int*)(p_map+2*sizeof(int)));
}

