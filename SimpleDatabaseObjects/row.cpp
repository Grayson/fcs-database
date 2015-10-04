#include "row.h"

namespace fcs
{
namespace database
{

int row::number_of_columns() const
{
	return sqlite3_column_count(m_statement);
}
	
int row::operator [](int columnIndex) const
{
	return sqlite3_column_int(m_statement, columnIndex);
}
	
}
}