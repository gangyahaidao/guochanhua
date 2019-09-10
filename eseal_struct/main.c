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
UTCTime_t* checkTime2UT(struct tm* tm) {
    static UTCTime_t output;
    UTCTime_t* ret = asn_time2UT(&output, tm, 0);
    printf("output = %s\n", ret->buf);
    return ret;
}

int readFileContent(char* fileName, uint8_t *fileDataBuffer, size_t* fileDataLength) {
    // 获取文件大小
    FILE  *file = fopen(fileName,"r");
    if(file == NULL){
        perror("打开文件失败");
        return -1;
    }
    fseek(file,0,SEEK_END);//定位到文件尾部
    *fileDataLength = ftell(file);
    printf("fileDatalength = %f.2 kb\n", *fileDataLength/1024.0);

    fseek(file, 0, SEEK_SET); // 恢复文件指针到开始位置
    fileDataBuffer = calloc(1, *fileDataLength);
    size_t ret = fread(fileDataBuffer, *fileDataLength, 1, file);
    if(ret <= 0 ) {
        perror("读取文件错误");
        return -1;
    }
    fclose(file); // 关闭文件

    return 0;
}


int main(int argc, char* argv[]) {
    // 将UTC时间转换成本地时间
    checkUTC2Time("190824120917+0800", 1000000, 0);

    // 将本地时间转换成UTC时间
    time_t lt = time(NULL);
    struct tm * tmp_localtime = localtime(&lt);
    checkTime2UT(tmp_localtime);

    //return encode(argc, argv);
    return decode(argc, argv);
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
    printf("填充1.1信息\n");
    //1.2 填充印章头信息
    eseal->esealInfo.header.id = calloc(1, sizeof(IA5String_t)); // 头信息ID分配空间
    char* headerID = "ES";
    eseal->esealInfo.header.id->buf = headerID;
    eseal->esealInfo.header.id->size = strlen(headerID);
    long headerVersion = 10;
    eseal->esealInfo.header.version = headerVersion;
    char* headerVid = "电子印章平台";
    eseal->esealInfo.header.vid.buf = headerVid;
    eseal->esealInfo.header.vid.size = strlen(headerVid);
    printf("填充1.2信息\n");
    // 1.3填充印章属性信息
    int infoType = 2;
    eseal->esealInfo.property.type = infoType;
    char* infoName = "测试导出公章";
    eseal->esealInfo.property.name.buf = infoName;
    eseal->esealInfo.property.name.size = strlen(infoName);

    OCTET_STRING_t userCert;
    char* userCertStr = "MIIElDCCA3ygAwIBAgIKZEpPCQAAAABmHTANBgkqhkiG9w0BAQUFADAjMSEwHwYDVQQDDBjmoIflh4bnlLXlrZDljbDnq6DlubPlj7AwHhcNMTcwNjIxMDczOTI2WhcNMjcwNjIxMDc0OTI2WjA4MQswCQYDVQQGEwJDTjEpMCcGA1UEAwwg5ryU56S65LiT55So6K+B5LmmLS3nmb7miJDnp5HmioAwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAIQlqi2J9T279jd3c3/67GbarhWN6qD3mtSs6ek2g4uy/n759VvXrGH5guIUa4U+kv6IP9+TCYhf31tZcHh3tzyTNvwxhAwBdPh8rRsI3NCb6vp0vn5KbqZp+IowyC4CL5KgvQHwl+nP2lt3J1F2CmydxileVutwYv+AK4DLqhtXAgMBAAGjggI3MIICMzAOBgNVHQ8BAf8EBAMCBsAwHQYDVR0OBBYEFBWb6sXQ2up2OyWaSdxQrTbVPT76MB8GA1UdIwQYMBaAFLGoT2EIG7cCaIf/Sjhn0aV5RVnqMIH6BgNVHR8EgfIwge8wgeyggemggeaGSGZpbGU6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC8hNjgwNyE1MWM2ITc1MzUhNWI1MCE1MzcwITdhZTAhNWU3MyE1M2YwLmNybIZNaHR0cDovLzU4LjIxMS4xMDEuMTMzL0NlcnRFbnJvbGwvITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcmyGS2h0dHA6Ly9jYS5lc2VhbHMuY24vQ2VydEVucm9sbC8hNjgwNyE1MWM2ITc1MzUhNWI1MCE1MzcwITdhZTAhNWU3MyE1M2YwLmNybDCB1QYIKwYBBQUHAQEEgcgwgcUwXgYIKwYBBQUHMAKGUmZpbGU6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC9iaWNlbmctMDFfITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcnQwYwYIKwYBBQUHMAKGV2h0dHA6Ly81OC4yMTEuMTAxLjEzMy9DZXJ0RW5yb2xsL2JpY2VuZy0wMV8hNjgwNyE1MWM2ITc1MzUhNWI1MCE1MzcwITdhZTAhNWU3MyE1M2YwLmNydDAMBgNVHRMBAf8EAjAAMA0GCSqGSIb3DQEBBQUAA4IBAQANujl7KW6lBPCTKXR6m/56N60+fEUTwjC11hGbYIGdOS0E1IIL5uG2kE0Ao7+4AuP+hnR7bOs9F1cdgKytMkdPGsLzuJldyZ3mfK3lyLkM1DUfk7hd8lgHiQzvhQCELj+q5w2VEyoApMpbfsqKzwbOMIGmTsz+9v4odPMYtUVm1xBK3m4XOAMTMGLRrnv9Ej807eWxk3L/92QDx8+nIGqI7M1nBQO+swp8KkGd6ID5UusqHi3vv9nA1IgdEg4w7pzNJwTjKACrrdHGk1QaPjZsqYxIAVjyhDTjg2UMJ/6BJkHNyKzLAtN7Z/WLDg1E8YaNBTYa8WIPqOli0joraUZK";
    userCert.buf = userCertStr;
    userCert.size = strlen(userCertStr);    
    ASN_SEQUENCE_ADD(&(eseal->esealInfo.property.certList.list), &userCert); //增加了一个证书

    time_t lt = time(NULL);
    struct tm* tmp_localtime = localtime(&lt);
    UTCTime_t* utcRet = checkTime2UT(tmp_localtime);
    eseal->esealInfo.property.createDate = *utcRet; // 填充印章创建的时间
    eseal->esealInfo.property.validStart = *utcRet;
    eseal->esealInfo.property.validEnd = *utcRet;
    printf("填充1.3信息\n");

    // 1.4填充印章图片信息
    char* picType = "png";
    eseal->esealInfo.picture.type.buf = picType;
    eseal->esealInfo.picture.type.size = strlen(picType);
    uint8_t * picDataBuffer = calloc(1, 1000*1000); // 图片base64编码最大是1000kb
    size_t picFileLength = 0;
    int returnValue = readFileContent("./resource/sealpic.txt", picDataBuffer, &picFileLength);
    if(returnValue < 0) {
        printf("读取文件失败\n");
        return -1;
    }

    eseal->esealInfo.picture.data.buf = picDataBuffer;
    eseal->esealInfo.picture.data.size = picFileLength;
    eseal->esealInfo.picture.width = 248;
    eseal->esealInfo.picture.height = 248;
    printf("填充1.4信息\n");

    // 1.5填充印章签名信息
    char* sealMakerCert = "MIIEJjCCAw6gAwIBAgIKFfs+pAAAAAAtLTANBgkqhkiG9w0BAQUFADAjMSEwHwYDVQQDDBjmoIflh4bnlLXlrZDljbDnq6DlubPlj7AwHhcNMTQwODI2MDAxNTQ2WhcNMjQwODI2MDAyNTQ2WjAhMQswCQYDVQQGEwJDTjESMBAGA1UEAwwJ6bqm5r2u55SfMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDeFfI737GlKPsKm+yBaHA29M1iOlY5Qu3jUmBybgzYUcFlOv8A/ZUXCMmeV/hR5K1o5Xk/88kTRDykqjToVuwoTgx03BdEAmBzvxW+9OmRi61us2CReTa5ryREyR6fR0eLwZMgQtiOQNX0Mnnp+jHy/f4I2OH6DRdgtIqcsMtJRQIDAQABo4IB4DCCAdwwDgYDVR0PAQH/BAQDAgbAMB0GA1UdDgQWBBQx0jyeM68WyFC6KVnCciD1RPbjYTAfBgNVHSMEGDAWgBSxqE9hCBu3AmiH/0o4Z9GleUVZ6jCBqAYDVR0fBIGgMIGdMIGaoIGXoIGUhkhodHRwOi8vYmljZW5nLTAxL0NlcnRFbnJvbGwvITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcmyGSGZpbGU6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC8hNjgwNyE1MWM2ITc1MzUhNWI1MCE1MzcwITdhZTAhNWU3MyE1M2YwLmNybDCB0AYIKwYBBQUHAQEEgcMwgcAwXgYIKwYBBQUHMAKGUmh0dHA6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC9iaWNlbmctMDFfITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcnQwXgYIKwYBBQUHMAKGUmZpbGU6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC9iaWNlbmctMDFfITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcnQwDAYDVR0TAQH/BAIwADANBgkqhkiG9w0BAQUFAAOCAQEAGqqomYF4SpJvHIQ/qbDHIYNgVNYWEoDNoecxikCptUooUOcqXAANMQHuGjHzAipi/ZtaGIQadHRtfpDRLU6zLTHFyjxoFQr1+QwkSVUdEBK4DlNdTE0i213Ff4qhZOQGDecZHJEGCnwW09Ty5E88YvC2yyxb0W05BlYsq/YH1pjnAT+iVA6QNgBIJFYoh3Q0veDnb3m8k+ZJ0N18bXTbQLw1+fesDcacVWQt6E49HMdlzXGi3twfAS/DU40uLnjfowz933DFADeod0nZwfFT6r8+o5+UG6d6mpCewX2/5vGnhD3zJbA/TESxydKDk2ssvv5hp0rZ30+P9p+/GVkq1g==";
    eseal->signInfo.cert.buf = sealMakerCert;
    eseal->signInfo.cert.size = strlen(sealMakerCert);
    char* signType = "1.2.156.10197.1.501";
    eseal->signInfo.signatureAlgorithm.buf = signType;
    eseal->signInfo.signatureAlgorithm.size = strlen(signType);
    char* sealVerifyMsg= "MIIFPwYJKoZIhvcNAQcCoIIFMDCCBSwCAQExDzANBgkqhkiG9w0BAQUFADALBgkqhkiG9w0BBwGgggQqMIIEJjCCAw6gAwIBAgIKFfs+pAAAAAAtLTANBgkqhkiG9w0BAQUFADAjMSEwHwYDVQQDDBjmoIflh4bnlLXlrZDljbDnq6DlubPlj7AwHhcNMTQwODI2MDAxNTQ2WhcNMjQwODI2MDAyNTQ2WjAhMQswCQYDVQQGEwJDTjESMBAGA1UEAwwJ6bqm5r2u55SfMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDeFfI737GlKPsKm+yBaHA29M1iOlY5Qu3jUmBybgzYUcFlOv8A/ZUXCMmeV/hR5K1o5Xk/88kTRDykqjToVuwoTgx03BdEAmBzvxW+9OmRi61us2CReTa5ryREyR6fR0eLwZMgQtiOQNX0Mnnp+jHy/f4I2OH6DRdgtIqcsMtJRQIDAQABo4IB4DCCAdwwDgYDVR0PAQH/BAQDAgbAMB0GA1UdDgQWBBQx0jyeM68WyFC6KVnCciD1RPbjYTAfBgNVHSMEGDAWgBSxqE9hCBu3AmiH/0o4Z9GleUVZ6jCBqAYDVR0fBIGgMIGdMIGaoIGXoIGUhkhodHRwOi8vYmljZW5nLTAxL0NlcnRFbnJvbGwvITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcmyGSGZpbGU6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC8hNjgwNyE1MWM2ITc1MzUhNWI1MCE1MzcwITdhZTAhNWU3MyE1M2YwLmNybDCB0AYIKwYBBQUHAQEEgcMwgcAwXgYIKwYBBQUHMAKGUmh0dHA6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC9iaWNlbmctMDFfITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcnQwXgYIKwYBBQUHMAKGUmZpbGU6Ly9iaWNlbmctMDEvQ2VydEVucm9sbC9iaWNlbmctMDFfITY4MDchNTFjNiE3NTM1ITViNTAhNTM3MCE3YWUwITVlNzMhNTNmMC5jcnQwDAYDVR0TAQH/BAIwADANBgkqhkiG9w0BAQUFAAOCAQEAGqqomYF4SpJvHIQ/qbDHIYNgVNYWEoDNoecxikCptUooUOcqXAANMQHuGjHzAipi/ZtaGIQadHRtfpDRLU6zLTHFyjxoFQr1+QwkSVUdEBK4DlNdTE0i213Ff4qhZOQGDecZHJEGCnwW09Ty5E88YvC2yyxb0W05BlYsq/YH1pjnAT+iVA6QNgBIJFYoh3Q0veDnb3m8k+ZJ0N18bXTbQLw1+fesDcacVWQt6E49HMdlzXGi3twfAS/DU40uLnjfowz933DFADeod0nZwfFT6r8+o5+UG6d6mpCewX2/5vGnhD3zJbA/TESxydKDk2ssvv5hp0rZ30+P9p+/GVkq1jGB2jCB1wIBATAxMCMxITAfBgNVBAMMGOagh+WHhueUteWtkOWNsOeroOW5s+WPsAIKFfs+pAAAAAAtLTANBgkqhkiG9w0BAQUFADANBgkqhkiG9w0BAQEFAASBgETg61tee9o8DcNwkM4bPVqMjroiim01F5Fk6oNo5kB02myehLID2zB2gtu5Z+dgfnJcHiozu54G1XhlqEObkya7NERBCJ1ArR52E9RLc+kq4tRb61UFj+00jBoZBjvzesgLQRq0hfT/HrdxJUdhGXAxjoaD1GwoZ0ntCv1fzArE";
    eseal->signInfo.signData.buf = sealVerifyMsg;
    eseal->signInfo.signData.size = strlen(sealVerifyMsg);
    printf("填充1.5信息\n");
    
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
    char buf[1024*500] = {0}; /* Temporary buffer */
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

    printf("Certificate = %s\n", eseal->signInfo.cert.buf);
    return 0; /* Decoding finished successfully */
}