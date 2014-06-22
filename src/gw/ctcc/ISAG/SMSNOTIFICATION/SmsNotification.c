//#define _FILE_OFFSET_BITS 64
#include "soapH.h"
#include "SmsNotificationBinding.nsmap"
#include <ccl/ccl.h>
#include "mysqllib.h"
//#include "apache_gsoap.h"
//IMPLEMENT_GSOAP_SERVER()

//char logbuf[1024];
char mdname[]="SmsNotification";
char logpath[128];
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
	ccl_parse(&config, "ctccgw.ccl");
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

static workdir(char *path)
{
        char workdir[128];
        strcpy(workdir,path);
        char *p=NULL;
        p=strrchr(workdir,'/');
        *p=0;
        //printf("workdir:%s\n", workdir);

        chdir(workdir);


}
static void my_init()
{
	read_config();
	mysql_create_connect(&mysql, ip, user,pass,db);
}
int __ns1__notifySmsDeliveryReceipt(struct soap *soap, struct ns2__notifySmsDeliveryReceipt *ns2__notifySmsDeliveryReceipt, struct ns2__notifySmsDeliveryReceiptResponse* ns2__notifySmsDeliveryReceiptResponse)
{
	proclog("[notifySmsDeliveryReceipt]corr[%s]status[%d]",ns2__notifySmsDeliveryReceipt->correlator,ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus);
	char sql[512];
	/*
	sprintf(sql,"insert into wraith_mr( in_date, match_id, report, gwid ) values (NOW(),'%s', '%d', '%s');",
			ns2__notifySmsDeliveryReceipt->correlator,
			ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus,
			gwid
			);
	*/
	int report = ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus==4?1:2;
	sprintf(sql,"update wraith_message set report='%d',report_orig='%d',report_time=NOW() where id=%s",report,ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus,ns2__notifySmsDeliveryReceipt->correlator);
	mysql_exec(&mysql, sql);
	//writing heapfile
	/*
	int fd;
	fd=open(heapfile,O_WRONLY|O_APPEND);
	if(fd==-1)
	{
		proclog("open heapfile error!\n");
		exit(0);
	}
	

	//corr 2
	//status 30

	char buf[256];
	strcpy(buf,"3");
	strcpy(buf+2,ns2__notifySmsDeliveryReceipt->correlator);
	char tmp[4];
	sprintf(tmp,"%d",ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus);
	strcpy(buf+30,tmp);
	
	write(fd,buf,sizeof(buf));
	close(fd);
	
	*/
	return SOAP_OK;
}
 
int __ns1__notifySmsReception(struct soap *soap, struct ns2__notifySmsReception *ns2__notifySmsReception, struct ns2__notifySmsReceptionResponse* ns2__notifySmsReceptionResponse)
{

	//printf("1:%s\n",soap->header->ns4__NotifySOAPHeader->spId);
	//printf("2:%s\n",ns2__notifySmsReception->message->message);

	/*
	char gbcontent[512];
	memset(gbcontent,0,sizeof(gbcontent));
	to_gb(ns2__notifySmsReception->message->message,gbcontent);
	*/

	
	proclog("[Reception]regId[%s]message[%s]sender[%s]servnumber[%s]linkid[%s]",
			ns2__notifySmsReception->registrationIdentifier,
			ns2__notifySmsReception->message->message,
			//ns2__notifySmsReception->message->message,
			ns2__notifySmsReception->message->senderAddress,
			ns2__notifySmsReception->message->smsServiceActivationNumber,
			soap->header->ns4__NotifySOAPHeader->linkId);

	

	char phone_number[32]={0};
	strcpy(phone_number,ns2__notifySmsReception->message->senderAddress+4);
	char sp_number[32]={0};
	strcpy(sp_number,ns2__notifySmsReception->message->smsServiceActivationNumber+4);


	char sql[512];
	sprintf(sql,"insert into wraith_message( motime, phone_number, mo_message, sp_number, linkid, gwid ) values (NOW(),'%s', '%s', '%s', '%s', '%s');",
			phone_number,
			ns2__notifySmsReception->message->message,
			sp_number,
			soap->header->ns4__NotifySOAPHeader->linkId,
			gwid
			);
	mysql_exec(&mysql,"set names utf8");
	mysql_exec(&mysql, sql);



//writing heapfile
	/*
	int fd;
	fd=open(heapfile,O_WRONLY|O_APPEND);
	if(fd==-1)
	{
		proclog("open heapfile error!\n");
		exit(0);
	}
	

//	regID 2
//	message 30
//	sender 180
//	servnumber 200
//	linkid 220

	char buf[256];
	strcpy(buf,"1");
	if(ns2__notifySmsReception->registrationIdentifier)
		strcpy(buf+2,ns2__notifySmsReception->registrationIdentifier);
	if(ns2__notifySmsReception->message->message,gbcontent)
		//strcpy(buf+30,ns2__notifySmsReception->message->message);
		strcpy(buf+30,gbcontent);
	if(ns2__notifySmsReception->message->senderAddress)
		strcpy(buf+180,ns2__notifySmsReception->message->senderAddress+4);
	if(ns2__notifySmsReception->message->smsServiceActivationNumber)
		strcpy(buf+200,ns2__notifySmsReception->message->smsServiceActivationNumber+4);
	if(soap->header->ns4__NotifySOAPHeader->linkId)
		strcpy(buf+220,soap->header->ns4__NotifySOAPHeader->linkId);
	
	write(fd,buf,sizeof(buf));
	close(fd);
	
	
*/

	return SOAP_OK;
}
int main(int argc, char **argv)
{ 
	SOAP_SOCKET m, s; /* master and slave sockets */
	workdir(argv[0]);
	my_init();
	struct soap soap;
	soap_init(&soap);
	soap_set_mode(&soap, SOAP_C_UTFSTRING);
	soap_serve(&soap);
	return 0;
}
