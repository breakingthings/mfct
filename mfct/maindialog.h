#ifndef _MAINDIALOG_H
#define _MAINDIALOG_H
#include <SDKDDKVer.h>

#ifndef _UNICODE
#define _UNICODE
#endif

#include <afx.h>
#include <afxext.h>
#include <afxwin.h>
#include <afxdisp.h>
#include <afxcontrolbars.h>
#include <afxdtctl.h>   

#include <map>
#include <vector>
#include "resource.h"
#include "tripfactory.h"
#include "xmlimporter.h"
#include "deliveryfactory.h"
#include "customerfactory.h"

using namespace std;

class MainDialog : public CDialogEx
{
private:
	CStatic m_versionString;
	CComboBox m_Combo;

public:
	enum { IDD = IDD_DIALOG1 };
	MainDialog(CWnd* parent = NULL);
	void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

	BOOL OnInitDialog();
	void OnFileExit();
	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnFileImport();
	afx_msg void OnLvnItemchangedCustomers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedDeliveryItems(NMHDR *pNMHDR, LRESULT *pResult);
private:
	std::vector<int> m_vector_combo_ids;
private:
	void InitColumns();
	void BindTrips();
	void BindTripCustomers(int trip_id);
	void BindTripDeliveries(int trip_id, int customer_id);
};


#endif