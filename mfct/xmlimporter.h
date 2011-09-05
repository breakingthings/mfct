#ifndef _XMLIMPORTER_H
#define _XMLIMPORTER_H
#include <afx.h>
#include "resource.h"
#include <xercesc/dom/DOM.hpp>
#include "trip.h"

class XMLImporter
{
public:
	~XMLImporter(void);
	static void LoadData(const CString&);

private:
	bool m_hasError;
	XMLImporter &operator =(const XMLImporter&);
	XMLImporter(const XMLImporter&);
	XMLImporter(void);
	XMLImporter(const CString&);
	void ParseAndStoreItems(xercesc::DOMDocument *doc) const;
	void StoreDelivery(shared_ptr<Trip> trip,const CString &customer_name, const CString &customer_address, 
		const CString &name, const CString &unit, int quantity, double price, double total) const;
	CString m_xml_file;
	const void *GetXMLSchema(DWORD*);
	bool ParseFile();
};

#endif