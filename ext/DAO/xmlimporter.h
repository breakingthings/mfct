#ifndef _XMLIMPORTER_H
#define _XMLIMPORTER_H

#include <xercesc/dom/DOM.hpp>
#include <afx.h>
#include "trip.h"
#include "resource.h"

using namespace BLL;
/**
* Class to Import XML files into database
*/
class XMLImporter
{
public:
	~XMLImporter(void);
	/**
	* Load data from XML file 
	* @param file full path to xml file
	*/
	static void LoadData(const CString& file);
private:
	bool m_hasError;
	XMLImporter &operator =(const XMLImporter&);
	XMLImporter(const XMLImporter&);
	XMLImporter(void);
	XMLImporter(const CString&);
	/**
	* Parses DOM document and stores data to database
	*/
	void ParseAndStoreItems(xercesc::DOMDocument *doc) const;
	/**
	* Store individual delivery
	* @param trip Trip object
	* @param customer_name customer name
	* @param customer_address csutomer address
	* @param name delivery item name
	* @param unit delivery item unit
	* @param quantity delivery item quantity
	* @param price single item price
	* @param total total sum (price * quantity)
	*/
	void StoreDelivery(shared_ptr<Trip> trip,const CString &customer_name, const CString &customer_address, 
		const CString &name, const CString &unit, int quantity, double price, double total) const;
	/**
	* Holds current xml file
	*/
	CString m_xml_file;
	/**
	* Get pointer to xml schema resource
	* @param sizePtr where size of data will be stored
	* @return pointer to xsd data
	*/
	const void *GetXMLSchema(DWORD *sizePtr);
	/**
	* Function to call from XMLImporter::LoadData
	* @return whether parse was successful
	*/
	bool ParseFile();
};

#endif