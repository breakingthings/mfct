#ifndef _OBJECTBASE_H
#define _OBJECTBASE_H

#include <afxtempl.h>
#include <afxcoll.h>
#include <memory>
#include <cstring>
#include <unordered_map>
#include <map>
#include "varinfo.h"
#include "globals.h"

using namespace std;
//typedef CMap<CString, const TCHAR*, VarInfo, VarInfo&> VarInfoMap; leaks memory :P

typedef unordered_map<tstring, VarInfo> VarInfoMap;

#define MAPTABLE(a) this->m_table_name = _T(a);
//#define MAPCOLUMN(a, b, c) this->m_varmap->SetAt(_T(a), VarInfo(b,c));
#define MAPCOLUMN(a, b, c) this->m_varmap.insert(make_pair<tstring, VarInfo>(tstring(_T(a)), VarInfo(b,c)));

#define IS_OBJECT void IsObjectBaseObject(){};

class ObjectBase
{
	
public:
	friend class Session;
	ObjectBase(void);
	virtual ~ObjectBase(void);

	/**
	* Gets object table name - would be useful if full implementation 
	*/
	const CString &GetTableName() const;
	ObjectBase &operator =(const ObjectBase&);
	/**
	* Whether object is loaded or not
	*/
	bool IsNull();
	virtual void IsObjectBaseObject() = 0;
protected:	
	/**
	* Returns object map (column name to variable pointer)
	*/
	const VarInfoMap &GetMap() const;
	/**
	* contains map of sql result field names to object variable pointers
	*/
	VarInfoMap m_varmap;
	CString m_table_name;
	bool m_is_null;
};

#endif