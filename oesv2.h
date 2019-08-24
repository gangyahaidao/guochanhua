/**
 * 版式软件签章接入接口，该文件由版式技术联盟维护
 */
#ifndef _OES_H_
#define _OES_H_

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)

#ifndef OES_CALL
#define OES_CALL __stdcall
#endif

#ifndef OES_API
#ifdef OES_EXPORTS
#define OES_API __declspec(dllexport)
#else
#define OES_API __declspec(dllimport)
#endif
#endif

#else

#ifndef OES_CALL
#ifdef _MIPS_ARCH
#define OES_CALL
#else
#define OES_CALL __attribute__((stdcall))
#endif
#endif

#ifndef OES_API
#ifdef OES_EXPORTS
#define OES_API __attribute__((visibility("default")))
#else
#define OES_API
#endif
#endif

#endif
/**************************2014年7月03日更新**************************/
//    添加 OES_GetSignMethod、OES_GetDigestMethod方法（这两个方法要同步）
//    添加 OES_Digest 方法
//    更改 OES_Sign方法，增加DocProperty、SignDateTime入参，修改SignValue出参定义
//    更改 OES_Verify方法，增加DocProperty入参，调整SignDateTime、SignValue顺序
//    只保留OES_OK常量定义，其他错误码可去掉，通过OES_GetErrMessage获得字符解析即可

/**************************2014年7月27日更新**************************/
//    添加 OES_GetSealImage 方法，增加 OES_SEALIMAGE_FLAG_XXXX 常量定义
//    对GetProviderInfo中版本返回值做出约定

/**************************2016年1月19日更新**************************/
//    添加 OES_GetSignImage 方法，签名值也包含图像数据，必要时可从中提取使用。

/**************************2016年8月15日更新**************************/
//    添加 OES_CANCEL，区分使用者主动取消和用

/**************************函数返回值常量定义*******************************/
#define OES_OK					        0x00000000	//调用成功
#define OES_CANCEL				        0x00000010	//使用者主动取消
//建议0x00000001~0x00001111之间预留使用,错误码不得占用

/**************************获取印章图像常量定义*******************************/
#define OES_SEALIMAGE_FLAG_DISPLAY		0x00000000	//显示
#define OES_SEALIMAGE_FLAG_PRINT		0x00000001	//打印
#define OES_SEALIMAGE_FLAG_PREVIEW		0x00000002	//打印预览

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 返回签章算法提供者信息 [Required]
 * @param[out]     puchName      名称（UTF-8编码），当为NULL时，通过piNameLen给出长度
 * @param[out/in]  piNameLen     名称长度
 * @param[out]     puchCompany   公司名称（UTF-8编码），当为NULL时，通过piCompanyLen给出长度
 * @param[out/in]  piCompanyLen  公司名称长度
 * @param[out]     puchVersion   版本（UTF-8编码,固定为2.0），当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen  版本长度
 * @param[out]     puchExtend    扩展信息（UTF-8编码），当为NULL时，通过piExtendLen给出长度
 * @param[out/in]  piExtendLen   扩展信息长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetProviderInfo(unsigned char* puchName, int* piNameLen,
                                           unsigned char* puchCompany, int* piCompanyLen,
                                           unsigned char* puchVersion, int* piVersionLen,
                                           unsigned char* puchExtend, int* piExtendLen);

/**
 * @brief 获取可用的电子印章列表，该函数可用来进行Name到ID的转换  [Required]
 * @param[out]     puchSealListData            印章列表数据（UTF-8编码），当为NULL时，通过piSealListDataLen给出长度
 *                                             形如 ID1\0Name1\0ID2\0Name2\0\0
 * @param[out/in]  piSealListDataLen           印章列表数据长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSealList(unsigned char* puchSealListData,int* piSealListDataLen);

/**
 * @brief 获取电子印章数据（含印模图像、校验证书和其他数据,符合国密标准）  [Required]
 * @param[in]      puchSealId              印章标识或名称（字符串）
 * @param[in]      iSealIdLen              印章标识或名称长度
 * @param[out]     puchSealData            印章数据（符合国密标准），当为NULL时，通过piSealListDataLen给出长度
 * @param[out/in]  piSealDataLen           印章数据长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSeal(unsigned char* puchSealId,int iSealIdLen,
                                   unsigned char* puchSealData,int* piSealDataLen);

/**
 * @brief 从电子印章数据中获取印章图像及尺寸  [Optional]
 * @param[in]      puchSealData             印章数据（符合国密标准）
 * @param[in]      iSealDataLen             印章数据长度
 * @param[in]      iRenderFlag              绘制用途标记，0表示显示，1表示打印，2表示预览
 * @param[out]     puchSealImage            印章图像数据，当为NULL时，通过piSealImageLen给出长度
 * @param[out/in]  piSealImageLen           印章图像数据长度
 * @param[out/in]  piSealWidth              印章宽度（单位mm）
 * @param[out/in]  piSealHeight             印章高度（单位mm）
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSealImage(unsigned char* puchSealData,int iSealDataLen,int iRenderFlag,unsigned char* puchSealImage,int* piSealImageLen,int* piSealWidth,int* piSealHeight);


/**
 * @brief 从电子签章章数据中获取印章图像及尺寸  [Optional]
 * @param[in]      puchSignedValueData      签章数据（符合国密标准）
 * @param[in]      iSignedValueLen          签章数据长度
 * @param[in]      iRenderFlag              绘制用途标记，0表示显示，1表示打印，2表示预览
 * @param[out]     puchSealImage            印章图像数据，当为NULL时，通过piSealImageLen给出长度
 * @param[out/in]  piSealImageLen           印章图像数据长度
 * @param[out/in]  piSealWidth              印章宽度（单位mm）
 * @param[out/in]  piSealHeight             印章高度（单位mm）
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSignImage(unsigned char* puchSignedValueData,int iSignedValueLen,int iRenderFlag,unsigned char* puchSealImage,int* piSealImageLen,int* piSealWidth,int* piSealHeight);

/**
 * @brief 获取电子印章信息  [Required]
 * @param[in]  puchSealData           印章数据（符合国密标准）
 * @param[in]  iSealDataLen           印章数据长度
 * @param[out]     puchSealId             头信息-印章标识，当为NULL时，通过piSealIdLen给出长度
 * @param[out/in]  piSealIdLen            头信息-印章标识长度
 * @param[out]     puchVersion            头信息-版本，当为NULL时，通过piVersionLen给出长度
 * @param[out/in]  piVersionLen           头信息-版本数据长度
 * @param[out]     puchVenderId           头信息-厂商标识，当为NULL时，通过piVenderIdLen给出长度
 * @param[out/in]  piVenderIdLen          头信息-厂商标识长度
 * @param[out]     puchSealType           印章信息-印章类型，当为NULL时，通过piSealTypeLen给出长度
 * @param[out/in]  piSealTypeLen          印章信息-印章类型长度
 * @param[out]     puchSealName           印章信息-印章名称，当为NULL时，通过piSealNameLen给出长度
 * @param[out/in]  piSealNameLen          印章信息-印章名称长度
 * @param[out]     puchCertInfo           印章信息-证书列表信息，当为NULL时，通过piCertInfoLen给出长度
 * @param[out/in]  piCertInfoLen          印章信息-证书列表信息长度
 * @param[out]     puchValidStart         印章信息-有效起始时间，当为NULL时，通过piValidStartLen给出长度
 * @param[out/in]  piValidStartLen        印章信息-有效起始时间长度
 * @param[out]     puchValidEnd           印章信息-有效结束时间，当为NULL时，通过piValidEndLen给出长度
 * @param[out/in]  piValidEndLen          印章信息-有效结束长度
 * @param[out]     puchSignedDate         印章信息-制作日期，当为NULL时，通过piSignedDateLen给出长度
 * @param[out/in]  piSignedDateLen        印章信息-制作日期长度
 * @param[out]     puchSignerName         签名信息-制章人，当为NULL时，通过piSignerNameLen给出长度
 * @param[out/in]  piSignerNameLen        签名信息-制章人长度
 * @param[out]     puchSignMethod         签名信息-制章签名方法，当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen        签名信息-制章签名方法长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSealInfo(unsigned char* puchSealData,int iSealDataLen,
                                        unsigned char* puchSealId,int* piSealIdLen,
                                        unsigned char* puchVersion,int* piVersionLen,
                                        unsigned char* puchVenderId,int* piVenderIdLen,
                                        unsigned char* puchSealType,int* piSealTypeLen,
                                        unsigned char* puchSealName,int* piSealNameLen,
                                        unsigned char* puchCertInfo,int* piCertInfoLen,
                                        unsigned char* puchValidStart,int* piValidStartLen,
                                        unsigned char* puchValidEnd,int* piValidEndLen,
                                        unsigned char* puchSignedDate,int* piSignedDateLen,
                                        unsigned char* puchSignerName,int* piSignerNameLen,
                                        unsigned char* puchSignMethod,int* piSignMethodLen);

/**
 * @brief 获取签名时间（时间戳或明文形式） [Required]
 * @param[out]     puchSignDateTime          签名时间（字符时用UTF-8编码，形如yyyy-MM-dd hh:mm:ss,时间戳时二进制值），当为NULL时，通过piSignDateTimeLen给出长度
 * @param[out/in]  piSignDateTimeLen         签名时间长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSignDateTime(unsigned char* puchSignDateTime,int* piSignDateTimeLen);

/**
 * @brief 获取签名算法标识 [Required]
 * @param[out]     puchSignMethod            签名算法（UTF-8编码），当为NULL时，通过piSignMethodLen给出长度
 * @param[out/in]  piSignMethodLen           签名算法长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetSignMethod(unsigned char* puchSignMethod,int* piSignMethodLen);

/**
 * @brief 获取摘要算法标识 [Required]
 * @param[out]     puchDigestMethod          摘要算法（UTF-8编码），当为NULL时，通过piDigestMethodLen给出长度
 * @param[out/in]  piDigestMethodLen         摘要算法长度
 * @return 调用成功返回OES_OK，否则是错误代码
 */
OES_API int OES_CALL    OES_GetDigestMethod(unsigned char* puchDigestMethod,int* piDigestMethodLen);

/**
 * @brief 代理计算摘要  [Required]
 * @param[in]      puchData                  待摘要的数据
 * @param[in]      iDataLen                  待摘要的数据长度
 * @param[in]      puchDigestMethod          摘要算法
 * @param[in]      iDigestMethodLen          摘要算法长度
 * @param[out]     puchDigestValue           摘要值，当为NULL时，通过piDigestValueLen给出长度
 * @param[out/in]  piDigestValueLen          摘要值长度
 * @return 调用成功返回OES_OK，否则是错误代码，可调用OES_GetErrMessage()获取详细信息。
 */
OES_API unsigned long OES_CALL    OES_Digest(unsigned char* puchData,int iDataLen,
                                unsigned char* puchDigestMethod,int iDigestMethodLen,
                                unsigned char* puchDigestValue,int* piDigestValueLen);

/**
 * @brief 代理计算签名,如果计算前需要输入密码，插件实现者需要提供输入界面  [Required]
 * @param[in]      puchSealId              印章标识
 * @param[in]      iSealIdLen              印章标识长度
 * @param[in]      puchDocProperty         文档信息，固定为Signature.xml的绝对路径
 * @param[in]      iDocPropertyLen         文档信息长度
 * @param[in]      puchDigestData          摘要数据
 * @param[in]      iDigestDataLen          摘要数据长度
 * @param[in]      puchSignMethod          签名算法
 * @param[in]      iSignMethodLen          签名算法长度
 * @param[in]      puchSignDateTime        签名时间
 * @param[in]      iSignDateTimeLen        签名时间长度
 * @param[out]     puchSignValue           签名值（符合国密标准），当为NULL时，通过piSignValueLen给出长度
 * @param[out/in]  piSignValueLen          签名值长度
 * @return 调用成功返回OES_OK，否则是错误代码，可调用OES_GetErrMessage()获取详细信息。
 */
OES_API unsigned long OES_CALL    OES_Sign(unsigned char* puchSealId,int iSealIdLen,
                                unsigned char* puchDocProperty,int iDocPropertyLen,
                                unsigned char* puchDigestData,int iDigestDataLen,
                                unsigned char* puchSignMethod,int iSignMethodLen,
                                unsigned char* puchSignDateTime,int iSignDateTimeLen,
                                unsigned char* puchSignValue,int* piSignValueLen);

/**
 * @brief 代理验签实现  [Required]
 * @param[in]  puchSealData            印章数据
 * @param[in]  iSealDataLen            印章数据长度
 * @param[in]  puchDocProperty         文档信息，固定为Signature.xml的绝对路径
 * @param[in]  iDocPropertyLen         文档信息长度
 * @param[in]  puchSignMethod          签名算法
 * @param[in]  iSignMethodLen          签名算法长度
 * @param[in]  puchSignDateTime        签名时间
 * @param[in]  piSignDateTimeLen       签名时间长度
 * @param[in]  puchSignValue           签名值
 * @param[in]  iSignValueLen           签名值长度
 * @param[in]  iOnline                 是否在线验证
 * @return 调用成功返回OES_OK，否则是错误代码，返回值包括三段（印章本地验证结果，印章在线验证结果，文档验证结果），
 *         在发生错误时，详细的错误信息需要调用OES_GetErrMessage()。
 */
OES_API unsigned long OES_CALL    OES_Verify(unsigned char* puchSealData,int iSealDataLen,
                                    unsigned char* puchDocProperty,int iDocPropertyLen,
                                    unsigned char* puchDigestData,int iDigestDataLen,
                                    unsigned char* puchSignMethod,int iSignMethodLen,
                                    unsigned char* puchSignDateTime,int iSignDateTimeLen,
                                    unsigned char* puchSignValue,int iSignValueLen,
                                    int iOnline);

/**
 * @brief 取得错误信息
 * @param[in]      errCode              错误代码，获得于OES_GetErrMessage()以外的任意函数
 * @param[out]     puchErrMessage       错误信息（UTF-8编码），应使用UTF-8编码，当为NULL时，通过piErrMessageLen给出长度
 * @param[out/in]  piErrMessageLen      错误信息长度
*/
OES_API int OES_CALL OES_GetErrMessage(unsigned long errCode,unsigned char* puchErrMessage,int* piErrMessageLen);


OES_API int OES_CALL OES_TestAdd(long a, long b, long* result);


#ifdef __cplusplus
}
#endif

#endif