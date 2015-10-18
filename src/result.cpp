#include "result.h"

namespace fcs
{
namespace database
{

result::~result()
{
	if (!m_hasStepped)
		step();
}
	
bool result::is_error() const
{
	auto lastResult = m_statement.last_result();
	return lastResult != SQLITE_OK && lastResult != SQLITE_DONE && lastResult != SQLITE_ROW;
}
	
int result::error_code() const
{
	return m_statement.last_result();
}

bool result::has_rows() const
{
	return m_statement.last_result() == SQLITE_ROW;
}

row result::current() const
{
	return row { m_statement };
}

result::step_status result::step()
{
	m_hasStepped = true;
	switch (m_statement.step()) {
	case SQLITE_ROW:
		return result::step_status::more;
	case SQLITE_DONE:
		return result::step_status::done;
	default:
		return result::step_status::error;
	}
}
	
}
}