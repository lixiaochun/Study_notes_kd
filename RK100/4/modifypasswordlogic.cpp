#include "stdafx.h"
#include "modifypasswordlogic.h"

template<> CModifyPasswordLogic* Singleton<CModifyPasswordLogic>::ms_pSingleton  = NULL;

APP_BEGIN_MSG_MAP(CModifyPasswordLogic, CNotifyUIImpl)
    MSG_SETFOCUS(_T("InputCurPwd"), OnInputCurPassword)
    MSG_KILLFOCUS(_T("InputCurPwd"), OnJudgeCurPassword)
    MSG_SETFOCUS(_T("InputNewPwd"), OnInputNewPassword)
    MSG_TEXTCHANGED(_T("InputNewPwd"), OnInputNewPwdChange)
    MSG_KILLFOCUS(_T("InputNewPwd"), OnJudgeMewPassword)
    MSG_SETFOCUS(_T("ConfirmNewPwd"), OnInputConfirmNewPassword)
    MSG_KILLFOCUS(_T("ConfirmNewPwd"), OnJudgeConfirmNewPassword)
    MSG_CLICK(_T("TextHelp"), OnShowPasswordHelp)
    MSG_KILLFOCUS(_T("TextHelp"), OnHidePasswordHelp)
APP_END_MSG_MAP()

CModifyPasswordLogic::CModifyPasswordLogic()
{
}

CModifyPasswordLogic::~CModifyPasswordLogic()
{
}

//void CModifyPasswordLogic::OnModifyPasswordEntry()
//{
//    CSlideTabLayoutUI *pControl = (CSlideTabLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("ToolFrameSlideTab") );
//    if (pControl)
//    {
//        pControl->SelectItem(emTabID_ModifyPassword);
//    }
//}

bool CModifyPasswordLogic::OnInputCurPassword(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseInit"));
    return true;
}

bool CModifyPasswordLogic::OnJudgeCurPassword(TNotifyUI& msg)
{
    //CString strIniPath = CLoginLogic::GetIniFilePath();

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

bool CModifyPasswordLogic::OnInputNewPassword(TNotifyUI& msg)
{
    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    if (strGetInputNewPwd != _T(""))
    {
        OnInputNewPwdChange(msg);
        return true;
    }

    m_pm->DoCase(_T("caseInputConfirmNewPwd"));
    return true;
}

bool CModifyPasswordLogic::OnInputNewPwdChange(TNotifyUI& msg)
{
    bool bIsContainNum = false;
    bool bIsContainAlp = false;
    bool bIsContainSym = false;
    u32 dwSymbolCnt = 0;

    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    if (strGetInputNewPwd != _T(""))
    {
        s32 nStrLength = strGetInputNewPwd.GetLength();
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
        return true;
    }

    m_pm->DoCase(_T("caseInputNewPwd"));
    return true;
}

bool CModifyPasswordLogic::OnJudgeMewPassword(TNotifyUI& msg)
{
    return true;
}

bool CModifyPasswordLogic::OnInputConfirmNewPassword(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseInputConfirmNewPwd"));

    bool bIsContainNum = false;
    bool bIsContainAlp = false;
    bool bIsContainSym = false;
    u32 dwNumCnt = 0;

    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    if (strGetInputNewPwd)
    {
        s32 nStrLength = strGetInputNewPwd.GetLength();
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

bool CModifyPasswordLogic::OnJudgeConfirmNewPassword(TNotifyUI& msg)
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

bool CModifyPasswordLogic::OnShowPasswordHelp(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseShowHelpInfo"));
    
    return true;
}

bool CModifyPasswordLogic::OnHidePasswordHelp(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseHideHelpInfo"));

    return true;
}