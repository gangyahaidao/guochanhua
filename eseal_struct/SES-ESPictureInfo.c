/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EsealModule"
 * 	found in "EsealModule.asn1"
 */

#include "SES-ESPictureInfo.h"

asn_TYPE_member_t asn_MBR_SES_ESPictureInfo_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SES_ESPictureInfo, type),
		(ASN_TAG_CLASS_UNIVERSAL | (22 << 2)),
		0,
		&asn_DEF_IA5String,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"type"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SES_ESPictureInfo, data),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_OCTET_STRING,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"data"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SES_ESPictureInfo, width),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"width"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SES_ESPictureInfo, height),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"height"
		},
};
static const ber_tlv_tag_t asn_DEF_SES_ESPictureInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SES_ESPictureInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 2, 0, 1 }, /* width */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 3, -1, 0 }, /* height */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, 0, 0 }, /* data */
    { (ASN_TAG_CLASS_UNIVERSAL | (22 << 2)), 0, 0, 0 } /* type */
};
asn_SEQUENCE_specifics_t asn_SPC_SES_ESPictureInfo_specs_1 = {
	sizeof(struct SES_ESPictureInfo),
	offsetof(struct SES_ESPictureInfo, _asn_ctx),
	asn_MAP_SES_ESPictureInfo_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SES_ESPictureInfo = {
	"SES-ESPictureInfo",
	"SES-ESPictureInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_SES_ESPictureInfo_tags_1,
	sizeof(asn_DEF_SES_ESPictureInfo_tags_1)
		/sizeof(asn_DEF_SES_ESPictureInfo_tags_1[0]), /* 1 */
	asn_DEF_SES_ESPictureInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_SES_ESPictureInfo_tags_1)
		/sizeof(asn_DEF_SES_ESPictureInfo_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SES_ESPictureInfo_1,
	4,	/* Elements count */
	&asn_SPC_SES_ESPictureInfo_specs_1	/* Additional specs */
};

