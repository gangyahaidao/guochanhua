/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EsealModule"
 * 	found in "EsealModule.asn1"
 */

#ifndef	_SES_ESPropertyInfo_H_
#define	_SES_ESPropertyInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <UTF8String.h>
#include <UTCTime.h>
#include <OCTET_STRING.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SES-ESPropertyInfo */
typedef struct SES_ESPropertyInfo {
	long	 type;
	UTF8String_t	 name;
	struct certList {
		A_SEQUENCE_OF(OCTET_STRING_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} certList;
	UTCTime_t	 createDate;
	UTCTime_t	 validStart;
	UTCTime_t	 validEnd;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SES_ESPropertyInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SES_ESPropertyInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_SES_ESPropertyInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_SES_ESPropertyInfo_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _SES_ESPropertyInfo_H_ */
#include <asn_internal.h>