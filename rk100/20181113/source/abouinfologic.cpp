#include "stdafx.h"
#include "aboutinfologic.h"

template<> CAboutInfoLogic* Singleton<CAboutInfoLogic>::ms_pSingleton  = NULL;

APP_BEGIN_MSG_MAP(CAboutInfoLogic, CNotifyUIImpl)

APP_END_MSG_MAP()

CAboutInfoLogic::CAboutInfoLogic()
{
}

CAboutInfoLogic::~CAboutInfoLogic()
{
}
