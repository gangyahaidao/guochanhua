#include "common.h"
#include "device.h"

int main(int argc, char* argv[]) {
    // 等待设备连接
    CHAR devNameBuffer[128] = {0};    
    ULONG ret = SAR_FAIL;

    // ULONG pulDevNameLen = 128;
    // LPSTR szDevName = devNameBuffer;
    // ULONG pulEvent = -1;
    // ret = SKF_WaitForDevEvent(szDevName, &pulDevNameLen, &pulEvent);
    // if(ret == SAR_OK) {
    //     printf("阻塞设备返回， 事件类型 = %d[1-IN/2-OUT], name = %s\n", pulEvent, szDevName);
    // }
    // if(pulEvent == 2) {
    //     printf("Device pull out\n");
    //     return 0;
    // }

    // 枚举设备列表
    CHAR szNameList[512] = {0};
    ULONG szPulSize = 512;
    ret = SKF_EnumDev(TRUE, szNameList, &szPulSize);
    if(ret != SAR_OK) {
        printf("enum device list failed\n");
        return -1;
    }
    printf("Enum device list: \n");
    int i = 0;
    for(i = 0; i < szPulSize; i++) {
        if(szNameList[i] != '\0') {
            putchar(szNameList[i]);
            devNameBuffer[i] = szNameList[i]; // 存储第一个设备的名称
        } else if(szNameList[i] == '\0' && szNameList[i+1] != '\0') { // 列举完了一个设备名称
            putchar('\n');
            devNameBuffer[i] = '\0';
            break;
        } else if(szNameList[i] == '\0' && szNameList[i+1] == '\0') {
            printf("\nenum device name list over\n");
            break;
        }
    }

    // 获取设备状态
    ULONG pulDevState;
    ret = SKF_GetDevState(devNameBuffer, &pulDevState);
    printf("get device state ret = 0x%x, pulDevState = %d\n", ret, pulDevState);

    // 连接到设备
    DEVHANDLE phDev;
    ret = SKF_ConnectDev(devNameBuffer, &phDev);
    if(ret != SAR_OK) {
        printf("connect to device failed, ret = 0x%x\n", ret);
    } else {
        printf("connect to device success\n");
    }

    // 设备认证
    BYTE* ulRandom = calloc(1, 10);
    ULONG ulRandomLen = 8;
    ret = SKF_GenRandom(phDev, ulRandom, ulRandomLen);
    if(ret != SAR_OK) {
        printf("device genrandom failed, ret = 0x%x\n", ret);
    }

    // 枚举应用
    ULONG pulSize = 128;
    CHAR szAppNameList[128] = {0};
    CHAR szAppName[64] = {0};
    ret = SKF_EnumApplication(phDev, szAppNameList, &pulSize);
    if(ret != SAR_OK) {
        printf("enum application failed\n");
        return -1;
    }
    printf("enum application name list:\n");
    for(i = 0; i < pulSize; i++) {
        if(szAppNameList[i] != '\0') {
            putchar(szAppNameList[i]);
            szAppName[i] = szAppNameList[i];
        }
        if(szAppNameList[i] != '\0' && szAppNameList[i+1] == '\0') { // 列举完了一个应用名称
            putchar('\n');
            szAppName[i] = szAppNameList[i];
            szAppName[i+1] = '\0';
            break;
        } else if(szAppNameList[i] == '\0' && szAppNameList[i+1] == '\0') {
            printf("\nenum application name list over\n");
            break;
        }
    }

    // 打开szAppName名称的应用
    HAPPLICATION phApplication = NULL;
    ret = SKF_OpenApplication(phDev, szAppName, &phApplication);
    if(ret != SAR_OK) {
        printf("open application name = %s failed\n", szAppName);
        return -1;
    }
    printf("open application name = %s\n", szAppName);

    // 枚举容器
    CHAR szContainerName[128] = {0};
    ULONG szContanerNameSize = 128;
    ret = SKF_EnumContainer(phApplication, szContainerName, &szContanerNameSize);
    if(ret != SAR_OK) {
        printf("enum container failed, ret = 0x%x\n", ret);
        return -1;
    }
    printf("szContanerNameSize = %d, szContanerName = %s\n", szContanerNameSize, szContainerName);

    // 校验PIN
    ULONG pulRetryCount = 0;
    ret = SKF_VerifyPIN(phApplication, ADMIN_TYPE, "111111", &pulRetryCount);
    if(ret != SAR_OK) {
        printf("verify_PIN failed, ret = 0x%x", ret);
        return -1;
    }
    printf("pinRetryCount = %d\n", pulRetryCount);

    // 创建容器
    // HCONTAINER phContainer;
    // LPSTR containerName = "testContainer";
    // ret = SKF_CreateContainer(phApplication, containerName, &phContainer);
    // if(ret != SAR_OK) {
    //     printf("create container failed, ret = 0x%x\n", ret);
    //     return -1;
    // }
    // printf("create container success\n");

    // 读取指定名字的文件内容
    // BYTE* fileDataBuffer = NULL;
    // ULONG readFileDataLen = 0;
    // ret = readFileFromKeyByName(phApplication, "hello", fileDataBuffer, &readFileDataLen);
    // printf("readFileDataLen = %d, ret = 0x%x, fileDataBuffer = %s\n", readFileDataLen, ret, fileDataBuffer);

    // 删除文件
    // ret = deleteFileByName(phApplication, "hello");
    // if(ret != SAR_OK) {
    //     printf("delete file failed, ret = 0x%x", ret);
    //     return ret;
    // }

    // 写入文件
    // BYTE* fileWriteData = "AAAAAAAAAAAA";
    // ret = saveFileToKey(phApplication, "eseal",  fileWriteData, strlen((const char*)fileWriteData));
    // if(ret != SAR_OK) {
    //     printf("write file failed, ret = 0x%x\n", ret);
    // }
    // printf("write file to application success\n");

    // 枚举应用中的文件信息
    CHAR szFileList[512] = {0};
    ULONG szFileListSize = 512;
    CHAR fileName[128] = {0};
    ULONG fileNameIndex = 0;
    ret = SKF_EnumFiles(phApplication, szFileList, &szFileListSize);
    if(ret != SAR_OK) {
        printf("enum app files failed\n");
        return -1;
    }
    
    ULONG ulOffset = 0;
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
            printf("file info: name = %s, size = %d, readrights = %d, writerights = %d\n", 
                fileInfo.FileName, fileInfo.FileSize, fileInfo.ReadRights, fileInfo.WriteRights);
            
            // 读取文件
            // BYTE* pbOutData = calloc(1, fileInfo.FileSize); // 动态分配内存
            // ULONG pulOutLen = fileInfo.FileSize;
            // ret = SKF_ReadFile(phApplication, fileName, ulOffset, fileInfo.FileSize, pbOutData, &pulOutLen);
            // if(ret != SAR_OK) {
            //     printf("read file = %s, failed\n", fileName);
            // }
            // // 将数据保存在本地
            // int fd = open((const char*)fileInfo.FileName, O_WRONLY|O_CREAT);
            // if(fd < 0) {
            //     perror("open file failed");
            // }
            // ssize_t write_ret = write(fd, pbOutData, pulOutLen);
            // if(write_ret == pulOutLen) {
            //     printf("save file data success\n");
            // } else {
            //     perror("save file data failed");
            // }
            // free(pbOutData);

            ulOffset += fileInfo.FileSize;
            fileNameIndex = 0;
        } else if(szFileList[i] == '\0' && szFileList[i+1] == '\0') {
            printf("enum file list over\n");
            break;
        }
    }

    return 0;
}