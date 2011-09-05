#include <SDKDDKVer.h>
#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef _UNICODE
#define _AFXDLL
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

	MainDialog(CWnd* parent = NULL) : CDialogEx(MainDialog::IDD, parent)
	{

	}

	BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		this->InitColumns();
		this->BindTrips();
		return TRUE;
	}

	
	void OnFileExit()
	{

	}

	void InitColumns()
	{
		int result = 0;
		CListCtrl *cl = (CListCtrl*)this->GetDlgItem(IDC_CUSTOMERS);
		cl->SetExtendedStyle(LVS_EX_FULLROWSELECT);

		result = cl->InsertColumn(0, _T("No"), LVCFMT_CENTER, 30, 0);
		result = cl->InsertColumn(1, _T("Customer name"), LVCFMT_LEFT, 150, 1);
		result = cl->InsertColumn(2, _T("Address"), LVCFMT_LEFT, 150, 2);


		cl = (CListCtrl*)this->GetDlgItem(IDC_DELIVERY_ITEMS);
		cl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
		result = cl->InsertColumn(0, _T("Id"), LVCFMT_CENTER, 30, 0);
		result = cl->InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100, 1);
		result = cl->InsertColumn(2, _T("Unit"), LVCFMT_LEFT, 100, 2);
		result = cl->InsertColumn(3, _T("Qty"), LVCFMT_LEFT, 100, 3);
		result = cl->InsertColumn(4, _T("Price"), LVCFMT_LEFT, 100, 4);
		result = cl->InsertColumn(5, _T("Total"), LVCFMT_LEFT, 100, 5);
	}

	std::vector<int> m_vector_combo_ids;

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
		if(trips->size() > 0)
		{
			this->BindTripCustomers(trips->at(0).id);
		}

	}

	void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}

	DECLARE_MESSAGE_MAP();

	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnFileImport();

	void BindTripCustomers(int trip_id)
	{
		shared_ptr<Trip> trip = TripFactory::GetById(trip_id);

		if(!trip->IsNull())
		{
			shared_ptr<vector<Customer> > customers = CustomerFactory::GetByTripId(trip_id);
			vector<Customer>::reverse_iterator it;
			
			CListCtrl *cl = (CListCtrl*)this->GetDlgItem(IDC_CUSTOMERS);
			cl->DeleteAllItems();

			for(it = customers->rbegin(); it != customers->rend(); ++it)
			{
				int nIndex;
				int counter = 0;
				CString cs;
				cs.Format(_T("%d"), it->id);
				nIndex = cl->InsertItem(LVIF_TEXT|LVIF_STATE, 0, cs,0,0,0, NULL);
				cl->SetItemText(nIndex, 0, cs);
				cl->SetItemText(nIndex, 1, it->name);
				cl->SetItemText(nIndex, 2, it->address);
			}
		}
	}

	void BindTripDeliveries(int trip_id, int customer_id)
	{

		CListCtrl *cl = (CListCtrl*)this->GetDlgItem(IDC_DELIVERY_ITEMS);
		cl->DeleteAllItems();

		shared_ptr<vector<Delivery> > deliveries = DeliveryFactory::GetDeliveries(trip_id, customer_id);
		vector<Delivery>::reverse_iterator it;
		CString cs = "";

		for(it = deliveries->rbegin(); it != deliveries->rend(); ++it)
		{
			cs.Format(_T("%d"), it->id);
			int nIndex = cl->InsertItem(LVIF_TEXT|LVIF_STATE, 0, cs,0,0,0, NULL);


			cl->SetItemText(nIndex,0, cs);

			cl->SetItemText(nIndex, 1, it->name);

			cl->SetItemText(nIndex, 2, it->unit);

			cs.Format(_T("%d"), it->quantity);
			cl->SetItemText(nIndex, 3, cs);

			cs.Format(_T("%f"), it->price);
			cl->SetItemText(nIndex, 4, cs);

			cs.Format(_T("%f"), it->total);
			cl->SetItemText(nIndex, 5, cs);
		}

	}


	afx_msg void OnLvnItemchangedCustomers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedDeliveryItems(NMHDR *pNMHDR, LRESULT *pResult);
};


BEGIN_MESSAGE_MAP( MainDialog, CDialogEx)
	//ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_EXIT,OnFileExit)
	ON_CBN_SELENDOK(IDC_COMBO_TRIPS, &MainDialog::OnCbnSelendokCombo1)
	ON_COMMAND(ID_FILE_IMPORT, &MainDialog::OnFileImport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CUSTOMERS, &MainDialog::OnLvnItemchangedCustomers)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DELIVERY_ITEMS, &MainDialog::OnLvnItemchangedDeliveryItems)
END_MESSAGE_MAP()

void MainDialog::OnCbnSelendokCombo1()
{
	CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
	int selectedIndex = cb->GetCurSel();
	int trip_id = this->m_vector_combo_ids.at(selectedIndex);
	this->BindTripCustomers(trip_id);
}


void MainDialog::OnFileImport()
{

	CFileDialog fOpenDlg(TRUE, _T("xml"), _T(""), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("xml files (*.xml)|*.xml|"), this);

	fOpenDlg.m_pOFN->lpstrInitialDir= _T("c:");

	if(fOpenDlg.DoModal()==IDOK)
	{
		CString file = (LPCTSTR)fOpenDlg.GetPathName();
		XMLImporter::LoadData(file);
		this->BindTrips();
	}
}



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

void MainDialog::OnLvnItemchangedCustomers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString cs;
	CListCtrl* cl = (CListCtrl*)this->GetDlgItem(IDC_CUSTOMERS);
	cs = cl->GetItemText(pNMLV->iItem,0);
	int customer_id = _tstoi(cs);
	
	CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
	int selectedIndex = cb->GetCurSel();
	int trip_id = this->m_vector_combo_ids.at(selectedIndex);
	
	this->BindTripDeliveries(trip_id, customer_id);
	
	*pResult = 0;
}


void MainDialog::OnLvnItemchangedDeliveryItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
