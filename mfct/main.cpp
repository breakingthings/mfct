#include "maindialog.h"

class MyApp : public CWinApp
{
	CWnd *m_pMainWnd;
public:
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

