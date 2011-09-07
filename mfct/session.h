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
	/**
	* Adds statement which is to be executed
	*/
	Session &operator <<(const TCHAR *statement);
	/**
	* Adds bound parameter
	* @param parameter Parameter
	*/
	Session &operator <<(const Param &parameter);
	/**
	* Execute the statement without result
	*/
	void Execute();
	/**
	* Clear stored parameters list
	*/
	void ClearParams();
	/**
	* Get single result integer
	*/
	int GetInt();

	/*
	* Maps result to business object of base ObjectBase
	*/
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

	/*
	* Maps result to vector of business objects of base ObjectBase
	*/
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


				T t;
				//ObjectBase* oPtr = new T;
				_MapResultColumnsToObject(&t, statement);

				//T *oo = (oPtr); 
				resultVector->push_back(t);
				


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
	/**
	* Container for Parameter items
	*/
	list<Param> m_list_parameters;
	/**
	* Holds statement
	*/
	CString m_statement;
	/**
	* SQLITE handle
	*/
	sqlite3 *m_dbHandle;
	bool m_handle_open;
private:
	/**
	* Maps sql result columns to Business object
	*/
	void _MapResultColumnsToObject(ObjectBase *oPtr, sqlite3_stmt *statement);
	
	/**
	* Prepares statement and binds stored Parameters to statement
	*/
	sqlite3_stmt *_PrepareAndBind();

	Session &operator=(Session const &);
	/**
	* Binds Parameter to position
	*/
	void _BindParameter(sqlite3_stmt *statement, const Param &param, int position);
	/**
	* Handles SQLITE result codes - throws CString containing sqlite error message
	*/
	void _HandleResult(int result_code) const;
	
};

#endif