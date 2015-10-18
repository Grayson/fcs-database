#include "row.h"
#include <string>

namespace
{

bool column_is_null(sqlite3_stmt * statement, int columnIndex)
{
	return sqlite3_column_type(statement, columnIndex) == SQLITE_NULL;
}

}

namespace fcs
{
namespace database
{

int row::number_of_columns() const
{
	return sqlite3_column_count(m_statement);
}

template<>
nullable<int> row::get(int columnIndex) const
{
	return column_is_null(m_statement, columnIndex) ? nullable<int> {} :  nullable<int> { sqlite3_column_int(m_statement, columnIndex) };
}
	
template <>
nullable<std::string> row::get(int columnIndex) const
{
	return column_is_null(m_statement, columnIndex) ? nullable<std::string>{} : nullable<std::string>{ std::string { reinterpret_cast<const char *>(sqlite3_column_text(m_statement, columnIndex)) } };
}
	
template<>
nullable<data> row::get(int columnIndex) const
{
	if (column_is_null(m_statement, columnIndex))
		return nullable<data> { data { nullptr, 0 } };
	
	const void * pointer = sqlite3_column_blob(m_statement, columnIndex);
	int size = sqlite3_column_bytes(m_statement, columnIndex);
	return nullable<data> { data { const_cast<void *>(pointer), static_cast<std::size_t>(size) } };
}

}
}