#include "xmlexporter.h"
#include "resource.h"
#include "customerfactory.h"
#include "deliveryfactory.h"

using namespace std;

using namespace BLL;
using namespace DLL;


XMLExporter::XMLExporter(const CString &file_name) : m_has_start(false), m_finished(false)
{
	m_stream.open(file_name, ios::out | ios::binary);
	if(!m_stream.is_open())
	{
		throw _T("Error opening file");
	}
	 
}

XMLExporter::~XMLExporter(void)
{
	if(!m_finished)
	{
		this->Finish();
	}
//	delete m_schema_input_source;
	//XMLPlatformUtils::Terminate();
}

void XMLExporter::ExportTrip(const Trip &trip)
{
	if(!this->m_has_start && this->m_stream.is_open())
	{
		this->WriteStart();
	}
	shared_ptr<vector<Customer> > customers = CustomerFactory::GetByTripId(trip.id);
	vector<Customer>::iterator it;
	it = customers->begin();
	CString cs = _T("");
	this->WriteXmlString(_T("<trip>"));
	while(it != customers->end())
	{
		
		cs.Format(_T("<delivery customer_name=\"%s\" address=\"%s\">"), it->name, it->address);
		this->WriteXmlString(cs);
		shared_ptr<vector<Delivery> > deliveries = DeliveryFactory::GetDeliveries(trip.id, it->id);
		

		vector<Delivery>::iterator it_delivery;
		it_delivery = deliveries->begin();
		while(it_delivery != deliveries->end())
		{
			this->WriteXmlString(_T("<delivery_item>"));
			this->WriteElement(_T("name"), it_delivery->name);
			this->WriteElement(_T("unit"), it_delivery->name);
			this->WriteElement(_T("quantity"), it_delivery->quantity);
			this->WriteElement(_T("price"), it_delivery->price);
			this->WriteElement(_T("total"), it_delivery->total);
			this->WriteXmlString(_T("</delivery_item>"));
			++it_delivery;
		}		

		
		this->WriteXmlString(_T("</delivery>"));
		++it;
	}
	this->WriteXmlString(_T("</trip>"));
}

void XMLExporter::WriteElement(const TCHAR *name, int value)
{
	CString cs;
	cs.Format(_T("%d"), value);
	WriteElement(name, cs);
}

void XMLExporter::WriteElement(const TCHAR *name, double value)
{
	CString cs;
	cs.Format(_T("%f"), value);
	WriteElement(name, cs);
}

void XMLExporter::WriteElement(const TCHAR *name, const TCHAR *value)
{
	CString cname = name;
	cname.Replace(_T("<"), _T("&gt;"));
	cname.Replace(_T(">"), _T("&lt;"));
	CString cvalue = value;
	cvalue.Replace(_T("<"), _T("&gt;"));
	cvalue.Replace(_T(">"), _T("&lt;"));
	CString res;
	res.Format(_T("<%s>%s</%s>"), cname, cvalue, cname);
	this->WriteXmlString(res);
}

void XMLExporter::WriteStart()
{
	this->WriteXmlString(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
	this->WriteXmlString(_T("<data xmlns=\"http://mfctest.com\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"));
	this->m_has_start = true;
}

void XMLExporter::Finish()
{
	this->WriteXmlString(_T("</data>"));
	this->m_stream.flush();
	this->m_stream.close();
	this->m_finished = true;
}

void XMLExporter::WriteXmlString(const TCHAR *string, UINT codepage)
{
	this->m_stream << CT2A(string, codepage);
}

void XMLExporter::SetFile(const CString &file_name)
{
	
}

