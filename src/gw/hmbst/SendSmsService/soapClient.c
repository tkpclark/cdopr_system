/* soapClient.c
   Generated by gSOAP 2.8.15 from SendSmsService.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_SOURCE_STAMP("@(#) soapClient.c ver 2.8.15 2013-10-14 12:55:21 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__sendSms(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__SendSmsRequest *ns1__in0, struct ns1__SendSmsResponse *ns1__sendSmsReturn)
{	struct __ns1__sendSms soap_tmp___ns1__sendSms;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://localhost:8080/services/SendSmsService";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__sendSms.ns1__in0 = ns1__in0;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__sendSms(soap, &soap_tmp___ns1__sendSms);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__sendSms(soap, &soap_tmp___ns1__sendSms, "-ns1:sendSms", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_url(soap, soap_endpoint, NULL), soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__sendSms(soap, &soap_tmp___ns1__sendSms, "-ns1:sendSms", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__sendSmsReturn)
		return soap_closesock(soap);
	soap_default_ns1__SendSmsResponse(soap, ns1__sendSmsReturn);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get_ns1__SendSmsResponse(soap, ns1__sendSmsReturn, "ns1:sendSmsReturn", "ns1:SendSmsResponse");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapClient.c */
