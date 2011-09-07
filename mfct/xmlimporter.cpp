#include "stdafx.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/XMLUni.hpp>
#include "xmlimporter.h"
#include "TripFactory.h"
#include "CustomerFactory.h"
#include "DeliveryFactory.h"

class XErrorHandler : public xercesc::ErrorHandler
{
private:
	bool m_hasError;
	CString m_errorMsg;
	
public:
	XErrorHandler() : m_hasError(false)
	{
		
	}

	void warning(const xercesc::SAXParseException& exc)
	{
		m_hasError = true;
		m_errorMsg = exc.getMessage();
	}

	void error(const xercesc::SAXParseException& exc)
	{
		m_hasError = true;
		m_errorMsg = exc.getMessage();
	}

	void fatalError(const xercesc::SAXParseException& exc)
	{
		m_hasError = true;
		m_errorMsg = exc.getMessage();
	}

	void resetErrors()
	{
		m_hasError = false;
	}

	bool hasError()
	{
		return this->m_hasError;
	}

	CString getErrorMessage()
	{
		return m_errorMsg;
	}
};

HMODULE GetCurrentModule(void)
{
	HMODULE hModule = NULL;
	GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModule,
		&hModule);
	return hModule;
}

XMLImporter::~XMLImporter(void)
{
	xercesc::XMLPlatformUtils::Terminate();
}

void XMLImporter::LoadData(const CString &file)
{
	XMLImporter xi(file);
}

XMLImporter::XMLImporter(const CString &file) : m_hasError(false)
{
	this->m_xml_file = file;
	this->ParseFile();
}


bool XMLImporter::ParseFile()
{
	try
	{
		xercesc::XMLPlatformUtils::Initialize();
	} catch (const xercesc::XMLException &ex)
	{
		throw(ex.getMessage());
	}
	

	XErrorHandler e_handler;

	xercesc::XercesDOMParser *parser = new xercesc::XercesDOMParser();
	//NEXT ONE IS IMPORTANT !!!! - COSTS HOURS OF WORK FOR !#¤%¤"#%"#¤ SAKE
	parser->setIncludeIgnorableWhitespace(false);
	parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
	parser->setLoadExternalDTD(false);
	parser->setErrorHandler(&e_handler);
	parser->setDoNamespaces(true);
	parser->setDoSchema(true);
	
	DWORD size = 0;
	const void *schema_data = GetXMLSchema(&size);
	
	
	xercesc::MemBufInputSource mbuf ((const XMLByte*)schema_data, size, _T("schema"));
	parser->loadGrammar(mbuf, xercesc::Grammar::SchemaGrammarType, true);
	parser->useCachedGrammarInParse(true);
	xercesc::LocalFileInputSource src(this->m_xml_file);
	
	parser->parse(src);	
	if(e_handler.hasError())
	{
		throw(e_handler.getErrorMessage());
		return false;
	}
	
	xercesc::DOMDocument *doc = parser->getDocument();	
	this->ParseAndStoreItems(doc);
	
	parser->resetDocumentPool();

	
	delete parser;
	
	
	return true;
	
	
}

void XMLImporter::ParseAndStoreItems(xercesc::DOMDocument *doc) const
{
	xercesc::DOMElement *el = doc->getDocumentElement();
	xercesc::DOMNode *nod = el->getChildNodes()->item(0);
	
	CString customer_name, customer_address, d_name, d_unit;
	int d_quantity;
	double d_price, d_total;

	xercesc::DOMNode *node;
	xercesc::DOMNode *trip = el->getFirstChild();
	xercesc::DOMNode *delivery;
	xercesc::DOMNode *delivery_item;
	do{
		shared_ptr<Trip> t = TripFactory::AddTrip();
		delivery = trip->getFirstChild();
		do{
			customer_name = delivery->getAttributes()->getNamedItem(_T("customer_name"))->getTextContent();
			customer_address = delivery->getAttributes()->getNamedItem(_T("address"))->getTextContent();
			delivery_item = delivery->getFirstChild();

			do{
				node = delivery_item->getFirstChild();
				d_name = node->getTextContent();
				
				node = node->getNextSibling();
				d_unit = node->getTextContent();
				
				node = node->getNextSibling();
				d_quantity = _tstoi(node->getTextContent());

				node = node->getNextSibling();
				d_price = _tstof(node->getTextContent());

				node = node->getNextSibling();
				d_total = _tstof(node->getTextContent());
				
				//Store to DB
				
				this->StoreDelivery(t, customer_name, customer_address, d_name, d_unit, d_quantity,
					d_price, d_total);
				

			} while((delivery_item = delivery_item->getNextSibling()) != NULL);

		} while((delivery = delivery->getNextSibling()) != NULL);

	} while((trip = trip->getNextSibling()) != NULL);
}

void XMLImporter::StoreDelivery(shared_ptr<Trip> trip,const CString &customer_name, const CString &customer_address, 
		const CString &name, const CString &unit, int quantity, double price, double total) const
{
	shared_ptr<Customer> c = CustomerFactory::AddOrGetCustomer(customer_name, customer_address);
	DeliveryFactory::AddDelivery(trip, c, name, unit, quantity, price, total);
}

const void *XMLImporter::GetXMLSchema(DWORD *sizePtr)
{
	HMODULE hm = GetCurrentModule();
	*sizePtr = 0;
	HRSRC hResource = FindResource(hm, MAKEINTRESOURCE(IDR_XML_SCHEMA1), _T("XML_SCHEMA"));
	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hm, hResource);
		if (hLoadedResource)
		{
			LPVOID pLockedResource = LockResource(hLoadedResource);
			if (pLockedResource)
			{
				DWORD dwResourceSize = SizeofResource(hm, hResource);
				*sizePtr = dwResourceSize;
				if (0 != dwResourceSize)
				{
					return pLockedResource;
				}
			}
			return NULL;
		}
		return NULL;
	}
	return NULL;
}
