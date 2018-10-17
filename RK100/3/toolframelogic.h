/** @defgroup 工具主框架逻辑单元 
 *  @version V1.0.0
 *  @author  ylp
 *  @date    2018.10.9
 */
#if !defined(AFX_TOOLFRAME_H_)
#define AFX_TOOLFRAME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EmConfCtrlTabID
{
    emTabID_NetworkSetup = 0,
    emTabID_RadioSetup,
    emTabID_WorkingMode,
    emTabID_BackupUpgrade,
    emTabID_ModifyPassword
};

class CToolFrameLogic : public CNotifyUIImpl, public Singleton<CToolFrameLogic> 
{
public:
	CToolFrameLogic();
	~CToolFrameLogic();

public:
    //刷新界面下方控件
    void InitBottomLeb();
    //修改密码入口
    void OnModifyPasswordEntry();

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

    //网络设置
    bool OnTabNetworkSetup(TNotifyUI& msg);
    //射频设置
    bool OnTabRadioSetup(TNotifyUI& msg);
    //工作模式
    bool OnTabWorkingMode(TNotifyUI& msg);
    //备份升级
    bool OnTabBackupUpgrade(TNotifyUI& msg);

    //修改密码
    bool OnInputCurPassword(TNotifyUI& msg);
    bool OnJudgeCurPassword(TNotifyUI& msg);
    bool OnInputNewPassword(TNotifyUI& msg);
    bool OnInputNewPwdChange(TNotifyUI& msg);
    bool OnJudgeNewPassword(TNotifyUI& msg);
    bool OnInputConfirmNewPassword(TNotifyUI& msg);
    bool OnJudgeConfirmNewPassword(TNotifyUI& msg);
    bool OnCurPwdVisibleClicked(TNotifyUI& msg);
    bool OnNewPwdVisibleClicked(TNotifyUI& msg);
    bool OnCfmPwdVisibleClicked(TNotifyUI& msg);
    bool OnShowPasswordHelp(TNotifyUI& msg);
    bool OnHidePasswordHelp(TNotifyUI& msg);

    CString GetIniFilePath();

    APP_DECLARE_MSG_MAP()

private:

};

#endif // !defined(AFX_TOOLFRAME_H_)