#ifndef DEVICE_H
#define DEVICE_H

#include "common.h"

typedef int8_t      INT8;
typedef int16_t     INT16;
typedef int32_t     INT32;
typedef uint8_t     UINT8;
typedef uint16_t    UINT16;
typedef uint32_t    UINT32;
typedef bool        BOOL;

typedef UINT8       BYTE;
typedef UINT8       CHAR;
typedef INT16       SHORT;
typedef UINT16      USHORT;
typedef INT32       LONG;
typedef UINT32      ULONG;
typedef UINT32      UINT;
typedef UINT16      WORD;
typedef UINT32      DWORD;
typedef UINT32      FLAGS;
typedef CHAR*       LPSTR;
typedef void*       HANDLE;
typedef HANDLE      DEVHANDLE;
typedef HANDLE      HAPPLICATION;
typedef HANDLE      HCONTAINER;

#define TRUE    0x00000001
#define FALSE   0x00000000
#define DEVAPI  _stdcall
#define ADMIN_TYPE  0
#define USER_TYPE   1

// 符合类型数据的定义

//版本
typedef struct Struct_Version {
    BYTE major;
    BYTE minor;
}VERSION;

//设备信息
typedef struct Struct_DEVINFO {
    VERSION Version;
    CHAR    Manufacturer[64];
    CHAR    Issuer[64];
    CHAR    Label[32];
    CHAR    SerialNumber[32];
    VERSION HWVersion;
    VERSION FirmwareVersion;
    ULONG   AlgSymCap;
    ULONG   AlgAsymCap;
    ULONG   AlgHashCap;
    ULONG   DevAuthAlgId;
    ULONG   TotalSpace;
    ULONG   FreeSpace;
    ULONG   MaxECCBufferSize;
    ULONG   MaxBufferSize;
    BYTE    Reserved[64];
}DEVINFO, *PDEVINFO;

// RSA公钥数据结构
#define MAX_RSA_MODULUS_LEN 256
#define MAX_RSA_EXPONENT_LEN 4
typedef struct Struct_RSAPUBLICKEYBLOB {
    ULONG   AlgID;
    ULONG   BitLen;
    BYTE    Modulus[MAX_RSA_MODULUS_LEN];
    BYTE    PublicExponent[MAX_RSA_EXPONENT_LEN];
}RSAPUBLICKEYBLOB, *PRSAPUBLICKEYBLOB;

// RSA私钥数据结构
typedef struct Struct_RSAPRIVATEKEYBLOB {
    ULONG   AlgID;
    ULONG   BitLen;
    BYTE    Modules[MAX_RSA_MODULUS_LEN];
    BYTE    PublicExponent[MAX_RSA_EXPONENT_LEN];
    BYTE    PrivateExponent[MAX_RSA_MODULUS_LEN];
    BYTE    Prime1[MAX_RSA_MODULUS_LEN/2];
    BYTE    Prime2[MAX_RSA_MODULUS_LEN/2];
    BYTE    Prime1Exponent[MAX_RSA_MODULUS_LEN/2];
    BYTE    Prime2Exponent[MAX_RSA_MODULUS_LEN/2];
    BYTE    Coefficient[MAX_RSA_MODULUS_LEN/2];
}RSAPRIVATEKEYBLOB, *PRSAPRIVATEKEYBLOB;

// ECC公钥算法数据结构
#define ECC_MAX_XCOORDINATE_BITS_LEN 512
#define ECC_MAX_YCOORDINATE_BITS_LEN 512
typedef struct Struct_ECCPUBLICKEYBLOB {
    ULONG   BitLen;
    BYTE    XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
    BYTE    YCoordinate[ECC_MAX_YCOORDINATE_BITS_LEN/8];
}ECCPUBLICKEYBLOB, *PECCPUBLICKEYBLOB;

// ECC私钥数据结构
#define ECC_MAX_MODULUS_BITS_LEN 512
typedef struct Struct_ECCPRIVATEKEYBLOB {
    ULONG   BitLen;
    BYTE    PrivateKey[ECC_MAX_MODULUS_BITS_LEN/8];
}ECCPRIVATEKEYBLOB, *PECCPRIVATEKEYBLOB;

//ECC密文数据结构
typedef struct Struct_ECCCIPHERBLOB {
    BYTE XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
    BYTE YCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
    BYTE    HASH[32];
    ULONG   CipherLen;
    BYTE    Cipher[1];
}ECCCIPHERBLOB, *PECCCIPHERBLOB;

// ECC签名数据结构
typedef struct Struct_ECCSIGNATUREBLOB {
    BYTE    r[ECC_MAX_XCOORDINATE_BITS_LEN/8];
    BYTE    s[ECC_MAX_XCOORDINATE_BITS_LEN/8];
}ECCSIGNATUREBLOB, *PECCSIGNATUREBLOB;

//分组密码参数
#define MAX_IV_LEN 32
typedef struct Struct_BLOCKCIPHERPARAM {
    BYTE    IV[MAX_IV_LEN];
    ULONG   IVLen;
    ULONG   PaddingType;
    ULONG   FeedBitLen;
}BLOCKCIPHERPARAM, *PBLOCKCIPHERPARAM;

// ECC加密密钥对保护结构
typedef struct SKF_ENVELOPEDKEYBLOB {
    ULONG   Version;
    ULONG   ulSymmAlgID;
    ULONG   ulBits;
    BYTE    cbEncryptedPriKey[64];
    ECCPUBLICKEYBLOB    PubKey;
    ECCCIPHERBLOB       ECCCipherBlob;
}ENVELOPEDKEYBLOB, *PENVELOPEDKEYBLOB;

// 文件属性
typedef struct Struct_FILEATTRIBUTE {
    CHAR    FileName[32];
    ULONG   FileSize;
    ULONG   ReadRights;
    ULONG   WriteRights;
}FILEATTRIBUTE, *PFILEATTRIBUTE;

// 权限类型
#define SECURE_NEVER_ACCOUNT    0x00000000
#define SECURE_ADM_ACCOUNT      0x00000001
#define SECURE_USER_ACCOUNT     0x00000010
#define SECURE_ANYONE_ACCOUNT   0x000000FF

// 设备状态
#define DEV_ABSENT_STATE    0x00000000
#define DEV_PRESENT_STATE   0x00000001
#define DEV_UNKNOW_STATE    0x00000002

// 函数原型的声明
//等待设备拔插事件
ULONG SKF_WaitForDevEvent(LPSTR szDevName, ULONG* pulDevNameLen, ULONG* pulEvent);

// 取消等待设备拔插事件
ULONG SKF_CancelWaitForDevEvent();

// 枚举设备
ULONG SKF_EnumDev(BOOL bPresent, LPSTR szNameList, ULONG* pulSize);

// 连接设备
ULONG SKF_ConnectDev(LPSTR szName, DEVHANDLE* phDev);

// 断开连接
ULONG SKF_DisConnectDev(DEVHANDLE hDev);

// 获取设备状态
ULONG SKF_GetDevState(LPSTR szDevName, ULONG* pulDevSize);

// 设置设备标签
ULONG SKF_SetLabel(DEVHANDLE hDev, LPSTR szLabel);

// 获取设备信息
ULONG SKF_GetDevInfo(DEVHANDLE hDev, DEVINFO* pDevInfo);

// 锁定设备
ULONG SKF_LockDev(DEVHANDLE hDev, ULONG ulTimeOut);

// 解锁设备
ULONG SKF_UnlockDev(DEVHANDLE hDev);

// 设备命令传输
ULONG SKF_Transmit(DEVHANDLE hDev, BYTE* pbCommand, ULONG ulCommandLen, BYTE* pbData, ULONG* pullDataLen);

//************************访问控制相关函数*****************
// 修改设备认证密钥
ULONG SKF_ChangeDevAuthKey(DEVHANDLE hDev, BYTE* pbKeyValue, ULONG ulKeyLen);

// 设备认证
ULONG SKF_DevAuth(DEVHANDLE hDev, BYTE* pbAuthData, ULONG ulLen);

// 修改PIN
ULONG SKF_ChangePIN(HAPPLICATION hApplication, ULONG ulPINType, LPSTR szOldPin, LPSTR szNewPin, ULONG* pulRetryCount);

// 获取PIN信息
ULONG SKF_GetPINInfo(HAPPLICATION hApplication, ULONG ulPINType, ULONG* pulMaxRetryCount, ULONG* pulRetryCount, BOOL* pbDefaultPin);

// 校验PIN
ULONG SKF_VerifyPIN(HAPPLICATION hApplication, ULONG ulPINType, LPSTR szPIN, ULONG* pulRetryCount);

// 解锁PIN
ULONG SKF_UnblockPIN(HAPPLICATION hApplication, LPSTR szAdminPIN, LPSTR szNewUserPIN, ULONG* pulRetryCount);

// 清除应用安全状态
ULONG SKF_ClearSecureState(HAPPLICATION hApplication);

//***********************应用管理函数*************************
// 创建应用
ULONG SKF_CreateApplication(DEVHANDLE hDev, LPSTR szAppName, LPSTR szAdminPin, DWORD dwAdminPinRetryCount, LPSTR szUserPin, DWORD dwUserPinRetryCount, DWORD dwCreateFileRights, HAPPLICATION* phApplication);

// 枚举应用
ULONG SKF_EnumApplication(DEVHANDLE hDev, LPSTR szAppName, ULONG* pulSize);

// 删除应用
ULONG SKF_DeleteApplication(DEVHANDLE hDev, LPSTR szAppName);

// 打开应用
ULONG SKF_OpenApplication(DEVHANDLE hDev, LPSTR szAppName, HAPPLICATION* phApplication);

// 关闭应用
ULONG SKF_CloseApplication(HAPPLICATION hApplication);

//*********************文件管理***************************
// 创建文件
ULONG SKF_CreateFile(HAPPLICATION hApplication, LPSTR szFileName, ULONG ulFileSize, ULONG ulReadRights, ULONG ulWriteRights);

// 删除文件
ULONG SKF_DeleteFile(HAPPLICATION hApplication, LPSTR szFileName);

// 枚举文件
ULONG SKF_EnumFiles(HAPPLICATION hApplication, LPSTR szFileList, ULONG* pulSize);

//获取文件属性
ULONG SKF_GetFileInfo(HAPPLICATION hApplication, LPSTR szFileName, FILEATTRIBUTE* pFileInfo);

// 读文件
ULONG SKF_ReadFile(HAPPLICATION hApplication, LPSTR szFileName, ULONG ulOffset, ULONG ulSize, BYTE* pbOutData, ULONG* pulOutLen);

// 写文件
ULONG SKF_WriteFile(HAPPLICATION hApplication, LPSTR szFileName, ULONG ulOffset, BYTE* pbData, ULONG ulSize);

// **********************容器概述********************
// 创建容器
ULONG SKF_CreateContainer(HAPPLICATION hApplication, LPSTR szContainerName, HCONTAINER* phContainer);

// 删除容器
ULONG SKF_DeleteContainer(HAPPLICATION hApplication, LPSTR szContainerName);

// 打开容器
ULONG SKF_OpenContainer(HAPPLICATION hApplication, LPSTR szContainerName, HCONTAINER* phContainer);

// 关闭容器
ULONG SKF_CloseContainer(HCONTAINER hContainer);

// 获得容器类型
ULONG SKF_GetContainerType(HCONTAINER hContainer, ULONG* pulContainerType);

// 导入数字证书
ULONG SKF_ImportCertificate(HCONTAINER hContainer, BOOL bSignFlag, BYTE* pbCert, ULONG ulCertLen);

// 导出数字证书
ULONG SKF_ExportCertificate(HCONTAINER hContainer, BOOL bSignFlag, BYTE* pbCert, ULONG* pulCertLen);

//*************************密码服务*************************
// 生成随机数
ULONG SKF_GenRandom(DEVHANDLE hDev, BYTE* pbRandom, ULONG ulRandomLen);

// 生成RSA签名密钥对
ULONG SKF_GenRSAKeyPair(HCONTAINER hContainer, ULONG ulBitsLen, RSAPUBLICKEYBLOB* pBlob);

// 导入RSA加密密钥对
ULONG SKF_ImportRSAKeyPair(HCONTAINER hContainer, ULONG ulSymAlgId, BYTE* pbWrappedKey, ULONG ulWrppedKeyLen, BYTE* pbEncryptedData, ULONG ulEncryptedDataLen);

// RSA签名
ULONG SKF_RSASignData(HCONTAINER hContainer, BYTE* pbData, ULONG ulDataLen, BYTE* pbSignature, ULONG* pulSignLen);

// RSA验签
ULONG SKF_RSAVerify(DEVHANDLE hDev, RSAPUBLICKEYBLOB* pRSAPubKeyBlob, BYTE* pbData, ULONG ulDataLen, BYTE* pbSignature, ULONGulSignLen);

// RSA生成并导出会话密钥
ULONG SKF_RSAExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId, RSAPUBLICKEYBLOB* pPubKey, BYTE* pbData, ULONG* pulDataLen, HANDLE* phSessionKey);

// 生成ECC签名密钥对
ULONG SKF_GenECCKeyPair(HCONTAINER hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB* pBlob);

// 导入ECC加密密钥对
ULONG SKF_ImportEccKeyPair(HCONTAINER hContainer, PENVELOPEDKEYBLOB pEnvelopedKeyBlob);

// ECC签名
ULONG SKF_ECCSignData(HCONTAINER hContainer, BYTE* pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

// ECC验签
ULONG SKF_ECCVerify(DEVHANDLE hDev, ECCPUBLICKEYBLOB* pECCPubKeyBlob, BYTE* pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

// ECC生成并导出会话密钥
ULONG SKF_ECCExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB* pPubKey, PECCCIPHERBLOB pData, HANDLE* phSessionKey);

// ECC外来公钥加密
ULONG SKF_ExtECCEncrypt(DEVHANDLE hDev, ECCPUBLICKEYBLOB* pECCPubKeyBlob, BYTE* pbPlainText, ULONG ulPlainTextLen, PECCCIPHERBLOB pCipherText);

// ECC生成密钥协商参数并输出
ULONG SKF_GenerateAgreementDataWithECC(HCONTAINER hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB* pTempECCPubKeyBlob, BYTE* pbID, ULONG ulIDLen, HANDLE* phAgreementHandle);

//ECC产生协商数据并计算会话密钥
ULONG SKF_GenerateAgreementDataAndKeyWithECC(HANDLE hContainer, ULONG ulAlgId, ECCPUBLICKEYBLOB* pSponsorECCPubKeyBlob, ECCPUBLICKEYBLOB* pSponsorTempECCPubKeyBlob, ECCPUBLICKEYBLOB* pTempECCPubKeyBlob, BYTE* pbID, ULONG ulIDLen, BYTE* pbSponsorID, ULONG ulSponsorIDLen, HANDLE* pbKeyHandle);

// ECC计算会话密钥
ULONG SKF_GenerateKeyWithECC(HANDLE hAgreementHandle, ECCPUBLICKEYBLOB* pECCPubKeyBlob, BYTE* pbID, ULONG ulIDLen, HANDLE* phKeyHandle);

// 导出公钥
ULONG  SKF_ExportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE* pbBlob, ULONG* pulBlobLen);

// 导入会话密钥
ULONG  SKF_ImportSessionKey(HCONTAINER hContainer, ULONG ulAlgId, BYTE* pbWrapedData, ULONG ulWrapedLen, HANDLE* phKey);

// 加密初始化
ULONG  SKF_EncryptInit(HANDLE hKey, BLOCKCIPHERPARAM EncryptParam);

// 单组数据加密
ULONG  SKF_Encrypt(HANDLE hKey, BYTE* pbData, ULONG ulDataLen, BYTE* pbEncryptedData, ULONG* pulEncryptedLen);

// 多组数据加密
ULONG  SKF_EncryptUpdate(HANDLE hKey, BYTE* pbData, ULONG ulDataLen, BYTE* pbEncryptedData, ULONG* pulEncryptedLen);

// 结束加密
ULONG  SKF_EncryptFinal(HANDLE hKey, BYTE* pbEncryptedData, ULONG* ulEncryptedDataLen);

// 解密初始化
ULONG  SKF_DecryptInit(HANDLE hKey, BLOCKCIPHERPARAM DecryptParam);

// 单组数据解密
ULONG  SKF_Decrypt(HANDLE hKey, BYTE* pbEncryptedData,ULONG ulEncryptedLen, BYTE* pbData, ULONG* pulDataLen);

// 多组数据解密
ULONG  SKF_DecryptUpdate(HANDLE hKey, BYTE* pbEncryptedData, ULONG ulEncryptedLen,BYTE* pbData, ULONG* pulDataLen);

// 结束解密
ULONG  SKF_DecryptFinal(HANDLE hKey, BYTE* pbEncryptedData, ULONG* pulDecryptedDataLen);

// 密码杂凑初始化
ULONG  SKF_DigestInit(DEVHANDLE hDev, ULONG ulAlgId, ECCPUBLICKEYBLOB* pPubKey ,unsigned char* pucID,ULONG ulIDLen,HANDLE* phHash);

// 单组数据密码杂凑
ULONG  SKF_Digest(HANDLE hHash,BYTE* pbData, ULONG ulDataLen, BYTE* pbHashData, ULONG* pulHashLen);

// 多组数据密码杂凑
ULONG  SKF_DigestUpdate(HANDLE hHash, BYTE* pbData, ULONG ulDataLen);

// 结束密码杂凑
ULONG  SKF_DigestFinal(HANDLE hHash, BYTE* pHashData, ULONG* pulHashLen);

// 消息鉴别码运算初始化
ULONG  SKF_MacInit(HANDLE hKey, BLOCKCIPHERPARAM* pMacParam, HANDLE* phMac);

// 单组数据消息鉴别码运算
ULONG  SKF_Mac(HANDLE hMac, BYTE* pbData, ULONG ulDataLen, BYTE* pbMacData ,ULONG* pulMacLen);

// 多组数据消息鉴别码运算
ULONG  SKF_MacUpdate(HANDLE hMac, BYTE* pbData, ULONG ulDataLen);

// 结束消息鉴别码运算
ULONG  SKF_MacFinal(HANDLE hMac, BYTE* pbMacData, ULONG* pulMacDataLen);

// 关闭密码对象句柄
ULONG SKF_CloseHandle(HANDLE hHandle);

#endif