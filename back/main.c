#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <UTCTime.h>
#include <GeneralizedTime.c>

#include "SESeal.h"

int encode(int ac, char **av);
int decode(int ac, char **av);

static void checkUTC2Time(char *time_str, time_t sample, int as_gmt) {
	UTCTime_t gt;
	struct tm tm;
	time_t tloc;

	gt.buf = (uint8_t *)time_str;
	gt.size = strlen(time_str);

	tloc = asn_UT2time(&gt, &tm, as_gmt);
	printf("[%s] -> %ld == %ld\n", time_str, (long)tloc, (long)sample);
	if(tloc != -1)
	printf("\t%d-%d-%dT%02d:%02d:%02d %ld\n",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		GMTOFF(tm)
	);
}
static void checkTime2UT(struct tm* tm) {
    UTCTime_t output;
    asn_time2UT(&output, tm, 0);
    printf("output = %s\n", output.buf);
}


int main(int argc, char* argv[]) {
    // 将UTC时间转换成本地时间
    checkUTC2Time("190824120917+0800", 1000000, 0);

    // 将本地时间转换成UTC时间
    time_t lt = time(NULL);
    struct tm * tmp_localtime = localtime(&lt);
    checkTime2UT(tmp_localtime);

    //return encode(argc, argv);
    //return decode(argc, argv);
}

/* Write the encoded output into some FILE stream. */
static int write_out(const void *buffer, size_t size, void *app_key) {
    FILE *out_fp = app_key;
    size_t wrote = fwrite(buffer, 1, size, out_fp);
    return (wrote == size) ? 0 : -1;
}
int encode(int ac, char **av) {
    SESeal_t* eseal; /* Type to encode */
    asn_enc_rval_t ec; /* Encoder return value */
    /* Allocate the Rectangle_t */
    eseal = calloc(1, sizeof(SESeal_t)); /* not malloc! */
    if(!eseal) {
        perror("calloc() failed");
        exit(1);
    }
    /* Initialize the eseal members */
    // 1.填充印章信息
    // 1.1 电子印章标识，区分电子印章数据的唯一标识编码
    char* esID = "SEAL-10000000-CSDC";
    eseal->esealInfo.esID.buf = esID;
    eseal->esealInfo.esID.size = strlen(esID);
    //1.2 填充印章头信息
    char* headerID = "ES";
    eseal->esealInfo.header.id->buf = headerID;
    eseal->esealInfo.header.id->size = strlen(headerID);
    long headerVersion = 10;
    eseal->esealInfo.header.version = headerVersion;
    char* headerVid = "电子印章平台";
    eseal->esealInfo.header.vid.buf = headerVid;
    eseal->esealInfo.header.vid.size = strlen(headerVid);
    // 1.3填充印章属性信息
    int infoType = 2;
    eseal->esealInfo.property.type = infoType;
    char* infoName = "测试导出公章";
    eseal->esealInfo.property.name.buf = infoName;
    eseal->esealInfo.property.name.size = strlen(infoName);

    //eseal->esealInfo.property.certList.list.

    time_t lt = time(NULL);
    struct tm * tmp_localtime = localtime(&lt);
    UTCTime_t createDate;
    asn_time2UT(&createDate, tmp_localtime, 0);
    eseal->esealInfo.property.createDate = createDate;
    eseal->esealInfo.property.validStart = createDate;
    eseal->esealInfo.property.validEnd = createDate;

    // 1.4填充印章图片信息
    char* picType = "png";
    eseal->esealInfo.picture.type.buf = picType;
    eseal->esealInfo.picture.type.size = strlen(picType);
    char* picData = "base64-////9/f/19f/q6v/g4P/ZGT/UFD/LCz/HBz";
    eseal->esealInfo.picture.data.buf = picData;
    eseal->esealInfo.picture.data.size = strlen(picData);
    eseal->esealInfo.picture.width = 248;
    eseal->esealInfo.picture.height = 248;

    // 1.5填充印章签名信息
    eseal->signInfo.cert.cert.buf


    
    /* BER encode the data if filename is given */
    if(ac < 2) {
        fprintf(stderr, "Specify filename for BER output\n");
    } else {
        const char *filename = av[1];
        FILE *fp = fopen(filename, "wb"); /* for BER output */
        if(!fp) {
            perror(filename);
            exit(1);
        }
        /* Encode the Rectangle type as BER (DER) */
        ec = der_encode(&asn_DEF_SESeal, eseal, write_out, fp);
        fclose(fp);
        if(ec.encoded == -1) {
            fprintf(stderr, "Could not encode Rectangle (at %s)\n",
            ec.failed_type ? ec.failed_type->name : "unknown");
            exit(1);
        } else {
            fprintf(stderr, "Created %s with BER encoded Rectangle\n", filename);
        }
    }
    /* Also print the constructed Rectangle XER encoded (XML) */
    xer_fprint(stdout, &asn_DEF_SESeal, eseal);
    return 0; /* Encoding finished successfully */
}


//**************************************************************************************************
int decode(int argc, char* argv[]) {
    char buf[1024]; /* Temporary buffer */
    asn_dec_rval_t rval; /* Decoder return value */
    SESeal_t *eseal = 0; /* Type to decode. Note this 01! */
    FILE *fp; /* Input file handler */
    size_t size; /* Number of bytes read */
    char *filename; /* Input file name */

    /* Require a single filename argument */
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file.ber>\n", argv[0]);
        exit(1);
    } else {
        filename = argv[1];
    }

    /* Open input file as read-only binary */
    fp = fopen(filename, "rb");
    if(!fp) {
        perror(filename);
        exit(1);
    }

    /* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    if(!size) {
        fprintf(stderr, "%s: Empty or broken\n", filename);
        exit(1);
    }
    /* Decode the input buffer as Rectangle type */
    rval = ber_decode(0, &asn_DEF_SESeal, (void **)&eseal, buf, size);
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken Rectangle encoding at byte %ld\n", filename, (long)rval.consumed);
        exit(1);
    }
    /* Print the decoded Rectangle type as XML */
    xer_fprint(stdout, &asn_DEF_SESeal, eseal);
    return 0; /* Decoding finished successfully */
}