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
		bind(1 /* documented as being position of first parameter */, args...);
	}
	
	void bind(int location) { /* noop */ };
	
	template<typename... Args>
	void bind(int location, int value, Args... args)
	{
		int status = sqlite3_bind_int(m_statement, location, value);
		(void)status; /* TODO: Do something with status */
		bind(location+1, args...);
	}
	
	template<typename... Args>
	void bind(int location, double value, Args... args)
	{
		sqlite3_bind_double(m_statement, location+1, value);
		bind(location+1, args...);
	}
	
	template<typename... Args>
	void bind(int location, const std::string & string, Args... args)
	{
		sqlite3_bind_text(m_statement, location, string.c_str(), static_cast<int>(string.size()), SQLITE_STATIC);
		bind(location+1, args...);
	}
	
	template<typename TValue, typename ...Args>
	void bind(int _, const std::pair<const char *, TValue> & keyValuePair, Args... args)
	{
		int status = sqlite3_bind_int(m_statement, sqlite3_bind_parameter_index(m_statement, keyValuePair.first), keyValuePair.second);
		(void)status; /* TODO: Do something with status */
		bind(std::numeric_limits<int>::min(), args...);
	}
	
	template<typename TValue, typename ...Args>
	void bind(int _, const std::pair<std::string, TValue> & keyValuePair, Args... args)
	{
		bind(_, std::make_pair(keyValuePair.first.c_str(), keyValuePair.second), args...);
	}
	
	template<typename TValue, typename ...Args>
	void bind(int _, const std::pair<int, TValue> & indexValuePair, Args... args)
	{
		bind(indexValuePair.first, indexValuePair.second, args...);
	}
	
	friend class database;
};

}
}