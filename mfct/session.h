#ifndef _SESSION_H
#define _SESSION_H
#include <vector>
#include <list>
#include <sqlite3.h>
#include <type_traits>
#include "objectbase.h"
#include "Param.h"
#include "objectbase.h"

using namespace std;

class SessionException : public exception
{
public:
	SessionException(const TCHAR *exception)
	{
		this->m_exception_message = exception;
	}

	SessionException(const CString &exception)
	{
		this->m_exception_message = exception;
	}

	const CString &InnerMessage() const
	{
		return m_exception_message;
	}
private:
	CString m_exception_message;
};

class Session
{
public:
	Session(void);
	~Session(void);
	Session &operator <<(const TCHAR *statement);
	Session &operator <<(const Param &parameter);
	void Execute();
	void ClearParams();
	int GetInt();

	template <typename T>
	typename std::enable_if<std::is_base_of<ObjectBase, T>::value, shared_ptr<T> >::type 
	Get()
	{
		sqlite3_stmt *statement = this->_PrepareAndBind();
		int result = sqlite3_step(statement);

		 shared_ptr<T> objPtr = shared_ptr<T>(new T);

		if (result == SQLITE_ROW) {
			this->_MapResultColumnsToObject(objPtr.get(), statement);
		}

		sqlite3_reset(statement);
		sqlite3_finalize(statement);

		return objPtr;
	}

	
	template <typename T>
	typename enable_if<is_base_of<ObjectBase, T>::value, shared_ptr<vector<T> > >::type 
	ExecAndFetch()
	{
		int result;
		shared_ptr<vector<T> > resultVector(new vector<T>);

		sqlite3_stmt *statement = this->_PrepareAndBind();
		
		//int cols;
		bool is_first_result = true;
		while(1)
		{
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {


				ObjectBase* oPtr(new T);
				_MapResultColumnsToObject(oPtr, statement);

				T *oo = static_cast<T*>(oPtr); 
				resultVector.get()->push_back(*oo);
				delete oo;


			} else if (result == SQLITE_DONE)
			{
				break;
			} else 
			{
				_HandleResult(result);
			}
		}

		sqlite3_reset(statement);
		sqlite3_finalize(statement);

		return resultVector;


	}
private:
	void _MapResultColumnsToObject(ObjectBase *oPtr, sqlite3_stmt *statement);

	sqlite3_stmt *_PrepareAndBind();
	Session &operator=(Session const &);
	list<Param> m_list_parameters;
	CString m_statement;
	void _BindParameter(sqlite3_stmt *statement, const Param &param, int position);
	void _HandleResult(int result_code) const;
	sqlite3 *m_dbHandle;
	bool m_handle_open;
};

#endif