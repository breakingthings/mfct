#include "stdafx.h"
#include "maindialog.h"
#include "xmlexporter.h"

using namespace DLL;
using namespace BLL;

MainDialog::MainDialog(CWnd* parent) : CDialogEx(MainDialog::IDD, parent), 
	m_quantity_text(_T(""))
{

}

BOOL MainDialog::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	
	this->InitColumns();	
	this->BindTrips();
	return TRUE;
	
}

void MainDialog::OnFileExit()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void MainDialog::InitColumns()

{
	int result = 0;
	
	
	this->m_GridListCustomers.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_AUTOSIZECOLUMNS);

	result = this->m_GridListCustomers.InsertColumn(0, _T("No"), LVCFMT_CENTER, 30, 0);
	result = this->m_GridListCustomers.InsertColumn(1, _T("Customer name"), LVCFMT_LEFT, 150, 1);
	result = this->m_GridListCustomers.InsertColumn(2, _T("Address"), LVCFMT_LEFT, 150, 2);


	m_GridListDeliveryItems.InsertHiddenLabelColumn();
	CGridColumnTrait *pTrait = new CGridColumnTraitEdit();
	this->m_GridListDeliveryItems.InsertColumn(0, _T("Id"), LVCFMT_CENTER, 30, 0);
	this->m_GridListDeliveryItems.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100, 1);
	this->m_GridListDeliveryItems.InsertColumn(2, _T("Unit"), LVCFMT_LEFT, 100, 2);
	this->m_GridListDeliveryItems.InsertColumnTrait(3, _T("Qty"), LVCFMT_LEFT, 100, 3, pTrait);
	this->m_GridListDeliveryItems.InsertColumn(4, _T("Price"), LVCFMT_LEFT, 100, 4);
	this->m_GridListDeliveryItems.InsertColumn(5, _T("Total"), LVCFMT_LEFT, 100, 5);
}

void MainDialog::BindTripCustomers(int trip_id)
{
	shared_ptr<Trip> trip = TripFactory::GetById(trip_id);
	
	
	this->m_GridListDeliveryItems.DeleteAllItems();

	CString cs = _T("");
	int nIndex;

	if(!trip->IsNull())
	{
		shared_ptr<vector<Customer> > customers = CustomerFactory::GetByTripId(trip_id);
		vector<Customer>::reverse_iterator it;
		
		for(it = customers->rbegin(); it != customers->rend(); ++it)
		{
			cs.Format(_T("%d"), it->id);
			
			nIndex = this->m_GridListCustomers.InsertItem(LVIF_TEXT|LVIF_STATE, 0, cs,0,0,0, NULL);
			this->m_GridListCustomers.SetItemText(nIndex, 0, cs);
			this->m_GridListCustomers.SetItemText(nIndex, 1, it->name);
			this->m_GridListCustomers.SetItemText(nIndex, 2, it->address);
			
		}
	}
}

void MainDialog::BindTrips()
{
	
	int index = 0;
	m_vector_combo_ids.clear();

	CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
	cb->Clear();
	
	shared_ptr<vector<Trip> > trips = TripFactory::GetAllTrips();

	
	vector<Trip>::iterator it;
	CString number;
	for(it = trips->begin(); it != trips->end(); ++it)
	{
		number.Format(_T("%d"), it->id);
		cb->AddString(number);
		m_vector_combo_ids.push_back(it->id);
	}

	cb->SetCurSel(0);
	
	if(trips->size() > 0)
	{
		int id = trips->at(0).id;
/*#ifdef _DEBUG
		CMemoryState oldMemState, newMemState, diffMemState;
		oldMemState.Checkpoint();
#endif*/

 		this->BindTripCustomers(id);
/*
#ifdef _DEBUG
		newMemState.Checkpoint();
		if( diffMemState.Difference( oldMemState, newMemState ) )
		{
			TRACE( "Memory leaked!\n" );
			_CrtDumpMemoryLeaks();
		}
#endif*/
		
	}
	
}



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

void MainDialog::OnLvnItemchangedCustomers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if(pNMLV->uChanged && pNMLV->uNewState & LVIS_SELECTED)
	{
		CString cs;
		CListCtrl* cl = (CListCtrl*)this->GetDlgItem(IDC_CUSTOMERS);
		cs = cl->GetItemText(pNMLV->iItem,0);
	
	int customer_id = _tstoi(cs);
	//Get combobox selected trip id
	CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
	int selectedIndex = cb->GetCurSel();
	int trip_id = this->m_vector_combo_ids.at(selectedIndex);

 	this->BindTripDeliveries(trip_id, customer_id);
	
	*pResult = 0;
	}
}

void MainDialog::OnLvnStartLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO *pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
	if(pDispInfo->item.mask & LVIF_PARAM)
	{
		this->m_quantity_text = m_GridListDeliveryItems.GetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem);
	}
}

void MainDialog::OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO *pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
	
	if(pDispInfo->item.mask & LVIF_TEXT)
	{
		CString text = pDispInfo->item.pszText;
		int t_length = text.GetLength();
		bool is_correct = t_length > 0;
		
		for(int x = 0; x < t_length; ++x)
		{
			if(!_istdigit(text.GetAt(x)))
			{
				is_correct = false;
				break;
			}
		}
		
		if(!is_correct)
		{
			this->m_GridListDeliveryItems.SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, 
					this->m_quantity_text);
		} else {
			int quantity = _tstoi(text);
			text = this->m_GridListDeliveryItems.GetItemText(pDispInfo->item.iItem, 0);
			int delivery_id = _tstoi(text);
			try{
				DeliveryFactory::UpdateDeliveryCount(delivery_id, quantity);
				this->BindTripDeliveries();
			} catch (const CString &exception)
			{
				this->ShowErrorMsg(exception);
			}
		}
	} 

	*pResult = 0;
}

void MainDialog::ShowErrorMsg(const CString &msg)
{
	AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
}

void MainDialog::BindTripDeliveries()
{
	
	CComboBox *cb = (CComboBox*)this->GetDlgItem(IDC_COMBO_TRIPS);
	int selectedIndex = cb->GetCurSel();
	int trip_id = -1;
	int customer_id = -1;

	if(this->m_vector_combo_ids.size() >= selectedIndex)
	{
		trip_id = this->m_vector_combo_ids.at(selectedIndex);
	}
	
	CListCtrl* cl = (CListCtrl*)this->GetDlgItem(IDC_CUSTOMERS);
	int column = cl->GetSelectionMark();
	if(column > -1)
	{
		CString text = cl->GetItemText(column, 0);
		customer_id = _tstoi(text);
	}

	this->BindTripDeliveries(trip_id, customer_id);
}

void MainDialog::BindTripDeliveries(int trip_id, int customer_id)
{
	
	this->m_GridListDeliveryItems.DeleteAllItems();
	shared_ptr<vector<Delivery> > deliveries = DeliveryFactory::GetDeliveries(trip_id, customer_id);
	vector<Delivery>::reverse_iterator it;
	CString cs = "";

	for(it = deliveries->rbegin(); it != deliveries->rend(); ++it)
	{
		cs.Format(_T("%d"), it->id);
		int nIndex = this->m_GridListDeliveryItems.InsertItem(LVIF_TEXT|LVIF_STATE, 0, cs,0,0,0, NULL);
		
		this->m_GridListDeliveryItems.SetItemText(nIndex,0, cs);
		
		this->m_GridListDeliveryItems.SetItemText(nIndex, 1, it->name);
		
		this->m_GridListDeliveryItems.SetItemText(nIndex, 2, it->unit);

		cs.Format(_T("%d"), it->quantity);
		this->m_GridListDeliveryItems.SetItemText(nIndex, 3, cs);

		cs.Format(_T("%f"), it->price);
		this->m_GridListDeliveryItems.SetItemText(nIndex, 4, cs);

		cs.Format(_T("%f"), it->total);
		this->m_GridListDeliveryItems.SetItemText(nIndex, 5, cs);
	}

}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOMERS, m_GridListCustomers);
	DDX_Control(pDX, IDC_DELIVERY_ITEMS, m_GridListDeliveryItems);
	DDX_Control(pDX, IDC_CUSTOMERS, m_GridListCustomers);
}

BEGIN_MESSAGE_MAP( MainDialog, CDialogEx)
	ON_COMMAND(ID_FILE_EXIT,OnFileExit)
	ON_CBN_SELENDOK(IDC_COMBO_TRIPS, &MainDialog::OnCbnSelendokCombo1)
	ON_COMMAND(ID_FILE_IMPORT, &MainDialog::OnFileImport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CUSTOMERS, &MainDialog::OnLvnItemchangedCustomers)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_DELIVERY_ITEMS, &MainDialog::OnLvnEndLabelEdit)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_DELIVERY_ITEMS, &MainDialog::OnLvnStartLabelEdit)
	ON_COMMAND(ID_FILE_EXPORT, &MainDialog::OnFileExport)
END_MESSAGE_MAP()


void MainDialog::OnFileExport()
{

	CFileDialog fOpenDlg(TRUE, _T("xml"), _T(""), OFN_OVERWRITEPROMPT, 
		_T("xml files (*.xml)|*.xml|"), this);

	fOpenDlg.m_pOFN->lpstrInitialDir= _T("c:");

	if(fOpenDlg.DoModal()==IDOK)
	{
	
	try
	{
		XMLExporter ex((LPCTSTR)fOpenDlg.GetPathName());
		shared_ptr<vector<Trip> > trips = TripFactory::GetAllTrips();
		vector<Trip>::iterator i;
		i = trips->begin();
		while(i != trips->end())
		{
			ex.ExportTrip(*i);
			i++;
		}
	} catch (const CString &msg)
	{
		this->ShowErrorMsg(msg);
	}
	}
}
