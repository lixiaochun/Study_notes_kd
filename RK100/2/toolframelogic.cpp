#include "stdafx.h"
#include "ToolFrameLogic.h"

#define    BOTTOM_STRING    "设备IP:%s  设备型号:%s  版本号:%s  功耗:%s  电流:%s"      //用于下行leb显示

template<> CToolFrameLogic* Singleton<CToolFrameLogic>::ms_pSingleton  = NULL;

APP_BEGIN_MSG_MAP(CToolFrameLogic, CNotifyUIImpl)
    MSG_CREATEWINDOW(_T("SearchEquipment"), OnCreate)
    MSG_INIWINDOW(_T("SearchEquipment"), OnInit)

    MSG_SELECTCHANGE(_T("NetworkSetupOpt"), OnTabNetworkSetup)
    MSG_SELECTCHANGE(_T("RadioSetupOpt"), OnTabRadioSetup)
    MSG_SELECTCHANGE(_T("WorkingModeOpt"), OnTabWorkingMode)
    MSG_SELECTCHANGE(_T("BackupUpgradeOpt"), OnTabBackupUpgrade)
    //MSG_SELECTCHANGE(_T("ModifyPasswordOpt"), OnModifyPassword)

    //修改密码
    MSG_SETFOCUS(_T("InputCurPwd"), OnInputCurPassword)
    MSG_KILLFOCUS(_T("InputCurPwd"), OnJudgeCurPassword)
    MSG_SETFOCUS(_T("InputNewPwd"), OnInputNewPassword)
    MSG_TEXTCHANGED(_T("InputNewPwd"), OnInputNewPwdChange)
    MSG_KILLFOCUS(_T("InputNewPwd"), OnJudgeMewPassword)
    MSG_SETFOCUS(_T("ConfirmNewPwd"), OnInputConfirmNewPassword)
    MSG_KILLFOCUS(_T("ConfirmNewPwd"), OnJudgeConfirmNewPassword)

    MSG_CLICK(_T("TextHelp"), OnInputNewPwdHelp)
    MSG_CLICK(_T("save"), OnInputNewPwdHelp)
    //USER_MSG(UI_RKC_MODIFY_PASSWORD, OnModifyPassword)
APP_END_MSG_MAP()

CToolFrameLogic::CToolFrameLogic()
{
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
}

bool CToolFrameLogic::OnTabNetworkSetup(TNotifyUI& msg)
{
    CSlideTabLayoutUI *pControl = (CSlideTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_NetworkSetup);
    }
    return true;
}

bool CToolFrameLogic::OnTabRadioSetup(TNotifyUI& msg)
{
    CSlideTabLayoutUI *pControl = (CSlideTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_RadioSetup);
    }
    return true;
}

bool CToolFrameLogic::OnTabWorkingMode(TNotifyUI& msg)
{
    CSlideTabLayoutUI *pControl = (CSlideTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_WorkingMode);
    }
    return true;
}

bool CToolFrameLogic::OnTabBackupUpgrade(TNotifyUI& msg)
{
    CSlideTabLayoutUI *pControl = (CSlideTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_BackupUpgrade);
    }
    return true;
}

void CToolFrameLogic::OnModifyPasswordEntry()
{
    CSlideTabLayoutUI *pControl = (CSlideTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
    if (pControl)
    {
        pControl->SelectItem(emTabID_ModifyPassword);
    }
}

bool CToolFrameLogic::OnInputCurPassword(TNotifyUI& msg)
{
    /*CString strGetInputPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputPassword"))).c_str();
    if (!strGetInputPwd)
    {
    m_pm->DoCase(_T("caseInit"));

    }*/
    m_pm->DoCase(_T("caseInit"));

    return true;
}

bool CToolFrameLogic::OnJudgeCurPassword(TNotifyUI& msg)
{
    CString strGetInputCurPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputCurPwd"))).c_str();
    if (strGetInputCurPwd)
    {
        if (strGetInputCurPwd == _T("123123123qwe"))
        {
            m_pm->DoCase(_T("caseIsPwdTrue"));
        }
        else
        {
            m_pm->DoCase(_T("caseIsPwdFalse"));
        }
        
    }
    return true;
}

bool CToolFrameLogic::OnInputNewPassword(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseInputNewPwd"));
    return true;
}

bool CToolFrameLogic::OnInputNewPwdChange(TNotifyUI& msg)
{
    bool bIsContainNum = false;
    bool bIsContainAlp = false;
    bool bIsContainSym = false;
    u32 dwSymbolCnt = 0;

    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    s32 nStrLength = strGetInputNewPwd.GetLength();
    if (strGetInputNewPwd)
    {
        for (s32 nIndex = 0; nIndex < nStrLength; nIndex++)
        {
            s8 chUnit = strGetInputNewPwd.GetAt(nIndex);
            if (isalpha(chUnit))
            {
                bIsContainAlp = true;
            }
            else if (!isalnum(chUnit))
            {
                bIsContainSym = true;
                dwSymbolCnt++;
            }
            else
            {
                bIsContainNum = true;
            }
        }

        if (!bIsContainSym)
        {
            m_pm->DoCase(_T("caseNewPwdIsWeak"));
        }
        else if (dwSymbolCnt == 1)
        {
            m_pm->DoCase(_T("caseNewPwdIsMedium"));
        }
        else
        {
            m_pm->DoCase(_T("caseNewPwdIsStrong"));
        }
    }

    return true;
}

bool CToolFrameLogic::OnJudgeMewPassword(TNotifyUI& msg)
{
    /*bool bIsContainNum = false;
    bool bIsContainAlp = false;
    bool bIsContainSym = false;
    u32 dwNumCnt = 0;

    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    s32 nStrLength = strGetInputNewPwd.GetLength();
    if (strGetInputNewPwd)
    {
    for (s32 nIndex = 0; nIndex < nStrLength; nIndex++)
    {
    s8 chUnit = strGetInputNewPwd.GetAt(nIndex);
    if (isalpha(chUnit))
    {
    bIsContainAlp = true;
    }
    else if (!isalnum(chUnit))
    {
    bIsContainSym = true;
    }
    else
    {
    bIsContainNum = true;
    dwNumCnt++;
    }
    }

    if (dwNumCnt < 6 || dwNumCnt > 16)
    {
    m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
    return true;
    }

    if ((bIsContainNum + bIsContainSym + bIsContainAlp) < 2)
    {
    m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
    return true;
    }
    }*/

    return true;
}

bool CToolFrameLogic::OnInputConfirmNewPassword(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseInputConfirmNewPwd"));

    bool bIsContainNum = false;
    bool bIsContainAlp = false;
    bool bIsContainSym = false;
    u32 dwNumCnt = 0;

    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    s32 nStrLength = strGetInputNewPwd.GetLength();
    if (strGetInputNewPwd)
    {
        for (s32 nIndex = 0; nIndex < nStrLength; nIndex++)
        {
            s8 chUnit = strGetInputNewPwd.GetAt(nIndex);
            if (isalpha(chUnit))
            {
                bIsContainAlp = true;
            }
            else if (!isalnum(chUnit))
            {
                bIsContainSym = true;
            }
            else
            {
                bIsContainNum = true;
                dwNumCnt++;
            }
        }

        if (dwNumCnt < 6 || dwNumCnt > 16)
        {
            m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
            return true;
        }

        if ((bIsContainNum + bIsContainSym + bIsContainAlp) < 2)
        {
            m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
            return true;
        }
    }

    return true;
}

bool CToolFrameLogic::OnJudgeConfirmNewPassword(TNotifyUI& msg)
{
    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    CString strGetConfirmNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("ConfirmNewPwd"))).c_str();

    if (strGetInputNewPwd == strGetConfirmNewPwd)
    {
        m_pm->DoCase(_T("caseConfirmNewPwdPass"));
        return true;
    }
    m_pm->DoCase(_T("caseConfirmNewPwdFail"));

    return true;
}

bool CToolFrameLogic::OnInputNewPwdHelp(TNotifyUI& msg)
{
    WINDOW_MGR_PTR->ShowWindow(g_stcStrPasswordHelpDlg.c_str(), true);
    
    return true;
}
