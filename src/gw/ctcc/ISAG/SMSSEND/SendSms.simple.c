#include "soapH.h"
#include "SendSmsBinding.nsmap"
int main(int argc, char **argv)
{
	SOAP_SOCKET m, s; /* master and slave sockets */
	struct soap soap;
	soap_init(&soap);


	//header
	char *linkid="999999999999999";
	struct ns4__RequestSOAPHeader ns4_RequestSOAPHeader;
	soap.header = (struct SOAP_ENV__Header *)soap_malloc(&soap, sizeof(struct SOAP_ENV__Header));
	soap.header->ns4__RequestSOAPHeader=&ns4_RequestSOAPHeader;


	/////////////////


	const char *soap_endpoint="http://118.85.200.55:9081/SendSmsService";
	struct ns2__sendSms ns2__sendSms;
				struct ns2__sendSmsResponse ns2__sendSmsResponse;
				struct ns4__SimpleReference receiptRequest;
				struct ns4__ChargingInformation charging;

	soap_call___ns1__sendSms(&soap, soap_endpoint, NULL,&ns2__sendSms,&ns2__sendSmsResponse);
	if (soap.error)
	{
		soap_print_fault(&soap, stderr);
	}
	return 0;
}
