#pragma once

#include "row.h"
#include "statement.h"

#include "sqlite3.h"
#include <limits>
#include <string>
#include <utility>

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
		bind(*this, 1 /* documented as being position of first parameter */, args...);
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

	template<typename TValue, typename ...Args>
	void bind(const result & res, int _, const std::pair<const char *, TValue> & keyValuePair, Args... args)
	{
		int status = sqlite3_bind_int(res.m_statement, sqlite3_bind_parameter_index(res.m_statement, keyValuePair.first), keyValuePair.second);
		(void)status; /* TODO: Do something with status */
		bind(res, std::numeric_limits<int>::min(), args...);
	}
	
	template<typename TValue, typename ...Args>
	void bind(const result & res, int _, const std::pair<std::string, TValue> & keyValuePair, Args... args)
	{
		bind(res, _, std::make_pair(keyValuePair.first.c_str(), keyValuePair.second), args...);
	}
	
	template<typename TValue, typename ...Args>
	void bind(const result & res, int _, const std::pair<int, TValue> & indexValuePair, Args... args)
	{
		bind(res, indexValuePair.first, indexValuePair.second, args...);
	}
	
	friend class database;
};
	
}
}