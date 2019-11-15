/* 客户端和服务端之间通信的消息头部结构定义 */
#define u32 unsigned int
#define u16 unsigned short int
#define s32 int
#define u8 unsigned char

typedef struct
{
    u32 dwHandle;             /* 消息令牌,透传 */
    u32 dwRsvd;               /* 备用 */
    u32 dwSerial;             /* 流水号 一个请求/应答中,该值保持不变 */
    u16 wExtLen;              /* 扩展头字节数,含填充字节 */
    u16 wReserved1;           /* 保留 */

    u32 dwProtocolVer;        /* 协议版本号, 由各单板自行定义,型如：1.1.1.1或0x01010101 */
    u32 dwEvent;              /* 事件,表示不同的操作码,见EMRK100EvtId定义 */
    s32 nArgv;                /* 设备处理函数返回的私有错误码或客户端传递的参数数据 */
    u16 wOptRtn;              /* 事件处理返回值。详见: EMRK100OptRtn */
    u16 wMsgLen;              /* 后续消息体长度 */
} TRK100MsgHead;