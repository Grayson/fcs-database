#pragma once

#include "sqlite3.h"
#include <memory>

namespace fcs
{
namespace database
{
	
class statement
{
public:
	operator sqlite3_stmt *() const;
	
private:
	statement(sqlite3_stmt * stmt);
	int step();
	int last_result() const;

	std::shared_ptr<sqlite3_stmt> m_statement;
	int m_result = 0;
	
	friend class database;
	friend class row;
	friend class result;
};
}
}