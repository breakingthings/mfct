#include "stdafx.h"
#include "maindialog.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class MyApp : public CWinApp
{
	CWnd *m_pMainWnd;
public:
	MyApp()
	{
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	}

	~MyApp()
	{
	}

	BOOL InitInstance()
	{
		this->InitTables();
		MainDialog dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
		return TRUE;
	}

	void InitTables()
	{

		Session sess;
		sess << _T("CREATE TABLE IF NOT EXISTS trips (id INTEGER PRIMARY KEY ASC AUTOINCREMENT, deleted BOOLEAN)");
		sess.Execute();
		sess.ClearParams();
		sess << _T("CREATE TABLE IF NOT EXISTS customers (id INTEGER PRIMARY KEY ASC AUTOINCREMENT")
			_T(",name TEXT NOT NULL, address TEXT NOT NULL)");
		sess.Execute();

		sess << _T("CREATE TABLE IF NOT EXISTS deliveries (id INTEGER PRIMARY KEY ASC AUTOINCREMENT")
			_T(",trip_id INTEGER NOT NULL, customer_id INTEGER NOT NULL")
			_T(",name TEXT NOT NULL, unit TEXT NOT NULL")
			_T(",quantity INTEGER NOT NULL, price REAL, total REAL NOT NULL)");
		sess.Execute();
	}
};


MyApp app;

