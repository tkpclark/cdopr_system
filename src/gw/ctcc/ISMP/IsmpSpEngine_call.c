#include "soapH.h"
#include "IsmpSpEngineSoapBinding.nsmap"
int main(int argc, char **argv)
{
	/*
	if(argc < 2)
	{
		printf("content please!\n");
		exit(0);
	}
	*/
	SOAP_SOCKET m, s; /* master and slave sockets */
	struct soap soap;
	soap_init(&soap);


	/////////////////


	const char *soap_endpoint="http://42.62.78.247/services/IsmpSpEngine";




	int OPType=0;
	char *packageID=NULL;	/* optional element of type xsd:string */
	char *oldPackageID=NULL;	/* optional element of type xsd:string */
	char *productID="135000000000000230100";	/* optional element of type xsd:string */
	char *oldProductID=NULL;	/* optional element of type xsd:string */
	char *streamingNo="11111111111111111111";	/* optional element of type xsd:string */
	char *userID="13366233899";	/* optional element of type xsd:string */
	int userIDType=0;	/* required element of type xsd:int */
	int __sizeVerUserID=0;	/* sequence of elements <VerUserID> */
	char **VerUserID=NULL;



	struct ns2__OrderRelationUpdateNotifyReq OrderRelationUpdateNotifyReq;
	OrderRelationUpdateNotifyReq.OPType=0;
	OrderRelationUpdateNotifyReq.packageID=packageID;
	OrderRelationUpdateNotifyReq.oldPackageID=oldPackageID;
	OrderRelationUpdateNotifyReq.productID=productID;
	OrderRelationUpdateNotifyReq.oldProductID=oldProductID;
	OrderRelationUpdateNotifyReq.streamingNo=streamingNo;
	OrderRelationUpdateNotifyReq.userID=userID;
	OrderRelationUpdateNotifyReq.userIDType=userIDType;
	OrderRelationUpdateNotifyReq.__sizeVerUserID=__sizeVerUserID;
	OrderRelationUpdateNotifyReq.VerUserID=VerUserID;



	struct ns3__Response Response;
	//soap_call___ns1__notifySmsReception(&soap, soap_endpoint, NULL, &NotifySmsReception, &notifySmsReceptionResponse);
	soap_call___ns1__orderRelationUpdateNotify(&soap, soap_endpoint, NULL, &OrderRelationUpdateNotifyReq, &Response);
	if (soap.error)
	{
		soap_print_fault(&soap, stderr);
	}
	return 0;
}
