
#include "mysqllib.h"

void sql_err_log(MYSQL *mysql)
{
	//proclog(logfd,mdname,mysql_error(mysql));
	proclog(mysql_error(mysql));
	mysql_close(mysql);
}
void mysql_create_connect(MYSQL *mysql,char *ip,char *usr,char *pwd,char *dbname)
{
	mysql_init(mysql);
	if(!mysql_real_connect(mysql,ip,usr,pwd,dbname,0,NULL,0))
	{
		sql_err_log(mysql);
		exit(0);
	}
}
void mysql_exec(MYSQL *mysql,char *buf)
{
	unsigned int myerr;
	int i;
	i=1;
	//proclog("%s",buf);
	MYRE:
	if (mysql_query(mysql,buf))
	{
		myerr=mysql_errno(mysql);
		if(myerr==CR_SERVER_GONE_ERROR || myerr==CR_SERVER_LOST)
		{	/*
			sprintf(logbuf,"redo SQL %d time",i);
			proclog(logfd,mdname,logbuf);
			*/
			proclog("redo SQL %d time",i);
			i++;
			if(i>3)
				exit(0);
			goto MYRE;
		}
		else
		{	/*
			sprintf(logbuf,"error sql:%s",buf);
			proclog(logfd,mdname,logbuf);
			*/
			proclog("error sql:%s",buf);
			sql_err_log(mysql);
			//exit(0);
		}
	}
	else
	{
		return ;
	}
}
int mysql_get_data(MYSQL *mysql,char *sql,char p[][DATA_MAX_FIELD_NUM][DATA_MAX_REC_LEN])
{
	int row_num=0,field_num=0;
	char snull[]="NULL";
	MYSQL_ROW row;
	MYSQL_RES *result;
	
	mysql_exec(mysql,"set names gb2312");

	//proclog("%s",sql);
	mysql_exec(mysql,sql);

	result=mysql_store_result(mysql);
	row_num=mysql_num_rows(result);
	field_num=mysql_num_fields(result);
	//proclog("row_num:%d,field_num:%d\n",row_num,field_num);
	if(!row_num)
	{
		mysql_free_result(result);
		return 0;
	}


	int i=0,j=0;
	while(row=mysql_fetch_row(result))
	{
		//proclog("[%d]  ",i+1);
		for(j=0;j<field_num;j++)
		{
			if(row[j]!=NULL)
			{
				if(strlen(row[j]) >= DATA_MAX_REC_LEN)
				{
					//proclog("TOO LONG");
				}
				else
				{
			 		strcpy(p[i][j],row[j]);
					//proclog("%-20s",p[i][j]);
				}
			}
			else
			{
				p[i][j][0]=0;
				//proclog("%-20s",snull);
			}
		}
		i++;
		//proclog("\n");
	}
	mysql_free_result(result);
	//proclog("\n");

	return row_num;
}



int mysql_get_data_long(MYSQL *mysql,char *sql,char p[][DATA_MAX_FIELD_NUM][DATA_MAX_REC_LONG])
{
	int row_num=0,field_num=0;
	char snull[]="NULL";
	MYSQL_ROW row;
	MYSQL_RES *result;
	
	mysql_exec(mysql,"set names gb2312");

	//proclog("%s\n",sql);
	mysql_exec(mysql,sql);

	result=mysql_store_result(mysql);
	row_num=mysql_num_rows(result);
	field_num=mysql_num_fields(result);
	proclog("row_num:%d,field_num:%d\n",row_num,field_num);
	if(!row_num)
	{
		mysql_free_result(result);
		return 0;
	}


	int i=0,j=0;
	while(row=mysql_fetch_row(result))
	{
		//proclog("[%d]  ",i+1);
		for(j=0;j<field_num;j++)
		{
			if(row[j]!=NULL)
			{
				if(strlen(row[j]) >= DATA_MAX_REC_LONG)
				{
					//proclog("TOO LONG");
				}
				else
				{
			 		strcpy(p[i][j],row[j]);
			 		//printf("%-20s",p[i][j]);
				}
			}
			else
			{
				p[i][j][0]=0;
				//proclog("%-20s",snull);
			}
		}
		i++;
		//proclog("\n");
	}
	mysql_free_result(result);
	//proclog("\n");

	return row_num;
}

