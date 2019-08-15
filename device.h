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

#endif