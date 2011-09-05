#include "session.h"
#include "Param.h"
#include "trip.h"

Session::Session(void) : m_handle_open(false)
{

}

Session::~Session(void)
{
	sqlite3_close(m_dbHandle);	
}

Session &Session::operator <<(const TCHAR *statement)
{
	m_statement = statement;
	return *this;
}

Session &Session::operator <<(const Param &parameter)
{
	this->m_list_parameters.push_back(parameter);
	return *this;
}



void Session::Execute()
{
	int result;

	sqlite3_stmt *statement = this->_PrepareAndBind();
	result = sqlite3_step(statement);
	HandleResult(result);
	sqlite3_reset(statement);
	sqlite3_finalize(statement);
}


sqlite3_stmt *Session::_PrepareAndBind()
{
	int result;

	if(!m_handle_open)
	{

#ifdef UNICODE
		result = sqlite3_open16(_T("data.db"), &m_dbHandle);
#else
		result = sqlite3_open(_T("data.db"), &m_dbHandle);
#endif
		if(result != SQLITE_OK)
		{
			throw SessionException(_T("Could not open database"));
			return NULL;
		}
		m_handle_open = true;
	}
	
	

	sqlite3_stmt *statement;

#ifdef UNICODE
	result = sqlite3_prepare16_v2(m_dbHandle, this->m_statement, -1, &statement, NULL);
#else
	result = sqlite3_prepare_v2(m_dbHandle, this->m_statement, -1, &statement, NULL);
#endif

	HandleResult(result);

	list<Param>::iterator it;
	int counter = 1;

	for(it = this->m_list_parameters.begin(); it != this->m_list_parameters.end(); ++it)
	{
		this->BindParameter(statement, *it, counter);
		++counter;
	}

	return statement;
}

void Session::HandleResult(int result_code) const
{
	if(result_code != SQLITE_OK && result_code != SQLITE_ROW && result_code != SQLITE_DONE)
	{
		CString errorMessage;
		TCHAR *error;
#ifdef UNICODE
		error = (TCHAR*)sqlite3_errmsg16(m_dbHandle);
#else
		error = sqlite3_errmsg(handle);
#endif
		errorMessage = error;
		sqlite3_free(error);
		throw SessionException(errorMessage);
	}
}


void Session::BindParameter(sqlite3_stmt *statement, const Param &param, int position)
{
	Param::ParamType type = param.GetType();
	int result;
	switch(type)
	{
	case Param::TYPE_STRING:
#ifdef UNICODE
		result = sqlite3_bind_text16(statement, position, param.GetValue(),-1,NULL);
#else 
		sqlite3_bind_text(statement, position, (TCHAR)param.GetValue()),-1,NULL);
#endif
		break;
	case Param::TYPE_INT:
		result = sqlite3_bind_int(statement, position, *((int*)param.GetValue()));
		break;
	default:
		result = sqlite3_bind_double(statement, position, *((float*)param.GetValue()));
		break;
	}

	if(result != SQLITE_OK)
	{
		throw SessionException(_T("Could not bind parameter"));
	}

}

void Session::ClearParams()
{
	this->m_list_parameters.clear();
}

void Session::_MapResultColumnsToObject(ObjectBase *oPtr, sqlite3_stmt *statement)
{
	int cols = sqlite3_column_count(statement);	
		TCHAR *column_name;

		for(int x = 0; x < cols; ++x)
		{
#ifdef UNICODE
			column_name = (TCHAR*)sqlite3_column_name16(statement, x);
#else
			column_name = sqlite3_column_name(statement, x);
#endif
			if(oPtr->GetMap().PLookup(column_name) == NULL)
			{
				delete oPtr;
				continue;
			}

			VarInfo p(oPtr->GetMap().PLookup(column_name)->value);
			switch(p.type)
			{
			case VarInfo::STRING:
#ifdef UNICODE
				*((CString*)p.dataPtr) = (TCHAR*)sqlite3_column_text16(statement, x);
#else
				*((CString*)p.dataPtr) = sqlite3_column_text(statement, x);
#endif
				break;
			case VarInfo::INT:
				*((int*)p.dataPtr) = sqlite3_column_int(statement, x);
				break;
			case VarInfo::INT64:
				*((__int64*)p.dataPtr) = sqlite3_column_int64(statement, x);
				break;
			case VarInfo::DOUBLE:
				*((double*)p.dataPtr) = sqlite3_column_double(statement, x);
				break;
			}

		}
}