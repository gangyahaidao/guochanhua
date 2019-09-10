#include "device_utils.h"

/**
    读取应用中指定名字的文件
*/
int readFileFromKeyByName(HAPPLICATION phApplication, CHAR* readFileName, BYTE* fileDataBuffer, ULONG* fileDataLen) {
    // 枚举文件
    CHAR szFileList[512] = {0};
    ULONG szFileListSize = 512;
    CHAR fileName[128] = {0};
    ULONG fileNameIndex = 0;
    ULONG ret = SKF_EnumFiles(phApplication, szFileList, &szFileListSize);
    if(ret != SAR_OK) {
        printf("enum app files failed\n");
        return ret;
    }
    ULONG ulOffset = 0;
    int i = 0;
    for(i = 0; i < szFileListSize; i++) {
        if(szFileList[i] != '\0') {
            fileName[fileNameIndex] = szFileList[i];
            fileNameIndex++;
        }
        if(szFileList[i] != '\0' && szFileList[i+1] == '\0') { // 获取了一个完整的文件名称
            fileName[fileNameIndex] = '\0';
            // 获取文件属性信息
            FILEATTRIBUTE fileInfo;
            ret = SKF_GetFileInfo(phApplication, fileName, &fileInfo);
            if(strcmp((const char*)readFileName, (const char*)fileName) == 0) { // 如果找到指定名字的文件，则将数据读出
                // 动态分配内存
                fileDataBuffer = calloc(1, fileInfo.FileSize); // 动态分配内存
                *fileDataLen = fileInfo.FileSize;
                ret = SKF_ReadFile(phApplication, fileName, ulOffset, fileInfo.FileSize, fileDataBuffer, fileDataLen);
                if(ret != SAR_OK) {
                    printf("read file = %s, failed\n", fileName);
                }                
                break;
            } else { // 如果不是指定名字的文件，则累加文件的偏移量
                ulOffset += fileInfo.FileSize;
                fileNameIndex = 0;
                memset(fileName, 0, 128); // 清空文件名字缓冲区
            }            
        }
    }
    return ret; // 失败，没有读取到指定名字的文件数据
}

/**
    将文件保存到key中
*/
int saveFileToKey(HAPPLICATION phApplication, CHAR* fileWriteName,  BYTE* fileWriteData, ULONG fileWriteLen) {
    // 删除原来的文件
    ULONG ret = deleteFileByName(phApplication, fileWriteName);
    if(ret != SAR_OK) {
        printf("delete file failed, ret = 0x%x\n", ret);
        return ret;
    }

    // 首先获取已有文件的数据偏移量
    CHAR szFileList[512] = {0};
    ULONG szFileListSize = 512;
    CHAR fileName[128] = {0};
    ULONG fileNameIndex = 0;
    ret = SKF_EnumFiles(phApplication, szFileList, &szFileListSize);
    if(ret != SAR_OK) {
        printf("enum app files failed\n");
        return ret;
    }
    ULONG ulOffset = 0;
    int i = 0;
    for(i = 0; i < szFileListSize; i++) { // 获取当前已存在的所有文件的偏移量
        if(szFileList[i] != '\0') {
            fileName[fileNameIndex] = szFileList[i];
            fileNameIndex++;
        }
        if(szFileList[i] != '\0' && szFileList[i+1] == '\0') { // 获取了一个完整的文件名称
            fileName[fileNameIndex] = '\0';
            // 获取文件属性信息
            FILEATTRIBUTE fileInfo;
            ret = SKF_GetFileInfo(phApplication, fileName, &fileInfo);
            ulOffset += fileInfo.FileSize;
            fileNameIndex = 0;
            memset(fileName, 0, 128); // 清空文件名字缓冲区
        }
    }
    printf("write file ulOffset = %d\n", ulOffset);

    // 创建文件
    ret = SKF_CreateFile(phApplication, fileWriteName, fileWriteLen, SECURE_ADM_ACCOUNT, SECURE_ADM_ACCOUNT);
    if(ret != SAR_OK) {
        printf("create file failed ret = 0x%x\n", ret);
        return ret;
    }    

    // 将数据追加写入
    printf("fileWriteName = %s, fileWriteData = %s\n", fileWriteName, fileWriteData);
    ret = SKF_WriteFile(phApplication, fileWriteName, ulOffset, fileWriteData, fileWriteLen);
    if(ret != SAR_OK) {
        printf("write file failed, ret = 0x%x\n", ret);
        return ret;
    }
    return ret;
}

/**
    删除指定名字的文件
*/
int deleteFileByName(HAPPLICATION phApplication, CHAR* fileName) {
    ULONG ret = SKF_DeleteFile(phApplication, fileName);
    if(ret != SAR_OK) {
        printf("delete file failed, ret = 0x%x\n", ret);
    }

    return ret;
}

/**
    进行设备认证
*/
int doDeviceAuth(HCONTAINER hContainer) {
    // 导入会话密钥
    //ULONG ret = SKF_ImportSessionKey()

    // 加密初始化

    // 获取随机数

    // 对随机数进行加密

    // 设备认证
}