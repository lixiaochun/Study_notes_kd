#include "stdafx.h"
#include "setmenulogic.h"
#include "toolframelogic.h"

template<> CSetMenuLogic* Singleton<CSetMenuLogic>::ms_pSingleton  = NULL;

APP_BEGIN_MSG_MAP(CSetMenuLogic, CNotifyUIImpl)
    MSG_CREATEWINDOW(_T("SetMenuLayout"), OnCreate)
    MSG_INIWINDOW(_T("SetMenuLayout"), OnInit)

    //MSG_KILLFOCUS(_T("SetMenuLayout"), OnCloseMenu)
    //MSG_KILLFOCUS(_T("SetMenuList"), OnCloseMenu)
    //MSG_KILLFOCUS(_T("ModifyPasswrd"), OnCloseMenu)
    //MSG_KILLFOCUS(_T("SetMenuList"), OnCloseMenu)
    MSG_ITEMCLICK(_T("ModifyPasswrd"), OnModifyPasswordEntry)
    MSG_ITEMCLICK(_T("About"), OnAboutItemClicked)
APP_END_MSG_MAP()

CSetMenuLogic::CSetMenuLogic()
{
}

CSetMenuLogic::~CSetMenuLogic()
{
}

bool CSetMenuLogic::OnCreate( TNotifyUI& msg )
{
    s32 nTop = 82;
    s32 nLeft = 980;

    if ( CPaintManagerUI::IsSelfAdaption() )
    {
        float fAdpX, fAdpY;
        CPaintManagerUI::GetAdpResolution( &fAdpX, &fAdpY );
        nLeft = s32(nLeft * fAdpX);
        nTop = s32(nTop * fAdpY);
    }

    SetWindowPos( m_pm->GetPaintWindow(), NULL, nLeft, nTop, 0, 0, SWP_NOSIZE |SWP_NOACTIVATE );
    //SetCapture(m_pm->GetPaintWindow());
    return false;
}

bool CSetMenuLogic::OnInit( TNotifyUI& msg )
{
    REG_RCKTOOL_MSG_WND_OB(m_pm->GetPaintWindow());

#if 0
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
#endif
    return true;
}

bool CSetMenuLogic::OnModifyPasswordEntry(TNotifyUI& msg)
{
    // 关闭设置菜单
    WINDOW_MGR_PTR->ShowWindow(g_stcStrSetMenuDlg.c_str(), false);
    m_pm->DoCase(_T("caseSetMenuClose"));

    // 进入密码修改
    CToolFrameLogic::GetSingletonPtr()->OnModifyPasswordEntry();

    return true;
}

bool CSetMenuLogic::OnAboutItemClicked(TNotifyUI& mst)
{
    // 关闭设置菜单
    WINDOW_MGR_PTR->ShowWindow(g_stcStrSetMenuDlg.c_str(), false);
    m_pm->DoCase(_T("caseSetMenuClose"));

    // 进入关于信息
    CToolFrameLogic::GetSingletonPtr()->OnTabAboutInfo();
    return true;
}

bool CSetMenuLogic::OnCloseMenu(TNotifyUI& msg)
{
    // 关闭设置菜单
    WINDOW_MGR_PTR->ShowWindow(g_stcStrSetMenuDlg.c_str(), false);
    return true;
}
