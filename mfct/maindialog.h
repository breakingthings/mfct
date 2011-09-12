#ifndef _MAINDIALOG_H
#define _MAINDIALOG_H
#include <map>
#include <vector>
#include "resource.h"
#include <tripfactory.h>
#include "xmlimporter.h"
#include <deliveryfactory.h>
#include "customerfactory.h"
#include "CGridListCtrlEx\CGridListCtrlGroups.h"
#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "d:\projects\mfct\mfct\cgridlistctrlex\cgridlistctrlgroups.h"

using namespace std;

class MainDialog : public CDialogEx
{
private:
	CStatic m_versionString;
	CComboBox m_Combo;
	/**
	* Holds quantity string when starting to edit quantity
	*/
	CString m_quantity_text;

public:
	enum { IDD = IDD_DIALOG1 };
	MainDialog(CWnd* parent = NULL);
	void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

	BOOL OnInitDialog();
	/**
	* Handler for File->Exit 
	*/
	void OnFileExit();
	/**
	* Handler for File->Import
	*/
	afx_msg void OnFileImport();
	/**
	* Handler for Trip list Dropdown item changed
	*/
	afx_msg void OnCbnSelendokCombo1();
	/**
	* Handler for Customer list item selected
	*/
	afx_msg void OnLvnItemchangedCustomers(NMHDR *pNMHDR, LRESULT *pResult);
	/**
	* Handler for Delivery list quantity field starting to edit
	*/
	afx_msg void OnLvnStartLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	/**
	* Handler for Delivery list quantity field editing finished 
	*/
	afx_msg void OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
protected:

private:
	std::vector<int> m_vector_combo_ids;
private:
	/**
	* Initialize all list controls olumns
	*/
	void InitColumns();
	/**
	* Bind Trips drowpdonwn
	*/
	void BindTrips();
	/**
	* Bind trip customers list
	* @param trip_id current trip
	*/
	void BindTripCustomers(int trip_id);
	/**
	* Bind deliveries done during the trip
	* @param trip_id currently selected Trip id
	* @customer_id currently selected Customer id
	*/
	void BindTripDeliveries(int trip_id, int customer_id);
	/**
	* Overloaded method that gets selected trip_id and delivery_id automatically
	*/
	void BindTripDeliveries();
	
	void ShowErrorMsg(const CString &msg);
public:
	CGridListCtrlGroups m_GridListCustomers;
	CGridListCtrlGroups m_GridListDeliveryItems;
	
	afx_msg void OnFileExport();
};


#endif