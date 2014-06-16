//#define _FILE_OFFSET_BITS 64
#include "soapH.h"
#include "IsmpSpEngineSoapBinding.nsmap"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ccl/ccl.h>
#include "mysqllib.h"
char mdname[]="IsmpSpEngine";
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
	ccl_parse(&config, "../conf/ctccgw.ccl");
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


int __ns1__orderRelationUpdateNotify(struct soap *soap, struct ns2__OrderRelationUpdateNotifyReq *ns2__OrderRelationUpdateNotifyReq,struct ns3__Response *ns3__Response)
{
	proclog("[orderRelationUpdateNotify] sNo[%s]userID[%s]prodID[%s]packID[%s]OPType[%d]",
		ns2__OrderRelationUpdateNotifyReq->streamingNo,
		ns2__OrderRelationUpdateNotifyReq->userID,
		ns2__OrderRelationUpdateNotifyReq->productID,
		ns2__OrderRelationUpdateNotifyReq->packageID,
		ns2__OrderRelationUpdateNotifyReq->OPType);

	ns3__Response->streamingNo=ns2__OrderRelationUpdateNotifyReq->streamingNo;
	    ns3__Response->resultCode=0;

	    char sql[512];


	    	sprintf(sql,"insert into wraith_subscribe_history( phone_number,service_id,optype,optime, gwid ) values ('%s', '%s', '%d', NOW(), '%s');",
	    			ns2__OrderRelationUpdateNotifyReq->userID,
	    			ns2__OrderRelationUpdateNotifyReq->productID,
	    			ns2__OrderRelationUpdateNotifyReq->OPType,
	    			gwid
	    			);
	    	proclog("%s",sql);
	    	mysql_exec(&mysql, sql);


	    	//delete first
	    	sprintf(sql,"delete from wraith_subscribe_users where phone_number='%s'",ns2__OrderRelationUpdateNotifyReq->userID);
	    	proclog("%s",sql);
	    	mysql_exec(&mysql, sql);

	    	//insert
		if(ns2__OrderRelationUpdateNotifyReq->OPType == 0 || ns2__OrderRelationUpdateNotifyReq->OPType == 2)
		{
			sprintf(sql,"insert into wraith_subscribe_users (phone_number,service_id,status,gwid) values('%s','%s','1','%s')",
					ns2__OrderRelationUpdateNotifyReq->userID,
					ns2__OrderRelationUpdateNotifyReq->productID,
					gwid
					);
			proclog("%s",sql);
			mysql_exec(&mysql, sql);
		}

	 return SOAP_OK;
}


int __ns1__serviceConsumeNotify(struct soap *soap, struct ns2__ServiceConsumeNotifyReq *ns2__ServiceConsumeNotifyReq, struct ns3__Response *ns3__Response)
{
	char gbcontent[512];
	memset(gbcontent,0,sizeof(gbcontent));
	//to_gb(ns2__ServiceConsumeNotifyReq->featureStr,gbcontent);
	proclog("[serviceConsumeNotify]sNo[%s]featureStr[%s]linkID[%s]userID[%s]productID[%s]",ns2__ServiceConsumeNotifyReq->streamingNo,gbcontent,ns2__ServiceConsumeNotifyReq->linkID,ns2__ServiceConsumeNotifyReq->userID,ns2__ServiceConsumeNotifyReq->productID);

	ns3__Response->streamingNo=ns2__ServiceConsumeNotifyReq->streamingNo;
	ns3__Response->resultCode=0;
	return SOAP_OK;
}


int __ns1__notifyManagementInfo(struct soap *soap, struct ns2__NotifyManagementInfoReq *ns2__NotifyManagementInfoReq,struct ns3__NotifyManagementInfoRsp *ns3__NotifyManagementInfoRsp)
{
	proclog("[notifyManagementInfo]sNo[%s]ID[%s]",ns2__NotifyManagementInfoReq->streamingNo,ns2__NotifyManagementInfoReq->ID);
	ns3__NotifyManagementInfoRsp->streamingNo=ns2__NotifyManagementInfoReq->streamingNo;
	ns3__NotifyManagementInfoRsp->resultCode=0;
	return SOAP_OK;
}

int main(int argc, char **argv)
{
	SOAP_SOCKET m, s; /* master and slave sockets */
	my_init();
	struct soap soap;
	soap_init(&soap);
	/*
	soap_set_recv_logfile(&soap, debuglog);
	soap_set_send_logfile(&soap, debuglog);
	soap_set_test_logfile(&soap, debuglog);
	*/
	soap_set_mode(&soap, SOAP_C_UTFSTRING);
	soap_serve(&soap);
	return 0;
}
