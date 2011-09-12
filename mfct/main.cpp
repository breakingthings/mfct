#include "stdafx.h"
#include "maindialog.h"
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

class MyApp : public CWinApp
{
	CWnd *m_pMainWnd;
public:
	MyApp()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	}

	~MyApp()
	{
	}

	BOOL InitInstance()
	{
		Session::InitDB();
		MainDialog dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
		return TRUE;
	}

};


MyApp app;

