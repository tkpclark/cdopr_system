//#define _FILE_OFFSET_BITS 64
#include "soapH.h"
#include "SmsNotificationServiceSoapBinding.nsmap"
#include <ccl/ccl.h>
#include "mysqllib.h"
//#include "apache_gsoap.h"
//IMPLEMENT_GSOAP_SERVER()

//char logbuf[1024];
char mdname[]="SmsNotification";
char logpath[] = "../logs/ctccgw/";
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
	ccl_parse(&config, "../conf/hmgw.ccl");
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
/*
int __ns1__notifySmsDeliveryReceipt(struct soap *soap, struct ns2__notifySmsDeliveryReceipt *ns2__notifySmsDeliveryReceipt, struct ns2__notifySmsDeliveryReceiptResponse* ns2__notifySmsDeliveryReceiptResponse)
{
	proclog("[notifySmsDeliveryReceipt]corr[%s]status[%d]",ns2__notifySmsDeliveryReceipt->correlator,ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus);
	char sql[512];

	sprintf(sql,"update wraith_mt set report='%d' where id=%s",ns2__notifySmsDeliveryReceipt->deliveryStatus->deliveryStatus,ns2__notifySmsDeliveryReceipt->correlator);
	mysql_exec(&mysql, sql);

	return SOAP_OK;
}
*/
int ns1__notifySmsDeliveryReceipt(struct soap *soap, char *correlator, struct ns1__DeliveryInformation *ns1__DeliveryInformation, struct ns1__notifySmsDeliveryReceiptResponse *ns1__notifySmsDeliveryReceiptResponse)
{

	proclog("[notifySmsDeliveryReceipt]corr[%s]status[%d]",correlator,ns1__DeliveryInformation->deliveryStatus);
	char sql[512];

	sprintf(sql,"update wraith_mt set report='%d' where id=%s",ns1__DeliveryInformation->deliveryStatus,correlator);
	mysql_exec(&mysql, sql);

	return SOAP_OK;
}
/*
int __ns1__notifySmsReception(struct soap *soap, struct ns2__notifySmsReception *ns2__notifySmsReception, struct ns2__notifySmsReceptionResponse* ns2__notifySmsReceptionResponse)
{


	proclog("[Reception]regId[%s]message[%s]sender[%s]servnumber[%s]linkID[%s]",
			ns2__notifySmsReception->registrationIdentifier,
			ns2__notifySmsReception->message->message,
			//ns2__notifySmsReception->message->message,
			ns2__notifySmsReception->message->senderAddress,
			ns2__notifySmsReception->message->smsServiceActivationNumber,
			soap->header->ns4__NotifySOAPHeader->linkID);



	char phone_number[32]={0};
	strcpy(phone_number,ns2__notifySmsReception->message->senderAddress+4);
	char sp_number[32]={0};
	strcpy(sp_number,ns2__notifySmsReception->message->smsServiceActivationNumber+4);


	char sql[512];
	sprintf(sql,"insert into wraith_mo( in_date, phone_number, message, sp_number, linkID, gwid ) values (NOW(),'%s', '%s', '%s', '%s', '%s');",
			phone_number,
			ns2__notifySmsReception->message->message,
			sp_number,
			soap->header->ns4__NotifySOAPHeader->linkID,
			gwid
			);
	mysql_exec(&mysql,"set names utf8");
	mysql_exec(&mysql, sql);


	return SOAP_OK;
}
*/
int ns1__notifySmsReception(struct soap *soap, struct ns1__SmsMessage *ns1__SmsMessage, struct ns1__notifySmsReceptionResponse *ns1__notifySmsReceptionResponse)
{
	proclog("[Reception]message[%s]sender[%s]servnumber[%s]linkID[%s]",
			ns1__SmsMessage->message,
			ns1__SmsMessage->senderAddress,
			ns1__SmsMessage->receiverAddress,
			soap->header->ns4__NotifySOAPHeader->linkID);

	char sql[512];
	sprintf(sql,"insert into wraith_mo( in_date, phone_number, message, sp_number, linkID, gwid ) values (NOW(),'%s', '%s', '%s', '%s', '%s');",
			ns1__SmsMessage->senderAddress,
			ns1__SmsMessage->message,
			ns1__SmsMessage->receiverAddress,
			soap->header->ns4__NotifySOAPHeader->linkID,
			gwid
			);
	mysql_exec(&mysql,"set names utf8");
	mysql_exec(&mysql, sql);


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
