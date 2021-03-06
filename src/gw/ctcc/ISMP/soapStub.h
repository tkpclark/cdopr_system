/* soapStub.h
   Generated by gSOAP 2.8.15 from IsmpSpEngine.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#define SOAP_NAMESPACE_OF_ns2	"http://req.sp.ismp.chinatelecom.com"
#define SOAP_NAMESPACE_OF_ns1	"http://sp.ismp.chinatelecom.com"
#define SOAP_NAMESPACE_OF_ns3	"http://rsp.sp.ismp.chinatelecom.com"
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

#ifndef SOAP_TYPE_ns2__OrderRelationUpdateNotifyReq
#define SOAP_TYPE_ns2__OrderRelationUpdateNotifyReq (7)
/* ns2:OrderRelationUpdateNotifyReq */
struct ns2__OrderRelationUpdateNotifyReq
{
	int OPType;	/* required element of type xsd:int */
	char *packageID;	/* optional element of type xsd:string */
	char *oldPackageID;	/* optional element of type xsd:string */
	char *productID;	/* optional element of type xsd:string */
	char *oldProductID;	/* optional element of type xsd:string */
	char *streamingNo;	/* optional element of type xsd:string */
	char *userID;	/* optional element of type xsd:string */
	int userIDType;	/* required element of type xsd:int */
	int __sizeVerUserID;	/* sequence of elements <VerUserID> */
	char **VerUserID;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns2__SPWithdrawSubscriptionReq
#define SOAP_TYPE_ns2__SPWithdrawSubscriptionReq (9)
/* ns2:SPWithdrawSubscriptionReq */
struct ns2__SPWithdrawSubscriptionReq
{
	int userIDType;	/* required element of type xsd:int */
	char *userID;	/* optional element of type xsd:string */
	int IDType;	/* required element of type xsd:int */
	char *ID;	/* optional element of type xsd:string */
	char *streamingNo;	/* optional element of type xsd:string */
	char *SPID;	/* optional element of type xsd:string */
	char *SPAdmin;	/* optional element of type xsd:string */
	char *SPAdminPwd;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns2__ServiceConsumeNotifyReq
#define SOAP_TYPE_ns2__ServiceConsumeNotifyReq (10)
/* ns2:ServiceConsumeNotifyReq */
struct ns2__ServiceConsumeNotifyReq
{
	char *featureStr;	/* optional element of type xsd:string */
	char *linkID;	/* optional element of type xsd:string */
	char *productID;	/* optional element of type xsd:string */
	char *streamingNo;	/* optional element of type xsd:string */
	char *userID;	/* optional element of type xsd:string */
	int userIDType;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_ns2__NotifyManagementInfoReq
#define SOAP_TYPE_ns2__NotifyManagementInfoReq (11)
/* ns2:NotifyManagementInfoReq */
struct ns2__NotifyManagementInfoReq
{
	char *ID;	/* optional element of type xsd:string */
	int IDType;	/* required element of type xsd:int */
	int status;	/* required element of type xsd:int */
	char *streamingNo;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns3__Response
#define SOAP_TYPE_ns3__Response (12)
/* ns3:Response */
struct ns3__Response
{
	int resultCode;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:int */
	char *streamingNo;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns3__NotifyManagementInfoRsp
#define SOAP_TYPE_ns3__NotifyManagementInfoRsp (13)
/* ns3:NotifyManagementInfoRsp */
struct ns3__NotifyManagementInfoRsp
{
	int resultCode;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:int */
	char *streamingNo;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE___ns1__orderRelationUpdateNotify
#define SOAP_TYPE___ns1__orderRelationUpdateNotify (17)
/* Operation wrapper: */
struct __ns1__orderRelationUpdateNotify
{
	struct ns2__OrderRelationUpdateNotifyReq *ns1__orderRelationUpdateNotifyReq;	/* optional element of type ns2:OrderRelationUpdateNotifyReq */
};
#endif

#ifndef SOAP_TYPE___ns1__spWithdrawSubscription
#define SOAP_TYPE___ns1__spWithdrawSubscription (20)
/* Operation wrapper: */
struct __ns1__spWithdrawSubscription
{
	struct ns2__SPWithdrawSubscriptionReq *ns1__spWithdrawSubscriptionReqPara;	/* optional element of type ns2:SPWithdrawSubscriptionReq */
};
#endif

#ifndef SOAP_TYPE___ns1__serviceConsumeNotify
#define SOAP_TYPE___ns1__serviceConsumeNotify (23)
/* Operation wrapper: */
struct __ns1__serviceConsumeNotify
{
	struct ns2__ServiceConsumeNotifyReq *ns1__serviceConsumeNotifyReqPara;	/* optional element of type ns2:ServiceConsumeNotifyReq */
};
#endif

#ifndef SOAP_TYPE___ns1__notifyManagementInfo
#define SOAP_TYPE___ns1__notifyManagementInfo (27)
/* Operation wrapper: */
struct __ns1__notifyManagementInfo
{
	struct ns2__NotifyManagementInfoReq *ns1__notifyManagementInfoReq;	/* optional element of type ns2:NotifyManagementInfoReq */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (28)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (29)
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
#define SOAP_TYPE_SOAP_ENV__Detail (31)
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
#define SOAP_TYPE_SOAP_ENV__Reason (34)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (35)
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


SOAP_FMAC5 int SOAP_FMAC6 __ns1__orderRelationUpdateNotify(struct soap*, struct ns2__OrderRelationUpdateNotifyReq *ns1__orderRelationUpdateNotifyReq, struct ns3__Response *ns1__orderRelationUpdateNotifyReturn);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__spWithdrawSubscription(struct soap*, struct ns2__SPWithdrawSubscriptionReq *ns1__spWithdrawSubscriptionReqPara, struct ns3__Response *ns1__spWithdrawSubscriptionReturn);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__serviceConsumeNotify(struct soap*, struct ns2__ServiceConsumeNotifyReq *ns1__serviceConsumeNotifyReqPara, struct ns3__Response *ns1__serviceConsumeNotifyReturn);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__notifyManagementInfo(struct soap*, struct ns2__NotifyManagementInfoReq *ns1__notifyManagementInfoReq, struct ns3__NotifyManagementInfoRsp *ns1__notifyManagementInfoReturn);

/******************************************************************************\
 *                                                                            *
 * Server-Side Skeletons to Invoke Service Operations                         *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__orderRelationUpdateNotify(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__spWithdrawSubscription(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__serviceConsumeNotify(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__notifyManagementInfo(struct soap*);

/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stubs                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__orderRelationUpdateNotify(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns2__OrderRelationUpdateNotifyReq *ns1__orderRelationUpdateNotifyReq, struct ns3__Response *ns1__orderRelationUpdateNotifyReturn);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__spWithdrawSubscription(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns2__SPWithdrawSubscriptionReq *ns1__spWithdrawSubscriptionReqPara, struct ns3__Response *ns1__spWithdrawSubscriptionReturn);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__serviceConsumeNotify(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns2__ServiceConsumeNotifyReq *ns1__serviceConsumeNotifyReqPara, struct ns3__Response *ns1__serviceConsumeNotifyReturn);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__notifyManagementInfo(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns2__NotifyManagementInfoReq *ns1__notifyManagementInfoReq, struct ns3__NotifyManagementInfoRsp *ns1__notifyManagementInfoReturn);

#ifdef __cplusplus
}
#endif

#endif

/* End of soapStub.h */
