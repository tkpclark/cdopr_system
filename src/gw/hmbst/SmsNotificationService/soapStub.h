/* soapStub.h
   Generated by gSOAP 2.8.15 from SmsNotificationService.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#define SOAP_NAMESPACE_OF_ns1	"http://besttone.nmsccharge.cp.sms.notification"
#include "stdsoap2.h"
#if GSOAP_VERSION != 20815
# error "GSOAP VERSION MISMATCH IN GENERATED CODE: PLEASE REINSTALL PACKAGE"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE_ns1__SmsMessage
#define SOAP_TYPE_ns1__SmsMessage (8)
/* ns1:SmsMessage */
struct ns1__SmsMessage
{
	char *message;	/* optional element of type xsd:string */
	char *receiverAddress;	/* optional element of type xsd:string */
	char *senderAddress;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__DeliveryInformation
#define SOAP_TYPE_ns1__DeliveryInformation (9)
/* ns1:DeliveryInformation */
struct ns1__DeliveryInformation
{
	char *address;	/* optional element of type xsd:anyURI */
	char *deliveryStatus;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__notifySmsReceptionResponse
#define SOAP_TYPE_ns1__notifySmsReceptionResponse (10)
/* ns1:notifySmsReceptionResponse */
struct ns1__notifySmsReceptionResponse
{
#ifdef WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE_ns1__notifySmsReception
#define SOAP_TYPE_ns1__notifySmsReception (14)
/* ns1:notifySmsReception */
struct ns1__notifySmsReception
{
	struct ns1__SmsMessage *_in0;	/* optional element of type ns1:SmsMessage */
};
#endif

#ifndef SOAP_TYPE_ns1__notifySmsDeliveryReceiptResponse
#define SOAP_TYPE_ns1__notifySmsDeliveryReceiptResponse (15)
/* ns1:notifySmsDeliveryReceiptResponse */
struct ns1__notifySmsDeliveryReceiptResponse
{
#ifdef WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE_ns1__notifySmsDeliveryReceipt
#define SOAP_TYPE_ns1__notifySmsDeliveryReceipt (19)
/* ns1:notifySmsDeliveryReceipt */
struct ns1__notifySmsDeliveryReceipt
{
	char *_in0;	/* optional element of type xsd:string */
	struct ns1__DeliveryInformation *_in1;	/* optional element of type ns1:DeliveryInformation */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (20)
/* SOAP Header: */
/*tkp modify for soap header*/
struct ns4__NotifySOAPHeader
{
	char *cpID;		/* optional element of type xsd:string */
	char *cpPassword;	/* optional element of type xsd:string */
	char *timeStamp;	/* required element of type xsd:string */
	char *productID;	/* optional element of type xsd:string */
	char *linkID;		/* optional element of type xsd:string */
};
struct SOAP_ENV__Header
{
	struct ns4__NotifySOAPHeader *ns4__NotifySOAPHeader;	/* mustUnderstand */
};
/***end***/
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (21)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (23)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (26)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (27)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE_xsd__anyURI
#define SOAP_TYPE_xsd__anyURI (7)
typedef char *xsd__anyURI;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 ns1__notifySmsReception(struct soap*, struct ns1__SmsMessage *_in0, struct ns1__notifySmsReceptionResponse *_param_1);

SOAP_FMAC5 int SOAP_FMAC6 ns1__notifySmsDeliveryReceipt(struct soap*, char *_in0, struct ns1__DeliveryInformation *_in1, struct ns1__notifySmsDeliveryReceiptResponse *_param_2);

/******************************************************************************\
 *                                                                            *
 * Server-Side Skeletons to Invoke Service Operations                         *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns1__notifySmsReception(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns1__notifySmsDeliveryReceipt(struct soap*);

/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stubs                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__notifySmsReception(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__SmsMessage *_in0, struct ns1__notifySmsReceptionResponse *_param_1);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__notifySmsDeliveryReceipt(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *_in0, struct ns1__DeliveryInformation *_in1, struct ns1__notifySmsDeliveryReceiptResponse *_param_2);

#ifdef __cplusplus
}
#endif

#endif

/* End of soapStub.h */
