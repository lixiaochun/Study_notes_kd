#include "stdafx.h"
#include "ToolFrameLogic.h"

#define    BOTTOM_STRING    "设备IP:%s  设备型号:%s  版本号:%s  功耗:%dW/H  电流:%dA"      //用于下行leb显示

template<> CToolFrameLogic* Singleton<CToolFrameLogic>::ms_pSingleton  = NULL;

static UINT g_nMsgPowerTimerID;   //功耗信息实时获取定时器
VOID  CALLBACK  CToolFrameTimerPoc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    if ( idEvent == g_nMsgPowerTimerID )
    {
        CRkcComInterface->GetPowerConfig();
    }
}

APP_BEGIN_MSG_MAP(CToolFrameLogic, CNotifyUIImpl)
    MSG_CREATEWINDOW(_T("ToolFrameLayout"), OnCreate)
    MSG_INIWINDOW(_T("ToolFrameLayout"), OnInit)

    MSG_SELECTCHANGE(_T("NetworkSetupOpt"), OnTabNetworkSetup)
    MSG_SELECTCHANGE(_T("RadioSetupOpt"), OnTabRadioSetup)
    MSG_SELECTCHANGE(_T("WorkingModeOpt"), OnTabWorkingMode)
    MSG_SELECTCHANGE(_T("BackupUpgradeOpt"), OnTabBackupUpgrade)

    USER_MSG(UI_RKC_POWER_FLESH , OnRkcPowerReflesh)
    USER_MSG(UI_RKC_DISCONNECTED , OnRkcDisconnected)

APP_END_MSG_MAP()

CToolFrameLogic::CToolFrameLogic()
{
    g_nMsgPowerTimerID = 0;
    m_bIsFirstLogin = false;
}

CToolFrameLogic::~CToolFrameLogic()
{
}

bool CToolFrameLogic::OnCreate( TNotifyUI& msg )
{
    s32 nTop = DEFAULT_TITLE_HEIGHT;
    if ( CPaintManagerUI::IsSelfAdaption() )
    {
        float fAdpX, fAdpY;
        CPaintManagerUI::GetAdpResolution( &fAdpX, &fAdpY );
        nTop = s32(nTop * fAdpY);
    }
    SetWindowPos( m_pm->GetPaintWindow(), NULL, 0, nTop, 0, 0, SWP_NOSIZE |SWP_NOACTIVATE );
    return false;
}

bool CToolFrameLogic::OnInit( TNotifyUI& msg )
{
    REG_RCKTOOL_MSG_WND_OB(m_pm->GetPaintWindow());
    CRkcComInterface->KillTftpProcess();
    NOTIFY_MSG(UI_RKC_POWER_FLESH);
    NOTIFY_MSG(UI_RKC_NETWORK_REFLESH);
    NOTIFY_MSG(UI_RKC_WORKMODE_REFLESH);
    if (m_bIsFirstLogin)
    {
        CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
        if (pControl)
        {
            pControl->SelectItem(emTabID_ModifyPassword);
        }
        m_pm->DoCase(_T("caseMdPwdLaySel"));
    }
    return true;
}

void CToolFrameLogic::InitBottomLeb()
{
    TDevItem tLoginDevitem;
    CRkcComInterface->GetLoginDevItem(tLoginDevitem);
    u32 dwIP = htonl(tLoginDevitem.tDevInfo.tAddr.dwIpaddr);
    char achBottom[256] = {0};
    sprintf(achBottom, BOTTOM_STRING, inet_ntoa(*(in_addr*)&dwIP), tLoginDevitem.tDevID.achDevType, tLoginDevitem.tDevEx.achDevSoftVersion, "50W/H", "50A");
    IRkcToolCommonOp::SetControlText( CA2T(achBottom) , m_pm ,_T("BottomLeb"));
//     IRkcToolCommonOp::SetControlText( CA2T(inet_ntoa(*(in_addr*)&dwIP)) , m_pm ,_T("BottomIPLeb"));
//     IRkcToolCommonOp::SetControlText( CA2T(tLoginDevitem.tDevID.achDevType) , m_pm ,_T("BottomTypeLeb"));
//     IRkcToolCommonOp::SetControlText( CA2T(tLoginDevitem.tDevEx.achDevSoftVersion) , m_pm ,_T("BottomVerLeb"));

    //登录成功 获取配置数据内容
    CRkcComInterface->GetWorkModeConfig();
    CRkcComInterface->GetNetWorkConfig();
    CRkcComInterface->GetPowerConfig();
    g_nMsgPowerTimerID = SetTimer( NULL, NULL, 5000, CToolFrameTimerPoc );
}

void CToolFrameLogic::SetFirstLogin(bool bFirstLogin)
{
    m_bIsFirstLogin = bFirstLogin;
    if (m_bIsFirstLogin)
    {
        CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
        if (pControl)
        {
            pControl->SelectItem(emTabID_ModifyPassword);
        }
        m_pm->DoCase(_T("caseMdPwdLaySel"));
    }
}

bool CToolFrameLogic::OnTabNetworkSetup(TNotifyUI& msg)
{
    CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_NetworkSetup);
    }
    return true;
}

bool CToolFrameLogic::OnTabRadioSetup(TNotifyUI& msg)
{
    CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_RadioSetup);
    }
    return true;
}

bool CToolFrameLogic::OnTabWorkingMode(TNotifyUI& msg)
{
    CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_WorkingMode);
    }
    return true;
}

bool CToolFrameLogic::OnTabBackupUpgrade(TNotifyUI& msg)
{
    CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_BackupUpgrade);
    }
    return true;
}

void CToolFrameLogic::OnModifyPasswordEntry()
{
    CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_ModifyPassword);
    }

    m_pm->DoCase(_T("caseMdPwdLaySel"));
}

void CToolFrameLogic::OnTabAboutInfo()
{
    CTabLayoutUI *pControl = (CTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_AboutInfo);
    }

    m_pm->DoCase(_T("caseMdPwdLaySel"));
}

bool CToolFrameLogic::OnRkcPowerReflesh( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    TRK100PowerInfo tRK100PowerInfo;
    memset(&tRK100PowerInfo, 0, sizeof(TRK100PowerInfo));
    CRkcComInterface->GetPowerConfig(tRK100PowerInfo);

    TDevItem tLoginDevitem;
    CRkcComInterface->GetLoginDevItem(tLoginDevitem);

    u32 dwIP = htonl(tLoginDevitem.tDevInfo.tAddr.dwIpaddr);
    char achBottom[256] = {0};
    sprintf(achBottom, BOTTOM_STRING, inet_ntoa(*(in_addr*)&dwIP), tLoginDevitem.tDevID.achDevType, tLoginDevitem.tDevEx.achDevSoftVersion, tRK100PowerInfo.dwPower, tRK100PowerInfo.dwCurrent);
    IRkcToolCommonOp::SetControlText( CA2T(achBottom) , m_pm ,_T("BottomLeb"));

    return true;
}

bool CToolFrameLogic::OnRkcDisconnected( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    m_bIsFirstLogin = false;
    KillTimer(NULL, g_nMsgPowerTimerID);
    g_nMsgPowerTimerID = 0;
    WINDOW_MGR_PTR->ShowWindow(g_stcStrToolFrameDlg.c_str(),false);
    m_pm->DoCase(_T("caseIsnotLogin"));
    return true;
}