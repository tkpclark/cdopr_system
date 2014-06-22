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

	
char mdname[32]="sendsms_month";
char logpath[256];
char version[]="1.02";
static char logfile[128];
static MYSQL mysql;
static char serverurl[256];
static char spid[32];
static char key[32];
static char gwid[4];
static char dbip[16];
static char dbuser[16];
static char dbpass[32];
static char dbname[32];
static int rcdlen;
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
		if(!strcmp(iter->key,"logpath"))
			strcpy(logpath,iter->value);
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
		else if(!strcmp(iter->key,"rcdlen"))
			rcdlen=atoi(iter->value);
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
			//ns4_RequestSOAPHeader.linkId=p_data+320;
			ns4_RequestSOAPHeader.linkId="";
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


fulfil_by_sql(char *sql)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	char p_data[rcdlen];

	proclog(sql);
	mysql_query(&mysql,"set names utf8");
	mysql_query(&mysql,sql);
	result=mysql_store_result(&mysql);
	while(row=mysql_fetch_row(result))
	{
		memset(p_data,0,rcdlen);

		if(row[0]!=NULL)//seqid
			*(int*)(p_data+350)=atol(row[0]);

		if(row[2]!=NULL)//address
			strcpy(p_data+260,row[2]);

		if(row[4]!=NULL)//senderName
			strcpy(p_data+40,row[4]);

		if(row[10]!=NULL)//message
			strcpy(p_data+60,row[10]);

		if(row[11]!=NULL)//productID
			strcpy(p_data,row[11]);

		if(row[5]!=NULL)//linkID
			strcpy(p_data+320,row[5]);

		if(row[8]!=NULL)//amount==feecode
			strcpy(p_data+280,row[8]);

		/*
		if(row[8]!=NULL)//code
			strcpy(p_data+290,row[8]);
		*/

		fulfil(p_data);

	}
	mysql_free_result(result);

}

static int send_welcome_data()
{
	int n;
	char *tmp;
	MYSQL_RES *result;
	MYSQL_RES *result1;
	  MYSQL_ROW row;
	  MYSQL_ROW row1;
	  int gotnum=0;

	char sql[256];

	char id_str[256];
	memset(id_str,0,sizeof(id_str));

	//获取未发送欢迎消息的包月记录
	sprintf(sql,"select id,phone_number,service_id from wraith_subscribe_history where optime < NOW()-interval 10 second and optime >NOW()-interval 10 minute and optype=0 and welcome=0 limit 50", gwid);
	//proclog(sql);
	mysql_query(&mysql,sql);
	result=mysql_store_result(&mysql);
	gotnum=mysql_num_rows(result);
	if(!gotnum)
	{
		//proclog("no data,return!");
		return;
	}
//	sprintf(logbuf,"got [%d] rows",gotnum);
//	proclog(logbuf);


	int i=0;
	while(row=mysql_fetch_row(result))
	{

		//  //#####取得订购的消息ID#####
	    	sprintf(sql,"select max(id) from wraith_message where phone_number='%s' and service_id='%s' and motime > NOW() - interval 10 minute",
	    			row[1],
	    			row[2]
	    			);
	    	//mysql_exec(&mysql,"set names utf8");
	    	//proclog("%s",sql);
	    	mysql_exec(&mysql, sql);

	    	//////////每条消息strcat一下id
	    	result1=mysql_store_result(&mysql);
	    	row1=mysql_fetch_row(result1);
		if(row1[0])
		{
			strcat(id_str,row1[0]);
			strcat(id_str,",");
		}
		else
		{
			proclog("error: no order_id!!!!");
		}
		mysql_free_result(result1);


		//对已发送欢迎消息的记录进行置位
		sprintf(sql,"update wraith_subscribe_history set welcome=1 where id='%s' ", row[0]);
		mysql_exec(&mysql, sql);

	}
	mysql_free_result(result);


	//有订购记录，但是都没找到相应的订购指令消息
	if(id_str[0]==0)
	{
		//proclog("no data,return!");
		return;
	}


	//生成fulfil sql语句
	id_str[strlen(id_str)-1]=0;
	char fulfil_sql[256];
	sprintf(fulfil_sql,"select * from wraith_message where id in (%s)",id_str);
	fulfil_by_sql(fulfil_sql);



}
main(int argc, char **argv)
{
	if(argc!=2)
	{
		printf("please tell me config file!\n");
		exit(0);
	}
	if(atexit(&procquit))
	{
	   printf("quit code can't install!");
	   exit(0);
	}
	read_config(argv[1]);

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,dbip,dbuser,dbpass,dbname,0,NULL,0))
	{
		sql_err_log(&mysql);
		exit(0);
	}
	while(1)
	{
		send_welcome_data();
		sleep(10);
	}
}
