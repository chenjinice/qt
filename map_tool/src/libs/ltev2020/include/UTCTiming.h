/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "SPATIntersectionState"
 * 	found in "./asn/SPATIntersectionState.asn"
 * 	`asn1c -fskeletons-copy -fnative-types -gen-PER -pdu=auto -no-gen-example -D ./src`
 */

#ifndef	_UTCTiming_H_
#define	_UTCTiming_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TimeMark.h"
#include "Confidence.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UTCTiming */
typedef struct UTCTiming {
	TimeMark_t	 startUTCTime;
	TimeMark_t	*minEndUTCTime	/* OPTIONAL */;
	TimeMark_t	*maxEndUTCTime	/* OPTIONAL */;
	TimeMark_t	 likelyEndUTCTime;
	Confidence_t	*timeConfidence	/* OPTIONAL */;
	TimeMark_t	*nextStartUTCTime	/* OPTIONAL */;
	TimeMark_t	*nextEndUTCTime	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UTCTiming_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UTCTiming;
extern asn_SEQUENCE_specifics_t asn_SPC_UTCTiming_specs_1;
extern asn_TYPE_member_t asn_MBR_UTCTiming_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _UTCTiming_H_ */
#include <asn_internal.h>
