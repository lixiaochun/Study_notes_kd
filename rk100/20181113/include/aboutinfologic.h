/** @defgroup 关于信息逻辑单元 
 *  @version V1.0.0
 *  @author  csq
 *  @date    2018.11.13
 */
#if !defined(AFX_ABOUTINFO_H_)
#define AFX_ABOUTINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define KILL_FOCUS_DELAY 200

class CAboutInfoLogic : public CNotifyUIImpl, public Singleton<CAboutInfoLogic> 
{
public:
	CAboutInfoLogic();
	~CAboutInfoLogic();

public:

protected:

    APP_DECLARE_MSG_MAP()

private:

};

#endif // !defined(AFX_ABOUTINFO_H_)