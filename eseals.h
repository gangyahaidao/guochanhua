#ifndef ESEALS_H
#define ESEALS_H
// 主要用于定义符合国密标准的印章数据结构

//*******************************电子印章数据格式****************************
typedef struct {
    SES_Header header; //头信息

}
typedef struct {
    SES_SealInfo esealInfo; // 印章信息
    SES_SignInfo signInfo; // 制章人对印章签名的信息
} SESeal, *pSESeal;

#endif