/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "RSI"
 * 	found in "./asn/RSI.asn"
 * 	`asn1c -fskeletons-copy -fnative-types -gen-PER -pdu=auto -no-gen-example -D ./src`
 */

#ifndef	_Radius_H_
#define	_Radius_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Radius */
typedef long	 Radius_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_Radius_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_Radius;
asn_struct_free_f Radius_free;
asn_struct_print_f Radius_print;
asn_constr_check_f Radius_constraint;
ber_type_decoder_f Radius_decode_ber;
der_type_encoder_f Radius_encode_der;
xer_type_decoder_f Radius_decode_xer;
xer_type_encoder_f Radius_encode_xer;
oer_type_decoder_f Radius_decode_oer;
oer_type_encoder_f Radius_encode_oer;
per_type_decoder_f Radius_decode_uper;
per_type_encoder_f Radius_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _Radius_H_ */
#include <asn_internal.h>
