#include "soapH.h"
#include "SmsNotificationBinding.nsmap"
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("content please!\n");
		exit(0);
	}
	SOAP_SOCKET m, s; /* master and slave sockets */
	struct soap soap;
	soap_init(&soap);


	//header
	char *linkid="999999999999999";
	struct ns4__NotifySOAPHeader NotifySOAPHeader;
	NotifySOAPHeader.linkId=linkid;
	soap.header = (struct SOAP_ENV__Header *)soap_malloc(&soap, sizeof(struct SOAP_ENV__Header));
	soap.header->ns4__NotifySOAPHeader=&NotifySOAPHeader;


	/////////////////


	const char *soap_endpoint="http://202.85.209.109/services/SmsNotification";

	char *message=argv[1];
	char *senderAddress="tel:13910002000";
	char *smsServiceActivationNumber="tel:10660766";

	char *registrationIdentifier="registrationIdentifier";

	struct ns3__SmsMessage SmsMessage;
	struct ns2__notifySmsReception NotifySmsReception;
	SmsMessage.message=message;
	SmsMessage.senderAddress=senderAddress;
	SmsMessage.smsServiceActivationNumber=smsServiceActivationNumber;

	NotifySmsReception.registrationIdentifier=registrationIdentifier;
	NotifySmsReception.message=&SmsMessage;

	struct ns2__notifySmsReceptionResponse notifySmsReceptionResponse;

	soap_call___ns1__notifySmsReception(&soap, soap_endpoint, NULL, &NotifySmsReception, &notifySmsReceptionResponse);
	if (soap.error)
	{
		soap_print_fault(&soap, stderr);
	}
	return 0;
}
