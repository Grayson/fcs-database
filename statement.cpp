#include "statement.h"

#include <iostream>

namespace
{
void finalize(sqlite3_stmt *stmt)
{
	sqlite3_finalize(stmt);
}
}

namespace fcs
{
namespace database
{

statement::statement(sqlite3_stmt * stmt)
	: m_statement { stmt, finalize }
{
}

statement::operator sqlite3_stmt *() const
{
	return m_statement.get();
}

int statement::step()
{
	return m_result = sqlite3_step(m_statement.get());
}
	
int statement::last_result() const
{
	return m_result;
}

}
}