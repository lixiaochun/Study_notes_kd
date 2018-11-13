#include "stdafx.h"
#include "searchequipmentlogic.h"
#include "toolframelogic.h"
#include "setmenulogic.h"
#include "modifypasswordlogic.h"
#include "messageboxlogic.h"
#include "tipboxlogic.h"
#include <dbt.h>

template<> CSearchEquipmentLogic* Singleton<CSearchEquipmentLogic>::ms_pSingleton  = NULL;

APP_BEGIN_MSG_MAP(CSearchEquipmentLogic, CNotifyUIImpl)
	MSG_CREATEWINDOW(_T("SearchEquipment"), OnCreate)
	MSG_INIWINDOW(_T("SearchEquipment"), OnInit)
	MSG_WINDOWDESTORY(_T("SearchEquipment"), OnDestory)

    MSG_CLICK(_T("setbtn"), OnOpenMenu)
    MSG_KILLFOCUS(_T("setbtn"), OnCloseMenu)

    MSG_CLICK(_T("minbtn"), OnMinBtnClicked)
    MSG_CLICK(_T("closebtn"), OnCloseBtnClicked)
    MSG_CLICK(_T("SearchBtn"), OnSearchBtnClicked)
    MSG_CLICK(_T("ResetBtn"), OnResetBtnClicked)
    MSG_CLICK(_T("OperateBtn"), OnItemOperateBtnClicked)
    MSG_CLICK(_T("exitbtn"), OnExitBtnClicked)

    USER_MSG(UI_RKC_RKDEV_REFLESH , OnRkcDevReflesh)
    USER_MSG(UI_RKC_CONNECTED , OnRkcConnected)
    USER_MSG(UI_RKC_DISCONNECTED , OnRkcDisconnected)
    USER_MSG(UI_RKC_REBOOT , OnRkcReBoot)
APP_END_MSG_MAP()

const String CSearchEquipmentLogic::strEquipmentList = _T("EquipmentList");
const String CSearchEquipmentLogic::strEquipmentListItem = _T("EquipmentListItem");

CSearchEquipmentLogic::CSearchEquipmentLogic()
{
}

CSearchEquipmentLogic::~CSearchEquipmentLogic()
{
}

bool CSearchEquipmentLogic::OnCreate( TNotifyUI& msg )
{
	HWND hWnd = m_pm->GetPaintWindow();
	LONG styleValue = ::GetWindowLong(hWnd, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	styleValue &= ~WS_SIZEBOX;//窗口大小不可拖拽
	::SetWindowLong(hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	return false;
}

bool CSearchEquipmentLogic::OnDestory( TNotifyUI& msg )
{
	TerminateProcess(GetCurrentProcess(), 0);
	return true;
}

bool CSearchEquipmentLogic::OnInit( TNotifyUI& msg )
{
	REG_RCKTOOL_MSG_WND_OB(m_pm->GetPaintWindow());

    CString strIniPath = GetIniFilePath();
    int wAutoL = GetPrivateProfileInt(_T("LoginInfo"),_T("AutoLogin"), 0,strIniPath);
    if (wAutoL != 0) //自动登录
    {
        TCHAR tchLoginIP[MAX_NAME_LEN]={0};
        TCHAR tchLoginType[MAX_NAME_LEN]={0};
        TCHAR tchLoginVer[MAX_NAME_LEN]={0};
        TCHAR tchUserName[MAX_NAME_LEN]={0};
        TCHAR tchPassWord[MAX_NAME_LEN]={0};
        GetPrivateProfileString(_T("LoginInfo"),_T("LoginIP"),_T(""), tchLoginIP, MAX_NAME_LEN-1, strIniPath);
        GetPrivateProfileString(_T("LoginInfo"),_T("LoginType"),_T(""), tchLoginType, MAX_NAME_LEN-1, strIniPath);
        GetPrivateProfileString(_T("LoginInfo"),_T("LoginVer"),_T(""), tchLoginVer, MAX_NAME_LEN-1, strIniPath);
        GetPrivateProfileString(_T("LoginInfo"),_T("UserName"),_T(""), tchUserName, MAX_NAME_LEN-1, strIniPath);
        GetPrivateProfileString(_T("LoginInfo"), _T("PassWord"),_T(""), tchPassWord, MAX_NAME_LEN-1, strIniPath);
        m_tDevItem.tDevInfo.tAddr.dwIpaddr = ntohl(inet_addr(CT2A(tchLoginIP)));
        memcpy(m_tDevItem.tDevID.achDevType, CT2A(tchLoginType), MAX_NAME_LEN);
        memcpy(m_tDevItem.tDevEx.achDevSoftVersion, CT2A(tchLoginVer), MAX_NAME_LEN);

        CRkcComInterface->SocketConnect(m_tDevItem, CT2A(tchUserName), CT2A(tchPassWord));
    }

    return true;
}


bool CSearchEquipmentLogic::OnMinBtnClicked(TNotifyUI& msg)
{
    WINDOW_MGR_PTR->ShowWindowMinsize(g_stcStrMainFrameDlg.c_str());
    return false;
}

bool CSearchEquipmentLogic::OnCloseBtnClicked(TNotifyUI& msg)
{
    CRkcComInterface->KillTftpProcess();
    TerminateProcess(GetCurrentProcess(), 0);  
    return false;
}

bool CSearchEquipmentLogic::OnSearchBtnClicked(TNotifyUI& msg)
{
    COptionUI* pTypeAll = (COptionUI*)IRkcToolCommonOp::FindControl( m_pm, _T("OptSearchTypeALL") );
    COptionUI* pTypeIP = (COptionUI*)IRkcToolCommonOp::FindControl( m_pm, _T("OptSearchTypeIP") );

    if (pTypeAll && pTypeIP)
    {
        EmSearchType emtype = emTypeAll;
        u32 dwStartIP = 0;
        u32 dwEndIP = 0;
        if (pTypeAll->IsSelected())
        {
            CRkcComInterface->BeginSearch(emTypeAll);
        }
        else if (pTypeIP->IsSelected())
        {
            CEditUI* pStartIP = (CEditUI*)IRkcToolCommonOp::FindControl( m_pm, _T("EdtStartIP") );
            CEditUI* pEndIP = (CEditUI*)IRkcToolCommonOp::FindControl( m_pm, _T("EdtEndIP") );
            if (pStartIP && pEndIP)
            {
                CString strstartIp = pStartIP->GetText();
                CString strendIp = pEndIP->GetText();
                if (!strstartIp.IsEmpty())
                {
                    dwStartIP = htonl(inet_addr(CT2A(strstartIp)));
                }
                if (!strendIp.IsEmpty())
                {
                    dwEndIP = htonl(inet_addr(CT2A(strendIp)));
                }
                CRkcComInterface->BeginSearch(emTypeIP , dwStartIP , dwEndIP);
            }
        }
    }
    return true;
}

bool CSearchEquipmentLogic::OnResetBtnClicked(TNotifyUI& msg)
{
#if 0
    CRkcComInterface->ReSet();

    COptionUI* pTypeAll = (COptionUI*)IRkcToolCommonOp::FindControl( m_pm, _T("OptSearchTypeALL") );
    COptionUI* pTypeIP = (COptionUI*)IRkcToolCommonOp::FindControl( m_pm, _T("OptSearchTypeIP") );
    if (pTypeAll)
    {
        pTypeAll->Selected(true);
    }
    IRkcToolCommonOp::SetControlText( _T("") , m_pm, _T("EdtStartIP") );
    IRkcToolCommonOp::SetControlText( _T("") , m_pm, _T("EdtEndIP") );

    CListUI* pEquipmentList = (CListUI*)IRkcToolCommonOp::FindControl( m_pm, strEquipmentList.c_str() );
    if (pEquipmentList)
    {
        pEquipmentList->RemoveAll();
    }
#else
    //界面变更
    m_pm->DoCase(_T("caseIsLogin"));
    //窗口变更
    //WINDOW_MGR_PTR->CloseWindow(g_stcStrLoginDlg.c_str());
    WINDOW_MGR_PTR->ShowWindow(g_stcStrToolFrameDlg.c_str(), true);
    CToolFrameLogic::GetSingletonPtr()->InitBottomLeb();
#endif
    return true;
}

bool CSearchEquipmentLogic::OnItemOperateBtnClicked(TNotifyUI& msg)
{
    CListUI* pEquipmentList = (CListUI*)IRkcToolCommonOp::FindControl( m_pm, strEquipmentList.c_str() );
    if (pEquipmentList)
    {
        int nIndex = pEquipmentList->GetItemIndex(msg.pSender->GetParent());
        std::vector<TDevItem> vecDevItem;
        CRkcComInterface->GetDevList(vecDevItem);
        if (nIndex >= vecDevItem.size())
        {
            return false;
        }
        SetDevItem(vecDevItem.at(nIndex));
        WINDOW_MGR_PTR->ShowWindow(g_stcStrLoginDlg.c_str());
    }
    return true;
}

bool CSearchEquipmentLogic::OnExitBtnClicked(TNotifyUI& msg)
{
    int nBoxRe = ShowMessageBox(_T("即将退出登录"),2);
    if (nBoxRe == IDOK)
    {
        CRkcComInterface->CloseSocket();
        WINDOW_MGR_PTR->ShowWindow(g_stcStrToolFrameDlg.c_str(),false);
        m_pm->DoCase(_T("caseIsnotLogin"));
    }
    return true;
}

bool CSearchEquipmentLogic::OnRkcDevReflesh(WPARAM wparam, LPARAM lparam, bool& bHandle)
{
    CListUI* pEquipmentList = (CListUI*)IRkcToolCommonOp::FindControl( m_pm, strEquipmentList.c_str() );
    if (pEquipmentList)
    {
        pEquipmentList->RemoveAll();
        std::vector<TDevItem> vecDevItem;
        CRkcComInterface->GetDevList(vecDevItem);
        for (unsigned int i = 0 ; i < vecDevItem.size() ; i++)
        {
            CListContainerElementUI *pListContainer = (CListContainerElementUI*)CONTROLFACTORY()->GetControl( strEquipmentListItem.c_str() );
            if (pListContainer)
            {
                CString str = _T("");
                str.Format(_T("%d"), pEquipmentList->GetCount() + 1);
                IRkcToolCommonOp::SetControlText( str , m_pm ,_T("IndexLab"), pListContainer);
                //类型
                IRkcToolCommonOp::SetControlText( CA2T(vecDevItem.at(i).tDevID.achDevType) , m_pm ,_T("ModelLab"), pListContainer);
                //IP
                u32 dwIP = htonl(vecDevItem.at(i).tDevInfo.tAddr.dwIpaddr);
                IRkcToolCommonOp::SetControlText( CA2T(inet_ntoa(*(in_addr*)&dwIP)) , m_pm ,_T("IPLab"), pListContainer);
                //SN
                IRkcToolCommonOp::SetControlText( CA2T(vecDevItem.at(i).tDevID.achDevSerial) , m_pm ,_T("SNLab"), pListContainer);
                //Version
                IRkcToolCommonOp::SetControlText( CA2T(vecDevItem.at(i).tDevEx.achDevSoftVersion) , m_pm ,_T("VersionLab"), pListContainer);
                pEquipmentList->Add(pListContainer);
            }
        }
    }
    return true;
}

bool CSearchEquipmentLogic::OnRkcConnected( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    bool bIsLogin = (bool)wparam;
    EMRK100OptRtn emErr = (EMRK100OptRtn)lparam;
    if (bIsLogin || emErr == RK100_OPT_ERR_FIRST_LOGIN)
    {
        //界面变更
        m_pm->DoCase(_T("caseIsLogin"));
        TRK100LoginInfo tLoginInfo;
        CRkcComInterface->GetLoginInfo(tLoginInfo);
        IRkcToolCommonOp::SetControlText( CA2T(tLoginInfo.szUsr) , m_pm ,_T("nameLab"));
        //窗口变更
        WINDOW_MGR_PTR->CloseWindow(g_stcStrLoginDlg.c_str());
        WINDOW_MGR_PTR->ShowWindow(g_stcStrToolFrameDlg.c_str(), true);
        //第一次登录强制改密码
        CToolFrameLogic::GetSingletonPtr()->SetFirstLogin(emErr == RK100_OPT_ERR_FIRST_LOGIN);
        //更新显示
        CToolFrameLogic::GetSingletonPtr()->InitBottomLeb();
    }
    else
    {
        m_pm->DoCase(_T("caseIsnotLogin"));
    }
    return true;
}

bool CSearchEquipmentLogic::OnRkcDisconnected( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    u8 byReason = (u8)wparam;
    m_pm->DoCase(_T("caseIsnotLogin"));
    if (wparam == 1)
    {
        ShowMessageBox(_T("账号已在其他地方登录"),1);
    }
    return true;
}

bool CSearchEquipmentLogic::OnRkcReBoot( WPARAM wparam, LPARAM lparam, bool& bHandle )
{
    if (wparam)//重启消息发送成功
    {
        CRkcComInterface->CloseSocket();
        WINDOW_MGR_PTR->ShowWindow(g_stcStrToolFrameDlg.c_str(),false);
        m_pm->DoCase(_T("caseIsnotLogin"));
    }
    return true;
}

void CSearchEquipmentLogic::SetDevItem(TDevItem tDevItem)
{
    m_tDevItem = tDevItem;
}

TDevItem CSearchEquipmentLogic::GetDevItem()
{
    return m_tDevItem;
}

bool CSearchEquipmentLogic::OnOpenMenu(TNotifyUI& msg)
{
    // 菜单界面显示
    WINDOW_MGR_PTR->ShowWindow(g_stcStrSetMenuDlg.c_str(), true);
    m_pm->DoCase(_T("caseSetMenuClose"));


    return true;
}

bool CSearchEquipmentLogic::OnCloseMenu(TNotifyUI& msg)
{
    // 菜单界面隐藏
    WINDOW_MGR_PTR->ShowWindow(g_stcStrSetMenuDlg.c_str(), false);

    return true;
}

CString CSearchEquipmentLogic::GetIniFilePath()
{
    TCHAR tchPath[MAX_PATH] = {0};
    CString strIniFilePath;
    if (S_OK == SHGetFolderPathW(NULL,CSIDL_APPDATA,NULL,0,tchPath))
    {
        CString strIniFileDir(tchPath);
        strIniFileDir = strIniFileDir + _T("\\rkc100");		
        if(!PathFileExists(strIniFileDir))
        {
            CreateDirectory(strIniFileDir, NULL);
        }
        strIniFilePath = strIniFileDir + _T("\\conf.ini");
    }

    return strIniFilePath;
}
