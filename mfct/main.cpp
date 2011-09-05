#include "stdafx.h"
#include "resource.h"
#include "TripFactory.h"
#include "xmlimporter.h"


#include <map>
#include <vector>
namespace MSXML
{
#include <MsXml2.h>
}
//#import <msxml4.dll>

class MainDialog : public CDialogEx
{
private:
	//CMenu m_wndMainMenu;
	CStatic m_versionString;
	CComboBox m_Combo;

public:
	enum { IDD = IDD_DIALOG1 };

	MainDialog(CWnd* parent = NULL) : CDialogEx(MainDialog::IDD, parent)
	{
		
	}

	BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();
		std::shared_ptr<Trip> trip = TripFactory::AddTrip();
		this->InitColumns();
		this->BindTrips();
		return TRUE;
	}

	void BindComboBox(void)
	{
		this->m_Combo.AddString(_T("kama"));

	}


	void OnFileExit()
	{
		//this->SendMessage(
	}


	/*void OnSysCommand(UINT nID, LPARAM lParam)
	{

		if((nID & 0xFFF0) == SC_CLOSE)
		{
		}
	}*/

	void InitColumns()
	{
		int result = 0;
		CListCtrl *cl = (CListCtrl*)this->GetDlgItem(IDC_DELIVERIES);
		result = cl->InsertColumn(0, _T("No"), LVCFMT_CENTER, 30, 0);
		result = cl->InsertColumn(1, _T("Customer name"), LVCFMT_LEFT, 150, 1);
		result = cl->InsertColumn(2, _T("Address"), LVCFMT_LEFT, 150, 2);
		
		cl = (CListCtrl*)this->GetDlgItem(IDC_DELIVERY_ITEMS);
		result = cl->InsertColumn(0, _T("Id"), LVCFMT_CENTER, 30, 0);
		result = cl->InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100, 1);
		result = cl->InsertColumn(2, _T("Unit"), LVCFMT_LEFT, 100, 2);
		result = cl->InsertColumn(3, _T("Qty"), LVCFMT_LEFT, 100, 3);
		result = cl->InsertColumn(4, _T("Price"), LVCFMT_LEFT, 100, 4);
		result = cl->InsertColumn(5, _T("Total"), LVCFMT_LEFT, 100, 5);
	}

	std::vector<int> m_vector_combo_ids;
	//std::map<int, int> m_trip_combo_map;

	void BindTrips()
	{
		
		int index = 0;
		m_vector_combo_ids.clear();
		
		CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
		cb->Clear();
		
		shared_ptr<vector<Trip> > trips(TripFactory::GetAllTrips());
		

		vector<Trip>::iterator it;
		CString number = _T("%d");
		for(it = trips->begin(); it != trips->end(); ++it)
		{
			
			number.Format(_T("%d"), it->id);
			cb->AddString(number);
			m_vector_combo_ids.push_back(it->id);
			//m_trip_combo_map.insert(pair<int, int>(index, it->id));
		}

		cb->SetCurSel(0);

	}

	void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}

	DECLARE_MESSAGE_MAP();
	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnFileImport();
};


BEGIN_MESSAGE_MAP( MainDialog, CDialogEx)
	//ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_EXIT,OnFileExit)
	ON_CBN_SELENDOK(IDC_COMBO_TRIPS, &MainDialog::OnCbnSelendokCombo1)
	ON_COMMAND(ID_FILE_IMPORT, &MainDialog::OnFileImport)
END_MESSAGE_MAP()

void MainDialog::OnCbnSelendokCombo1()
{
	CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
	int selectedIndex = cb->GetCurSel();
	int tripIndex = this->m_vector_combo_ids.at(selectedIndex);
	CString cs;
	
	cs.Format(_T("%d"), tripIndex);

	this->GetDlgItem(IDC_TRIPID)->SetWindowText(cs);
}


void MainDialog::OnFileImport()
{
	
	CFileDialog fOpenDlg(TRUE, _T("xml"), _T(""), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
	_T("xml files (*.xml)|*.xml|"), this);

	///fOpenDlg.m_pOFN->lpstrTitle = _T("XML files");
	fOpenDlg.m_pOFN->lpstrInitialDir= _T("c:");
  
	if(fOpenDlg.DoModal()==IDOK)
	{
		CString file = (LPCTSTR)fOpenDlg.GetPathName();
		XMLImporter::LoadData(file);
		
	}
}



class MyApp : public CWinApp
{
	CWnd *m_pMainWnd;
public:
	BOOL InitInstance()
	{
		MainDialog dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();

		//this->m_pMainWnd = wnd;

		//this->m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};


MyApp app;