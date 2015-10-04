#pragma once

#include "row.h"
#include "statement.h"

#include "sqlite3.h"
#include <string>
#include <iostream>

namespace fcs
{
namespace database
{

class result
{
public:
	enum class step_status
	{
		more,
		done,
		error,
	};
	
	~result();
	
	bool is_error() const;
	int error_code() const;
	bool has_rows() const;
	
	row current() const;
	step_status step();
	
private:
	statement m_statement;
	bool m_hasStepped = false;
	
	template<typename ...Args>
	result(const statement & statement, Args... args)
		: m_statement { statement }
	{
		bind(*this, 1, args...);
	}
	
	void bind(const result & res, int location) { /* noop */ };
	
	template<typename... Args>
	void bind(const result & res, int location, int value, Args... args)
	{
		int status = sqlite3_bind_int(res.m_statement, location, value);
		(void)status; /* TODO: Do something with status */
		bind(res, location+1, args...);
	}
	
	template<typename... Args>
	void bind(const result & res, int location, double value, Args... args)
	{
		sqlite3_bind_double(res.m_statement, location+1, value);
		bind(res, location+1, args...);
	}
	
	template<typename... Args>
	void bind(const result & res, int location, const std::string & string, Args... args)
	{
		sqlite3_bind_text(res.m_statement, location, string.c_str(), static_cast<int>(string.size()), SQLITE_STATIC);
		bind(res, location+1, args...);
	}
	
	friend class database;
};
	
}
}