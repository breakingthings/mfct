#ifndef _OBJECTBASE_H
#define _OBJECTBASE_H

#include <afxtempl.h>
#include <afxcoll.h>
#include <memory>
#include <cstring>
#include <unordered_map>
#include "varinfo.h"

using namespace std;

typedef CMap<CString, const TCHAR*, VarInfo, VarInfo&> VarInfoMap;

#define MAPTABLE(a) this->m_table_name = _T(a);
#define MAPCOLUMN(a, b, c) this->m_varmap.SetAt(_T(a), VarInfo(b,c));


class ObjectBase
{
	
public:
	friend class Session;
	ObjectBase(void);
	virtual ~ObjectBase(void);
	/**
	* Returns object map (column name to variable pointer)
	*/
	virtual const VarInfoMap &GetMap() const = 0;
	/**
	* Gets object table name - would be useful if full implementation 
	*/
	const CString &GetTableName() const;
	ObjectBase &operator =(const ObjectBase&);
	/**
	* Whether object is loaded or not
	*/
	bool IsNull();
protected:	
	/**
	* contains map of sql result field names to object variable pointers
	*/
	VarInfoMap  m_varmap;
	CString m_table_name;
	bool m_is_null;
};

#endif