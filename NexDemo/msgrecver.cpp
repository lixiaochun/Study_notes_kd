#include "stdafx.h"
#include "msgrecver.h"
//////////////////////////////////////////////////////////////////////////
//  COspAppBase
//

CAppBase::CAppBase()
{
    m_dwNodeId = INVALID_NODE;
}

CAppBase::~CAppBase()
{
}

////////////////////////////////////////////////////////////////////////////////
// 全局Osp应用程序对象
zTemplate<CMsgHandler, INSTANCE_NUM, CAppBase> g_AirDisApp;

////////////////////////////////////////////////////////////////////////////////
// CMsgHandler

CMsgHandler* CMsgHandler::s_pMsgHandler = NULL;

CMsgHandler::CMsgHandler()
{
    s_pMsgHandler = this;
}

CMsgHandler::~CMsgHandler()
{
    s_pMsgHandler = NULL;
}

void CMsgHandler::InstanceEntry(CMessage *const pMsg)
{
    /*得到当前消息的类型;*/
    u16 wCurEvent = pMsg->event;

    // 根据不同的消息类型进行处理;
    switch (wCurEvent)
    {
    case EV_NVMPAPP_REGISTER_RSP:
        //注册回复
        break;
    case EV_NVMPAPP_VIEWQKSHARE_Ntf:
        //投屏结果通知 BOOL32
        break;
    case EV_NVMPAPP_IMIX_SOCKET_LISTEN_Ntf:
        //socket 监听结点通知 u32
        break;
    case EV_NVMPAPP_IMIX_SOCKET_ACCEPT_Ntf:
        //socket accept通知
        break;
    default:

        break;
    }

    return;
}
