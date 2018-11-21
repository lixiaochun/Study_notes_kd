#include "stdafx.h"
#include "toolframelogic.h"
#include "modifypasswordlogic.h"
#include "tipboxlogic.h"

template<> CModifyPasswordLogic* Singleton<CModifyPasswordLogic>::ms_pSingleton  = NULL;

APP_BEGIN_MSG_MAP(CModifyPasswordLogic, CNotifyUIImpl)
    MSG_SETFOCUS(_T("InputCurPwd"), OnInputCurPassword)
    MSG_SETFOCUS(_T("InputNewPwd"), OnInputNewPassword)
    MSG_SETFOCUS(_T("InputCfmPwd"), OnInputCfmPassword)

    MSG_KILLFOCUS(_T("PasswordHelp"), OnHidePasswordHelp)

    MSG_TEXTCHANGED(_T("InputNewPwd"), OnInputNewPwdChange)

    MSG_CLICK(_T("CurPwdVisSwitchBtn"), OnCurPwdVisibleClicked)
    MSG_CLICK(_T("NewPwdVisSwitchBtn"), OnNewPwdVisibleClicked)
    MSG_CLICK(_T("CfmPwdVisSwitchBtn"), OnCfmPwdVisibleClicked)
    MSG_CLICK(_T("TextHelp"), OnShowPasswordHelp)
    MSG_CLICK(_T("SaveBtn"), OnSaveNewPwdClicked)

    USER_MSG(UI_RKC_MODIFY_PASSWORD , OnRkcPasswordModified)
    USER_MSG(UI_RKC_DISCONNECTED , OnRkcDisconnected)
APP_END_MSG_MAP()

CModifyPasswordLogic::CModifyPasswordLogic()
{
    m_bCurPwdIsJudged = false;
    m_bCurPwdIsRight = false;
    m_bNewPwdIsJudged = false;
    m_bNewPwdIsStandard = false;
}

CModifyPasswordLogic::~CModifyPasswordLogic()
{
}

bool CModifyPasswordLogic::OnSaveNewPwdClicked()
{
    if (!m_bNewPwdIsJudged)
    {
        // 判断新密码的密码规范
        if (!OnJudgeNewPassword())
        {
            m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
            return false;
        }
    }

    if (!m_bCurPwdIsRight || !m_bNewPwdIsStandard )
    {
        return false;
    }

    if (!OnJudgeCfmPassword())
    {
        m_pm->DoCase(_T("caseConfirmNewPwdFail"));
        return false;
    }

    m_pm->DoCase(_T("caseConfirmNewPwdPass"));

    // 设置新密码
    CString strPassWord = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputCfmPwd"))).c_str();
    if (!CRkcComInterface->SetLoginInfo(CT2A(strPassWord)))
    {
        CRkcComInterface->SendModifyPasswordReq();
    }

    return true;
}

LRESULT CModifyPasswordLogic::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    if (uMsg == WM_KEYDOWN)
    {
        if (wParam == VK_CONTROL)
        {
            bHandled = true;
        }
    }

    return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

bool CModifyPasswordLogic::OnInputCurPassword(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseInputCurPwd"));
    m_bCurPwdIsJudged = false;
    m_bCurPwdIsRight = false;
    m_bNewPwdIsJudged = false;
    m_bNewPwdIsStandard = false;

    return true;
}

bool CModifyPasswordLogic::OnJudgeCurPassword()
{
    TRK100LoginInfo tLoginInfo;
    CRkcComInterface->GetLoginInfo(tLoginInfo);
    m_bCurPwdIsJudged = true;

    CString strGetInputCurPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputCurPwd"))).c_str();

    if (!strncmp(tLoginInfo.szPwd, (CT2A)strGetInputCurPwd, 64))
    {
        m_bCurPwdIsRight = true;
        return true;
    }

    return false;
}

bool CModifyPasswordLogic::OnInputNewPassword(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseInputNewPwd"));
    m_bCurPwdIsJudged = false;
    m_bCurPwdIsRight = false;
    m_bNewPwdIsJudged = false;
    m_bNewPwdIsStandard = false;

    CString strGetInputCurPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputCurPwd"))).c_str();
    if (strGetInputCurPwd == _T(""))
    {
        m_pm->DoCase(_T("caseCurPwdIsEmpty"));
        OnInputNewPwdChange(msg);
        return false;
    }

    //判断当前密码的正确性
    if (!OnJudgeCurPassword())
    {
        m_pm->DoCase(_T("caseIsPwdFalse"));
        OnInputNewPwdChange(msg);
        return false;
    }

    m_pm->DoCase(_T("caseIsPwdTrue"));
    OnInputNewPwdChange(msg);

    return true;
}

bool CModifyPasswordLogic::OnInputNewPwdChange(TNotifyUI& msg)
{
    bool bIsContainNum = false;
    bool bIsContainAlp = false;
    bool bIsContainSym = false;
    bool bIsForbiddenChar = false;           // 非"."和"_"的特殊字符
    u32  dwSymbolCnt = 0;                    // 特殊字符计数

    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    if (strGetInputNewPwd != _T(""))
    {
        s32 nStrLength = strGetInputNewPwd.GetLength();
        for (s32 nIndex = 0; nIndex < nStrLength; nIndex++)
        {
            s32 nUnit = strGetInputNewPwd.GetAt(nIndex);
            if (nUnit > 127)
            {
                m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
                return false;
            }

            if (isalpha(nUnit))
            {
                bIsContainAlp = true;
            }
            else if (!isalnum(nUnit))
            {
                bIsContainSym = true;

                if (nUnit == '.' || nUnit == '_')
                {
                    dwSymbolCnt++;
                }
                else
                {
                    bIsForbiddenChar = true;
                }
            }
            else
            {
                bIsContainNum = true;
            }
        }

        // 新密码规范性判断
        m_bNewPwdIsJudged = true;
        m_bNewPwdIsStandard = false;

        if (nStrLength < 8)
        {
            m_pm->DoCase(_T("caseInputNewPwd"));
            return false;
        }

        if (!bIsContainSym)
        {
            // 口令中仅包含数字、字母时，密码强度弱
            if (bIsContainAlp && bIsContainNum)
            {
                m_pm->DoCase(_T("caseNewPwdIsWeak"));
                m_bNewPwdIsStandard = true;
                return true;
            }

            // 密码不符合规范
            m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
            return false;
        }
        else if (!bIsForbiddenChar)
        {
            if (bIsContainAlp || bIsContainNum)
            {
                if (dwSymbolCnt == 1)
                {
                    // 口令中包含数字、字母和任一特殊字符时，密码强度中
                    m_pm->DoCase(_T("caseNewPwdIsMedium"));
                    m_bNewPwdIsStandard = true;
                    return true;
                }

                if (dwSymbolCnt > 1)
                {
                    // 口令中包含数字、字母和多个特殊字符时，密码强度强
                    m_pm->DoCase(_T("caseNewPwdIsStrong"));
                    m_bNewPwdIsStandard = true;
                    return true;
                }

                // 密码不符合规范
                m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
                return false;
            }

            // 密码不符合规范
            m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
            return false;
        }
        else
        {
            // 密码不符合规范
            m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
            return false;
        }
    }

    //m_pm->DoCase(_T("caseInputNewPwd"));
    return true;
}

bool CModifyPasswordLogic::OnJudgeNewPassword()
{
    // 判断新密码的密码规范
    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();

    if (strGetInputNewPwd.GetLength() < 8)
    {
        return false;
    }

    return true;
}

bool CModifyPasswordLogic::OnInputCfmPassword(TNotifyUI& msg)
{
    OnInputNewPwdChange(msg);

    m_pm->DoCase(_T("caseInputCfmNewPwd"));
    m_bNewPwdIsJudged = false;
    //m_bNewPwdIsStandard = false;

    if (!m_bCurPwdIsJudged)
    {
        CString strGetInputCurPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputCurPwd"))).c_str();
        if (strGetInputCurPwd == _T(""))
        {
            m_pm->DoCase(_T("caseCurPwdIsEmpty"));
        }
        else
        {
            //判断当前密码的正确性
            if (!OnJudgeCurPassword())
            {
                m_pm->DoCase(_T("caseIsPwdFalse"));
            }
            else
            {
                m_pm->DoCase(_T("caseIsPwdTrue"));
            }
        }
    }

    // 判断新密码的密码规范
    if (!OnJudgeNewPassword())
    {
        m_pm->DoCase(_T("caseNewPwdIsNonstandard"));
    }

    return true;    
}

bool CModifyPasswordLogic::OnJudgeCfmPassword()
{
    CString strGetInputNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputNewPwd"))).c_str();
    CString strGetConfirmNewPwd = (IRkcToolCommonOp::GetControlText( m_pm ,_T("InputCfmPwd"))).c_str();

    if (strGetInputNewPwd != strGetConfirmNewPwd)
    {
        return false;
    }

    return true;
}

bool CModifyPasswordLogic::OnCurPwdVisibleClicked(TNotifyUI& msg)
{
    CEditUI *pControl = (CEditUI*)IRkcToolCommonOp::FindControl( m_pm, _T("InputCurPwd") );
    if (pControl)
    {
        if (pControl->IsPasswordMode())
        {
            m_pm->DoCase(_T("caseCurPwdIsVisible"));
            m_pm->SetFocus(pControl);
            return true;
        }

        m_pm->DoCase(_T("caseCurPwdIsInvisible"));
        m_pm->SetFocus(pControl);
    }

    return true;
}

bool CModifyPasswordLogic::OnNewPwdVisibleClicked(TNotifyUI& msg)
{
    CEditUI *pControl = (CEditUI*)IRkcToolCommonOp::FindControl( m_pm, _T("InputNewPwd") );
    if (pControl)
    {
        if (pControl->IsPasswordMode())
        {
            m_pm->DoCase(_T("caseNewPwdIsVisible"));
            m_pm->SetFocus(pControl);
            return true;
        }

        m_pm->DoCase(_T("caseNewPwdIsInvisible"));
        m_pm->SetFocus(pControl);
    }

    return true;
}

bool CModifyPasswordLogic::OnCfmPwdVisibleClicked(TNotifyUI& msg)
{
    CEditUI *pControl = (CEditUI*)IRkcToolCommonOp::FindControl( m_pm, _T("InputCfmPwd") );
    if (pControl)
    {
        if (pControl->IsPasswordMode())
        {
            m_pm->DoCase(_T("caseCfmPwdIsVisible"));
            m_pm->SetFocus(pControl);
            return true;
        }

        m_pm->DoCase(_T("caseCfmPwdIsInvisible"));
        m_pm->SetFocus(pControl);
    }

    return true;
}

bool CModifyPasswordLogic::OnShowPasswordHelp(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseShowHelpInfo"));

    CHorizontalLayoutUI *pControl = (CHorizontalLayoutUI*)IRkcToolCommonOp::FindControl( m_pm, _T("PasswordHelp") );
    if (pControl)
    {
        m_pm->SetFocus(pControl);
    }

    return true;
}

bool CModifyPasswordLogic::OnHidePasswordHelp(TNotifyUI& msg)
{
    m_pm->DoCase(_T("caseHideHelpInfo"));

    return true;
}

bool CModifyPasswordLogic::OnSaveNewPwdClicked(TNotifyUI& msg)
{
    return OnSaveNewPwdClicked();
}

bool CModifyPasswordLogic::OnRkcPasswordModified( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    bool bIsModified = (bool)wparam;
    EMRK100OptRtn emErr = (EMRK100OptRtn)lparam;
    if (bIsModified == false)
    {
        /*switch (emErr)
        {
        case RK100_OPT_ERR_USER:
            ShowTip(_T("用户名错误"));
            break;
        case RK100_OPT_ERR_PASSWORD:
            ShowTip(_T("密码错误"));
            break;
        default:
            ShowTip(_T("登录失败"));
            break;
        }*/
    }
    else
    {
        CToolFrameLogic::GetSingletonPtr()->SetFirstLogin(false);
        ShowTipBox(_T("密码修改成功"));        
    }
    return true;
}

bool CModifyPasswordLogic::OnRkcDisconnected( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    OnResetAllInput();
    return true;
}

void CModifyPasswordLogic::OnResetAllInput()
{
    m_pm->DoCase(_T("caseInputCurPwd"));

    IRkcToolCommonOp::SetControlText(_T(""), m_pm, _T("InputCurPwd"));
    IRkcToolCommonOp::SetControlText(_T(""), m_pm, _T("InputNewPwd"));
    IRkcToolCommonOp::SetControlText(_T(""), m_pm, _T("InputCfmPwd"));

    return;
}