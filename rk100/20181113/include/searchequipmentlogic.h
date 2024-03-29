/** @defgroup 主窗口逻辑单元 
 *  @version V1.0.0
 *  @author  ylp
 *  @date    2018.9.12
 */
#if !defined(AFX_MAINFRAMELOGIC_H_)
#define AFX_MAINFRAMELOGIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSearchEquipmentLogic : public CNotifyUIImpl, public Singleton<CSearchEquipmentLogic> 
{
public:
	CSearchEquipmentLogic();
	~CSearchEquipmentLogic();

public:
    void SetDevItem(TDevItem tDevItem);
    TDevItem GetDevItem();

    CString GetIniFilePath();

protected:
	/** 窗口创建 
	*  @param[in] 消息
	*  @return 是否成功
	*/
	bool OnCreate(TNotifyUI& msg);

	/** 关闭窗口消息 
	 *  @param[in] 
	 *  @node 
	 *  @return 
     */
    bool OnDestory(TNotifyUI& msg);

	/** 初始化消息 
	 *  @param[in] 
	 *  @node 
	 *  @return 
     */
    bool OnInit(TNotifyUI& msg);

    //点击设置菜单
    bool OnOpenMenu(TNotifyUI& msg);
    //点击关闭菜单
    bool OnCloseMenu(TNotifyUI& msg);

    //点击最小化按钮
    bool OnMinBtnClicked(TNotifyUI& msg);
    //点击关闭按钮
    bool OnCloseBtnClicked(TNotifyUI& msg);
    //点击搜索按钮
    bool OnSearchBtnClicked(TNotifyUI& msg);
    //点击重置按钮
    bool OnResetBtnClicked(TNotifyUI& msg);
    //点击列表调试按钮
    bool OnItemOperateBtnClicked(TNotifyUI& msg);
    //登录后点击退出按钮
    bool OnExitBtnClicked(TNotifyUI& msg);

    bool OnRkcDevReflesh( WPARAM wparam, LPARAM lparam, bool& bHandle );

    bool OnRkcConnected( WPARAM wparam, LPARAM lparam, bool& bHandle );

    bool OnRkcDisconnected( WPARAM wparam, LPARAM lparam, bool& bHandle );

    bool OnRkcReBoot( WPARAM wparam, LPARAM lparam, bool& bHandle );

    APP_DECLARE_MSG_MAP()

private:
    static const String strEquipmentList;
    static const String strEquipmentListItem;

    TDevItem m_tDevItem;//保存调试的设备信息
};

#endif // !defined(AFX_MAINFRAMELOGIC_H_)