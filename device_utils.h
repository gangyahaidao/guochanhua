#ifndef DEVICE_UTILS_H
#define DEVICE_UTILS_H

#include "device.h"

/**
    读取应用中指定名字的文件
*/
int readFileFromKeyByName(HAPPLICATION phApplication, CHAR* readFileName, BYTE* fileDataBuffer, ULONG* fileDataLen);


/**
    将文件保存到key中
*/
int saveFileToKey(HAPPLICATION hApplication, CHAR* fileWriteName,  BYTE* fileWriteData, ULONG fileWriteLen);

/**
    删除指定名字的文件
*/
int deleteFileByName(HAPPLICATION phApplication, CHAR* fileName);


#endif